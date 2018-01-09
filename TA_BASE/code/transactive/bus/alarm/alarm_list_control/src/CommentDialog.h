/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/CommentDialog.h $
  * @author:  Karen Graham
  * @version: $Revision: #4 $
  *
  * Last modification: $DateTime: 2016/11/22 14:12:39 $
  * Last modified by:  $Author: Noel $
  * 
  * This class holds the comment dialog that allows the user to view and add/modify comments
  * The user can use this dialog to launch another dialog to edit comments.
  * There is also a button to allow users with appropriate privileges to delete comments.
  */

#if !defined(AFX_COMMENTDIALOG_H__3B556F26_E4E8_47F8_885F_E1D0DD5F8E5D__INCLUDED_)
#define AFX_COMMENTDIALOG_H__3B556F26_E4E8_47F8_885F_E1D0DD5F8E5D__INCLUDED_

 
#include "bus/alarm/alarm_list_control/src/IAlarmRetriever.h"
 

#include "bus/generic_gui_pmod/src/PModelInterface.h"

#include "bus\unicode_common_lib\src\TAUnicodeString.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#include "core\utilities\src\RunParams.h"
#include "core\synchronisation\src\ReEntrantThreadLockable.h"
 
#include "bus\alarm\alarm_list_control\GeneratedFiles\ui_CommentDialog.h"

namespace TA_Base_Bus
{
    class CommentDialog : public AbstractDialog
    {
		Q_OBJECT
    // Construction
    public:
	    CommentDialog(TA_Base_Bus::ITransActiveApp* app);   // standard constructor
		virtual ~CommentDialog();
 
	public:  //ITransActiveView interface
		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		void setAlarmRetriever(IAlarmRetriever* alarmRetriever) { m_alarmRetriever = alarmRetriever; };
		void setComment(const TAUtfString& comments) { m_strComment = comments; };

		// For dynamic language switching
		virtual void translateUI();
		virtual void OnInitDialog();

    // Implementation
	public slots:
	    void OnCancel();
	    void OnEdit();
	    void OnDelete();
		
	private:
		// Get the alarm details for the selected alarms.
		void RetrieveAlarms();

		// Is the user permitted to modify this alarm?
		bool IsActionPermitted(const TA_Base_Bus::AlarmDataItem* alarm) const;

		// Enable/disable the "edit" and "delete" buttons depending on the user's access
		void EnableButtons();

		// Update the alarm comments for the selected alarms (obtained using the alarm
		// retriever). Set them to newCommentStr
		void UpdateAlarmComments(const std::string& newCommentStr);

		// Delete all alarms that are currently stored to release the memory (obtained using the alarm
		// retriever).
		void DeleteAllAlarmsInVector();

	private:

		// The alarm retriever is used to extract the actual alarms from the DB
		IAlarmRetriever* m_alarmRetriever;

        // Vector of corba alarm defs for the selected alarms
		std::vector<TA_Base_Bus::AlarmDataItem*> m_selectedAlarmDetails;

        // This is the maximum length of the comment that can be stored in the database.
        //static const long MAXIMUM_COMMENT_LENGTH;
		Ui_CommentDlg  ui;

		// Display String
		TAUtfString m_strComment;
		
		// This is the maximum length of the comment that can be stored in the database.
		static const long MAXIMUM_COMMENT_LENGTH;
    };
 

}

#endif // !defined(AFX_COMMENTDIALOG_H__3B556F26_E4E8_47F8_885F_E1D0DD5F8E5D__INCLUDED_)
