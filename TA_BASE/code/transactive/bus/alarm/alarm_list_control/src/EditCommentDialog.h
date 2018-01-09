#ifndef EDITCOMMENTDIALOG_H
#define EDITCOMMENTDIALOG_H

#include <QWidget>
#include "ui_EditCommentDialog.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"
#include "boost/shared_ptr.hpp"

namespace TA_Base_Bus
{
	// Forward declaration
	class EditCommentDlgPModel;

	class EditCommentDialog : public AbstractDialog
	{
		Q_OBJECT
	public:
		EditCommentDialog(TA_Base_Bus::ITransActiveApp* app);
		virtual ~EditCommentDialog();

		//ITransActiveView interface
		virtual std::string  getMyPModelName(void) { return ""; };
		virtual void setupPModel(void);
		virtual void OnInitDialog();

		// For dynamic language switching
		virtual void translateUI();
		/*
		* This is to set the comment edit box to the param comment
		*
		* @param comment string to be shown
		* @return void
		*
		*/
		void setEditComment(TAUtfString comment);

		/*
		* This is to get the comment edit box to the param comment
		*
		* @param none
		* @return string of the edited comment
		*
		*/
		TAUtfString getEditComment();

		void setResultData(boost::shared_ptr<TAUtfString> resultData) 
				{ m_resultData = resultData; };

	protected:
		virtual void initUIControl();
	public slots:
		void onOK();
		void onCancel();
		void onCommentChange();
	private:
		Ui::EditCommentDialog ui;
		EditCommentDlgPModel* m_pMyModel;
		boost::shared_ptr<TAUtfString> m_resultData;
	};

}

#endif // EDITCOMMENTDIALOG_H
