//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameterYesNoEditDlg.cpp $
// @author:  Andy Parker
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#include "app/response_plans/plan_manager/src/StdAfx.h"
#include "app/response_plans/plan_manager/src/PlanManager.h"
#include "PlanStepParameterYesNoEditDlg.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/YesNoStepParameter.h"
#include "core/utilities/src/TAAssert.h"
#include "HelperMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterYesNoEditDlg dialog

CPlanStepParameterYesNoEditDlg::CPlanStepParameterYesNoEditDlg(YesNoStepParameter& parameter, bool canEdit)
    : CPlanStepParameterEditDlg(parameter, CPlanStepParameterYesNoEditDlg::IDD, canEdit)
{
    FUNCTION_ENTRY("CPlanStepParameterYesNoEditDlg");

    // {{AFX_DATA_INIT(CPlanStepParameterYesNoEditDlg)
    // NOTE: the ClassWizard will add member initialization here
    // }}AFX_DATA_INIT

    FUNCTION_EXIT;
}


void CPlanStepParameterYesNoEditDlg::DoDataExchange(CDataExchange* pDX)
{
    FUNCTION_ENTRY("DoDataExchange");

    CPlanStepParameterEditDlg::DoDataExchange(pDX);
    // {{AFX_DATA_MAP(CPlanStepParameterYesNoEditDlg)
    DDX_Control(pDX, IDC_PARAMETER_VALUE, m_valueCheck);
    // }}AFX_DATA_MAP

    FUNCTION_EXIT;
}


BEGIN_MESSAGE_MAP(CPlanStepParameterYesNoEditDlg, CPlanStepParameterEditDlg)
    // {{AFX_MSG_MAP(CPlanStepParameterYesNoEditDlg)
    ON_BN_CLICKED(IDOK, onOK)
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterYesNoEditDlg message handlers

void CPlanStepParameterYesNoEditDlg::onOK()
{
    FUNCTION_ENTRY("onOK");

    YesNoStepParameter* yesnoParameter = dynamic_cast<YesNoStepParameter*>(GetStepParameter());
    TA_ASSERT(yesnoParameter != NULL, "Not a yes/no step parameter");

    yesnoParameter->setAffirmative(m_valueCheck.GetCheck() == 1);

    EndDialog(IDOK);

    FUNCTION_EXIT;
}


BOOL CPlanStepParameterYesNoEditDlg::OnInitDialog()
{
    FUNCTION_ENTRY("OnInitDialog");

    CPlanStepParameterEditDlg::OnInitDialog();

    YesNoStepParameter* yesnoParameter = dynamic_cast<YesNoStepParameter*>(GetStepParameter());
    TA_ASSERT(yesnoParameter != NULL, "Not a yes/no step parameter");

    m_valueCheck.SetWindowText(SCOPEDMTOW(yesnoParameter->getName().c_str()));
    m_valueCheck.SetCheck(yesnoParameter->getAffirmative() ? 1 : 0);

    FUNCTION_EXIT;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


