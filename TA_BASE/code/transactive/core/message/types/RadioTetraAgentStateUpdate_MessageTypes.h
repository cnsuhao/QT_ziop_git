// File: RadioTetraAgentStateUpdate_MessageTypes.h
// Created: 12/04/2006 4:13:33 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef RADIOTETRAAGENTSTATEUPDATE_MESSAGETYPES_H
#define RADIOTETRAAGENTSTATEUPDATE_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace RadioTetraAgentStateUpdate
	{
		// Root context
		static MessageContext Context("StateUpdate", "RadioTetraAgentStateUpdate", StateUpdateMessage);

		// Message Types for context RadioTetraAgentStateUpdate
		static const MessageType RadioRemoveAuthorisationRequest(Context, "10060");
		static const MessageType RadioRemoveCallFromRadioCallStack(Context, "10061");
		static const MessageType RadioRemoveMonitoredSubscriberDetails(Context, "10062");
		static const MessageType RadioRemoveSessionFromAudioMap(Context, "10063");
		static const MessageType RadioUpdateAudioEntryInSessionAudioMap(Context, "10064");
		static const MessageType RadioUpdateAuthorisationRequest(Context, "10065");
		static const MessageType RadioUpdateCallInRadioCallStack(Context, "10066");
		static const MessageType RadioUpdateMonitoredCallProgression(Context, "10067");
		static const MessageType RadioUpdateRadioResourceManagerVars(Context, "10068");
		static const MessageType RadioUpdateRadioSession(Context, "10069");
		static const MessageType RadioConnectionSwitchRequest(Context, "10101");
	};
};

#endif // RADIOTETRAAGENTSTATEUPDATE_MESSAGETYPES_H