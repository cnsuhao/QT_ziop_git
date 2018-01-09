#ifndef MFTMESSAGEACTIVESTEP_H
#define MFTMESSAGEACTIVESTEP_H

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/MftMessageActiveStep.h $
  *  @author:  Huang Jian
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  A thread-safe Singleton factory for converting database data into
  *  application data and vice-versa.
  */

#include "app/response_plans/plan_mft_processor/src/ActivePlanStep.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

namespace TA_Base_Bus
{
    class MessageDialogHelper;
}

namespace TA_Base_App
{

class MftMessageActiveStep : public TA_Base_App::ActivePlanStep
{
    // Operations
public:
    MftMessageActiveStep(const TA_Base_Core::CurrentPlanStepDetail& stepDetail);

    // ActivePlanStep interface
    virtual void execute();
    virtual void stop();

    virtual ~MftMessageActiveStep();

private:
    void launch();

    TA_Base_Bus::MessageDialogHelper* m_pMessageDialog;
};

}

#endif //MFTMESSAGEACTIVESTEP_H