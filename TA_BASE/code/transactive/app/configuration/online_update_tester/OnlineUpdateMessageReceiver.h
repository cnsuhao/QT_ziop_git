/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/configuration/online_update_tester/OnlineUpdateMessageReceiver.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#include "core/message/src/SpecialisedMessageSubscriber.h"
#include "core/message/src/ConfigUpdateMessageSender.h"
#include "core/message/IDL/src/ConfigUpdateMessageCorbaDef.h"


class OnlineUpdateMessageReceiver : public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::ConfigUpdateMessageCorbaDef>
{
	public:

        OnlineUpdateMessageReceiver();

        virtual void receiveSpecialisedMessage(const TA_Base_Core::ConfigUpdateMessageCorbaDef& message);


};
