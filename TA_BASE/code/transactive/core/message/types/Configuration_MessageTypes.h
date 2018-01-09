// File: Configuration_MessageTypes.h
// Created: 12/04/2006 4:13:30 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef CONFIGURATION_MESSAGETYPES_H
#define CONFIGURATION_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace Configuration
	{
		// Root context
		static MessageContext Context("Audit", "Configuration", AuditMessage);

		// Message Types for context Configuration
		static const MessageType ConfigurationItemAdded(Context, "30052");
		static const MessageType ConfigurationItemDeleted(Context, "30053");
		static const MessageType ConfigurationItemUpdated(Context, "30054");
	};
};

#endif // CONFIGURATION_MESSAGETYPES_H
