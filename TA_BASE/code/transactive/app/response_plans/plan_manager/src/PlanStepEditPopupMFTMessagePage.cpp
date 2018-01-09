//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepEditDecisionParameterPage.cpp $
// @author:  Andy Parker
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2012/02/06 16:15:14 $
// Last modified by:  $Author: haijun.li $
//
// <description>

#include "stdafx.h"

#if defined (STEPTYPE_POPUP_MFT_MESSAGE)

#include "core/utilities/src/TAAssert.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "bus/response_plans/active_plans_display/src/StringUtilities.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "PlanManager.h"
#include "PlanStepEditPopupMFTMessagePage.h"
#include "PopupMFTMessageStep.h"
#include "StepNumberStepParameter.h"
#include "TextualStepParameter.h"
#include "YesNoStepParameter.h"
#include "PlanStepParameterStepNumberEditDlg.h"
#include "PlanNode.h"
#include "PlanManagerCommonDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// CPlanStepEditPopupMFTMessagePage property page

IMPLEMENT_DYNAMIC(CPlanStepEditPopupMFTMessagePage, CPlanStepEditParameterPage)

CPlanStepEditPopupMFTMessagePage::CPlanStepEditPopupMFTMessagePage(PopupMFTMessageStep* step) :
CPlanStepEditParameterPage(CPlanStepEditPopupMFTMessagePage::IDD, step),
m_noStepParameter(*step->getNoParameter()),
m_yesStepParameter(*step->getYesParameter()),
m_pendingStepParameter(*step->getPendingParameter()),
m_operatorProfileParameter(*step->getOperatorProfile())
{
    FUNCTION_ENTRY( "CPlanStepEditPopupMFTMessagePage" );

    // {{AFX_DATA_INIT(CPlanStepEditPopupMFTMessagePage)
    m_decisionMessage = _T("");
    m_yesStep = _T("");
    m_noStep = _T("");
    m_pendingStep = _T("");
    // }}AFX_DATA_INIT

    FUNCTION_EXIT;
}


CPlanStepEditPopupMFTMessagePage::~CPlanStepEditPopupMFTMessagePage()
{
    FUNCTION_ENTRY( "~CPlanStepEditPopupMFTMessagePage" );
    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::DoDataExchange(CDataExchange* pDX)
{
    FUNCTION_ENTRY( "DoDataExchange" );

    CPlanStepEditParameterPage::DoDataExchange(pDX);
    // {{AFX_DATA_MAP(CPlanStepEditPopupMFTMessagePage)
    DDX_Text(pDX, IDC_DECISION_MESSAGE, m_decisionMessage);
    DDX_Text(pDX, IDC_NO_STEP, m_noStep);
    DDX_Text(pDX, IDC_YES_STEP, m_yesStep);
    DDX_Text(pDX, IDC_SUCCESS_STEP, m_pendingStep);
    DDX_Control(pDX, IDC_OPERATOR_PROFILE_COMBO, m_operatorProfileCombo);
    // }}AFX_DATA_MAP

    FUNCTION_EXIT;
}


BEGIN_MESSAGE_MAP(CPlanStepEditPopupMFTMessagePage, CPlanStepEditParameterPage)
    // {{AFX_MSG_MAP(CPlanStepEditPopupMFTMessagePage)
    ON_BN_CLICKED(IDC_NO_PICK_STEP_BUTTON, OnNoPickStepButton)
    ON_BN_CLICKED(IDC_YES_PICK_STEP_BUTTON, OnYesPickStepButton)
    ON_BN_CLICKED(IDC_PICK_STEP_BUTTON, OnPendingPickStepButton)
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPlanStepEditPopupMFTMessagePage message handlers

BOOL CPlanStepEditPopupMFTMessagePage::OnInitDialog()
{
    FUNCTION_ENTRY( "OnInitDialog" );

    if (!CPlanStepEditParameterPage::OnInitDialog())
    {
        FUNCTION_EXIT;
        return FALSE;
    }

    try
    {
        typedef std::vector<TA_Base_Core::IProfile *> ProfileList;
        ProfileList allProfile = TA_Base_Core::ProfileAccessFactory::getInstance().getAllProfiles(false);
        for (ProfileList::const_iterator iter = allProfile.begin(); iter != allProfile.end(); iter++)
        {
            TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString((*iter)->getName().c_str());
            int appTypeIdx = m_operatorProfileCombo.AddString(uniStr.c_str());
            m_operatorProfileCombo.SetItemData(appTypeIdx, (*iter)->getKey());
            delete *iter;
        }
        allProfile.clear();
    }
    catch (...)
    {
        return FALSE;
    }

    FUNCTION_EXIT;
    return TRUE;
}


void CPlanStepEditPopupMFTMessagePage::OnNoPickStepButton()
{
    FUNCTION_ENTRY( "OnNoPickStepButton" );

    UpdateData(); // TD#1537

    // Select the required step
    m_noStepParameter.editParameter();
    m_noStep = m_noStepParameter.getDestinationStepDescription().c_str();

    UpdateData(FALSE);

    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::OnYesPickStepButton()
{
    FUNCTION_ENTRY( "OnYesPickStepButton" );

    UpdateData(); // TD#1537

    // Select the required step
    m_yesStepParameter.editParameter();
    m_yesStep = m_yesStepParameter.getDestinationStepDescription().c_str();

    UpdateData(FALSE);

    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::OnPendingPickStepButton()
{
    FUNCTION_ENTRY( "OnPendingPickStepButton" );

    UpdateData(); // TD#1537

    // Select the required step
    m_pendingStepParameter.editParameter();
    m_pendingStep = m_pendingStepParameter.getDestinationStepDescription().c_str();

    UpdateData(FALSE);

    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::EnableParameterControls(bool enable)  // TD#614/1170/1409
{
    FUNCTION_ENTRY( "EnableParameterControls" );

    CEdit* decisionMessage = static_cast<CEdit*>(GetDlgItem(IDC_DECISION_MESSAGE));
    if (decisionMessage)
    {
        decisionMessage->SetReadOnly(!enable);
    }

    CWnd* yesStepButton = GetDlgItem(IDC_YES_PICK_STEP_BUTTON);
    if (yesStepButton)
    {
        yesStepButton->EnableWindow(enable);
    }

    CWnd* noStepButton = GetDlgItem(IDC_NO_PICK_STEP_BUTTON);
    if (noStepButton)
    {
        noStepButton->EnableWindow(enable);
    }

    CWnd* pendingStepButton = GetDlgItem(IDC_PICK_STEP_BUTTON);
    if (pendingStepButton)
    {
        pendingStepButton->EnableWindow(enable);
    }

    CWnd* operatorProfileCombo = GetDlgItem(IDC_OPERATOR_PROFILE_COMBO);
    if (operatorProfileCombo)
    {
        operatorProfileCombo->EnableWindow(enable);
    }

    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::DisplayParameters()
{
    FUNCTION_ENTRY( "DisplayParameters" );

    UpdateData();

    PopupMFTMessageStep* step = dynamic_cast<PopupMFTMessageStep*>(GetPlanStep());
    TA_ASSERT(step!=NULL,"Incorrect step type for the decision step page");

    m_decisionMessage = step->getMessageParameter()->getMessageText().c_str();
    m_yesStep = m_yesStepParameter.getDestinationStepDescription().c_str();
    m_noStep = m_noStepParameter.getDestinationStepDescription().c_str();
    m_pendingStep = m_pendingStepParameter.getDestinationStepDescription().c_str();

    std::auto_ptr<TA_Base_Core::IProfile> iProfile(TA_Base_Core::ProfileAccessFactory::getInstance().getProfile(m_operatorProfileParameter.getNumericValue(), false));

    TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString(iProfile->getName().c_str());

    int appTypeIdx = m_operatorProfileCombo.FindStringExact(-1, uniStr.c_str());
    if (appTypeIdx != CB_ERR)
    {
        m_operatorProfileCombo.SetCurSel(appTypeIdx);
    }

    UpdateData(FALSE);

    FUNCTION_EXIT;
}


void CPlanStepEditPopupMFTMessagePage::UpdateParameters()
{
    FUNCTION_ENTRY( "UpdateParameters" );

    PopupMFTMessageStep* step = dynamic_cast<PopupMFTMessageStep*>(GetPlanStep());
    TA_ASSERT(step!=NULL,"Incorrect step type for the decision step page");

    UpdateData(TRUE);

    TAUnicodeString uniStr = m_decisionMessage;

    step->getMessageParameter()->setText(uniStr.toMultiByteStdString());
    step->getYesParameter()->setStepId(m_yesStepParameter.getStepId());
    step->getNoParameter()->setStepId(m_noStepParameter.getStepId());
    step->getPendingParameter()->setStepId(m_pendingStepParameter.getStepId());

    int appTypeIdx = m_operatorProfileCombo.GetCurSel();
    if (appTypeIdx != CB_ERR)
    {
        step->getOperatorProfile()->setValue(m_operatorProfileCombo.GetItemData(appTypeIdx));
    }

    FUNCTION_EXIT;
}


#endif // defined (STEPTYPE_POPUP_MFT_MESSAGE)
