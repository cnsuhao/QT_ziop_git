#include "SuppressAlarmDialog.h"
#include "bus/alarm/alarm_list_control/src/ConstDefine.h"
#include <QString>
#include <QRadioButton>

#include "bus/alarm/alarm_list_control/pmodel/src/SuppressAlarmDlgPModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"

#include "CSuppressAlarmConfirmDlg.h"
#include "bus\common_control\src\common\StringUtility.h"

namespace TA_Base_Bus
{
	SuppressAlarmDialog::SuppressAlarmDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_Suppression)
		, m_pTheRealPModel(0)
	{
		ui.setupUi(this);

		this->m_pModel = new SuppressAlarmDlgPModel(app->getApplicationController(), PMODEL_SUPPRESSION_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);
		
		//Setup Connection slots
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

	}

	SuppressAlarmDialog::~SuppressAlarmDialog()
	{

	}

	void SuppressAlarmDialog::initUIControl()
	{
		ui.rdoByAlarmType->setChecked(true);
	}

	std::string  SuppressAlarmDialog::getMyPModelName(void)
	{
		return  "";
	}
	void SuppressAlarmDialog::setupPModel(void)
	{
		m_pTheRealPModel = dynamic_cast<SuppressAlarmDlgPModel*> (m_pModel);
	}

	// For dynamic language switching
	void SuppressAlarmDialog::translateUI()
	{
		ui.retranslateUi(this);
	}

	void SuppressAlarmDialog::setAlarmDetails(boost::shared_ptr<TA_Base_Bus::AlarmDataItem> alarm, TAUtfString createdBySessionKey, unsigned long createdByProfileKey, TAUtfString createdByOperatorName)
	{
		m_pTheRealPModel->setAlarmDetails(alarm, 
			createdBySessionKey.toMultiByteStdString(), createdByProfileKey, 
			createdByOperatorName.toMultiByteStdString());

	}

	bool SuppressAlarmDialog::wasSuccessful()
	{
		return m_pTheRealPModel->wasSuccessful();
	}

	void SuppressAlarmDialog::inhibitAlarmByType()
	{
		ITransActiveView* pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_ConfirmSupression, "");
		CSuppressAlarmConfirmDlg* confirmDlg = dynamic_cast<CSuppressAlarmConfirmDlg*>(pView);
		
		TAUtfString confirmText;
		confirmText.fromMultiByteString(m_pTheRealPModel->getInhibitAlarmTypeConfirmText().c_str());
		confirmDlg->setConfirmationText(confirmText);

		int nRes = confirmDlg->doModal();
		if (nRes == QDialog::Rejected)
		{	
			if (m_pTheRealPModel->wasSuccessful())
				m_pTheRealPModel->setWasSuccessful(false);

			return;
		}

		m_pTheRealPModel->inhibitAlarmByType();
	}

	void SuppressAlarmDialog::inhibitAlarmByDescription()
	{
		ITransActiveView* pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_ConfirmSupression, "");
		CSuppressAlarmConfirmDlg* confirmDlg = dynamic_cast<CSuppressAlarmConfirmDlg*>(pView);
		TAUtfString confirmText = TAUtfString::createFromMultitByteString(m_pTheRealPModel->getInhibitAlarmByDescConfirmText());
		confirmDlg->setConfirmationText(confirmText);

		int nRes = confirmDlg->doModal();
		if (nRes == QDialog::Rejected)
		{
			if (m_pTheRealPModel->wasSuccessful())
				m_pTheRealPModel->setWasSuccessful(false);

			return;
		}

		m_pTheRealPModel->inhibitAlarmByType();
	}

	void SuppressAlarmDialog::inhibitAlarm()
	{
		FUNCTION_ENTRY("inhibitAlarm");

		if (ui.rdoByAlarmType->isChecked() == true)
		{
			inhibitAlarmByType();
		}
		else if (ui.rdoByDescription->isChecked() == true)
		{
			inhibitAlarmByDescription();
		}

		FUNCTION_EXIT;
	}

	void SuppressAlarmDialog::OnCancel()
	{
		this->reject();
	}

	void SuppressAlarmDialog::OnOK()
	{
		inhibitAlarm();
		*m_bResult.get() = m_pTheRealPModel->wasSuccessful();
		this->accept();
	}
}
