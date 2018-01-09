/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/CommentDialog.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #5 $
  *
  * Last modification: $DateTime: 2016/11/23 18:37:10 $
  * Last modified by:  $Author: Noel $
  * 
  * This class holds the comment dialog that allows the user to view comments (read-only)
  * The user can use this dialog to launch another dialog to edit comments.
  * There is also a button to allow users with appropriate privileges to delete comments.
  */


#include "bus/alarm/alarm_list_control/pmodel/src/CommentDlgModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/RightsChecker.h"
#include "bus/alarm/alarm_list_control/src/CommentDialog.h"
#include "bus/alarm/alarm_list_control/src/EditCommentDialog.h"
#include "bus/alarm/alarm_list_control/src/ConstDefine.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "bus/security/access_control/alarm_actions/src/AlarmActions.h"

#include "core/alarm/src/AlarmModificationHelper.h"
#include "core/alarm/src/AlarmHelperManager.h"

#include "core/exceptions/src/AlarmListCtrlException.h"
#include "core/exceptions/src/AlarmActionException.h"
#include "core/exceptions/src/NoAlarmAgentException.h"

#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::AlarmActionException;

 

namespace TA_Base_Bus
{
    // Ctor
	CommentDialog::CommentDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_Alarm_ViewComment)
		, m_alarmRetriever(0)
		   
    {
		 
		ui.setupUi(this);
		this->m_pModel =new CommentDlgModel(app->getApplicationController(),"CommentDlgModel");
		Qt::WindowFlags flags = windowFlags();
	    flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;		 
	    this->setWindowFlags(flags);
		//ui.btnClose->setControlID("aaaa");
	    //OnInitDialog();

		// Setup Connect Slots
		connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(OnCancel()));
		connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(OnEdit()));
		connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(OnDelete()));
    }


	// Dtor
	CommentDialog::~CommentDialog()
	{
		if (m_selectedAlarmDetails.size() > 0) DeleteAllAlarmsInVector();
	}


	//----AbstractTransActiveDialog----------------------------------------------------------------------------------

	std::string  CommentDialog::getMyPModelName(void)
	{
		return  "";
	}
	void CommentDialog::setupPModel(void)
	{
		//m_realPModel = dynamic_cast<AlarmGUIDlgModel*> (m_pModel);
	}

 

	void CommentDialog::DeleteAllAlarmsInVector()
	{
		// Cleanup alarm details in the vector
		std::vector<TA_Base_Bus::AlarmDataItem*>::iterator itr = m_selectedAlarmDetails.begin();
		for (itr; itr != m_selectedAlarmDetails.end(); itr++)
		{
			delete (*itr);
			(*itr) = NULL;
		}
		m_selectedAlarmDetails.clear();
	}
 
 
    void CommentDialog::OnInitDialog() 
    {
		FUNCTION_ENTRY("OnInitDialog");
		ui.txtComment->setText(m_strComment.q_str());

		RetrieveAlarms();
		EnableButtons();

		FUNCTION_EXIT;
	                  // EXCEPTION: OCX Property Pages should return FALSE
    }


    /////////////////////////////////////////////////////////////////////////////
	// Handle OK button press
	void CommentDialog::OnCancel()
    {
		FUNCTION_ENTRY("onCancel");

		QDialog::reject();
        this->closeView();
		
		FUNCTION_EXIT;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Handle Edit button press
	void CommentDialog::OnEdit()
    {
		FUNCTION_ENTRY("onEdit");
		if (m_selectedAlarmDetails.size() > 1)
		{
			char msg[250] = {0};
			TAUtfString format = tr("edit comments for %d alarms");
			sprintf(msg, format.std_str().c_str(), m_selectedAlarmDetails.size());
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << msg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);

			if (IDNO == selectedButton)
				return; // user canceled action
		}

		// Launch Edit Comment Dialog and update alarm comments if necessary
		ITransActiveView* pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_EditComment, "");
		EditCommentDialog* editDlg = dynamic_cast<EditCommentDialog*>(pView);
		if (m_selectedAlarmDetails.size() == 1) // only one alarm is selected
		{
			editDlg->setEditComment(m_strComment);
		}
		else if (m_selectedAlarmDetails.size() > 1) //multiple alarms selected
		{
			editDlg->setEditComment("");
		}

		boost::shared_ptr<TAUtfString> resultData(new TAUtfString());
		editDlg->setResultData(resultData);

		if (QDialog::Accepted == editDlg->doModal())
		{
			// Update the alarm in the agent
			setCursor(Qt::WaitCursor);

			try
			{
				
				m_strComment = (*resultData.get());//editDlg->getEditComment();
				ui.txtComment->setText(m_strComment.q_str());
				
				std::stringstream so;
				so << "Going to update alarm with comment: " << m_strComment.toMultiByteStdString();
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, so.str().c_str());

				UpdateAlarmComments(m_strComment.toMultiByteStdString());
			}
			catch (const TA_Base_Core::NoAlarmAgentException&)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "CommentDialogException", "Could not update the alarm's comment");

				//Todo: UE-040023
				TAUtfString actionName = tr("update comment");
				//actionName.LoadString(IDS_UPDATE_COMMENT);
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << actionName.std_str();
				UINT selectedButton = userMsg.showMsgBox(IDS_UE_040023);

				FUNCTION_EXIT;
				OnCancel(); // Close the dialog because we now don't really know the state of the alarms
			}

			setCursor(Qt::ArrowCursor);
		}

		FUNCTION_EXIT;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Handle Delete button press
	void CommentDialog::OnDelete()
    { 
		FUNCTION_ENTRY("OnDelete");
		// No comment means there is no comment to delete
		// No action required
		if (this->m_strComment.isEmpty() || this->m_strComment == "") return;

		if (m_selectedAlarmDetails.size() >= 1)
		{
			char msg[250] = { 0 };
			TAUtfString format = tr("delete comment for %d alarms");
			sprintf(msg, format.std_str().c_str(), m_selectedAlarmDetails.size());
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << msg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);

			if (IDNO == selectedButton)
			{
				FUNCTION_EXIT;
				return; // user canceled action
			}
		}

		// Delete alarm comments
		setCursor(Qt::WaitCursor);
		try
		{
			m_strComment = "";
			UpdateAlarmComments(m_strComment.toMultiByteStdString());
		}
		catch (const TA_Base_Core::NoAlarmAgentException&)
		{
			std::string actionName = "update comment";
			//actionName.LoadString(IDS_UPDATE_COMMENT);
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << actionName;
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_040023);

			FUNCTION_EXIT;
			OnCancel(); // close the dialog
		}

		FUNCTION_EXIT;
		OnCancel(); // close the dialog
    }


    /////////////////////////////////////////////////////////////////////////////
    // CommentDialog message helper functions
    /////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////
	// Retrieve the alarms and get the comment for the first alarm selected
	void CommentDialog::RetrieveAlarms()
	{
		FUNCTION_ENTRY("RetrieveAlarms");
		std::vector<std::string> selectedAlarmStrings = m_alarmRetriever->getSelectedAlarms();

		// One or Multiple alarms selected
		if (selectedAlarmStrings.size() >= 1)
		{
			// empty the selected alarm details before adding new alarm, if it is not empty
			if (m_selectedAlarmDetails.size() > 0)	DeleteAllAlarmsInVector();

			// Retrieve the alarm details for each of the alarms and add them to a vector
			for (std::vector<std::string>::iterator iter = selectedAlarmStrings.begin(); iter != selectedAlarmStrings.end(); ++iter)
			{
				try
				{
					TA_Base_Bus::AlarmDataItem* pAlarm = m_alarmRetriever->getAlarmItem(*iter);
					if (pAlarm != NULL)
					{
						m_selectedAlarmDetails.push_back(pAlarm);
					}
				}
				catch (...)
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "AlarmListCtrlException, Alarm comments could not be retrieved");
				}
			}
			//			m_selectedAlarmDetails.assign(selectedAlarmStrings.begin(), selectedAlarmStrings.end());
			TA_ASSERT(m_selectedAlarmDetails.size() == selectedAlarmStrings.size(), "Some of the alarm details failed to be retrieved");

		}
		// Either the selectedAlarmStrings size is 0 or something wrong with the retriever
		else
		{
			TA_THROW(AlarmActionException(AlarmActionException::CANNOT_FIND_ALARM));
		}
		
		FUNCTION_EXIT;
	}

    /////////////////////////////////////////////////////////////////////////////
	// Is the user permitted to modify this alarm?
	bool CommentDialog::IsActionPermitted(const TA_Base_Bus::AlarmDataItem* alarm) const
	{
        // Get the action we should be checking
        unsigned long action = TA_Base_Bus::AlarmActions::getAlarmAction(alarm->messageTypeKey,
                                                                    TA_Base_Bus::AlarmActions::COMMENT_ALARM);
        // Can we modify this alarm?
		return m_alarmRetriever->getRightsChecker().canPerformAction(alarm->assocEntityKey, action);
	}

    /////////////////////////////////////////////////////////////////////////////
	// Enable/disable the "edit" and "delete" buttons depending on the user's access
	void CommentDialog::EnableButtons()
	{
		FUNCTION_ENTRY("EnableButtons");
		// for one alarm selection
		if (m_selectedAlarmDetails.size() == 1)
		{
			bool editEnabled = IsActionPermitted(m_selectedAlarmDetails[0]);

			ui.btnEdit->setEnabled(editEnabled);
			ui.btnDelete->setEnabled(editEnabled);
		}
		// for multiple alarm selection
		// all alarms need to be check. throw the user out if any one of the alarm is not accessible
		else if (m_selectedAlarmDetails.size() > 1)// Error - no alarm details for selected alarms
		{
			bool editEnabled = false;

			for (int i = 0; i < static_cast<int> (m_selectedAlarmDetails.size()); ++i)
			{
				editEnabled = IsActionPermitted(m_selectedAlarmDetails[i]);

				if (editEnabled == false) {
					// Display a message to inform operator of his access limit
					std::string tempString = "You have no access to change comment for alarm: \r\n Alarm ID = ";
					tempString += m_selectedAlarmDetails[i]->alarmID.c_str();
					tempString += "\r\n Description = ";
					tempString += m_selectedAlarmDetails[i]->alarmDescription.c_str();
					tempString += "\r\n Edit and Delete is denied.";

					// TD14164 azenitha++
					TA_Base_Bus::TransActiveMessage userMsg;
					userMsg << tempString;
					//CString errMsg = userMsg.constructMessage(IDS_UW_050018);

					userMsg.showMsgBox(IDS_UW_050018, "Access Denied.");
					/*AfxMessageBox(tempString, MB_OK);*/
					//AfxMessageBox(errMsg, MB_OK);
					// TD14164 ++azenitha

					break;
				}
			}

			ui.btnEdit->setEnabled(editEnabled);
			ui.btnDelete->setEnabled(editEnabled);

		}
		// either no alarm selected or some thing is wrong with the retriever
		else
		{
			ui.btnEdit->setEnabled(false);
			ui.btnDelete->setEnabled(false);
		}

		FUNCTION_EXIT;
	}

    /////////////////////////////////////////////////////////////////////////////
	// Update the alarm comments for the selected alarms (obtained using the alarm
	// retriever). Set them to newCommentStr
	void CommentDialog::UpdateAlarmComments(const std::string& newCommentStr)
	{
		FUNCTION_ENTRY("UpdateAlarmComments");

		TA_ASSERT(m_selectedAlarmDetails.size() >= 1, "Attempted to update alarm comments but don't have any alarm details.")

		TA_Base_Core::AlarmHelperManager::getInstance().getModificationAlarmHelper();

		// Update the comments
		bool failureHappened = false;

		std::stringstream ss;
		std::vector<TA_Base_Core::AlarmModifyItem*> alarmModifyItems = m_alarmRetriever->getSelectedModItems();
		
		ss << "No of selected / retrieved alarm to update:" << alarmModifyItems.size();
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, ss.str().c_str());

		failureHappened = TA_Base_Core::AlarmHelperManager::getInstance().getModificationAlarmHelper().updateComments(alarmModifyItems, newCommentStr);

		m_alarmRetriever->freeAlarmModifyItems(alarmModifyItems);
		// Display message to inform user of failure to update comments
		if (failureHappened == true)
		{

			//UE-040023
			TAUtfString actionName = tr("Failed to update comments");
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << actionName.std_str();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_040023);

			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not update one or more alarms' comment");
			TA_THROW(AlarmActionException(AlarmActionException::CANNOT_UPDATE_ALARM_COMMENTS));
		}

		FUNCTION_EXIT;
	}

	// For dynamic language switching
	void CommentDialog::translateUI()
	{
		//step1 :translate components which are not in UI file
		//TODO
		//step2: call ui.retranslateUi
		ui.retranslateUi(this);
	}

}
