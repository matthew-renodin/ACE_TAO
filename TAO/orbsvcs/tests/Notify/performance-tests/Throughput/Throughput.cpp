// $Id$

#include "Throughput.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/OS.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/Notify/Notify_Default_CO_Factory.h"
#include "orbsvcs/Notify/Notify_Default_POA_Factory.h"
#include "orbsvcs/Notify/Notify_Default_Collection_Factory.h"
#include "orbsvcs/Notify/Notify_Default_EMO_Factory.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"

ACE_RCSID (Notify_Tests, Throughput, "$Id$")

/***************************************************************************/

Throughput_StructuredPushConsumer::Throughput_StructuredPushConsumer (
    Notify_Throughput *test_client
  )
  : test_client_ (test_client),
    push_count_ (0)
{
}

void
Throughput_StructuredPushConsumer::accumulate_into (
    ACE_Throughput_Stats &throughput
  ) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushConsumer::dump_stats (const char* msg,
                                               ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

void
Throughput_StructuredPushConsumer::push_structured_event (
    const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  // Extract payload.
  const char* msg;
  CORBA::Boolean ok = (notification.remainder_of_body >>= msg);

  if (!ok)
    ACE_DEBUG ((LM_DEBUG, "(%t) Error extracting message body\n"));

  TimeBase::TimeT Throughput_base_recorded;
  ACE_hrtime_t Throughput_base;

  notification.filterable_data[0].value >>= Throughput_base_recorded;

  ORBSVCS_Time::TimeT_to_hrtime (Throughput_base,
                                 Throughput_base_recorded);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->push_count_ == 0)
    {
    this->throughput_start_ = ACE_OS::gethrtime ();
    }

  ++this->push_count_;

  // Grab timestamp again.
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  // Record statistics.
  this->throughput_.sample (now - this->throughput_start_,
                            now - Throughput_base);

  if (this->push_count_%1000 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P)(%t) event count = %d\n",
                  this->push_count_));
    }

  if (push_count_ == test_client_->perconsumer_count_)
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%t)expected count reached\n"));
    test_client_->peer_done ();
  }
}

/***************************************************************************/

Throughput_StructuredPushSupplier::Throughput_StructuredPushSupplier (
    Notify_Throughput* test_client
  )
  :test_client_ (test_client),
   push_count_ (0)
{
}

Throughput_StructuredPushSupplier::~Throughput_StructuredPushSupplier ()
{
}

void
Throughput_StructuredPushSupplier::accumulate_into (
    ACE_Throughput_Stats &throughput
  ) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushSupplier::dump_stats (const char* msg,
                                               ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

int
Throughput_StructuredPushSupplier::svc (void)
{
  // Initialize a time value to pace the test.
  ACE_Time_Value tv (0, test_client_->burst_pause_);

  // Operations.
  CosNotification::StructuredEvent event;

  // EventHeader

  // FixedEventHeader
  // EventType
  // string
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  // OptionalHeaderFields
  // PropertySeq
  // sequence<Property>: string name, any value
  CosNotification::PropertySeq& qos =  event.header.variable_header;
  qos.length (0); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("Throughput_base");

  event.remainder_of_body <<= test_client_->payload_;

  ACE_DECLARE_NEW_CORBA_ENV;

  this->throughput_start_ = ACE_OS::gethrtime ();

  for (int i = 0; i < test_client_->burst_count_; ++i)
    {
      for (int j = 0; j < test_client_->burst_size_; ++j)
        {
          // Record current time.
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          TimeBase::TimeT Throughput_base;
          ORBSVCS_Time::hrtime_to_TimeT (Throughput_base,
                                         start);
          // Any.
          event.filterable_data[0].value <<= Throughput_base;

          this->proxy_consumer_->push_structured_event (event
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          this->throughput_.sample (end - this->throughput_start_,
                                    end - start);
        }

      ACE_OS::sleep (tv);
    }

  ACE_DEBUG ((LM_DEBUG, "(%P) (%t) Supplier done\n"));
  test_client_->peer_done ();
  return 0;
}

/***************************************************************************/
Notify_Throughput::Notify_Throughput (void)
  : colocated_ec_ (0),
    burst_count_ (1),
    burst_pause_ (10000),
    burst_size_ (1000),
    payload_size_ (1024),
    payload_ (0),
    consumer_count_ (1),
    supplier_count_ (1),
    perconsumer_count_ (burst_size_*burst_count_*supplier_count_),
    consumers_ (0),
    suppliers_ (0),
    nthreads_ (1),
    peer_done_count_ (consumer_count_ + supplier_count_),
    condition_ (lock_)
{
}

Notify_Throughput::~Notify_Throughput ()
{
  ACE_DECLARE_NEW_CORBA_ENV;
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);

  delete payload_;
}

int
Notify_Throughput::init (int argc, char* argv [] ACE_ENV_ARG_DECL)
{
  // Initialize base class.
  Notify_Test_Client::init_ORB (argc,
                                argv
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

#if (TAO_HAS_CORBA_MESSAGING == 1)
  CORBA::Object_var manager_object =
    orb_->resolve_initial_references ("ORBPolicyManager"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyManager_var policy_manager =
    CORBA::PolicyManager::_narrow (manager_object.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Any sync_scope;
  sync_scope <<= Messaging::SYNC_WITH_TARGET;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] =
    orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_scope
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::SET_OVERRIDE
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
#else
  ACE_DEBUG ((LM_DEBUG,
              "CORBA Messaging disabled in this configuration,"
              " test may not be optimally configured\n"));
#endif /* TAO_HAS_MESSAGING */

  worker_.orb (this->orb_.in ());

  if (worker_.activate (THR_NEW_LWP | THR_JOINABLE,
                        this->nthreads_) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot activate client threads\n"));
    }

  // Create all participents ...
  this->create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (consumers_,
                  Throughput_StructuredPushConsumer*[this->consumer_count_],
                  -1);
  ACE_NEW_RETURN (suppliers_,
                  Throughput_StructuredPushSupplier*[this->supplier_count_],
                  -1);

  // ----

  int i = 0;

  for (i = 0; i < this->consumer_count_; ++i)
    {
      ACE_NEW_RETURN (consumers_[i],
                      Throughput_StructuredPushConsumer (this),
                      -1);
      consumers_[i]->init (root_poa_.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      consumers_[i]->connect (this->consumer_admin_.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  for (i = 0; i < this->supplier_count_; ++i)
    {
      ACE_NEW_RETURN (suppliers_[i],
                      Throughput_StructuredPushSupplier (this),
                      -1);
      suppliers_[i]->TAO_Notify_StructuredPushSupplier::init (
                         root_poa_.in ()
                         ACE_ENV_ARG_PARAMETER
                       );
      ACE_CHECK_RETURN (-1);
      suppliers_[i]->connect (this->supplier_admin_.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

int
Notify_Throughput::parse_args(int argc, char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc, argv);

    const ACE_TCHAR* current_arg = 0;
    while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-colocated_ec") == 0)
        {
          this->colocated_ec_ = 1;
          arg_shifter.consume_arg ();

          // Init Factories
          TAO_Notify_Default_CO_Factory::init_svc ();
          TAO_Notify_Default_POA_Factory::init_svc ();
          TAO_Notify_Default_Collection_Factory::init_svc ();
          TAO_Notify_Default_EMO_Factory::init_svc ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-consumers")))
        {
          this->consumer_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-suppliers")))
        {
          this->supplier_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_size")))
        {
          this->burst_size_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_count")))
        {
          this->burst_count_ = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_pause")))
        {
          this->burst_pause_ = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-payload")))
        {
          this->payload_size_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (this->payload_,
                          char [this->payload_size_],
                          -1);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-EC")))
        {
          this->ec_name_ = current_arg;
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg =
                arg_shifter.get_the_parameter ("-ExpectedCount")))
        {
          this->perconsumer_count_ = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-colocated_ec, "
                     "-consumers [count], "
                     "-suppliers [count], "
                     "-burst_size [size], "
                     "-burst_count [count], "
                     "-burst_pause [time(uS)], "
                     "-payload [size]"
                     "-EC [Channel Name]"
                     "-ExpectedCount [count]\n",
                     argv[0], argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }
    // Recalculate.
    peer_done_count_ = consumer_count_ + supplier_count_;
    return 0;
}

void
Notify_Throughput::create_EC (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->colocated_ec_ == 1)
    {
      this->notify_factory_ =
        TAO_Notify_EventChannelFactory_i::create (this->root_poa_.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));
    }
  else
    {
      this->resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      this->resolve_Notify_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // A channel name was specified, use that to resolve the service.
  if (this->ec_name_.length () != 0)
    {
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (ec_name_.c_str ());

      CORBA::Object_var obj =
        this->naming_context_->resolve (name
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->ec_ =
        CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
else
  {
    CosNotifyChannelAdmin::ChannelID id;

    ec_ = notify_factory_->create_channel (initial_qos_,
                                           initial_admin_,
                                           id
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Notify_Throughput::run_test (ACE_ENV_SINGLE_ARG_DECL)
{

  ACE_DEBUG ((LM_DEBUG, "colocated_ec_ %d ,"
              "burst_count_ %d, "
              "burst_pause_ %d, "
              "burst_size_  %d, "
              "payload_size_ %d, "
              "consumer_count_ %d,  "
              "supplier_count_ %d "
              "expected count %d\n",
              colocated_ec_,
              burst_count_ ,
              burst_pause_ ,
              burst_size_ ,
              payload_size_,
              consumer_count_ ,
              supplier_count_ ,
              perconsumer_count_));

  for (int i = 0; i < this->supplier_count_; ++i)
    {
      suppliers_[i]->
        TAO_Notify_StructuredPushSupplier::init (root_poa_.in ()
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (suppliers_[i]->activate (THR_NEW_LWP | THR_JOINABLE) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate client threads\n"));
        }
    }

  // Wait till we're signalled done.
  {
    ACE_DEBUG ((LM_DEBUG, "(%t)Waiting for shutdown signal in main..\n"));
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lock_);

    while (this->peer_done_count_ != 0)
      {
        condition_.wait ();
      }
  }

  if (this->ec_name_.length () == 0) // we are not using a global EC
    {
      // Destroy the ec.
      this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Signal the workers.
  this->worker_.done_ = 1;
}

void
Notify_Throughput::peer_done (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lock_);

  if (--this->peer_done_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "calling shutdown\n"));
      condition_.broadcast ();
    }
}

void
Notify_Throughput::dump_results (void)
{
  ACE_Throughput_Stats throughput;
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  char buf[BUFSIZ];

  for (int j = 0; j < this->consumer_count_; ++j)
    {
      ACE_OS::sprintf (buf, "Consumer [%02d]", j);

      this->consumers_[j]->dump_stats (buf, gsf);
      this->consumers_[j]->accumulate_into (throughput);
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;

  for (int i = 0; i < this->supplier_count_; ++i)
    {
      ACE_OS::sprintf (buf, "Supplier [%02d]", i);

      this->suppliers_[i]->dump_stats (buf, gsf);
      this->suppliers_[i]->accumulate_into (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results ("Notify_Consumer/totals", gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results ("Notify_Supplier/totals", gsf);
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  ACE_High_Res_Timer::calibrate ();

  Notify_Throughput events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  ACE_TRY_NEW_ENV
    {
      events.init (argc, argv
                      ACE_ENV_ARG_PARAMETER); //Init the Client
      ACE_TRY_CHECK;

      events.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Waiting for threads to exit...\n"));
      ACE_Thread_Manager::instance ()->wait ();
      events.dump_results();

      ACE_DEBUG ((LM_DEBUG, "ending main...\n"));

    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Events user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Events system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}


// ****************************************************************

Worker::Worker (void)
:done_ (0)
{
}

void
Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

int
Worker::svc (void)
{
  ACE_Time_Value tv(5);

  do
    {
    this->orb_->run (tv);
  }while (!this->done_);

  ACE_DEBUG ((LM_DEBUG, "(%P) (%t) done\n"));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
