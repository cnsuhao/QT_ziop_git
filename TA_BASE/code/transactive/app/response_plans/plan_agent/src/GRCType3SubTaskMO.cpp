// GRCType3SubTaskMO.cpp: implementation of the GRCType3SubTaskMO class.
//
//////////////////////////////////////////////////////////////////////

#include "core/utilities/src/DebugUtil.h"
#include "app/response_plans/common/src/ProjectDefinition.h"
#include "app/response_plans/plan_agent/src/ActiveDataPointSetStep.h"
#include "app/response_plans/plan_agent/src/GRCType3SubTask.h"
#include "app/response_plans/plan_agent/src/GRCType3SubTaskMO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace TA_Base_App
{

	GRCType3SubTaskMO::GRCType3SubTaskMO( GRCType3SubTask * pSubTask, ACE_Future<bool> & execResult ) : 
    m_execResult(execResult),
    m_pSubTask(pSubTask)
	{
	}

	int GRCType3SubTaskMO::call()
	{
		m_pSubTask->getDataPointSetStep()->setExecResult(m_execResult);

		try
		{
			execute();
			m_execResult.set(true);
		}
        // TD9899: Added logging to allow peer communication to be debugged.
        catch( const std::exception& ex )
        {
            LOG1( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "TransactiveException, reason=%s", ex.what() );
			try
			{
				m_execResult.set(false);
			}
			catch(...)
			{
				LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Failed to set return value. Dead-lock has occurred." );
			}
        }
		catch( ... )
		{
			LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Unknown Error talking to peers." );

			try
			{
				m_execResult.set(false);
			}
			catch(...)
			{
				LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Failed to set return value. Dead-lock has occurred." );
			}
		}
		
		return 0 ;
	}

	void GRCType3SubTaskMO::execute()
	{
		m_pSubTask->doSetDataPointImpl();
	}
	


}
