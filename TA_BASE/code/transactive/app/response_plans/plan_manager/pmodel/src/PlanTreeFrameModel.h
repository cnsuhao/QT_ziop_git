//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeFrameModel.h $
// @author:  Bart Golab
// @version: $Revision: #5 $
//
// Last modification: $DateTime: 2012/10/08 17:44:33 $
// Last modified by:  $Author: qi.huang $
//
#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// PlanTreeFrameModel

	class PlanTreeFrameModel
		: public TA_Base_Bus::AbstractPModel
	{

	public:
		PlanTreeFrameModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		virtual ~PlanTreeFrameModel();

		void registerSettings();
		void deregisterSettings();
	};

} // namespace TA_Base_App