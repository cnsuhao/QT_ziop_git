// File: DataRecordingAgentAlarms_MessageTypes.h
// Created: 12/04/2006 4:13:31 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef DATARECORDINGAGENTALARMS_MESSAGETYPES_H
#define DATARECORDINGAGENTALARMS_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace DataRecordingAgentAlarms
	{
		// Root context
		static MessageContext Context("Alarm", "DataRecordingAgentAlarms", AlarmMessage);

		// Message Types for context DataRecordingAgentAlarms
		static const MessageType MaxTrendsPerLocationReached(Context, "50168");
		static const MessageType WriteToDatabaseFailure(Context, "50169");
	};
};

#endif // DATARECORDINGAGENTALARMS_MESSAGETYPES_H
