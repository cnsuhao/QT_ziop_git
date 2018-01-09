/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/banner/pmodel/src/BannerDlgPModel.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#include "app/system_control/banner/pmodel/src/BannerDlgBusLogic.h"
#include "app/system_control/banner/pmodel/src/BannerPModelConst.h"

#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/IProfile.h"

namespace TA_Base_App
{
	BannerDlgBusLogic::BannerDlgBusLogic(int xPos, int yPos, int width, int height)
		: m_sessionId("")
		, m_startTab("")
		, m_currentProfileId(0)
		, m_xPos(xPos)
		, m_yPos(yPos)
		, m_width(width)
		, m_height(height)
	{
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);

		m_sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "m_sessionId set to %s", m_sessionId.c_str());

	}

	BannerDlgBusLogic::~BannerDlgBusLogic(void)
	{
	}

	void BannerDlgBusLogic::onRunParamChange(const std::string& name, const std::string& value)
	{
		FUNCTION_ENTRY("onRunParamChange");

		if(name.compare(RPARAM_SESSIONID) == 0)
		{
			// Store the new session id.
			m_sessionId = value;

			// Reset the banner page based on the new profile
			setBannerPage();

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "RunParamChange received for session ID. New sesion ID is %s", value.c_str());
		}

		FUNCTION_EXIT;
	}

	void BannerDlgBusLogic::setBannerPage()
	{
		// TES 212: Retrieve the configured banner pages from the database
		TA_Base_Bus::SessionInfo sessionInfo;

		try
		{
			// Get some info about the current session.
			TA_Base_Bus::AuthenticationLibrary authLibrary(true);
			sessionInfo = authLibrary.getSessionInfo(m_sessionId, m_sessionId);
		}
		catch(TA_Base_Core::AuthenticationAgentException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", "Could not retrieve session information");
			return;
		}
		catch(TA_Base_Core::ObjectResolutionException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", "Could not resolve Authentication Agent");
			return;
		}
		catch(...)
		{
		}

		unsigned long locationKey = -1;

		TA_Base_Core::IProfile* profile = NULL;
		try
		{
			// Use the last profile
			unsigned int numProfiles = sessionInfo.ProfileId.size();
			TA_Base_Core::IProfile* profile = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile(sessionInfo.ProfileId[numProfiles - 1]);

			// TD 12254 - Only reset the banners to default if there is a change in profile 
			if(profile->getKey() == m_currentProfileId)
			{
				return;
			}
			else
			{
				m_currentProfileId = profile->getKey();
			}

			if (profile->getType() == PMODEL_PROFILE_LOCAL)
			{
				locationKey = sessionInfo.LocationId;
			}
			else  // PROFILE_CENTRAL
			{
				// Central profiles use the location id of 0
				locationKey = 0;
			}

			std::string entityName = TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME);
			std::string newStartTab;
			if(entityName == PMODEL_LEFT_BANNER)
			{
				newStartTab = profile->getParameterActualValue(locationKey, PMODEL_LEFT_BANNER_DEFAULT_STR);
			}
			else if(entityName == PMODEL_CENTRE_BANNER)
			{
				newStartTab = profile->getParameterActualValue(locationKey, PMODEL_CENTRE_BANNER_DEFAULT_STR);
			}
			else if(entityName == PMODEL_RIGHT_BANNER)
			{
				newStartTab = profile->getParameterActualValue(locationKey, PMODEL_RIGHT_BANNER_DEFAULT_STR);
			}
			if ( false == newStartTab.empty() )
			{
				m_startTab = newStartTab;
			}
			else
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError,
					"Profile parameter for start tab has no value" );
			}

			delete profile;
		}
		catch(TA_Base_Core::DataException&)
		{
			delete profile;
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", "Could not retrieve profile information.");
			return;
		}
		catch(TA_Base_Core::DatabaseException&)
		{
			delete profile;
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", "Could not retrieve profile information");
			return;
		}

		StartTabEventArgs* objEventArgs = 
			new StartTabEventArgs(Banner_EventArgs::START_TAB, m_startTab);

		notifyObservers(PMODEL_START_PAGE_UPDATE, objEventArgs);
	}


}// TA_Base_App