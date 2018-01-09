/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/AppServiceCenter.h $
* @author:  Dominique Beauregard
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* AdminManager is a singleton class that is the central class of the 
* System Manager. It retreives data from a System Controller and
* sends the GUI's orders to that System Controller.
*/

#ifndef DUTY_MANAGER_BRIDGE_H__
#define DUTY_MANAGER_BRIDGE_H__

#include "CommonDefine.h"

#include "core/message/src/NameValuePair.h"
#include "core/message/types/MessageTypes.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/process_management/IDL/src/ISystemControllerAdminCorbaDef.h"
#include "core/utilities/src/RunParams.h"
//#include "core/utilities/src/DebugUtil.h"
#include "core/threads/src/Thread.h"

#include "core/utilities/src/Matrix.h"
//#include "core/configuration_updates/src/IOnlineUpdatable.h"
//#include "core/configuration_updates/src/ConfigUpdateDetails.h"
//#include "core/data_access_interface/src/IOperator.h"
//#include "core/data_access_interface/src/IProfile.h"
//#include "core/data_access_interface/src/IActionGroup.h"
//#include "core/data_access_interface/src/IRegion.h"
//#include "core/data_access_interface/src/ISubsystem.h"
//#include "core/data_access_interface/entity_access/src/IConsole.h"
//#include "core/data_access_interface/src/OperatorAccessFactory.h"
//#include "core/data_access_interface/src/ProfileAccessFactory.h"
//#include "core/data_access_interface/src/ActionGroupAccessFactory.h"
//#include "core/data_access_interface/src/LocationAccessFactory.h"
//#include "core/data_access_interface/src/RegionAccessFactory.h"
//#include "core/data_access_interface/src/SubsystemAccessFactory.h"
//#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"

//#include "bus/security/security_utilities/src/DbCacheLookup.h"

#include "bus/security/duty_agent/idl/src/DutyAgentTypeCorbaDef.h"

#include "bus/security/duty_monitor_framework/src/DMFTypeDefs.h"

#include "bus/security/security_utilities/src/SeObserver.h"
#include "bus/security/duty_monitor_framework/src/SessionCache.h"
//#include "bus/security/duty_monitor_framework/src/DbCacheLookupEx.h"
//#include "bus/security/duty_monitor_framework/src/DataCache.h"

#include "bus/security/duty_monitor_framework/src/RegionDutyMatrixElement.h"
#include "bus/security/duty_monitor_framework/src/RegionDutyMatrix.h"
#include "bus/security/duty_monitor_framework/src/IRegionDutyObserver.h"

#include "bus/security/duty_monitor_framework/src/ISessionObserver.h"
#include "bus/security/duty_monitor_framework/src/IConfigDataObserver.h"

#include "bus/security/duty_monitor_framework/src/SubsystemDutyMatrixElement.h"
#include "bus/security/duty_monitor_framework/src/SubsystemDutyMatrix.h"
#include "bus/security/duty_monitor_framework/src/ISubsystemDutyObserver.h"

#include "bus/security/duty_monitor_framework/src/RegionDutyMatrixElement.h"
#include "bus/security/duty_monitor_framework/src/RegionDutyMatrix.h"

#include "bus/security/duty_monitor_framework/src/DefaultRegionAssignmentMatrix.h"

#include "bus/security/duty_monitor_framework/src/DutyAgentAccess.h"
#include "bus/security/duty_monitor_framework/src/DutyRights.h"
#include "bus/security/duty_monitor_framework/src/DutyDataStore.h"

#include "bus/generic_gui_pmod/src/PModelInterface.h"



using namespace TA_Base_Core;
using namespace TA_Base_Bus;

namespace TA_Base_App
{
	class DutyManagerBridge: 
		public TA_Base_Bus::IConfigDataObserver,
		public TA_Base_Bus::IRegionDutyObserver,
		public TA_Base_Bus::ISubsystemDutyObserver,	
		public TA_Base_Bus::ISessionObserver,					
		public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>,		
		public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>,
		public TA_Base_Core::Thread
	{
	public:
		DutyManagerBridge();
		virtual ~DutyManagerBridge();
		static DutyManagerBridge* getInstance();

		void initialiseCurrentSession();
		void setTitleText( const std::string & originalTitleText );

		TA_Base_Bus::ActionGroupKey getActionGroupDB(
			const TA_Base_Bus::ProfileKey profileKey,
			const TA_Base_Bus::SubsystemKey subsystemKey,
			const TA_Base_Bus::ESubsystemState subsystemState);
		void getTransferableRegionList(TransferableRegionList& trList);



		TA_Base_Bus::DataCache&				getDataCache();	
		TA_Base_Bus::SessionCache&			getSessionCashe();
		TA_Base_Bus::RegionDutyMatrix&		getRegionDutyMatrix();
		TA_Base_Bus::SubsystemDutyMatrix&	getSubsystemDutyMatrix();
		TA_Base_Bus::SessionInfoEx			getCurrentSessionInfo();
		TA_Base_Bus::ProfileKeyList			getPreviousProfileId();


		// HelpLauncher
		void OnAppAbout();
		void OnTransActiveHelp();

		// SessionCache
		SessionId getCurrentSessionId();

		// DataCache
		std::string getRegionDisplayName(RegionKey key);
		std::string getRegionName(RegionKey key);

		std::string getOperatorDescription(OperatorKey key);
		std::string getProfileName(ProfileKey key);
		
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

	public:
		// IConfigDataObserver 
		virtual void dataChanged(const TA_Base_Bus::DataUpdate& dataUpdate);

		// IRegionDutyObserver
		virtual void dutyChanged(const RegionDutyUpdate& dutyUpdate);

		// ISubsystemDutyObserver OtherSesSubDutyPropertyPageModel
		virtual void dutyChanged(const TA_Base_Bus::SubsystemDutyUpdate& dutyUpdate);	

        // ISessionObserver  OtherSesSubDutyPropertyPageModel
        virtual void sessionChanged(const TA_Base_Bus::SessionUpdate& sessionUpdate);

			
		
		// TA_Base_Core::SpecialisedMessageSubscriber 
		virtual void receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message);

		// thread
		void run();
		void terminate();

	protected:

		void loadData();

		// init data cache 
		uint16_t initialiseSubsystemDutyGridThreadFun( /*void* pParam*/  );
		uint16_t initSessionIds( /*void* pParam*/ );
		uint16_t initOperatorAccessFactoryAsync( /*void* pParam */ );

		void OnPopulateData(bool bStaticData, bool bDynamicData);

	private:
		 DutyManagerBridge(DutyManagerBridge &);
		 DutyManagerBridge& operator = (DutyManagerBridge &);
		
		 void OnReceiveSessionUpdate(const TA_Base_Bus::SessionUpdate& sessionUpdate);
		 std::string getDutyAgentRecoverMsg(int locationkey);

	private:

		static DutyManagerBridge* s_instance;
		static TA_Base_Core::NonReEntrantThreadLockable m_singletonLock;

		//DutyManagerDlgModel * m_parentWnd;
		TA_Base_Bus::DutyAgentAccess		m_dutyAgent;
		TA_Base_Bus::DutyRights				m_dutyRights;

        TA_Base_Bus::DataCache&				m_dataCache;
        TA_Base_Bus::SessionCache&			m_sessionCache;
		TA_Base_Bus::RegionDutyMatrix&		m_regionDutyMatrix;
		TA_Base_Bus::SubsystemDutyMatrix&	m_subsystemDutyMatrix;

		std::vector<TA_Base_App::PSSA>		m_actionGroupsDB;

		TA_Base_Bus::SessionInfoEx			m_currentSessionInfo;
		TA_Base_Bus::ProfileKeyList			m_previousProfileId;
		//TA_Base_Bus::SessionInfoEx			m_selectSessionInfo;
		//TA_Base_Bus::SessionId				m_selectSessionId;

		std::string m_originalTitleText;

		bool m_isCentralProfile;
	};
}
#endif // DUTY_MANAGER_BRIDGE_H__
