// File: TrainAudit_MessageTypes.h
// Created: 12/04/2006 4:13:34 PM
// This file is automatically generated from 9999D02170001-TransActive_CORBA_Messages_Register.xls
// Reference: 9999-D02-17-0001, Version: 34.06, Status: Draft, Dated: 06 Apr 06
//
// * DO NOT MODIFY! *

#ifndef TRAINAUDIT_MESSAGETYPES_H
#define TRAINAUDIT_MESSAGETYPES_H

#include "MessageTypes.h"

namespace TA_Base_Core
{
	namespace TrainAudit
	{
		// Root context
		static MessageContext Context("Audit", "TrainAudit", AuditMessage);

		// Message Types for context TrainAudit
		static const MessageType TrainCallDriverEndCall(Context, "30485");
		static const MessageType TrainCallDriverSetupCall(Context, "30486");
		static const MessageType TrainCCTVCarrier(Context, "30487");
		static const MessageType TrainCCTVScreen(Context, "30488");
		static const MessageType TrainInfoDeleteSelection(Context, "30489");
		static const MessageType TrainInfoSaveSelection(Context, "30490");
		static const MessageType TrainPaLiveBroadcastBegin(Context, "30491");
		static const MessageType TrainPaLiveBroadcastEnd(Context, "30492");
		static const MessageType TrainPaPidsPrerecordedMessage(Context, "30493");
		static const MessageType TrainPECActivated(Context, "30494");
		static const MessageType TrainPECAllReset(Context, "30495");
		static const MessageType TrainPECAnswerCall(Context, "30496");
		static const MessageType TrainPECCallInvalid(Context, "30497");
		static const MessageType TrainPECCallNotAnswered(Context, "30498");
		static const MessageType TrainPECCCTVActivate(Context, "30499");
		static const MessageType TrainPECContinueCall(Context, "30500");
		static const MessageType TrainPECEndCall(Context, "30501");
		static const MessageType TrainPECResetByTIMS(Context, "30502");
		static const MessageType TrainPECResetCall(Context, "30503");
		static const MessageType TrainPECResetWithoutAnswer(Context, "30504");
		static const MessageType TrainPidsFreeTextDisplay(Context, "30505");
		static const MessageType TrainTimeoutAuditEvent(Context, "30506");
		static const MessageType TrainTISFreeTextMessage(Context, "30507");
		static const MessageType TrainTISLibraryDownload(Context, "30508");
		static const MessageType TrainTISLibraryUpgrade(Context, "30509");
		static const MessageType TrainTISResetMessage(Context, "30510");
		static const MessageType TrainTISScheduleDownload(Context, "30511");
		static const MessageType TrainTISScheduleUpgrade(Context, "30512");
		static const MessageType TrainTISStoredMessage(Context, "30513");
		static const MessageType TrainCCTVConflict(Context, "30563");
		static const MessageType TrainTrnChanged(Context, "30606");
		static const MessageType TrainTpisMessage(Context, "30607");
		static const MessageType TrainPaLiveAnnouncementSetup(Context, "30608");
		static const MessageType TrainPaLiveAnnouncementBegin(Context, "30609");
		static const MessageType TrainPaLiveAnnouncementEnd(Context, "30610");
		static const MessageType TrainIncomingCallRequest(Context, "30611");
		static const MessageType TrainAnswerCallRequest(Context, "30612");
		static const MessageType TrainEndOfOccCall(Context, "30613");
		static const MessageType TrainEndOfTrainCall(Context, "30614");
		static const MessageType TrainCallTrain(Context, "30615");
		static const MessageType TrainPicuActivated(Context, "30616");
		static const MessageType TrainPicuAnswered(Context, "30617");
		static const MessageType TrainPicuReset(Context, "30618");
		static const MessageType OperatorTakeTrainControl(Context, "30627");
		static const MessageType TrainPECHoldCall(Context,"30647");
	};
};

#endif // TRAINAUDIT_MESSAGETYPES_H