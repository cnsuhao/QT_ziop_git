#if !defined(AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_)
#define AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/ActivePlanStep.h $
  *  @author:  Huang Jian
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  A thread-safe Singleton factory for converting database data into
  *  application data and vice-versa.
  */

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "core/threads/src/Thread.h"

namespace TA_Base_App
{
class ActivePlanStep: public TA_Base_Core::Thread
{

public:
    ActivePlanStep(const TA_Base_Core::CurrentPlanStepDetail &stepDetail);
    virtual ~ActivePlanStep();

    bool isRunning();

    // TA_Base_Core::Thread interface should be implemented by concrete step
    virtual void run();
    virtual void terminate();

    virtual void execute() = 0;
    virtual void stop() = 0;

private:
    volatile bool m_bIsRunning;

protected:
    TA_Base_Core::CurrentPlanStepDetail m_stepDetail;
};

} // namespace TA_Base_App


#endif // !defined(AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_)