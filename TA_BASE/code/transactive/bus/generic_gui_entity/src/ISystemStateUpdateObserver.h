/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/ISystemStateUpdateObserver.h $
  * @author:  Anita Lee
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This interface provides a contract between an application and a SystemControllerGroupUpdateSubscriber.
  * The SystemControllerGroupUpdateSubscriber notifies the class that implements this interface when
  * SystemController messages are received and of which type. A class should implement this interface
  * to be notified when the system has changed to degraded mode and recovered from degraded mode.
  *
  */

#if !defined(ISystemStateUpdateObserver_A8A39D5A_19B4_4f34_A9A4_D8EB8A947ADF__INCLUDED_)
#define ISystemStateUpdateObserver_A8A39D5A_19B4_4f34_A9A4_D8EB8A947ADF__INCLUDED_

namespace TA_Base_Bus
{
    class ISystemStateUpdateObserver
    {

    public:

        /**
          * Destructor
          */
        virtual ~ISystemStateUpdateObserver() { };


        /**
          * notifySystemStateDegraded
          *
          * This is called when the system is in degraded mode.
          *
          */
        virtual void notifySystemStateDegraded() =0;


        /**
          * notifySystemStateNormal
          *
          * This is called when the system has recovered from degraded mode
          * (ie. system is running normally).
          *
          */
        virtual void notifySystemStateNormal() =0;
    };
}

#endif // !defined(ISystemStateUpdateObserver_A8A39D5A_19B4_4f34_A9A4_D8EB8A947ADF__INCLUDED_)
