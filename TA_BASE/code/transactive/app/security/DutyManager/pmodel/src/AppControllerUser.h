#ifndef DUTY_MANAGER_APP_CONTROLLER_USER_H__
#define DUTY_MANAGER_APP_CONTROLLER_USER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"



/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerAppController.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class provides the entry point into the application.
  * It instantiates DutyManagerGUI, but leaves it to the
  * GenericGUI to display.
  *
  */

namespace TA_Base_App
{
    class AppControllerUser 
    {
    public:
        AppControllerUser(TA_Base_Bus::IGUIApplicationController* appController);
        ~AppControllerUser();

		// PropertyPageModel
		TA_Base_Bus::DataCache&				getDataCache();	
		TA_Base_Bus::SessionCache&			getSessionCashe();
		TA_Base_Bus::RegionDutyMatrix&		getRegionDutyMatrix();
		TA_Base_Bus::SubsystemDutyMatrix&	getSubsystemDutyMatrix();		
		TA_Base_Bus::ProfileKeyList			getPreviousProfileId();

		//RunParams
		bool								isUseLocationLabel();
		bool								isShowSessionLocation();
		bool								isShowOthereSessioin();
		std::string							getProjectName();
		std::string							getNormalDutyColour();
		std::string							getDelegatedDutyColour();
		std::string							getDegradedDutyColour();
		std::string							getUnallocatedSubsystemColour();

		// SessionCache
		SessionId							getCurrentSessionId();
		SessionInfoEx						getSessionInfo(const SessionId& sessionId);
		SessionInfoEx						getCurrentSessionInfo();

		// DataCache
		std::string							getRegionDisplayName(RegionKey key);
		std::string							getRegionName(RegionKey key);
		std::string							getSubsystemName(SubsystemKey key);
		bool								getSessionInfoName(SessionInfoEx& sessionInfo,std::string& operatorName,std::string& profileName,std::string& locationName,std::string& workstationName);
		bool								getRegionInfoName(const DutyAgentTypeCorbaDef::TransferableRegion& regionInfo,std::string& regionName,std::string& operatorName,std::string& profileName);
		std::string							getWorkstationName(unsigned long WorkstationId);
		std::string							getProfileName(ProfileKey key);
		std::string							getOperatorDescription(OperatorKey key);

		// LocationAccessFactory
		std::string							getLocationNameByLocationKey(const unsigned long locationKey);

		// DutyRighst
		bool								canTransferRegions();
		bool								canTransferSubsystems(long lActionId);
		bool								canDegradeSubsystems();

		// DutyAgent
		void								degradeAllSubsystems();
		void								transferAllSubsystemDuty();
		void								transferRegionDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const RegionKeyList& regionKeys);
		void								transferSubsystemDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const SubsystemTreeMap& subsystems);
		TransferableRegionList				getTransferableRegions(const SessionInfo& session);
		TransferableSubsystemList			getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys);
		
		TA_Base_Bus::ActionGroupKey			getActionGroupDB(
			const TA_Base_Bus::ProfileKey profileKey,
			const TA_Base_Bus::SubsystemKey subsystemKey,
			const TA_Base_Bus::ESubsystemState subsystemState);

	private:
		//RunParams
		bool								isSetParam(const char* name);
		std::string							getParam(const char* name);

    protected:
		DutyManagerAppController* m_pDmAppController;
		
    };

} // namespace TA_Base_App

#endif // DUTY_MANAGER_APP_CONTROLLER_USER_H__
