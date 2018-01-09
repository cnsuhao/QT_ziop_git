// File: PlanAgentAlarms_MessageTypes.h
// Created: 12/04/2006 4:13:33 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef PLANAGENTALARMS_MESSAGETYPES_H
#define PLANAGENTALARMS_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace PlanAgentAlarms
	{
		// Root context
		static MessageContext Context("Alarm", "PlanAgentAlarms", AlarmMessage);

		// Message Types for context PlanAgentAlarms
		static const MessageType PlanControlReleaseFailure(Context, "50032");
		static const MessageType PlanDatabaseContactFailure(Context, "50034");
		static const MessageType PlanDatabaseQueryFailure(Context, "50035");
		static const MessageType PlanDatabaseUpdateFailure(Context, "50036");
		static const MessageType PlanExecutionConflict(Context, "50038");
		static const MessageType PlanStepAdditionFailure(Context, "50041");
		static const MessageType PlanStepDeletionFailure(Context, "50042");
		static const MessageType PlanStepDetailsSaveFailure(Context, "50043");
		static const MessageType PlanStepExecutionFailure(Context, "50044");
		static const MessageType PlanStepAlarm(Context, "50224");
	};
};

#endif // PLANAGENTALARMS_MESSAGETYPES_H
