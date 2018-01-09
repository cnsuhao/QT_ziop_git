#ifndef DUTY_MANAGER_APP_CONTROLLER_H__
#define DUTY_MANAGER_APP_CONTROLLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"



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
    class DutyManagerAppController : public TA_Base_Bus::AbstractGUIAppController
    {
    public:
        DutyManagerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName);
        ~DutyManagerAppController();
		void setTitleText(const std::string & title);
		void loadData();

		// need
		void cleanup();
		void allocAppResource();			
		virtual unsigned long getApplicationType();
		virtual TA_Base_Bus::ITransActivePModel * createPModel(const std::string & pModelName);


		// maybe
		void processWorkflowDuringAppInit();
		//virtual void invoke(const char * methodName, const NameValueParams & params) ;

		// override
		void checkEntity( TA_Base_Core::IEntityData* guiEntity );

		// DutyManagerDlgModel
		void OnAppAbout();
		void OnTransActiveHelp();

		// PropertyPageModel
		TA_Base_Bus::DataCache&				getDataCache();	
		TA_Base_Bus::SessionCache&			getSessionCashe();
		TA_Base_Bus::RegionDutyMatrix&		getRegionDutyMatrix();
		TA_Base_Bus::SubsystemDutyMatrix&	getSubsystemDutyMatrix();
		TA_Base_Bus::SessionInfoEx			getCurrentSessionInfo();

		TA_Base_Bus::ProfileKeyList	getPreviousProfileId();

		//RunParams
		bool isSetParam(const char* name);
		std::string getParam(const char* name);

		// SessionCache
		SessionId getCurrentSessionId();

		// DataCache

		std::string getRegionDisplayName(RegionKey key);
		std::string getRegionName(RegionKey key);

		// LocationAccessFactory
		std::string getLocationNameByLocationKey(const unsigned long locationKey);

		// DutyRighst
		bool canTransferRegions();
		bool canTransferSubsystems(long lActionId);
		bool canDegradeSubsystems();
		// DutyAgent
		void degradeAllSubsystems();
		void transferAllSubsystemDuty();
		void transferRegionDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const RegionKeyList& regionKeys);
		void transferSubsystemDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const SubsystemTreeMap& subsystems);
		TransferableRegionList getTransferableRegions(const SessionInfo& session);
		TransferableSubsystemList getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys);
		
		TA_Base_Bus::ActionGroupKey getActionGroupDB(
			const TA_Base_Bus::ProfileKey profileKey,
			const TA_Base_Bus::SubsystemKey subsystemKey,
			const TA_Base_Bus::ESubsystemState subsystemState);
    private:
		DutyManagerBridge*	m_pDmBridge;
		//AppServiceCenter*	m_AppServiceCenter;
		//TA_Base_Bus::DutyAgentAccess m_dutyAgent;
		//TA_Base_Bus::DutyRights m_dutyRights;
		
    };

} // namespace TA_Base_App

#endif // DUTY_MANAGER_APP_CONTROLLER_H__
