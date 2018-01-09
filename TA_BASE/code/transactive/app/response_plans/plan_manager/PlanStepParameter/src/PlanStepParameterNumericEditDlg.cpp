//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameterNumericEditDlg.cpp $
// @author:  Andy Parker
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#include "app/response_plans/plan_manager/src/StdAfx.h"
#include "app/response_plans/plan_manager/src/planmanager.h"
#include "PlanStepParameterNumericEditDlg.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/NumericStepParameter.h"
#include "core/utilities/src/TAAssert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterNumericEditDlg dialog

CPlanStepParameterNumericEditDlg::CPlanStepParameterNumericEditDlg(NumericStepParameter& parameter, bool canEdit)
    : CPlanStepParameterEditDlg(parameter, CPlanStepParameterNumericEditDlg::IDD, canEdit)
{
    FUNCTION_ENTRY("CPlanStepParameterNumericEditDlg");

    // {{AFX_DATA_INIT(CPlanStepParameterNumericEditDlg)
    m_units = _T("");
    m_value = _T("");
    // }}AFX_DATA_INIT

    FUNCTION_EXIT;
}


void CPlanStepParameterNumericEditDlg::DoDataExchange(CDataExchange* pDX)
{
    FUNCTION_ENTRY("DoDataExchange");

    CPlanStepParameterEditDlg::DoDataExchange(pDX);
    // {{AFX_DATA_MAP(CPlanStepParameterNumericEditDlg)
    DDX_Text(pDX, ID_VALUE_UNITS, m_units);
    DDX_Text(pDX, IDC_PARAMETER_VALUE, m_value);
    // }}AFX_DATA_MAP

    FUNCTION_EXIT;
}


BEGIN_MESSAGE_MAP(CPlanStepParameterNumericEditDlg, CPlanStepParameterEditDlg)
    // {{AFX_MSG_MAP(CPlanStepParameterNumericEditDlg)
    ON_BN_CLICKED(IDOK, onOK)
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterNumericEditDlg message handlers

//DEL void CPlanStepParameterNumericEditDlg::setUnitLabel(const char *unitName)
//DEL {
//DEL     m_units = unitName;
//DEL }

void CPlanStepParameterNumericEditDlg::onOK()
{
    FUNCTION_ENTRY("onOK");

    UpdateData(true);
    NumericStepParameter* numericParameter = dynamic_cast<NumericStepParameter*>(GetStepParameter());
    TA_ASSERT(numericParameter != NULL, "Not a numeric step parameter");
    numericParameter->setValue(_ttol(m_value));

    // Normal exit
    EndDialog(IDOK);

    FUNCTION_EXIT;
}


BOOL CPlanStepParameterNumericEditDlg::OnInitDialog()
{
    FUNCTION_ENTRY("OnInitDialog");

    NumericStepParameter* numericParameter = dynamic_cast<NumericStepParameter*>(GetStepParameter());
    TA_ASSERT(numericParameter != NULL, "Not a numeric step parameter");

    char buffer[33];
    _ltoa(numericParameter->getNumericValue(), buffer, 10);

    m_value = buffer;

    FUNCTION_EXIT;
    return CPlanStepParameterEditDlg::OnInitDialog();
}


