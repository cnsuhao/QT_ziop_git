//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanModel.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_ActivePlanModel_H__E9ACE209_C546_4AC6_9197_81EAC428667D__INCLUDED_)
#define AFX_ActivePlanModel_H__E9ACE209_C546_4AC6_9197_81EAC428667D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlanBrowserService.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // ActivePlanModel document

    class ActivePlanModel : public PlanBrowserService,
							public TA_Base_Bus::AbstractPModel
    {

    public:
		ActivePlanModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);           // protected constructor used by dynamic creation
		virtual ~ActivePlanModel();

		void registerSettings();
		void deregisterSettings();

    };

} // namespace TA_Base_App

#endif // !defined(AFX_ActivePlanModel_H__E9ACE209_C546_4AC6_9197_81EAC428667D__INCLUDED_)
