// File: DataPointAlarms_MessageTypes.h
// Created: 12/04/2006 4:13:30 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef DATAPOINTALARMS_MESSAGETYPES_H
#define DATAPOINTALARMS_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace DataPointAlarms
	{
		// Root context
		static MessageContext Context("Alarm", "DataPointAlarms", AlarmMessage);

		// Message Types for context DataPointAlarms
		static const MessageType DpFalse(Context, "50008");
		static const MessageType DpGeneral(Context, "50009");
		static const MessageType DpTrue(Context, "50010");
		static const MessageType HI(Context, "50011");
		static const MessageType LO(Context, "50012");
		static const MessageType OH(Context, "50013");
		static const MessageType OL(Context, "50014");
		static const MessageType VH(Context, "50015");
		static const MessageType VL(Context, "50016");
	};
};

#endif // DATAPOINTALARMS_MESSAGETYPES_H
