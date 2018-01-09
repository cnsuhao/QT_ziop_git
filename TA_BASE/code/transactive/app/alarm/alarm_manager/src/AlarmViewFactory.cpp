
#include "AlarmViewFactory.h"
#include "AlarmsGuiDlg.h"
#include "AlarmFilterDlg.h"
#include "multipleselguidlg.h"
#include "suppressionrulesguidlg.h"

#include "globals.h"

namespace TA_Base_App
{


	AlarmViewFactory::AlarmViewFactory(TA_Base_Bus::ITransActiveApp* winApp): m_winApp(winApp)
	{

	}

	TA_Base_Bus::ITransActiveView * AlarmViewFactory::createView(const char * viewType) 
	{
		TA_Base_Bus::ITransActiveView * viewRetVal = 0;

		if (viewType==ViewType_AlarmManager)
		{
			viewRetVal = new AlarmsGuiDlg(m_winApp);
			viewRetVal->setMainView(true);
			
		}
		else if (viewType == ViewType_AlarmFilter)
		{
			viewRetVal = new AlarmFilterDlg(m_winApp);
			
		}
		else if (viewType == ViewType_MultiSel)
		{
			viewRetVal = new multipleSelGuiDlg(m_winApp);
		}
		else if (viewType == ViewType_SuppressionRule)
		{
			viewRetVal = new SuppressionRulesGUIDlg(m_winApp);
		}

		return viewRetVal;
	}



} 


