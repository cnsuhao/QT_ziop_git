// File: ATSAGENTSTATEUPDATE_MESSAGETYPES_H.h
// Created: 12/04/2006 4:13:33 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef ATSAGENTSTATEUPDATE_MESSAGETYPES_H
#define ATSAGENTSTATEUPDATE_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace AtsAgentStateUpdate
	{
		// Root context
		static MessageContext Context("StateUpdate", "AtsAgentStateUpdate", StateUpdateMessage);

		// Message Types for context AtsAgentStateUpdate
		static const MessageType AtsTrainStateUpdate(Context, "10099");

	};
};

#endif // ATSAGENTSTATEUPDATE_MESSAGETYPES_H