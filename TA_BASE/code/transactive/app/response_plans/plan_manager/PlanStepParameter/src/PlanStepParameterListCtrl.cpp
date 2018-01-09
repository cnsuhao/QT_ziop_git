//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameterListCtrl.cpp $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#include "app/response_plans/plan_manager/src/stdafx.h"
#include "app/response_plans/plan_manager/src/resource.h"
#include "core/utilities/src/DebugUtil.h"
#include "app/response_plans/plan_manager/PlanStepParameter/src/PlanStepParameterListCtrl.h"
#include "app/response_plans/plan_manager/src/LabelMaker.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterConstantsDef.h"
#include "app/response_plans/plan_manager/PlanStepParameter/src/HelperMacro.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterListCtrl

CPlanStepParameterListCtrl::CPlanStepParameterListCtrl()
    : m_realPModel(NULL)
{
    FUNCTION_ENTRY("CPlanStepParameterListCtrl");
    FUNCTION_EXIT;
}


CPlanStepParameterListCtrl::~CPlanStepParameterListCtrl()
{
    FUNCTION_ENTRY("~CPlanStepParameterListCtrl");
    FUNCTION_EXIT;
}


BEGIN_MESSAGE_MAP(CPlanStepParameterListCtrl, CListCtrl)
    // {{AFX_MSG_MAP(CPlanStepParameterListCtrl)
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemChanging)
    ON_WM_DESTROY()
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanStepParameterListCtrl message handlers

void CPlanStepParameterListCtrl::OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
    FUNCTION_ENTRY("OnItemChanging");

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // TODO: Add your control notification handler code here

    // The parameter list is used for display purposes only, entry selection is redundant.
    *pResult = (pNMListView->uNewState & LVIS_SELECTED);

    FUNCTION_EXIT;
}


void CPlanStepParameterListCtrl::PreSubclassWindow()
{
    FUNCTION_ENTRY("PreSubclassWindow");

    // TODO: Add your specialized code here and/or call the base class
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);

    int subItem = 0;

    InsertColumn(subItem++, LabelMaker::getStepParameterListNameColumnLabel(), LVCFMT_LEFT, 200);
    InsertColumn(subItem++, LabelMaker::getStepParameterListValueColumnLabel(), LVCFMT_LEFT, 400);

    CListCtrl::PreSubclassWindow();
    createPModel();

    FUNCTION_EXIT;
}


BOOL CPlanStepParameterListCtrl::SetReadOnly(BOOL readOnly)
{
    FUNCTION_ENTRY("SetReadOnly");

    DWORD colorRef = ::GetSysColor(readOnly ? COLOR_3DFACE : COLOR_WINDOW);

    if(SetBkColor(colorRef) && SetTextBkColor(colorRef))
    {
        FUNCTION_EXIT;
        return TRUE;
    }

    FUNCTION_EXIT;
    return FALSE;
}


void CPlanStepParameterListCtrl::AddParameter(CString parameterName, CString parameterValue, PlanStepParameter* planStepParameter)
{
    FUNCTION_ENTRY("AddParameter");

    int item = GetItemCount();

    InsertItem(item, parameterName);
    SetItemText(item, 1, parameterValue);

    SetItemData(item, (DWORD) planStepParameter);

    FUNCTION_EXIT;
}



void CPlanStepParameterListCtrl::createPModel()
{
    m_realPModel = new CPlanStepParameterListCtrlModel();
    m_pModel = m_realPModel;
}

void CPlanStepParameterListCtrl::notifyUpdate(const char* updateType, TA_Base_Bus::EventArgs* args/*=NULL*/)
{
    if(strcmp(updateType, PARAMETER_ADD_NOTIFY) == 0)
    {
        CPlanStepParameterListCtrlModel::AddedParameterInfo newParam = m_realPModel->getCurrentAddedParameterInfo();
        CString name = SCOPEDMTOW(newParam.name.c_str());
        CString value = SCOPEDMTOW(newParam.value.c_str());
        AddParameter(name, value, newParam.planStep);
    }
    else if(strcmp(updateType, PARAMETER_ADD_ERROR_NOTIFY) == 0)
    {
        TA_Base_Bus::TransActiveMessage msg;
        CString param;
        param.LoadString(IDS_RETRIEVE_PARAMETER_VALUE);
        msg << param;
        TAUnicodeString error = msg.constructMessage(IDS_UE_010028);
        std::string currentPlanStepParameterName = m_realPModel->getCurrentOperationPlanStepParameter()->getName();
        AddParameter(SCOPEDMTOW(currentPlanStepParameterName.c_str()),
                     error.c_str(),
                     m_realPModel->getCurrentOperationPlanStepParameter());
    }
}

void CPlanStepParameterListCtrl::setupPModel()
{
    TA_ASSERT(m_pModel != NULL, "");
    m_pModel->attachView(this, PARAMETER_ADD_NOTIFY);
    m_pModel->attachView(this, PARAMETER_ADD_ERROR_NOTIFY);
}

void CPlanStepParameterListCtrl::OnDestroy()
{
    m_pModel->detachView(this);
    CListCtrl::OnDestroy();
}
