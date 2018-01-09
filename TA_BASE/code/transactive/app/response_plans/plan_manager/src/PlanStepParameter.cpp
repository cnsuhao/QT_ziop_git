//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameter.cpp $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#include "StdAfx.h"
#include "core/utilities/src/TAAssert.h"
#include "PlanStepParameter.h"
#include "PlanStep.h"
#include "PlanStepParameterEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

PlanStepParameter::PlanStepParameter(PlanStep& parent,std::string name) : m_name(name) , m_parent(parent)
{
    FUNCTION_ENTRY( "PlanStepParameter" );
    FUNCTION_EXIT;
}


PlanStepParameter::~PlanStepParameter()
{
    FUNCTION_ENTRY( "~PlanStepParameter" );
    FUNCTION_EXIT;
}


const std::string &PlanStepParameter::getName() const
{
    FUNCTION_ENTRY( "getName" );
    FUNCTION_EXIT;
    return m_name;
}


void PlanStepParameter::editParameter()
{
    FUNCTION_ENTRY( "editParameter" );

    CPlanStepParameterEditDlg* dlg = getEditDialog(true);
    TA_ASSERT(dlg!=NULL,"No dialog defined for step parameter editing.");
    dlg->DoModal();
    delete dlg;

    FUNCTION_EXIT;
}


void PlanStepParameter::viewParameter()
{
    FUNCTION_ENTRY( "viewParameter" );

    CPlanStepParameterEditDlg* dlg = getEditDialog(false);
    TA_ASSERT(dlg!=NULL,"No dialog defined for step parameter viewing.");
    dlg->DoModal();
    delete dlg;

    FUNCTION_EXIT;
}


