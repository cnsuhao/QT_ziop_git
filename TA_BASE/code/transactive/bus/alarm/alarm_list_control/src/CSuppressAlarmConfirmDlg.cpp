#include "CSuppressAlarmConfirmDlg.h"
#include "ConstDefine.h"
#include <QString>

#include "bus/alarm/alarm_list_control/pmodel/src/CSuppressAlarmConfirmDlgPModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"

namespace TA_Base_Bus
{

	CSuppressAlarmConfirmDlg::CSuppressAlarmConfirmDlg(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_ConfirmSupression)
	{
		ui.setupUi(this);
		
		this->m_pModel = new CSuppressAlarmConfirmDlgPModel(app->getApplicationController(), PMODEL_LOCATION_SEL_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);

		// Setup Connect Slots
		connect(ui.btnYes, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(ui.btnNo, SIGNAL(clicked()), this, SLOT(OnCancel()));
	}

	CSuppressAlarmConfirmDlg::~CSuppressAlarmConfirmDlg()
	{
	}

	void CSuppressAlarmConfirmDlg::setupPModel(void)
	{
		m_pTheRealPModel = dynamic_cast<CSuppressAlarmConfirmDlgPModel*>(m_pModel);
	}

	void CSuppressAlarmConfirmDlg::translateUI()
	{
		ui.retranslateUi(this);
	}

	void CSuppressAlarmConfirmDlg::OnCancel()
	{
		QDialog::reject();
		this->closeView();
	}

	void CSuppressAlarmConfirmDlg::OnOK()
	{
		QDialog::accept();
		this->closeView();
	}

	void CSuppressAlarmConfirmDlg::setConfirmationText(TAUtfString confirmText)
	{
		ui.txtConfirmMsg->setText(confirmText.toMultiByteStdString());
	}
}
