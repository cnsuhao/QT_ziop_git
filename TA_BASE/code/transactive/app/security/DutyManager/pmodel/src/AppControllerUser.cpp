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

#include "app/security/DutyManager/pmodel/src/AppControllerUser.h"

namespace TA_Base_App
{

AppControllerUser::AppControllerUser(IGUIApplicationController* appController) :
m_pDmAppController(NULL)
{
    FUNCTION_ENTRY("AppControllerUser()");
	m_pDmAppController =  dynamic_cast<DutyManagerAppController*> (appController);
    FUNCTION_EXIT;
}


AppControllerUser::~AppControllerUser()
{
    FUNCTION_ENTRY("~AppControllerUser()");

    FUNCTION_EXIT;
}

// PropertyPageModel
TA_Base_Bus::DataCache&				AppControllerUser::getDataCache()
{
	return m_pDmAppController->getDataCache();
}
TA_Base_Bus::SessionCache&			AppControllerUser::getSessionCashe()
{
	return m_pDmAppController->getSessionCashe();
}
TA_Base_Bus::RegionDutyMatrix&		AppControllerUser::getRegionDutyMatrix()
{
	return m_pDmAppController->getRegionDutyMatrix();
}
TA_Base_Bus::SubsystemDutyMatrix&	AppControllerUser::getSubsystemDutyMatrix()
{
	return m_pDmAppController->getSubsystemDutyMatrix();
}

TA_Base_Bus::ProfileKeyList		AppControllerUser::getPreviousProfileId()
{
	return m_pDmAppController->getPreviousProfileId();
}

//RunParams
bool AppControllerUser::isSetParam(const char* name)
{
	return m_pDmAppController->isSetParam( name);
}

std::string AppControllerUser::getParam(const char* name)
{
	return m_pDmAppController->getParam( name);
}

bool AppControllerUser::isUseLocationLabel()
{
	return isSetParam(RPARAM_USELOCATIONLABEL);
}
bool AppControllerUser::isShowSessionLocation()
{
	bool returnValue = false;
	std::string showSessionlocation = getParam(RPARAM_SHOWSESSIONWORKSTATION);
	if ( showSessionlocation.compare("OPEN") == 0 )
	{
		returnValue = true;
	}
	return returnValue;
}
bool AppControllerUser::isShowOthereSessioin()
{
	bool returnValue = false;
	std::string showSessionlocation = getParam(RPARAM_SHOWOTHERSES);
	if ( showSessionlocation.compare("OPEN") == 0 )
	{
		returnValue = true;
	}
	return returnValue;
}
std::string	AppControllerUser::getProjectName()
{
	return getParam( RPARAM_PROJECTNAME );
}
std::string	AppControllerUser::getNormalDutyColour()
{
	return getParam( RPARAM_NORMALDUTYCOLOUR );
}
std::string	AppControllerUser::getDelegatedDutyColour()
{
	return getParam( RPARAM_NORMALDUTYCOLOUR );
}
std::string	AppControllerUser::getDegradedDutyColour()
{
	return getParam( RPARAM_DELEGATEDDUTYCOLOUR );
}
std::string	AppControllerUser::getUnallocatedSubsystemColour()
{
	return getParam( RPARAM_UNALLOCATEDSUBSYSTEMCOLOUR );
}

// SessionCache
SessionId AppControllerUser::getCurrentSessionId()
{
	return m_pDmAppController->getCurrentSessionId();
}

SessionInfoEx AppControllerUser::getSessionInfo(const SessionId& sessionId)
{
	return getSessionCashe().getSessionInfo( sessionId );
}

//SessionInfoEx	AppControllerUser::getCurrentSessionInfo()
//{
//	return getSessionInfo( getCurrentSessionId() );
//}

SessionInfoEx	AppControllerUser::getCurrentSessionInfo()
{
	return m_pDmAppController->getCurrentSessionInfo();
}

// DataCache
std::string	AppControllerUser::getRegionDisplayName(RegionKey key)
{
	return m_pDmAppController->getRegionDisplayName( key);
}
std::string	AppControllerUser::getRegionName(RegionKey key)
{
	return m_pDmAppController->getRegionName( key);
}

std::string	AppControllerUser::getSubsystemName(SubsystemKey key)
{
	return getDataCache().getSubsystemName(key);
}

bool AppControllerUser::getSessionInfoName(SessionInfoEx& sessionInfo,std::string& operatorName,std::string& profileName,std::string& locationName,std::string& workstationName)
{
	bool returnValue = false;
	if ( (sessionInfo.SessionId == "")
		|| (sessionInfo.OperatorId.empty()) 
		|| (sessionInfo.ProfileId.empty()) )
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "[CL-21882] OperatorId is empty, or ProfileId is empty.");
		returnValue = false;
	}
	else
	{
		operatorName = getOperatorDescription(sessionInfo.OperatorId[0]);
		profileName = getProfileName(sessionInfo.ProfileId[0]);
		if ( (sessionInfo.OperatorId.size() > 1) && (sessionInfo.ProfileId.size() > 1) )
		{
			operatorName += "/";
			operatorName += getOperatorDescription(sessionInfo.OperatorId[1]);

			profileName += "/";
			profileName += getProfileName(sessionInfo.ProfileId[1]);
		}
		locationName = getDataCache().getLocationDisplayName(sessionInfo.LocationId);
		workstationName = getWorkstationName(sessionInfo.WorkstationId);

		returnValue = true;
	}

	return returnValue;

}

bool AppControllerUser::getRegionInfoName(const DutyAgentTypeCorbaDef::TransferableRegion& regionInfo,std::string& regionName,std::string& operatorName,std::string& profileName)
{
	//std::string regionName( getRegionDisplayName(regionInfo.regionKey));
	//std::string operatorName(""), profileName("");
	regionName = getRegionDisplayName(regionInfo.regionKey);

	// Unassigned regions have the operator key and profile key set to "invalid". 
	// In such case operator name and profile name should be left blank.
	if (regionInfo.operatorKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
	{
		operatorName = getOperatorDescription(regionInfo.operatorKey);
	}

	if (regionInfo.profileKey != TA_Base_Bus::DutyAgentTypeCorbaDef::INVALID_KEY)
	{
		profileName = getProfileName(regionInfo.profileKey);
	}

	return true;
}

std::string	AppControllerUser::getWorkstationName(unsigned long WorkstationId)
{
	return getDataCache().getWorkstationName(WorkstationId);
}
std::string AppControllerUser::getProfileName(ProfileKey key)
{
	return getDataCache().getProfileName(key);
}
std::string AppControllerUser::getOperatorDescription(OperatorKey key)
{
	return getDataCache().getOperatorDescription(key);
}

// LocationAccessFactory
std::string	AppControllerUser::getLocationNameByLocationKey(const unsigned long locationKey)
{
	return m_pDmAppController->getLocationNameByLocationKey( locationKey);
}

// DutyRighst
bool AppControllerUser::canTransferRegions()
{
	return m_pDmAppController->canTransferRegions();
}
bool AppControllerUser::canTransferSubsystems(long lActionId)
{
	return m_pDmAppController->canTransferSubsystems( lActionId);
}
bool AppControllerUser::canDegradeSubsystems()
{
	return m_pDmAppController->canDegradeSubsystems();
}

// DutyAgent
void AppControllerUser::degradeAllSubsystems()
{
	 m_pDmAppController->degradeAllSubsystems();
}
void AppControllerUser::transferAllSubsystemDuty()
{
	 m_pDmAppController->transferAllSubsystemDuty();
}
void								AppControllerUser::transferRegionDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const RegionKeyList& regionKeys)
{
	 m_pDmAppController->transferRegionDuty( sessionInfo,  regionKeys);
}
void								AppControllerUser::transferSubsystemDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const SubsystemTreeMap& subsystems)
{
	 m_pDmAppController->transferSubsystemDuty( sessionInfo,  subsystems);
}
TransferableRegionList				AppControllerUser::getTransferableRegions(const SessionInfo& session)
{
	return m_pDmAppController->getTransferableRegions( session);
}
TransferableSubsystemList			AppControllerUser::getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys)
{
	return m_pDmAppController->getTransferableSubsystems(session, regionKeys);
}

TA_Base_Bus::ActionGroupKey			AppControllerUser::getActionGroupDB(
	const TA_Base_Bus::ProfileKey profileKey,
	const TA_Base_Bus::SubsystemKey subsystemKey,
	const TA_Base_Bus::ESubsystemState subsystemState)
{
	return m_pDmAppController->getActionGroupDB(
		 profileKey,
		 subsystemKey,
		 subsystemState);
}

}





