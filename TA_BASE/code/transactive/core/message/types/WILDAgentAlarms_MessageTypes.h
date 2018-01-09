// File: WILDAgentAlarms_MessageTypes.h
// Created: 12/04/2006 4:13:35 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef WILDAGENTALARMS_MESSAGETYPES_H
#define WILDAGENTALARMS_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace WILDAgentAlarms
	{
		// Root context
		static MessageContext Context("Alarm", "WILDAgentAlarms", AlarmMessage);

		// Message Types for context WILDAgentAlarms
		static const MessageType CorrelationAlarm(Context, "50123");
		static const MessageType CriticalAlarm(Context, "50124");
		static const MessageType InvalidWILDConfiguration(Context, "50125");
		static const MessageType NonRevenueTrainAlarm(Context, "50126");
		static const MessageType WheelAlarm(Context, "50127");
		static const MessageType WILDError(Context, "50128");
	};
};

#endif // WILDAGENTALARMS_MESSAGETYPES_H
