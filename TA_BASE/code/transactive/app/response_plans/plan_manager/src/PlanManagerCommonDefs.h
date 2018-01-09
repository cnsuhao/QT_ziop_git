//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerCommonDefs.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2017/05/22 15:28:33 $
// Last modified by:  $Author: hoa.le $
//
// Definitions commonly used by various Plan Manager classes.

#if !defined(AFX_PLANMANAGERCOMMONDEFS_H__9A958DFD_A9E3_46FC_8EF8_61E132D3C1D0__INCLUDED_)
#define AFX_PLANMANAGERCOMMONDEFS_H__9A958DFD_A9E3_46FC_8EF8_61E132D3C1D0__INCLUDED_


#include <vector>
#include "boost/shared_ptr.hpp"
#include "bus/response_plans/active_plans_display/src/ActivePlansDisplayCommonDefs.h"

#include "app/response_plans/plan_manager/pmodel/src/StringDefn.h"

// View Type Identification
#define VIEWTYPE_PLANMANAGER	"PlanManager"
#define VIEWTYPE_ALLPLANTREEFRAME	"AllPlanTreeFrame"
#define VIEWTYPE_APPROVEDPLANTREEFRAME	"ApprovedPlanTreeFrame"
#define VIEWTYPE_UNAPPROVEDPLANTREEFRAME	"UnapprovedPlanTreeFrame"
#define VIEWTYPE_ACTIVEPLANLISTFRAME	"ActivePlanListFrame"

#define VIEWID_ALLPLANTREEFRAME	"AllPlanTreeFrameID"
#define VIEWID_APPROVEDPLANTREEFRAME	"ApprovedPlanTreeFrameID"
#define VIEWID_UNAPPROVEDPLANTREEFRAME	"UnapprovedPlanTreeFrameID"
#define VIEWID_ACTIVEPLANLISTFRAME	"ActivePlanListFrameID"

// Main Window subtitles
#define VIEWTITLE_ALLPLAN "All Plans"
#define VIEWTITLE_APPROVEDPLAN "Approved Plans"
#define VIEWTITLE_UNAPPROVEDPLAN "Unapproved Plans"
#define VIEWTITLE_ACTIVEPLAN "Active Plans"


typedef enum
{
	PT_ALLPLANS,
	PT_APPROVEDPLANS,
	PT_UNAPPROVEDPLANS,
	PT_ACTIVEPLANS,
	PT_UNDEFINED
}
EPlanTreeType;


namespace TA_Base_App
{

    // Custom window messages
    #define WM_WINDOW_HAS_BECOME_VISIBLE                (WM_USER + 510)
    #define WM_RESET_TREE_NODE_VIEW                     (WM_USER + 520)
    #define WM_TREE_NODE_SELECTION_CHANGED              (WM_USER + 530) // OnTreeNodeSelectionChanged
    #define WM_CATEGORY_VIEW_SELECTION_CHANGED          (WM_USER + 531)
    #define WM_ADJUST_CONTROL_POSITIONS                 (WM_USER + 535)
    #define WM_ADJUST_PLAN_FILTER_BUTTON_STATES         (WM_USER + 536)
    #define WM_PLAN_DETAIL_PAGE_SELECTION_CHANGED       (WM_USER + 560)
    #define WM_INTERACTIVE_STEP_EXEC_REQUEST            (WM_USER + 570)
    #define WM_PLAN_CUSTOMISATION_UPDATE                (WM_USER + 580)
    #define WM_REFRESH_PLAN_GENERAL_DETAIL              (WM_USER + 610)
    #define WM_REFRESH_PLAN_STEP_LIST                   (WM_USER + 620)
    #define WM_REFRESH_PLAN_STEP_PARAMETER_LIST         (WM_USER + 630)
    #define WM_SELECT_ACTIVE_PLAN_INSTANCE              (WM_USER + 640)
    #define WM_INTERACTIVE_STEP_EXECUTION_INTERRUPTED   (WM_USER + 650)
    #define WM_DISPLAY_STEP_PARAMETERS                  (WM_USER + 660)
    #define WM_CONTROLLED_PLAN_APPROVAL_STATE_CHANGED   (WM_USER + 670)
    #define WM_PLAN_STEP_FAILED                         (WM_USER + 690)

} // namespace TA_Base_App


#endif // !defined(AFX_PLANMANAGERCOMMONDEFS_H__9A958DFD_A9E3_46FC_8EF8_61E132D3C1D0__INCLUDED_)
