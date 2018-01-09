// File: ECSAgentAudit_MessageTypes.h
// Created: 12/04/2006 4:13:31 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef ECSAGENTAUDIT_MESSAGETYPES_H
#define ECSAGENTAUDIT_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace ECSAgentAudit
	{
		// Root context
		static MessageContext Context("Audit", "ECSAgentAudit", AuditMessage);

		// Message Types for context ECSAgentAudit
		static const MessageType EcsAgentExecutionState(Context, "30116");
		static const MessageType EcsAgentOnlineUpdate(Context, "30117");
		static const MessageType EcsMasterModeRequest(Context, "30118");
		static const MessageType EcsMasterModeResult(Context, "30119");
		static const MessageType EcsStationModeRequest(Context, "30120");
		static const MessageType EcsStationModeResult(Context, "30121");
	};
};

#endif // ECSAGENTAUDIT_MESSAGETYPES_H
