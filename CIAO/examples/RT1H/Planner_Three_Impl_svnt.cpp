// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#include "Planner_Three_Impl_svnt.h"
#include "Cookies.h"
#include "ciao/Servant_Activator.h"
#include "ciao/Port_Activator_T.h"
#include "ace/SString.h"

namespace TSCE
{
  namespace CIDL_Planner_Three_Impl
  {
    Planner_Three_Context::Planner_Three_Context (
      ::Components::CCMHome_ptr h,
      ::CIAO::Session_Container *c,
      Planner_Three_Servant *sv)
      : ::CIAO::Context_Impl_Base (h, c), 
        ::CIAO::Context_Impl<
            ::TSCE::CCM_Planner_Three_Context,
            Planner_Three_Servant,
            ::TSCE::Planner_Three,
            ::TSCE::Planner_Three_var
          > (h, c, sv)
    {
    }

    Planner_Three_Context::~Planner_Three_Context (void)
    {
    }

    // Operations for Planner_Three receptacles and event sources,
    // defined in ::TSCE::CCM_Planner_Three_Context.

    void
    Planner_Three_Context::push_command (
      ::TSCE::Command_Event *ev
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      for (ACE_Active_Map_Manager< 
             ::TSCE::Command_EventConsumer_var>::iterator iter =
               this->ciao_publishes_command_map_.begin ();
           iter != this->ciao_publishes_command_map_.end ();
           ++iter)
      {
        (*iter).int_id_->push_Command_Event (
          ev
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }

      for (ACE_Active_Map_Manager< 
             ::Components::EventConsumerBase_var>::iterator giter =
               this->ciao_publishes_command_generic_map_.begin ();
           giter != this->ciao_publishes_command_generic_map_.end ();
           ++giter)
      {
        (*giter).int_id_->push_event (
          ev
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
    }

    ::Components::Cookie *
    Planner_Three_Context::subscribe_command (
      ::TSCE::Command_EventConsumer_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      if ( ::CORBA::is_nil (c))
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (), 0);
      }

      ::TSCE::Command_EventConsumer_var sub =
        ::TSCE::Command_EventConsumer::_duplicate (c);

      ACE_Active_Map_Manager_Key key;
      this->ciao_publishes_command_map_.bind (sub.in (), key);
      sub._retn ();

      ::Components::Cookie * retv = 0;
      ACE_NEW_THROW_EX (retv,
                        ::CIAO::Map_Key_Cookie (key),
                        ::CORBA::NO_MEMORY ());

      return retv;
    }

    ::Components::Cookie *
    Planner_Three_Context::subscribe_command_generic (
      ::Components::EventConsumerBase_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      if ( ::CORBA::is_nil (c))
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (), 0);
      }

      ::Components::EventConsumerBase_var sub =
        ::Components::EventConsumerBase::_duplicate (c);

      ACE_Active_Map_Manager_Key key;
      this->ciao_publishes_command_generic_map_.bind (sub.in (), key);
      sub._retn ();

      ::Components::Cookie * retv = 0;
      ACE_NEW_THROW_EX (retv,
                        ::CIAO::Map_Key_Cookie (key),
                        ::CORBA::NO_MEMORY ());

      return retv;
    }

    ::TSCE::Command_EventConsumer_ptr
    Planner_Three_Context::unsubscribe_command (
      ::Components::Cookie *ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidConnection))
    {
      ACE_Active_Map_Manager_Key key;

      if (ck == 0 || ::CIAO::Map_Key_Cookie::extract (ck, key) == false)
      {
        ACE_THROW_RETURN ( ::Components::InvalidConnection (),
                          ::TSCE::Command_EventConsumer::_nil ());
      }

      ::TSCE::Command_EventConsumer_var retv;
      if (this->ciao_publishes_command_map_.unbind (key, retv) == 0)
      {
        return retv._retn ();
      }

      ::Components::EventConsumerBase_var ecb;

      if (this->ciao_publishes_command_generic_map_.unbind (key, ecb) != 0)
      {
        ACE_THROW_RETURN ( ::Components::InvalidConnection (),
                          ::TSCE::Command_EventConsumer::_nil ());
      }

      return ::TSCE::Command_EventConsumer::_nil ();
    }

    void
    Planner_Three_Context::push_situation (
      ::TSCE::Situation_Event *ev
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      for (ACE_Active_Map_Manager< 
             ::TSCE::Situation_EventConsumer_var>::iterator iter =
               this->ciao_publishes_situation_map_.begin ();
           iter != this->ciao_publishes_situation_map_.end ();
           ++iter)
      {
        (*iter).int_id_->push_Situation_Event (
          ev
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }

      for (ACE_Active_Map_Manager< 
             ::Components::EventConsumerBase_var>::iterator giter =
               this->ciao_publishes_situation_generic_map_.begin ();
           giter != this->ciao_publishes_situation_generic_map_.end ();
           ++giter)
      {
        (*giter).int_id_->push_event (
          ev
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
    }

    ::Components::Cookie *
    Planner_Three_Context::subscribe_situation (
      ::TSCE::Situation_EventConsumer_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      if ( ::CORBA::is_nil (c))
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (), 0);
      }

      ::TSCE::Situation_EventConsumer_var sub =
        ::TSCE::Situation_EventConsumer::_duplicate (c);

      ACE_Active_Map_Manager_Key key;
      this->ciao_publishes_situation_map_.bind (sub.in (), key);
      sub._retn ();

      ::Components::Cookie * retv = 0;
      ACE_NEW_THROW_EX (retv,
                        ::CIAO::Map_Key_Cookie (key),
                        ::CORBA::NO_MEMORY ());

      return retv;
    }

    ::Components::Cookie *
    Planner_Three_Context::subscribe_situation_generic (
      ::Components::EventConsumerBase_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      if ( ::CORBA::is_nil (c))
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (), 0);
      }

      ::Components::EventConsumerBase_var sub =
        ::Components::EventConsumerBase::_duplicate (c);

      ACE_Active_Map_Manager_Key key;
      this->ciao_publishes_situation_generic_map_.bind (sub.in (), key);
      sub._retn ();

      ::Components::Cookie * retv = 0;
      ACE_NEW_THROW_EX (retv,
                        ::CIAO::Map_Key_Cookie (key),
                        ::CORBA::NO_MEMORY ());

      return retv;
    }

    ::TSCE::Situation_EventConsumer_ptr
    Planner_Three_Context::unsubscribe_situation (
      ::Components::Cookie *ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidConnection))
    {
      ACE_Active_Map_Manager_Key key;

      if (ck == 0 || ::CIAO::Map_Key_Cookie::extract (ck, key) == false)
      {
        ACE_THROW_RETURN ( ::Components::InvalidConnection (),
                          ::TSCE::Situation_EventConsumer::_nil ());
      }

      ::TSCE::Situation_EventConsumer_var retv;
      if (this->ciao_publishes_situation_map_.unbind (key, retv) == 0)
      {
        return retv._retn ();
      }

      ::Components::EventConsumerBase_var ecb;

      if (this->ciao_publishes_situation_generic_map_.unbind (key, ecb) != 0)
      {
        ACE_THROW_RETURN ( ::Components::InvalidConnection (),
                          ::TSCE::Situation_EventConsumer::_nil ());
      }

      return ::TSCE::Situation_EventConsumer::_nil ();
    }

    ::CUTS::Testing_Service_ptr
    Planner_Three_Context::get_connection_cuts_testing_service (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      return ::CUTS::Testing_Service::_duplicate (
        this->ciao_uses_cuts_testing_service_.in ());
    }

    void
    Planner_Three_Context::connect_cuts_testing_service (
      ::CUTS::Testing_Service_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::AlreadyConnected,
                     ::Components::InvalidConnection))
    {
      if (! ::CORBA::is_nil (this->ciao_uses_cuts_testing_service_.in ()))
      {
        ACE_THROW ( ::Components::AlreadyConnected ());
      }

      if ( ::CORBA::is_nil (c))
      {
        ACE_THROW ( ::Components::InvalidConnection ());
      }

      this->ciao_uses_cuts_testing_service_ =
        ::CUTS::Testing_Service::_duplicate (c);
    }

    ::CUTS::Testing_Service_ptr
    Planner_Three_Context::disconnect_cuts_testing_service (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::NoConnection))
    {
      if ( ::CORBA::is_nil (this->ciao_uses_cuts_testing_service_.in ()))
      {
        ACE_THROW_RETURN ( ::Components::NoConnection (),
                          ::CUTS::Testing_Service::_nil ());
      }

      return this->ciao_uses_cuts_testing_service_._retn ();
    }

    // CIAO-specific.

    Planner_Three_Context *
    Planner_Three_Context::_narrow (
      ::Components::SessionContext_ptr p
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      return dynamic_cast<Planner_Three_Context *> (p);
    }

    Planner_Three_Servant::Planner_Three_Servant (
      ::TSCE::CCM_Planner_Three_ptr exe,
      ::Components::CCMHome_ptr h,
      const char *ins_name,
      ::CIAO::Home_Servant_Impl_Base *hs,
      ::CIAO::Session_Container *c,
      ::CIAO::REC_POL_MAP &rec_pol_map)
      : ::CIAO::Servant_Impl_Base (h, hs, c, rec_pol_map),
        ::CIAO::Servant_Impl<
            ::POA_TSCE::Planner_Three,
            ::TSCE::CCM_Planner_Three,
            Planner_Three_Context
          > (exe, h, hs, c, rec_pol_map),
        ins_name_ (ins_name)
    {
      ACE_NEW (this->context_,
               Planner_Three_Context (h, c, this));

      // Set the instance id of the component on the context

      this->context_->_ciao_instance_id (this->ins_name_);

      CIAO_REGISTER_OBV_FACTORY (
        ::TSCE::Track_Event_init,
        ::TSCE::Track_Event);

      ACE_TRY_NEW_ENV
      {
        ::Components::SessionComponent_var scom =
          ::Components::SessionComponent::_narrow (
            exe
            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (! ::CORBA::is_nil (scom.in ()))
        {
          scom->set_session_context (
            this->context_
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

        this->populate_port_tables (
          ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      ACE_CATCHANY
      {
      }

      ACE_ENDTRY;
    }

    Planner_Three_Servant::~Planner_Three_Servant (void)
    {
    }

    void
    Planner_Three_Servant::set_attributes (
      const ::Components::ConfigValues &descr
      ACE_ENV_ARG_DECL)
    {
      ACE_ENV_EMIT_CODE (ACE_UNUSED_ARG (ACE_TRY_ENV));

      for ( ::CORBA::ULong i = 0; i < descr.length (); ++i)
      {
        const char *descr_name = descr[i]->name ();
        ::CORBA::Any &descr_value = descr[i]->value ();

        ACE_UNUSED_ARG (descr_name);
        ACE_UNUSED_ARG (descr_value);
      }
    }

    ::Components::Cookie *
    Planner_Three_Servant::subscribe_command (
      ::TSCE::Command_EventConsumer_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      return this->context_->subscribe_command (
        c
        ACE_ENV_ARG_PARAMETER);
    }

    ::Components::Cookie *
    Planner_Three_Servant::subscribe_command_generic (
      ::Components::EventConsumerBase_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      return this->context_->subscribe_command_generic (
        c
        ACE_ENV_ARG_PARAMETER);
    }

    ::TSCE::Command_EventConsumer_ptr
    Planner_Three_Servant::unsubscribe_command (
      ::Components::Cookie *ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidConnection))
    {
      return this->context_->unsubscribe_command (
        ck
        ACE_ENV_ARG_PARAMETER);
    }

    ::Components::Cookie *
    Planner_Three_Servant::subscribe_situation (
      ::TSCE::Situation_EventConsumer_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      return this->context_->subscribe_situation (
        c
        ACE_ENV_ARG_PARAMETER);
    }

    ::Components::Cookie *
    Planner_Three_Servant::subscribe_situation_generic (
      ::Components::EventConsumerBase_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::ExceededConnectionLimit))
    {
      return this->context_->subscribe_situation_generic (
        c
        ACE_ENV_ARG_PARAMETER);
    }

    ::TSCE::Situation_EventConsumer_ptr
    Planner_Three_Servant::unsubscribe_situation (
      ::Components::Cookie *ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidConnection))
    {
      return this->context_->unsubscribe_situation (
        ck
        ACE_ENV_ARG_PARAMETER);
    }

    Planner_Three_Servant::Track_EventConsumer_track_Servant::Track_EventConsumer_track_Servant (
      ::TSCE::CCM_Planner_Three_ptr executor,
      ::TSCE::CCM_Planner_Three_Context_ptr c)
      : executor_ ( ::TSCE::CCM_Planner_Three::_duplicate (executor)),
        ctx_ ( ::TSCE::CCM_Planner_Three_Context::_duplicate (c))
    {
    }

    Planner_Three_Servant::Track_EventConsumer_track_Servant::~Track_EventConsumer_track_Servant (void)
    {
    }

    ::CORBA::Object_ptr
    Planner_Three_Servant::Track_EventConsumer_track_Servant::_get_component (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      return this->ctx_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    void
    Planner_Three_Servant::Track_EventConsumer_track_Servant::push_Track_Event (
      ::TSCE::Track_Event *evt
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      this->executor_->push_track (
        evt
        ACE_ENV_ARG_PARAMETER);
    }

    // Inherited from ::Components::EventConsumerBase.
    void
    Planner_Three_Servant::Track_EventConsumer_track_Servant::push_event (
      ::Components::EventBase *ev
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::BadEventType))
    {
      ::TSCE::Track_Event_var ev_type =
        ::TSCE::Track_Event::_downcast (ev);

      if (ev_type.in () != 0)
      {
        this->push_Track_Event (
          ev_type.in ()
          ACE_ENV_ARG_PARAMETER);

        return;
      }

      ACE_THROW ( ::Components::BadEventType ());
    }

    CORBA::Boolean
    Planner_Three_Servant::Track_EventConsumer_track_Servant::ciao_is_substitutable (
      const char * event_repo_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      if (event_repo_id == 0)
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (), false);
      }

      Planner_Three_Context *ctx =
        Planner_Three_Context::_narrow (
          this->ctx_.in ()
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      CORBA::ORB_ptr orb = ctx->_ciao_the_Container ()->the_ORB ();

      CORBA::ValueFactory f =
        orb->lookup_value_factory (
          event_repo_id
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      if (f == 0)
      {
        return false;
      }

      CORBA::ValueBase_var v =
        f->create_for_unmarshal (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);

      f->_remove_ref ();

      if (v.in () == 0)
      {
        return false;
      }

      return dynamic_cast< ::TSCE::Track_Event *> (v.in ()) != 0;
    }


    ::TSCE::Track_EventConsumer_ptr
    Planner_Three_Servant::get_consumer_track (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      if (! ::CORBA::is_nil (this->consumes_track_.in ()))
      {
        return ::TSCE::Track_EventConsumer::_duplicate (this->consumes_track_.in ());
      }

      ::Components::EventConsumerBase_var obj =
        this->get_consumer_track_i (
          ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN ( ::TSCE::Track_EventConsumer::_nil ());

      ::TSCE::Track_EventConsumer_var eco =
        ::TSCE::Track_EventConsumer::_narrow (
          obj.in ()
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN ( ::TSCE::Track_EventConsumer::_nil ());

      this->consumes_track_ = eco;
      return ::TSCE::Track_EventConsumer::_duplicate (this->consumes_track_.in ());
    }

    ::Components::EventConsumerBase_ptr
    Planner_Three_Servant::get_consumer_track_i (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      ::Components::EventConsumerBase_ptr ret =
        this->lookup_consumer ("track");

      if (! ::CORBA::is_nil (ret))
      {
        return ret;
      }

      ::CIAO::Port_Activator_T<
      Planner_Three_Servant::Track_EventConsumer_track_Servant,
      ::TSCE::CCM_Planner_Three,
      ::TSCE::CCM_Planner_Three_Context,
      Planner_Three_Servant > *tmp = 0;

      typedef  CIAO::Port_Activator_T<
          Planner_Three_Servant::Track_EventConsumer_track_Servant,
          ::TSCE::CCM_Planner_Three,
          ::TSCE::CCM_Planner_Three_Context, 
          Planner_Three_Servant
        >
      MACRO_MADNESS_TYPEDEF;

      ACE_CString obj_id (this->ins_name_);
      obj_id += "_track";

      ACE_NEW_THROW_EX (
        tmp,
        MACRO_MADNESS_TYPEDEF (obj_id.c_str (),
                               "track",
                               ::CIAO::Port_Activator::Sink,
                               this->executor_.in (),
                               this->context_,
                               this),
        ::CORBA::NO_MEMORY ());

      ::CIAO::Servant_Activator *sa =
        this->container_->ports_servant_activator ();

      if (!sa->register_port_activator (tmp))
      {
        return ::TSCE::Track_EventConsumer::_nil ();
      }

      ::CORBA::Object_var obj =
        this->container_->generate_reference (
          obj_id.c_str (),
          "IDL:TSCE/Track_EventConsumer:1.0",
          ::CIAO::Container::Facet_Consumer
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN ( ::TSCE::Track_EventConsumer::_nil ());

      ::Components::EventConsumerBase_var ecb =
        ::Components::EventConsumerBase::_narrow (
          obj.in ()
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN ( ::TSCE::Track_EventConsumer::_nil ());

      this->add_consumer ("track",
                          ecb.in ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN ( ::TSCE::Track_EventConsumer::_nil ());

      return ecb._retn ();
    }

    ::Components::Cookie *
    Planner_Three_Servant::connect (
      const char *name,
      ::CORBA::Object_ptr connection
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::InvalidConnection,
                     ::Components::AlreadyConnected,
                     ::Components::ExceededConnectionLimit))
    {
      // If the component has no receptacles, this will be unused.
      ACE_UNUSED_ARG (connection);

      if (name == 0)
      {
        ACE_THROW_RETURN ( ::Components::InvalidName (), 0);
      }

      if (ACE_OS::strcmp (name, "cuts_testing_service") == 0)
      {
        ::CUTS::Testing_Service_var _ciao_conn =
        ::CUTS::Testing_Service::_narrow (
          connection
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if ( ::CORBA::is_nil (_ciao_conn.in ()))
        {
          ACE_THROW_RETURN ( ::Components::InvalidConnection (), 0);
        }

        // Simplex connect.
        this->connect_cuts_testing_service (
          _ciao_conn.in ()
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        return 0;
      }

      ACE_THROW_RETURN ( ::Components::InvalidName (), 0);
    }

    ::CORBA::Object_ptr
    Planner_Three_Servant::disconnect (
      const char *name,
      ::Components::Cookie * ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::InvalidConnection,
                     ::Components::CookieRequired,
                     ::Components::NoConnection))
    {
      ACE_UNUSED_ARG (ck);

      if (name == 0)
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (),
                          ::CORBA::Object::_nil ());
      }

      if (ACE_OS::strcmp (name, "cuts_testing_service") == 0)
      {
        // Simplex disconnect.
        return this->disconnect_cuts_testing_service (ACE_ENV_SINGLE_ARG_PARAMETER);
      }

      ACE_THROW_RETURN ( ::Components::InvalidName (),
                        ::CORBA::Object::_nil ());
    }

    ::Components::ReceptacleDescriptions *
    Planner_Three_Servant::get_all_receptacles (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      ::Components::ReceptacleDescriptions *retval = 0;
      ACE_NEW_RETURN (retval,
                      ::Components::ReceptacleDescriptions,
                      0);
      ::Components::ReceptacleDescriptions_var safe_retval = retval;
      safe_retval->length (1UL);

      ::CIAO::Servant_Impl_Base::describe_simplex_receptacle<
          ::CUTS::Testing_Service_var
        > ("cuts_testing_service",
           "IDL:CUTS/Testing_Service:1.0",
           this->context_->ciao_uses_cuts_testing_service_,
           safe_retval,
           0UL);

      return safe_retval._retn ();
    }

    void
    Planner_Three_Servant::connect_cuts_testing_service (
      ::CUTS::Testing_Service_ptr c
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::AlreadyConnected,
                     ::Components::InvalidConnection))
    {
      this->context_->connect_cuts_testing_service (
        c
        ACE_ENV_ARG_PARAMETER);

      this->add_receptacle ("cuts_testing_service", c, 0);
    }

    ::CUTS::Testing_Service_ptr
    Planner_Three_Servant::disconnect_cuts_testing_service (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::NoConnection))
    {
      return this->context_->disconnect_cuts_testing_service (
        ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    ::CUTS::Testing_Service_ptr
    Planner_Three_Servant::get_connection_cuts_testing_service (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      return this->context_->get_connection_cuts_testing_service (
        ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    void
    Planner_Three_Servant::connect_consumer (
      const char * emitter_name,
      ::Components::EventConsumerBase_ptr consumer
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::AlreadyConnected,
                     ::Components::InvalidConnection))
    {
      if (emitter_name == 0)
      {
        ACE_THROW ( ::CORBA::BAD_PARAM ());
      }

      ACE_UNUSED_ARG (consumer);
      ACE_THROW ( ::Components::InvalidName ());
    }

    ::Components::EventConsumerBase_ptr
    Planner_Three_Servant::disconnect_consumer (
      const char *source_name
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::NoConnection))
    {
      if (source_name == 0)
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (),
                          ::Components::EventConsumerBase::_nil ());
      }

      ACE_THROW_RETURN ( ::Components::InvalidName (),
                        ::Components::EventConsumerBase::_nil ());
    }

    ::Components::PublisherDescriptions *
    Planner_Three_Servant::get_all_publishers (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      ::Components::PublisherDescriptions *retval = 0;
      ACE_NEW_RETURN (retval,
                      ::Components::PublisherDescriptions,
                      0);
      ::Components::PublisherDescriptions_var safe_retval = retval;
      safe_retval->length (2UL);

      ::CIAO::Servant_Impl_Base::describe_pub_event_source<
          ::TSCE::Command_EventConsumer_var
        > ("command",
           "IDL:TSCE/Command_Event:1.0",
           this->context_->ciao_publishes_command_map_,
           safe_retval,
           0UL);

      ::CIAO::Servant_Impl_Base::describe_pub_event_source<
          ::TSCE::Situation_EventConsumer_var
        > ("situation",
           "IDL:TSCE/Situation_Event:1.0",
           this->context_->ciao_publishes_situation_map_,
           safe_retval,
           1UL);

      return safe_retval._retn ();
    }

    ::Components::EmitterDescriptions *
    Planner_Three_Servant::get_all_emitters (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      ::Components::EmitterDescriptions *retval = 0;
      ACE_NEW_RETURN (retval,
                      ::Components::EmitterDescriptions,
                      0);
      ::Components::EmitterDescriptions_var safe_retval = retval;
      safe_retval->length (0UL);

      return safe_retval._retn ();
    }

    ::Components::Cookie *
    Planner_Three_Servant::subscribe (
      const char *publisher_name,
      ::Components::EventConsumerBase_ptr subscribe
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::InvalidConnection,
                     ::Components::ExceededConnectionLimit))
    {
      // Just in case there are no if blocks
      ACE_UNUSED_ARG (subscribe);

      if (publisher_name == 0)
      {
        ACE_THROW_RETURN ( ::Components::InvalidName (), 0);
      }

      if (ACE_OS::strcmp (publisher_name, "command") == 0)
      {
        ::TSCE::Command_EventConsumer_var sub =
          ::TSCE::Command_EventConsumer::_narrow (
          subscribe
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if ( ::CORBA::is_nil (sub.in ()))
        {
          ::CORBA::Boolean substitutable =
            subscribe->ciao_is_substitutable (
              ::TSCE::Command_Event::_tao_obv_static_repository_id ()
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (substitutable)
          {
            return this->subscribe_command_generic (
              subscribe
              ACE_ENV_ARG_PARAMETER);
          }

          else
          {
            ACE_THROW_RETURN ( ::Components::InvalidConnection (), 0);
          }
        }

        else
        {
          return this->subscribe_command (
            sub.in ()
            ACE_ENV_ARG_PARAMETER);
        }
      }

      if (ACE_OS::strcmp (publisher_name, "situation") == 0)
      {
        ::TSCE::Situation_EventConsumer_var sub =
          ::TSCE::Situation_EventConsumer::_narrow (
          subscribe
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if ( ::CORBA::is_nil (sub.in ()))
        {
          ::CORBA::Boolean substitutable =
            subscribe->ciao_is_substitutable (
              ::TSCE::Situation_Event::_tao_obv_static_repository_id ()
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (substitutable)
          {
            return this->subscribe_situation_generic (
              subscribe
              ACE_ENV_ARG_PARAMETER);
          }

          else
          {
            ACE_THROW_RETURN ( ::Components::InvalidConnection (), 0);
          }
        }

        else
        {
          return this->subscribe_situation (
            sub.in ()
            ACE_ENV_ARG_PARAMETER);
        }
      }

      ACE_THROW_RETURN ( ::Components::InvalidName (), 0);
    }

    ::Components::EventConsumerBase_ptr
    Planner_Three_Servant::unsubscribe (
      const char *publisher_name,
      ::Components::Cookie *ck
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException,
                     ::Components::InvalidName,
                     ::Components::InvalidConnection))
    {
      // Just in case there are no if blocks
      ACE_UNUSED_ARG (ck);

      if (publisher_name == 0)
      {
        ACE_THROW_RETURN ( ::Components::InvalidName (),
                          ::Components::EventConsumerBase::_nil ());
      }

      if (ACE_OS::strcmp (publisher_name, "command") == 0)
      {
        return this->unsubscribe_command (
          ck
          ACE_ENV_ARG_PARAMETER);
      }

      if (ACE_OS::strcmp (publisher_name, "situation") == 0)
      {
        return this->unsubscribe_situation (
          ck
          ACE_ENV_ARG_PARAMETER);
      }

      ACE_THROW_RETURN ( ::Components::InvalidName (),
                        ::Components::EventConsumerBase::_nil ());
    }

    ::CORBA::Object_ptr
    Planner_Three_Servant::get_facet_executor (
      const char *name
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      if (name == 0)
      {
        ACE_THROW_RETURN ( ::CORBA::BAD_PARAM (),
                          ::CORBA::Object::_nil ());
      }

      return ::CORBA::Object::_nil ();
    }

    // Supported operations.

    // Component attribute operations.

    // Private method to populate the port tables.
    void
    Planner_Three_Servant::populate_port_tables (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
    {
      ACE_ENV_ARG_NOT_USED;
      ::CORBA::Object_var obj_var;
      ::Components::EventConsumerBase_var ecb_var;

      ecb_var =
        this->get_consumer_track_i (
          ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

    Planner_Three_Factory_Servant::Planner_Three_Factory_Servant (
      ::TSCE::CCM_Planner_Three_Factory_ptr exe,
      const char *ins_name,
      ::CIAO::Session_Container *c,
      ::CIAO::REC_POL_MAP &rec_pol_map)
      : ::CIAO::Home_Servant_Impl_Base (c),
        ::CIAO::Home_Servant_Impl<
              ::POA_TSCE::Planner_Three_Factory,
              ::TSCE::CCM_Planner_Three_Factory,
              Planner_Three_Servant
            > (exe, c, ins_name, rec_pol_map)
    {
    }

    Planner_Three_Factory_Servant::~Planner_Three_Factory_Servant (void)
    {
    }

    // Home operations.

    // Home supported interface operations.

    // Home factory and finder operations.

    // Home attribute operations.

    extern "C" PLANNER_THREE_IMPL_SVNT_Export ::PortableServer::Servant
    create_TSCE_Planner_Three_Factory_Servant (
      ::Components::HomeExecutorBase_ptr p,
      ::CIAO::Session_Container *c,
      const char *ins_name,
      ::CIAO::REC_POL_MAP &rec_pol_map
      ACE_ENV_ARG_DECL)
    {
      if (p == 0)
      {
        return 0;
      }

      ::TSCE::CCM_Planner_Three_Factory_var x =
      ::TSCE::CCM_Planner_Three_Factory::_narrow (
        p
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (::CORBA::is_nil (x.in ()))
      {
        return 0;
      }

      return new
      Planner_Three_Factory_Servant (
        x.in (),
        ins_name,
        c, rec_pol_map);
    }
  }
}

