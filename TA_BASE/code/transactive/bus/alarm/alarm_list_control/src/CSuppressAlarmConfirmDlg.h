#ifndef CSUPPRESSALARMCONFIRMDLG_H
#define CSUPPRESSALARMCONFIRMDLG_H

#include <QWidget>
#include "ui_CSuppressAlarmConfirmDlg.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"

namespace TA_Base_Bus
{
	class CSuppressAlarmConfirmDlgPModel;

	class CSuppressAlarmConfirmDlg : public AbstractDialog
	{
		Q_OBJECT
	public:
		CSuppressAlarmConfirmDlg(TA_Base_Bus::ITransActiveApp* app);
		virtual ~CSuppressAlarmConfirmDlg();

	    //ITransActiveView interface
		virtual std::string  getMyPModelName(void) { return ""; };
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();

		void setConfirmationText(TAUtfString confirmText);

	public slots:
		void OnCancel();
		void OnOK();

	private:
		Ui::CSuppressAlarmConfirmDlg ui;
		CSuppressAlarmConfirmDlgPModel* m_pTheRealPModel;
	};
}
#endif // CSUPPRESSALARMCONFIRMDLG_H
