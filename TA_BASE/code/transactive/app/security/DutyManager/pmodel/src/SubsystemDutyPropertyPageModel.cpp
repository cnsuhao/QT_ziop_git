/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/SubsystemDutyPropertyPageModel.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the "Subsystems" tab. It displays the active 
  * subsystem duty state for every physical subsystem in every region.
  *
  */

#include "app/security/DutyManager/pmodel/src/SubsystemDutyPropertyPageModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

//--------move to bridge--------------
#include "core/data_access_interface/src/RightsAccessFactory.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace TA_Base_App;
using namespace TA_Base_Bus;

/////////////////////////////////////////////////////////////////////////////
// SubsystemDutyPropertyPageModel property page

SubsystemDutyPropertyPageModel::SubsystemDutyPropertyPageModel(IGUIApplicationController* appController, const char * pmodelName) : 
    TA_Base_Bus::AbstractPModel(appController,  pmodelName),
	TA_Base_App::AppControllerUser(appController)
	{
		FUNCTION_ENTRY("SubsystemDutyPropertyPageModel()");
		

		FUNCTION_EXIT;
	}


SubsystemDutyPropertyPageModel::~SubsystemDutyPropertyPageModel()
{
    FUNCTION_ENTRY("~SubsystemDutyPropertyPageModel()");
	m_actionGroupsDB.clear();
    FUNCTION_EXIT;
}

TA_Base_Bus::RegionKeyList SubsystemDutyPropertyPageModel::getLocations()
{
	TA_Base_Bus::RegionKeyList regionKeys;

	/* NOTE: for local profile, only current location should be displayed.
	* for central profile, all locations should be displayed.
	* if local override with central, then all locations are displayed.
	* if local override with local, same location is displayed
	* if central is override with local, there is no such case.
	* this has nothing to do with rights or duties, only the location.
	*/

	//based on session, get location from DB
	//if 0 then central, if not then local
	//TA_Base_Core::ISession* session;
	//session = TA_Base_Core::SessionAccessFactory::getInstance().getSession( m_currentSessionInfo.SessionId );
	//TA_Base_Bus::AuthenticationLibrary authLib(true);
	//TA_Base_Bus::SessionInfo sesInfo = authLib.getSessionInfo( m_currentSessionInfo.SessionId, m_currentSessionInfo.SessionId );
	//TA_Base_Bus::SessionInfo sesInfo = m_sessionCache.getSessionInfo(m_currentSessionInfo.SessionId);

	//LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,
	//	"In SubsystemDutyPropertyPage::getLocations()..., sessionId = %s",sesInfo.SessionId.c_str());

	LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugDebug,
		"In SubsystemDutyPropertyPage::getLocations()..., sessionId = %s",m_pDmAppController->getCurrentSessionInfo().SessionId.c_str());

	// check if it has ended or not
	//unsigned long locationKey = session->getLocationKey();
	unsigned long locationKey = m_pDmAppController->getCurrentSessionInfo().LocationId;
	//unsigned long locationKey = sesInfo.LocationId;

	if (locationKey == ALL_LOCATION) 
	{
		//central profile
		regionKeys.push_back(locationKey);

		//no need to check overrided profile's location
	}
	else
	{
		//local profile
		regionKeys.push_back(locationKey);

		//need to check overrided profile's location
		//always assume base is 0th element and override is 1st element

		try
		{
			//overrided
			if (m_pDmAppController->getCurrentSessionInfo().ProfileId.size() == 2)
			{
				//get profile from DB if local or central for base
				unsigned long baseProfile = m_pDmAppController->getCurrentSessionInfo().ProfileId[0];
				TA_Base_Core::IProfile* profileB;
				profileB = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile( baseProfile );
				if (profileB->getType() == CENTRAL_PROFILE_TYPE)
				{
					//currently we detected a local profile, so need to add central profile
					regionKeys.push_back(ALL_LOCATION); 
				}
				//no need to add local profile with assumption that location is the same

				//get profile from DB if local or central for override
				unsigned long overrideProfile = m_pDmAppController->getCurrentSessionInfo().ProfileId[1];
				TA_Base_Core::IProfile* profileO;
				profileO = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile( overrideProfile );
				if (profileO->getType() == CENTRAL_PROFILE_TYPE)
				{
					//currently we detected a local profile, so need to add central profile
					regionKeys.push_back(ALL_LOCATION); 
				}
				//no need to add local profile with assumption that location is the same
			}
			//single operator
			else if (m_pDmAppController->getCurrentSessionInfo().ProfileId.size() == 1)
			{
				//get profile from DB if local or central for base
				unsigned long baseProfile = m_pDmAppController->getCurrentSessionInfo().ProfileId[0];
				TA_Base_Core::IProfile* profileB;
				profileB = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile( baseProfile );
				if (profileB->getType() == CENTRAL_PROFILE_TYPE)
				{
					//currently we detected a local profile, so need to add central profile
					regionKeys.push_back(ALL_LOCATION); 
				}
				//no need to add local profile with assumption that location is the same
			}
			//else... profile cant be 0
		}
		catch (...) 
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "TD15533: Unknown exception in getLocations()");
		}
	}    

	return regionKeys;
}

bool SubsystemDutyPropertyPageModel::isRegionAccessible(
	const TA_Base_Bus::RegionKey regionKey,
	const TA_Base_Bus::RegionKeyList regionKeys)
{
	bool hasLocationDuty = false;

	for (int i=0; i<(int)regionKeys.size(); i++)
	{
		if (regionKeys[i] == ALL_LOCATION)
		{
			//if central location then grant duty for this region
			hasLocationDuty = true;
			break;
		}
		else if (regionKeys[i] == regionKey)
		{
			hasLocationDuty = true;
			break;
		}
	}

	return hasLocationDuty;
}



