/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/ActivePlanStep.cpp $
  * @author:  Huang Jian
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  *
  **/

#include "app/response_plans/plan_mft_processor/src/ActivePlanStep.h"
#include "app/response_plans/plan_mft_processor/src/PlanAgentUpdateProcessor.h"


namespace TA_Base_App
{


    ActivePlanStep::ActivePlanStep( const TA_Base_Core::CurrentPlanStepDetail &stepDetail ):
    m_bIsRunning(false),
    m_stepDetail(stepDetail)
    {
    }

    ActivePlanStep::~ActivePlanStep()
    {
    }

    void ActivePlanStep::run()
    {
        m_bIsRunning = true;

        try
        {
            execute();
        }
        catch ( const std::exception& expWhat )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, typeid(expWhat).name(), expWhat.what() );
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "UnknownException", "Caught unknown exception when execute" );
        }

        m_bIsRunning = false;
    }

    void ActivePlanStep::terminate()
    {
        if ( !m_bIsRunning )
        {
            return;
        }

        m_bIsRunning = false;

        try
        {
            stop();
        }
        catch ( const std::exception& expWhat )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, typeid(expWhat).name(), expWhat.what() );
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "UnknownException", "Caught unknown exception when stop" );
        }
    }

    bool ActivePlanStep::isRunning()
    {
        return m_bIsRunning;
    }
}