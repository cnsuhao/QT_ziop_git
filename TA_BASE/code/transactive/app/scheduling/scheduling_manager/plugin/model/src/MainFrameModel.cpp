#include "app/scheduling/scheduling_manager/plugin/model/src/MainFrameModel.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingModelConstants.h"
#include "bus/security/rights_library/src/RightsLibrary.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"


namespace TA_Base_App
{

    MainFrameModel::MainFrameModel( TA_Base_Bus::IApplicationController* appController, const char * pmodelName ):
        AbstractPModel(appController, pmodelName),
		m_groupUpdateSubscriber(NULL),
		m_rightsLibrary(NULL),
		m_resourceKey(0)
    {

    }

    MainFrameModel::~MainFrameModel()
    {
		if( m_rightsLibrary != NULL )
		{
			delete m_rightsLibrary;
			m_rightsLibrary = NULL;
		}

		// TD16358++
		if(m_groupUpdateSubscriber != NULL)
		{
			delete m_groupUpdateSubscriber;
			m_groupUpdateSubscriber = NULL;
		}
		// ++TD16358
    }

	void MainFrameModel::subscribeGroupUpdate()
	{
		// TD16358++
		m_groupUpdateSubscriber = new TA_Base_Bus::SystemControllerGroupUpdateSubscriber(*this);
		//TD16358
		// TD16538: Start receiving message informing GUI when system is degraded/recovered
		LOG_GENERIC(
			SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,
			"[TD16068]Starting online update subscriptions");
		m_groupUpdateSubscriber->startNotifications();
		// ++TD16358
	}

    void MainFrameModel::notifyUpdate( const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/ )
    {
		if (_stricmp(updateType, SYSTEM_INDEGRADEDMODE) == 0)
		{
			notifyViews(SYSTEM_INDEGRADEDMODE, args);
		}
		if (_stricmp(updateType, SYSTEM_INDEGRADEDMODE) == 0)
		{
			notifyViews(SYSTEM_INNORMALMODE, args);
		}
    }

	//++TD16358
	void MainFrameModel::notifySystemStateDegraded()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "notifySystemStateDegraded" );

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "[TD16358]Occ group now offline!");

		// To disable the "Add/Delete/Edit/Enable/Disable" item
		//AfxGetMainWnd()->PostMessage(WM_SYSTEM_INDEGRADEDMODE);
		notifyUpdate(SYSTEM_INDEGRADEDMODE, NULL);

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "notifySystemStateDegraded" );
	}


	void MainFrameModel::notifySystemStateNormal()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "notifySystemStateNormal" );

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "[TD16358]Occ group now online!");

		// To enable the "Add/Delete/Edit/Enable/Disable" item
		//AfxGetMainWnd()->PostMessage(WM_SYSTEM_INNORMALMODE);
		notifyUpdate(SYSTEM_INNORMALMODE, NULL);

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "notifySystemStateNormal" );
	}
	//TD16358++



	int MainFrameModel::compareDateContent( Job* job1, Job* job2 )
	{
		int result = 0;
		if( (job1 == NULL) || (job2 == NULL) )
		{
            return result;
		}
		
		COleDateTime* date1 = job1->getStartDate();
		COleDateTime* date2 = job2->getStartDate();
		// both dates are not specified
		if ( ( NULL == date1 ) && (NULL ==date2 ))
		{
			result = 0;
		}
		else if ((date1 != NULL) && (date2 != NULL))
		{
			if (*date1 == *date2)
			{
				result = 0;
			}
			else if (*date1 < *date2)
			{
				result = -1;
			}
			else
			{
				result = 1;
			}
		}
		// only 1 date is specified
		else if (date1 != NULL)
		{
			result = -1;
		}
		else // (date2 != NULL)
		{
			result = 1;
		}

		return result;
	}

	bool MainFrameModel::isRightsAgentContactable()
	{
		std::string reason("");
		TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;

		// TD #13519
		// attempt to contact the rights agent - we don't care about the result.
		// if no exception was thrown, we were able to contact the rights agent.
		//
		// there must be a better way of doing this..?
		try
		{
			// must clear the cache first, to get rid of the results of last time we did this
			m_rightsLibrary->resetCache();
			m_rightsLibrary->isActionPermittedOnResource(
				TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID),
				m_resourceKey,
				TA_Base_Bus::aca_RUN_APPLICATION,
				reason,
				decisionModule);

			// no exception thrown - rights agent contacted without issue
			return true;
		}
		catch(TA_Base_Core::RightsException&)
		{
		}

		return false;
	}

	void MainFrameModel::initRightLib()
	{
		if (m_rightsLibrary != NULL) 
		{
			return;
		}

		TA_Base_Bus::RightsLibraryFactory rlf;
		m_rightsLibrary = rlf.buildRightsLibrary(TA_Base_Bus::DEFAULT_RL, TA_Base_Bus::RightsLibraryFactory::MANAGER);

		TA_Base_Core::IEntityData* guiEntity;
		TA_Base_Core::IResource* resource;

		std::string entityName = TA_Base_Core::RunParams::getInstance().get( RPARAM_ENTITYNAME );
		if ( !entityName.empty() )
		{
			try
			{
				guiEntity = TA_Base_Core::EntityAccessFactory::getInstance().getEntity( entityName );
				resource = TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity( guiEntity->getKey() );
				m_resourceKey = resource->getKey();

				delete resource;
				resource = NULL;

				delete guiEntity;
				guiEntity = NULL;

			}
			catch(...)
			{
				if (resource != NULL)
				{
					delete resource;
					resource = NULL;
				}
				if (guiEntity != NULL)
				{
					delete guiEntity;
					guiEntity = NULL;
				}

				LOG_EXCEPTION_CATCH( SourceInfo, "Unknown", "Error while trying to retrieve resource key");
			}
		}

	}

}