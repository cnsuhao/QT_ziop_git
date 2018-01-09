#ifndef ALARM_LIST_MODEL_CONSTS_H__
#define ALARM_LIST_MODEL_CONSTS_H__

#include <string>

 
 
namespace TA_Base_Bus
{
    #define PMODEL_ALARM_LIST_CONTROL "AlarmListControlModel"
	#define PMODEL_PRE_CONFLICT_DLG "PreConflictDlgModel"
	#define PMODEL_EDIT_COMMENT_DLG	"EditCommentDlgModel"
	#define PMODEL_LOCATION_SEL_DLG	"LocationSelectionDlgModel"
	#define PMODEL_PLAN_ASSOC_DLG	"PlanAssociationDlgModel"
	#define PMODEL_SUPPRESSION_DLG	"AlarmSuppressionDlgModel"

	#define DOMAIN_DATA_CHANGE_AlarmStore_Refresh "AlarmStoreDataUpdate"
    #define DOMAIN_DATA_CHANGE_AlarmStore_Restart "AlarmStoreRestart"
    #define DOMAIN_DATA_CHANGE_AlarmStore_Remap "AlarmStoreRemap"
	#define DOMAIN_DATA_CHANGE_AlarmStore_Unmap "AlarmStoreUnmap"
	 

	const int APP_NOTIFY_INHIBIT_ERROR = 1;  ///< confirm save
}
#endif