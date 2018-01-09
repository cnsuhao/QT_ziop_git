/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerAppController.cpp $
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

#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerPropertySheetModel.h"
#include "app/security/DutyManager/pmodel/src/OtherSesSubDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/RegionDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/RegionDutyTransferDlgModel.h"
#include "app/security/DutyManager/pmodel/src/RegionSelectionDlgModel.h"
#include "app/security/DutyManager/pmodel/src/SelectSesSubDutyDlgModel.h"
#include "app/security/DutyManager/pmodel/src/SubsystemDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/SubsystemDutyTransferDlgModel.h"

#include "bus/application_types/src/apptypes.h"

#include "core/data_access_interface/entity_access/src/DutyManagerEntityData.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

using namespace TA_Base_App;


DutyManagerAppController::DutyManagerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName) : 
           TA_Base_Bus::AbstractGUIAppController(pApp, appName)
{
    FUNCTION_ENTRY("DutyManagerAppController()");
	m_pDmBridge = NULL;
    FUNCTION_EXIT;
}


DutyManagerAppController::~DutyManagerAppController()
{
    LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerAppController()");
}

unsigned long DutyManagerAppController::getApplicationType()
{
	return DUTY_MANAGER_GUI_APPTYPE;
}

void DutyManagerAppController::setTitleText(const std::string & title)
{
	 DutyManagerBridge::getInstance()->setTitleText(title);
}

void DutyManagerAppController::loadData()
{
	DutyManagerBridge::getInstance()->start();
}

void DutyManagerAppController::allocAppResource()
{
	// init Business
	m_pDmBridge = DutyManagerBridge::getInstance();
	//m_AppServiceCenter = AppServiceCenter::getInstance();
}

void DutyManagerAppController::cleanup()
{

	if (m_pDmBridge != NULL)
	{
		delete m_pDmBridge;
		m_pDmBridge = NULL;
	}

	LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyManagerAppController.cleanup() over");

	//if (m_AppServiceCenter != NULL)
	//{
	//	delete m_AppServiceCenter;
	//	m_AppServiceCenter = NULL;
	//}
}


TA_Base_Bus::ITransActivePModel* DutyManagerAppController::createPModel( const std::string & pModelName )
{
	ITransActivePModel * pModel = NULL;
	if (pModelName == PMODEL_DUTY_MANAGER_DLG)
	{
		pModel = new DutyManagerDlgModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_DUTY_MANAGER_PROPERTY_SHEET)
	{
		pModel = new DutyManagerPropertySheetModel( this, pModelName.c_str() );
	}//page
	else if (pModelName == PMODEL_REGION_DUTY_PROPERTY_PAGE)
	{
		pModel = new RegionDutyPropertyPageModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_SUBSYSTEM_DUTY_PROPERTY_PAGE)
	{
		pModel = new SubsystemDutyPropertyPageModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_OTHER_SES_SUB_DUTY_PROPERTY_PAGE)
	{
		pModel = new OtherSesSubDutyPropertyPageModel( this, pModelName.c_str() );
	}// dlg	
	else if (pModelName == PMODEL_REGION_DUTY_TRANSFER_DLG)
	{
		pModel = new RegionDutyTransferDlgModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_SUBSYSTEM_DUTY_TRANSFER_DLG)
	{
		pModel = new SubsystemDutyTransferDlgModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_REGION_SELECTION_DLG)
	{
		pModel = new RegionSelectionDlgModel( this, pModelName.c_str() );
	}
	else if (pModelName == PMODEL_SELECT_SE_SUB_DUTY_DLG)
	{
		pModel = new SelectSesSubDutyDlgModel( this, pModelName.c_str() );
	}

	

	return pModel;
}

//raise exception necessary after catch and log 
void DutyManagerAppController::processWorkflowDuringAppInit()
{
	setTitleText(DUTY_MANAGER);
	//bool hasConnectedControlStation = isConnectedControlStation();
	//if (! hasConnectedControlStation)
	//{
	//	getRunParamFromSystemController();
	//	doLoginOperation();
	//}
}

void DutyManagerAppController::checkEntity( TA_Base_Core::IEntityData* guiEntity )
{
	LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "DutyManagerAppController check GUI Entity");

	TA_Base_Core::DutyManagerEntityData* dmEntityData = dynamic_cast<TA_Base_Core::DutyManagerEntityData*>(guiEntity);
	TA_ASSERT(guiEntity  != NULL, "the GUI entity should not be NULL! ");
	// Store away Rights Manager parameters for future reference.
	if (dmEntityData->useLocationLabel())
	{
		TA_Base_Core::RunParams::getInstance().set(RPARAM_USELOCATIONLABEL, "enable"); 
	}
	if (!dmEntityData->getNormalDutyColour().empty())
	{
		TA_Base_Core::RunParams::getInstance().set( RPARAM_NORMALDUTYCOLOUR, dmEntityData->getNormalDutyColour().c_str() );
	}
	if (!dmEntityData->getDelegatedDutyColour().empty())
	{
		TA_Base_Core::RunParams::getInstance().set( RPARAM_DELEGATEDDUTYCOLOUR, dmEntityData->getDelegatedDutyColour().c_str() );
	}
	if (!dmEntityData->getDegradedDutyColour().empty())
	{
		TA_Base_Core::RunParams::getInstance().set( RPARAM_DEGRADEDDUTYCOLOUR, dmEntityData->getDegradedDutyColour().c_str() );
	}
	if (!dmEntityData->getUnallocatedSubsystemColour().empty())
	{
		TA_Base_Core::RunParams::getInstance().set( RPARAM_UNALLOCATEDSUBSYSTEMCOLOUR, dmEntityData->getUnallocatedSubsystemColour().c_str() );
	}

	// Entity config OK
	//setGuiEntity(*guiEntity);-----------------


	AbstractGUIAppController::checkEntity( guiEntity );
}

//void DutyManagerAppController::invoke(const char * methodName, const NameValueParams & params)  
//{
//
//}

void DutyManagerAppController::OnAppAbout()
{
	if ( NULL != m_pDmBridge)
	{
		m_pDmBridge->OnAppAbout();
	}
	else
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "DutyManagerAppController::OnAppAbout(): m_AppServiceCenter==NULL.")

	}
	
}

void DutyManagerAppController::OnTransActiveHelp()
{
	if ( NULL != m_pDmBridge)
	{
		m_pDmBridge->OnTransActiveHelp();
	}
	else
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "DutyManagerAppController::OnTransActiveHelp(): m_AppServiceCenter==NULL.")

	}
}

//void DutyManagerAppController::sessionChanged(const TA_Base_Bus::SessionUpdate& sessionUpdate)
//{
//	// Let the thread that created the window process the update.
//	//HWND wndHandle(GetSafeHwnd());
//
//	//if (wndHandle != NULL)
//	//{
//		// Post the message so as not to hold up the notifying thread.
//		// Clone the session update and throw onto the heap to make sure 
//		// that by the time the message is processed we still have a valid
//		// object. The message handler takes care of destroying it once consumed.
//		::PostMessage(wndHandle, WM_USER_RECEIVE_SESSION_UPDATE, 
//			reinterpret_cast<WPARAM>(new SessionUpdate(sessionUpdate)), 0);
//	//}
//}
//
//void DutyManagerAppController::dutyChanged(const TA_Base_Bus::SubsystemDutyUpdate& dutyUpdate)
//{
//	// Let the thread that created the window process the update.
//	HWND wndHandle(GetSafeHwnd());
//
//	if (wndHandle != NULL)
//	{
//		// Post the message so as not to hold up the notifying thread.
//		// Clone the duty update and throw onto the heap to make sure 
//		// that by the time the message is processed we still have a valid
//		// object. The message handler takes care of destroying it once consumed.
//		::PostMessage(wndHandle, WM_USER_RECEIVE_SUB_DUTY_UPDATE, 
//			reinterpret_cast<WPARAM>(new SubsystemDutyUpdate(dutyUpdate)), 0);
//	}
//}



// get
TA_Base_Bus::DataCache&	DutyManagerAppController::getDataCache()
{	
	return m_pDmBridge->getDataCache();
}
TA_Base_Bus::SessionCache&	DutyManagerAppController::getSessionCashe()
{
	return m_pDmBridge->getSessionCashe();
}
TA_Base_Bus::RegionDutyMatrix& DutyManagerAppController::getRegionDutyMatrix()
{
	return m_pDmBridge->getRegionDutyMatrix();
}
TA_Base_Bus::SubsystemDutyMatrix& DutyManagerAppController::getSubsystemDutyMatrix()
{
	return m_pDmBridge->getSubsystemDutyMatrix();
}



TA_Base_Bus::SessionInfoEx DutyManagerAppController::getCurrentSessionInfo()
{
	return m_pDmBridge->getCurrentSessionInfo();
}

TA_Base_Bus::ProfileKeyList	DutyManagerAppController::getPreviousProfileId()
{
	return m_pDmBridge->getPreviousProfileId();
}

//RunParams
bool DutyManagerAppController::isSetParam(const char* name)
{
	return TA_Base_Core::RunParams::getInstance().isSet(name);
}

std::string DutyManagerAppController::getParam(const char* name)
{
	return TA_Base_Core::RunParams::getInstance().get( name );
}

//SessionCache
SessionId DutyManagerAppController::getCurrentSessionId()
{
	return m_pDmBridge->getCurrentSessionId();
}

std::string DutyManagerAppController::getRegionDisplayName(RegionKey key)
{
	return m_pDmBridge->getRegionDisplayName(key);
}

std::string DutyManagerAppController::getRegionName(RegionKey key)
{
	return m_pDmBridge->getRegionName(key);
}

std::string DutyManagerAppController::getLocationNameByLocationKey(const unsigned long locationKey)
{
	return m_pDmBridge->getLocationNameByLocationKey(locationKey);
}

// DutyRighst
bool DutyManagerAppController::canTransferRegions()
{
	return m_pDmBridge->canTransferRegions();
}
bool DutyManagerAppController::canTransferSubsystems(long lActionId)
{
	return m_pDmBridge->canTransferSubsystems(lActionId);
}
bool DutyManagerAppController::canDegradeSubsystems()
{
	return m_pDmBridge->canDegradeSubsystems();
}
// DutyAgent
void DutyManagerAppController::degradeAllSubsystems()
{
	m_pDmBridge->degradeAllSubsystems();
}

void DutyManagerAppController::transferAllSubsystemDuty()
{
	m_pDmBridge->transferAllSubsystemDuty();
}
void DutyManagerAppController::transferRegionDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const RegionKeyList& regionKeys)
{
	m_pDmBridge->transferRegionDuty(sessionInfo, regionKeys);
}
void DutyManagerAppController::transferSubsystemDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const SubsystemTreeMap& subsystems)
{
	m_pDmBridge->transferSubsystemDuty(sessionInfo, subsystems);
}

TransferableRegionList DutyManagerAppController::getTransferableRegions(const SessionInfo& session)
{
	return m_pDmBridge->getTransferableRegions(session);
}

TransferableSubsystemList DutyManagerAppController::getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys)
{
	return m_pDmBridge->getTransferableSubsystems(session,  regionKeys);
}

TA_Base_Bus::ActionGroupKey DutyManagerAppController::getActionGroupDB(
	const TA_Base_Bus::ProfileKey profileKey,
	const TA_Base_Bus::SubsystemKey subsystemKey,
	const TA_Base_Bus::ESubsystemState subsystemState)
{
	return m_pDmBridge->getActionGroupDB(profileKey,subsystemKey,subsystemState);
}






