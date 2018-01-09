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

#include "app/event/event_viewer/pmodel/src/NotifyGUIPostMessage.h"


namespace TA_Base_App
{
	NotifyGUIPostMessage* NotifyGUIPostMessage::m_instance = NULL;
	NotifyGUIPostMessage& NotifyGUIPostMessage::getInstance()
	{
		if(m_instance == NULL)
		{
			m_instance = new NotifyGUIPostMessage();
		}
		return *m_instance;
	}

	void NotifyGUIPostMessage::releaseInstance()
	{
		if(m_instance != NULL)
		{
			delete m_instance;
			m_instance = NULL;
		}
	}

	NotifyGUIPostMessage::NotifyGUIPostMessage()
	{
	}

	NotifyGUIPostMessage::~NotifyGUIPostMessage()
	{
	}

	void NotifyGUIPostMessage::setPModel(EventViewerDlgPModel* pmodel)
	{
		m_mainPModel = pmodel;
	}

	void NotifyGUIPostMessage::PostMessageToMainDlg(UINT messageId, WPARAM wParam, LPARAM lParam )
	{
		GenericEventArgs * args = new GenericEventArgs();
		//UINT * messagePtr = new UINT();
		//*messagePtr = messageId;
		args->Param1 = &(messageId);
		args->Param2 = &(wParam);
		args->Param3 = &(lParam);
		m_mainPModel->postMessageNotifyViews(args);
		delete args;
		args = NULL;
	}

	void NotifyGUIPostMessage::showUserMsg(int message, std::string param1, std::string param2)
	{
		m_mainPModel->showUserMessageNotify(message,param1,param2);
	}


}