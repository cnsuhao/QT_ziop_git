/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/NotifyGUIPostMessage.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef NOTIFY_GUI_POST_MESSAGE_0123456789_H
#define NOTIFY_GUI_POST_MESSAGE_0123456789_H
#include "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"
#include "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/GenericEventArgs.h"


namespace TA_Base_App
{
	class NotifyGUIPostMessage
	{
	public:
		static NotifyGUIPostMessage& getInstance();	
		static void releaseInstance();
		void setPModel(EventViewerDlgPModel* pmodel);
		void PostMessageToMainDlg(UINT messageId, WPARAM wParam, LPARAM lParam );
		void showUserMsg(int message, std::string param1 = "", std::string param2 = "");
		virtual ~NotifyGUIPostMessage();
	private:
		NotifyGUIPostMessage();
		static NotifyGUIPostMessage* m_instance;
		EventViewerDlgPModel * m_mainPModel;
	};
	
}
#endif  // NOTIFY_GUI_POST_MESSAGE_0123456789_H