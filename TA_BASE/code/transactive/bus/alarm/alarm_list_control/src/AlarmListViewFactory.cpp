
#include "AlarmListViewFactory.h"

#include "bus\alarm\alarm_list_control\src\CommentDialog.h"
#include "bus\alarm\alarm_list_control\src\CSuppressAlarmConfirmDlg.h"
#include "bus\alarm\alarm_list_control\src\EditCommentDialog.h"
#include "bus\alarm\alarm_list_control\src\locationselectiondialog.h"
#include "bus\alarm\alarm_list_control\src\PlanAssociationDialog.h"
#include "bus\alarm\alarm_list_control\src\preconflickdialog.h"
#include "bus\alarm\alarm_list_control\src\SuppressAlarmDialog.h"
																							
namespace TA_Base_Bus																		
{																							
																							
	AlarmListViewFactory::AlarmListViewFactory(TA_Base_Bus::ITransActiveApp* winApp): m_winApp(winApp)
	{

	}

	TA_Base_Bus::ITransActiveView * AlarmListViewFactory::createView(const char * viewType) 
	{
		TA_Base_Bus::ITransActiveView * pRetVal = 0;
		if (viewType == ViewType_Alarm_ViewComment)
		{
			pRetVal = new CommentDialog(m_winApp);
		}
		else if (viewType == ViewType_PreConflict)
		{
			pRetVal = new PreConflickDialog(m_winApp);
		}
		else if (viewType == ViewType_PlanAssociation)
		{
			pRetVal = new PlanAssociationDialog(m_winApp);
		}
		else if (viewType == ViewType_Suppression)
		{
			pRetVal = new SuppressAlarmDialog(m_winApp);
		}
		else if (viewType == ViewType_ConfirmSupression)
		{
			pRetVal = new CSuppressAlarmConfirmDlg(m_winApp);
		}
		else if (viewType == ViewType_EditComment)
		{
			pRetVal = new EditCommentDialog(m_winApp);
		}
		else if (viewType == ViewType_LocationSelDlg)
		{
			pRetVal = new LocationSelectionDialog(m_winApp);
		}

		return pRetVal;
	}



} 


