//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanInstanceComboBox.cpp $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#include "stdafx.h"
#include "core/utilities/src/TAAssert.h"
#include "bus/response_plans/active_plans_display/src/StringUtilities.h"
#include "bus/response_plans/active_plans_display/src/PlanAgentLocationAccess.h"
#include "PlanManager.h"
#include "PlanInstanceComboBox.h"
#include "PlanBrowserDoc.h"
#include "PlanNode.h"
#include "PlanInstance.h"
#include "PlanInstanceNone.h"
#include "PlanInstanceFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// CPlanInstanceComboBox

CPlanInstanceComboBox::CPlanInstanceComboBox() : m_currentSelection(NULL), m_planInstanceFilter(NULL)
{
    FUNCTION_ENTRY( "CPlanInstanceComboBox" );
    FUNCTION_EXIT;
}


CPlanInstanceComboBox::~CPlanInstanceComboBox()
{
    FUNCTION_ENTRY( "~CPlanInstanceComboBox" );
    FUNCTION_EXIT;
}


BEGIN_MESSAGE_MAP(CPlanInstanceComboBox, CComboBox)
    // {{AFX_MSG_MAP(CPlanInstanceComboBox)
    ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnItemSelected)
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanInstanceComboBox message handlers

BOOL CPlanInstanceComboBox::OnItemSelected()
{
    FUNCTION_ENTRY( "OnItemSelected" );

    // Ignore same-instance selections
    PlanInstance *newSelection = static_cast<PlanInstance *>(GetItemDataPtr(GetCurSel()));
    if (newSelection != m_currentSelection)
    {
        m_currentSelection = newSelection;

        FUNCTION_EXIT;
        return FALSE;
    }

    FUNCTION_EXIT;
    return TRUE; // Prevent the parent handling the notification
}


void CPlanInstanceComboBox::SetPlanInstanceFilter(PlanInstanceFilter *planInstanceFilter)
{
    FUNCTION_ENTRY( "SetPlanInstanceFilter" );

    m_planInstanceFilter = planInstanceFilter;

    FUNCTION_EXIT;
}


bool CPlanInstanceComboBox::IsNewInstance(PlanInstance *planInstance)
{
    FUNCTION_ENTRY( "IsNewInstance" );
    FUNCTION_EXIT;
    return (FindStringExact(0, GetPlanInstanceLabel(planInstance)) == CB_ERR);
}


void CPlanInstanceComboBox::AddInstance(PlanInstance *planInstance)
{
    FUNCTION_ENTRY( "AddInstance" );

    TA_ASSERT(planInstance, "Plan instance is null");

    // TD10997: Ignore any updates for a "running" plan where the current step state is not valid.
    const int nActivePlanState = planInstance->getActivePlanState();

    if ((( nActivePlanState == TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE ) || 
         ( nActivePlanState == TA_Base_Core::OPERATOR_PAUSED_ACTIVE_PLAN_STATE )) &&
         ( nActivePlanState == TA_Base_Core::UNDEFINED_ACTIVE_STEP_STATE ))
    {
        FUNCTION_EXIT;
        return;
    }

    // OK to add the specified instance.
    int insertPos = -1;

    for (int item = 0; item < GetCount(); item++)
    {
        PlanInstance *tmpInstance = static_cast<PlanInstance *>(GetItemDataPtr(item));

        if (planInstance == tmpInstance)
        {
            FUNCTION_EXIT;
            return;
        }

        if (*tmpInstance < *planInstance)
        {
            insertPos = item + 1;
        }
    }

    // Before the instance is added, make sure it meets the filtering criteria
    if (m_planInstanceFilter && !m_planInstanceFilter->isRelevant(*planInstance))
    {
        FUNCTION_EXIT;
        return;
    }

    insertPos = InsertString(insertPos, GetPlanInstanceLabel(planInstance));

    SetItemDataPtr(insertPos, planInstance);

    FUNCTION_EXIT;
}


void CPlanInstanceComboBox::RepositionInstance(PlanInstance *planInstance)
{
    FUNCTION_ENTRY( "RepositionInstance" );

    TA_ASSERT(planInstance, "Plan instance is null");

    CString instanceLabel = GetPlanInstanceLabel(planInstance);
    int item = FindStringExact(0, instanceLabel);

    if (item != CB_ERR)
    {
        // Assume no repositioning will be required
        bool currentPositionOK = true;

        // If there's a preceding instance in the list compare the two
        if (item > 0)
        {
            PlanInstance *instanceBefore = static_cast<PlanInstance *>(GetItemDataPtr(item - 1));

            currentPositionOK = (*instanceBefore < *planInstance);
        }

        // If OK wrt to preceding instance, compare with the one that follows (if any)
        if (currentPositionOK && item < (GetCount() - 1))
        {
            PlanInstance *instanceAfter = static_cast<PlanInstance *>(GetItemDataPtr(item + 1));

            currentPositionOK = (*planInstance < *instanceAfter);
        }

        // If the order has changed
        if (!currentPositionOK)
        {
            // Remove the instance and add it back in at the correct position
            DeleteString(item);
            AddInstance(planInstance);

            // If the instance was selected before moving then re-select. Don't call SelectItem()
            // as this selection should be transparent to the parent PlanStepDetailPage.
            if (planInstance == GetSelectedInstance())
            {
                item = FindStringExact(0, GetPlanInstanceLabel(planInstance));
                if (item != CB_ERR)
                {
                    SetCurSel(item);
                }
            }
        }
    }

    FUNCTION_EXIT;
}


void CPlanInstanceComboBox::RemoveInstance(const PlanInstanceId &planInstanceId)
{
    FUNCTION_ENTRY( "RemoveInstance" );

    int item = FindStringExact(0, GetPlanInstanceLabel(planInstanceId));

    if (item != CB_ERR)
    {
        void *removedInstance = GetItemDataPtr(item);

        DeleteString(item);

        // The removed PlanInstance object may have been deleted so it shouldn't be accessed.
        // It's still ok to compare pointers.
        if (removedInstance == GetSelectedInstance())
        {
            m_currentSelection = NULL;
            SelectItem(__min(item, GetCount() - 1));
        }
    }

    FUNCTION_EXIT;
}


void CPlanInstanceComboBox::Purge()
{
    FUNCTION_ENTRY( "Purge" );

    if (m_planInstanceFilter)
    {
        for (int item = GetCount() - 1; item >= 0; item--)
        {
            PlanInstance *planInstance = static_cast<PlanInstance *>(GetItemDataPtr(item));

            if (!m_planInstanceFilter->isRelevant(*planInstance))
            {
                bool isSelected = (item == GetCurSel());

                DeleteString(item);

                if (isSelected)
                {
                    m_currentSelection = NULL;

                    // Change the selection to the next closest item. SetCurSel() will not
                    // notify the parent PlanStepDetailPage of the change, so doing this
                    // will not result in unnecessary updates of the plan step list.
                    SetCurSel(__min(item, GetCount() - 1));
                }
            }
        }

        // If the instance that was selected before the purge is no longer in the list, the selection
        // would have been transferred to another item. Now is the time to let the parent PlanStepDetailPage
        // know to update the plan step list based on the new selection.
        if (!GetSelectedInstance())
        {
            SelectItem(GetCurSel());
        }
    }

    FUNCTION_EXIT;
}


void CPlanInstanceComboBox::SetSelectedInstance(PlanInstance *planInstance)
{
    FUNCTION_ENTRY( "SetSelectedInstance" );

    if (!planInstance)
    {
        SelectItem(0); // instance "None"
        FUNCTION_EXIT;
        return;
    }

    if (planInstance != GetSelectedInstance())
    {
        int item = FindStringExact(0, GetPlanInstanceLabel(planInstance));

        if (item != CB_ERR)
        {
            SelectItem(item);
        }
    }

    FUNCTION_EXIT;
}


PlanInstance *CPlanInstanceComboBox::GetSelectedInstance()
{
    FUNCTION_ENTRY( "GetSelectedInstance" );
    FUNCTION_EXIT;
    return m_currentSelection;
}


CString CPlanInstanceComboBox::GetPlanInstanceLabel(PlanInstance *planInstance)
{
    FUNCTION_ENTRY( "GetPlanInstanceLabel" );

    if (dynamic_cast<PlanInstanceNone *>(planInstance))
    {
        FUNCTION_EXIT;
        return "None";
    }

    FUNCTION_EXIT;
    return GetPlanInstanceLabel(planInstance->getInstanceId());
}


CString CPlanInstanceComboBox::GetPlanInstanceLabel(const PlanInstanceId &planInstanceId)
{
    FUNCTION_ENTRY( "GetPlanInstanceLabel" );

    CString planInstanceLabel;
    planInstanceLabel.Format("%d / %s",
                             planInstanceId.m_instance,
                             TA_Base_Bus::PlanAgentLocationAccess::getInstance().getLocationName(planInstanceId.m_location).c_str());

    FUNCTION_EXIT;
    return planInstanceLabel;
}


void CPlanInstanceComboBox::SelectItem(int item)
{
    FUNCTION_ENTRY( "SelectItem" );

    if (SetCurSel(item) != CB_ERR)
    {
        GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) GetSafeHwnd());
    }

    FUNCTION_EXIT;
}


