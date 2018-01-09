/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/MftMessageActiveStep.cpp $
  * @author:  Huang Jian
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  *
  **/

#include "StdAfx.h"
#include "app/response_plans/plan_mft_processor/src/ActivePlanAgency.h"
#include "app/response_plans/plan_mft_processor/src/RightsManager.h"
#include "app/response_plans/plan_mft_processor/src/MftMessageActiveStep.h"
#include "bus/mfc_extensions/src/message_dialog_helper/MessageDialogHelper.h"
#include "bus/response_plans/plan_agent/src/PlanAgentLibrary.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/security/authentication_library/src/SessionInfo.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"


namespace TA_Base_App
{
    MftMessageActiveStep::MftMessageActiveStep( const TA_Base_Core::CurrentPlanStepDetail& stepDetail ) : ActivePlanStep(stepDetail),
    m_pMessageDialog(NULL)
    {
        FUNCTION_ENTRY( "MftMessageActiveStep" );

        FUNCTION_EXIT;
    }

    MftMessageActiveStep::~MftMessageActiveStep()
    {
        FUNCTION_ENTRY( "~MftMessageActiveStep" );

        if (NULL != m_pMessageDialog)
        {
            delete m_pMessageDialog;
            m_pMessageDialog = NULL;
        }

        FUNCTION_EXIT;
    }

    void MftMessageActiveStep::launch()
    {
        FUNCTION_ENTRY( "launch" );
        
        if ( !isRunning() )
        {
            FUNCTION_EXIT;
            return;
        }

        if ( !RightsManager::getInstance().isProfileMatched( m_stepDetail.data.popupMFTMessage().operatorProfileNumber ))
        {
            LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Can not lunch the dialog, request profile = %d", m_stepDetail.data.popupMFTMessage().operatorProfileNumber);
            FUNCTION_EXIT;
            return;
        }

        if ( !isRunning() )
        {
            FUNCTION_EXIT;
            return;
        }

        try
        {
            ActivePlanAgency::instance()->getPlanAgent()->processPopupMftMessageStepResponse( m_stepDetail, TA_Base_Core::PSCS_SUCCESS );
        }
        catch(...)
        {
            LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Execute MftMessageActiveStep fail");
        }

        if ( !isRunning() )
        {
            FUNCTION_EXIT;
            return;
        }

        if ( NULL != m_pMessageDialog )
        {
            m_pMessageDialog->closeMessageBox();
        }
        else
        {
            m_pMessageDialog = new TA_Base_Bus::MessageDialogHelper();
        }

        if ( !isRunning() )
        {
            FUNCTION_EXIT;
            return;
        }

        static const unsigned int MsgBoxStyle = MB_ICONWARNING | MB_YESNO | MB_SYSTEMMODAL | MB_DEFBUTTON2;
        int nRes = m_pMessageDialog->showMessageBox( NULL, m_stepDetail.data.popupMFTMessage().decisionMessage.in(), "", MsgBoxStyle );

        TA_Base_Core::EPlanStepCompletionStatus completionStatus = TA_Base_Core::PSCS_FAILURE;

        if ( IDYES == nRes )
        {
            completionStatus = TA_Base_Core::PSCS_YES_RESPONSE;
        }
        else if ( IDNO == nRes )
        {
            completionStatus = TA_Base_Core::PSCS_NO_RESPONSE;
        }
        else if ( IDCLOSE == nRes )
        {
            completionStatus = TA_Base_Core::PSCS_UNDEFINED;
        }
        else
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Result %d can not be recognized.", nRes );
            completionStatus = TA_Base_Core::PSCS_FAILURE;
        }

        if ( !isRunning() )
        {
            FUNCTION_EXIT;
            return;
        }

        try
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "[MftMsg] Send response %d after user response", completionStatus );

            ActivePlanAgency::instance()->getPlanAgent()->processPopupMftMessageStepResponse( m_stepDetail, completionStatus );
        }
        catch(...)
        {
            LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Execute MftMessageActiveStep fail");
        }

        FUNCTION_EXIT;
    }

    void MftMessageActiveStep::execute()
    {
        launch();
    }

    void MftMessageActiveStep::stop()
    {
        if (NULL != m_pMessageDialog)
        {
            m_pMessageDialog->closeMessageBox();
        }
    }

}