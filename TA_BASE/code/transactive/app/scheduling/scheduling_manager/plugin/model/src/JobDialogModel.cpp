#include "app/scheduling/scheduling_manager/plugin/model/src/JobDialogModel.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingModelConstants.h"
#include "bus/security/rights_library/src/RightsLibrary.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
//#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/PlanManagerEntityData.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DateFormat.h" 

namespace TA_Base_App
{

    JobDialogModel::JobDialogModel( TA_Base_Bus::IApplicationController* appController, const char * pmodelName ):
        AbstractPModel(appController, pmodelName),
		m_rightsLibrary(NULL),
		m_planManagerKey(0)
    {

    }

    JobDialogModel::~JobDialogModel()
    {
		if( m_rightsLibrary != NULL )
		{
			delete m_rightsLibrary;
			m_rightsLibrary = NULL;
		}
    }

    void JobDialogModel::notifyUpdate( const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/ )
    {

	}

	void JobDialogModel::initRightLib()
	{
		// Initialise the rights library
		try
		{
			TA_Base_Bus::RightsLibraryFactory rightsFactory;
			m_rightsLibrary = rightsFactory.buildRightsLibrary(TA_Base_Bus::DEFAULT_RL, TA_Base_Bus::RightsLibraryFactory::MANAGER);
		}
		catch (const TA_Base_Core::TransactiveException &ex)
		{
			delete m_rightsLibrary;
			m_rightsLibrary = NULL;

			LOG_EXCEPTION_CATCH(SourceInfo, "Rights exception", ex.what());
		}
		catch (...)
		{
			delete m_rightsLibrary;
			m_rightsLibrary = NULL;

			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown exception", "");
		}
	}

	bool JobDialogModel::isJobDetailsButtonEnabled()
	{
		FUNCTION_ENTRY( "isJobDetailsButtonEnabled" );

		bool enabled(false);
		std::string reason("");
		TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;

		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		TA_ASSERT(!sessionId.empty(), "Session ID not set");

		// get the plan manager key if we haven't already
		if(m_planManagerKey == 0)
		{
			try
			{
				// get all the entities associated with the plan manager type - there should only be one
				TA_Base_Core::IEntityDataList planManagers = 
					TA_Base_Core::EntityAccessFactory::getInstance().getEntitiesOfType(TA_Base_Core::PlanManagerEntityData::getStaticType());

				if (1 == planManagers.size())
				{
					m_planManagerKey = planManagers[0]->getKey();
				}
				else
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, 
						"There are %lu plan managers entities in the database. There should be one only.",
						planManagers.size());
					if ( planManagers.size() > 1)
					{
						// use the first one
						m_planManagerKey = planManagers[0]->getKey();
					}
				}

				// free the entity data
				while (! planManagers.empty())
				{
					delete planManagers.back();
					planManagers.pop_back();
				}
			}
			catch(...)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "Unknown Exception", "Could not look up the Plan Managers entity key");
			}
		}

		try
		{
			std::auto_ptr<TA_Base_Core::IResource> r(TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity( m_planManagerKey ));
			unsigned long resourceId = r->getKey();

			//TD17144++
			TA_Base_Core::IEntityData* entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(resourceId);		
			unsigned long locationKey = entityData->getLocation();
			unsigned long subsystemKey = entityData->getSubsystem();
			delete entityData;

			enabled = m_rightsLibrary->isActionPermittedOnResourceAndLocSub(  // isActionPermittedOnResource(
				sessionId, 
				resourceId, 
				locationKey,
				subsystemKey,
				TA_Base_Bus::aca_RUN_APPLICATION, 
				reason, 
				decisionModule);
			enabled = enabled && m_rightsLibrary->isActionPermittedOnResourceAndLocSub( //isActionPermittedOnResource(
				sessionId, 
				resourceId,
				locationKey,
				subsystemKey,													
				TA_Base_Bus::aca_PLAN_CONTROL_PLAN, 
				reason, 
				decisionModule);
			//++TD17144
		}
		catch(...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown Exception", "Could not look determine rights for running plan manager or controlling plan.");
		}

		FUNCTION_EXIT;
		return enabled;
	}

	TAUnicodeString JobDialogModel::getTimeString(COleDateTime& theTime) const
	{
		TAUnicodeString strUni = theTime.Format(TA_Base_App::SchedulingManager::TIME_FORMAT); 
		return strUni;
	}

	TAUnicodeString JobDialogModel::getDateString(COleDateTime& theTime, const bool showYear /* = true */) const
	{
		// TD12474 ++
		//return theTime.Format(showYear ? COMPLETE_DATE_FORMAT : DATE_FORMAT);

		TA_Base_Core::DateFormat dateFormat; 
		std::string dateFormatStr1 = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateFormat4);
		std::string dateFormatStr2 = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormatNoSec5);

		TAUnicodeString tmpUniStrForDate = TAUnicodeString::CreateFromMultitByteString(showYear ? dateFormatStr1.c_str() : dateFormatStr2.c_str());
		TAUnicodeString tmpUniStrForDateFormat = theTime.Format(tmpUniStrForDate.c_str());
		return tmpUniStrForDateFormat;
		// ++ TD12474
	}
}