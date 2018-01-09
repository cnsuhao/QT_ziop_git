#if !defined(AlarmListViewFactory_H)
#define AlarmListViewFactory_H

#include "ConstDefine.h"
#include "CommentDialog.h"
#include "preconflickdialog.h"
#include "SuppressAlarmDialog.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
 

namespace TA_Base_Bus
{

	class AlarmListViewFactory : public TA_Base_Bus::IViewFactory
	{	

	public:

		AlarmListViewFactory(TA_Base_Bus::ITransActiveApp* winApp);
		virtual ~AlarmListViewFactory(){};
		/** get the view based on specific name
		* 
		*
		* @param viewType
		* @param viewID
		* @return ITransActiveView *
		*/
		TA_Base_Bus::ITransActiveView * AlarmListViewFactory::createView(const char * viewType);

	private:
		TA_Base_Bus::ITransActiveApp* m_winApp;
		 

	};

}; // namespace TA_Base_App

#define GET_ALARM_VIEW( result, winApp, viewType, viewID, parameters )						\
{																							\
	try																						\
	{																						\
		if (viewType == ViewType_Alarm_ViewComment)											\
		{																					\
			result = new TA_Base_Bus::CommentDialog(winApp);								\
			result parameters;																\
		}																					\
		else if (viewType == ViewType_PreConflict)											\
		{																					\
			result = new TA_Base_Bus::PreConflickDialog(winApp);							\
			result parameters;																\
		}																					\
																							\
		if (result != 0)																	\
		{																					\
			result->setViewID(viewID);														\
			result->init();																	\
			winApp->getViewManager()->addView(result);										\
		}																					\
	}																						\
	catch(...)  																			\
	{																						\
		/*Nothing for now*/																	\
	}																						\
}																							

#endif