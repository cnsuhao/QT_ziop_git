
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "bus/security/duty_monitor_framework/src/DutyMonitorFrameworkException.h"
#include "core/message/types/DutyAgentBroadcastComms_MessageTypes.h"

using namespace TA_Base_Bus;

namespace TA_Base_App
{


	void DutyManagerBridge::loadData()
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyManagerBridge load data ..." );
		initOperatorAccessFactoryAsync( /*NULL*/  );
		initialiseSubsystemDutyGridThreadFun( /*NULL*/  );
		initSessionIds(/* NULL*/ );		
		//m_currentSessionInfo(m_sessionCache.getSessionInfo(m_sessionCache.getCurrentSessionId()));
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyManagerBridge load data over" );
	}

	void DutyManagerBridge::initialiseCurrentSession()
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"begin DutyManagerBridge::initialiseCurrentSession");
		//TA_ASSERT(m_parentWnd != NULL, "Parent window is NULL!");
		try
		{
			SessionInfoEx sessionInfoEx(m_sessionCache.getSessionInfo(m_sessionCache.getCurrentSessionId()));
			
			// TD11122: Operator/Profile information is now placed in the title bar.
			std::ostringstream titleText;
			titleText << m_originalTitleText << " - ";
			titleText << m_dataCache.getOperatorName(sessionInfoEx.OperatorId[0]) << "/"
				<< m_dataCache.getProfileName(sessionInfoEx.ProfileId[0]);

			if ( !m_dataCache.isCentralProfile(sessionInfoEx.ProfileId[0]) )
			{
				m_isCentralProfile = false;
			}
			
			TA_ASSERT( sessionInfoEx.OperatorId.size()==sessionInfoEx.ProfileId.size(), "Operator <-> Profile mismatch!" );
			
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"DutyManagerBridge::initialiseCurrentSession, sessionInfoEx.OperatorId.size() = %d",sessionInfoEx.OperatorId.size());

			// Remaining operators and profiles.
			for ( unsigned int i=1; i<sessionInfoEx.OperatorId.size(); i++ )
			{
				// There is always at least one profile at this point so we always include the comma.2
				titleText << ", " << m_dataCache.getOperatorName(sessionInfoEx.OperatorId[i]) << "/"
					<< m_dataCache.getProfileName(sessionInfoEx.ProfileId[i]);

				if ( !m_dataCache.isCentralProfile(sessionInfoEx.ProfileId[i]) )
				{
					m_isCentralProfile = false;
				}

				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,
					"DutyManagerBridge::initialiseCurrentSession, for - loop, titletext(iterator %d) : %s",
					i,titleText.str().c_str());
			}

			if ( !m_isCentralProfile )
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "[MW-151]: Using Local Profile");
				titleText<<" : "<<m_dataCache.getLocationDisplayName(sessionInfoEx.LocationId);
			}
			else
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "[MW-151]: Using Central Profile");
			}
			
			//m_parentWnd->SetWindowText( _T(titleText.str().c_str()) );
			DutyManagerEventArgs* pDmea = new DutyManagerEventArgs( DM_UPDATE_TITLE_TEXT,titleText.str() );
			notifyObservers(VIEW_DATA, pDmea);
			
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"end DutyManagerBridge::initialiseCurrentSession");
			return;
		}
		catch (const TA_Base_Bus::DutyMonitorFrameworkException& ex)
		{
			//MessageBox::error("Error while initialising current session details.", ex);
			
			std::string msg("Error while initialising current session details.");
			std::string exStr(ex.what());
			std::string details(ex.getDetails());
			DutyManagerEventArgs* pDmea = new DutyManagerEventArgs( DM_MESSAGE_BOX_ERROR,msg,exStr,details );
			notifyObservers(VIEW_DATA, pDmea);
		}
		catch (const TA_Base_Core::TransactiveException& ex)
		{
			//MessageBox::error("Error while initialising current session details.", ex);
			std::string msg("Error while initialising current session details.");
			std::string exStr(ex.what());
			std::string details("");
			DutyManagerEventArgs* pDmea = new DutyManagerEventArgs( DM_MESSAGE_BOX_ERROR,msg,exStr,details );
			notifyObservers(VIEW_DATA, pDmea);
		}
		catch (...)
		{
			//MessageBox::error("Error while initialising current session details.");
			std::string msg("Error while initialising current session details.");
			DutyManagerEventArgs* pDmea = new DutyManagerEventArgs( DM_MESSAGE_BOX_ERROR,msg );
			notifyObservers(VIEW_DATA, pDmea);
		}    

		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"end DutyManagerBridge::initialiseCurrentSession");

		//m_parentWnd->SetWindowText( _T(m_originalTitleText.c_str()) );
		DutyManagerEventArgs* pDmea = new DutyManagerEventArgs( DM_UPDATE_TITLE_TEXT,m_originalTitleText );
		notifyObservers(VIEW_DATA, pDmea);
	}

	uint16_t  DutyManagerBridge::initialiseSubsystemDutyGridThreadFun( /*LPVOID pParam */ )
	{	
		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"[CL-21875] begin DutyManagerDlg::initialiseSubsystemDutyGridThreadFun");

		//DutyManagerDlg* pDlg = reinterpret_cast <DutyManagerDlg*>( pParam );
		//ASSERT( pDlg != NULL );
		// Initialise the contents of the duty data store. Tell it to ignore all 
		// information relating to system operators and non-physical subsystems,
		// as we do not want to display these.	
		bool bStaticData = false;
		bool bDynamicData = false;
		try
		{
			//AfxGetApp()->DoWaitCursor(1);
			TA_Base_Bus::DutyDataStore::populateStaticData();			
			bStaticData = true;	
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyDataStore::populateStaticData() finish");
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyDataStore::populateStaticData() failed!");
		}
		//pDlg->PostMessage(WM_POPULATE_DATA, rtnCode, TRUE);
		OnPopulateData(bStaticData, false);

		try
		{
			//take a long time
			TA_Base_Bus::DutyDataStore::populateDynamicData(); 
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "populateDynamicData() finish");
			bDynamicData = true;	
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "populateDynamicData failed!");
		}
		OnPopulateData(false, bDynamicData);

		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"[CL-21875] end DutyManagerDlg::initialiseSubsystemDutyGridThreadFun");

		return 0;
	}

	uint16_t DutyManagerBridge::initSessionIds( /*LPVOID pParam*/ )
	{
		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"getSessionIdsAsync begin");

		try
		{
			TA_Base_Bus::DutyDataStore::getSessionCache().getSessionIds();
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"getSessionIdsAsync failed!");
		}

		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"getSessionIdsAsync end");
		return 0;
	}

	uint16_t DutyManagerBridge::initOperatorAccessFactoryAsync( /*LPVOID pParam*/  )
	{
		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"initOperatorAccessFactoryAsync begin");

		try
		{
			TA_Base_Core::OperatorAccessFactory::getInstanceNoInit().init();
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"initOperatorAccessFactoryAsync failed!");
		}

		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"initOperatorAccessFactoryAsync end");
		return 0;
	}

	void DutyManagerBridge::OnPopulateData(bool bStaticData, bool bDynamicData)
	{
		if ( true == bStaticData )
		{

		}

		if ( true == bDynamicData )
		{
			//DutyManagerBridge::getInstance()->initialiseCurrentSession();
			initialiseCurrentSession();

			// Register for subsystem duty add and remove messages
			TA_Base_Core::MessageSubscriptionManager::getInstance().subscribeToBroadcastCommsMessage(
				TA_Base_Core::DutyAgentBroadcastComms::DutyAgentRecoverMessage, this, NULL);			
		}

		PopulateDataEventArgs* pDmea = new PopulateDataEventArgs( DM_POPULATE_DATA,bStaticData,bDynamicData );
		notifyObservers(VIEW_DATA, pDmea);
	}

	std::string DutyManagerBridge::getDutyAgentRecoverMsg(int locationkey)
	{
		std::stringstream messageInfo;

		try
		{
			TA_Base_Core::ILocation* locattion = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationkey);
			messageInfo << "the DutyAgent at location "<< locattion->getName();
			messageInfo << " is just recover from broken down. If you have some operation related to that location rights. Please close the Rights manager ane reopen it.";
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "can't get location info for location key %d", locationkey);
			messageInfo << " One of the DutyAgent is just recover from broken down. If you have some operation related to the rights. Please close the Rihgts manager ane reopen it.";
		}

		return messageInfo.str();
	}

}