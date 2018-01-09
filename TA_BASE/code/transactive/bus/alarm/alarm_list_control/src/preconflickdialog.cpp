
#include "preconflickdialog.h"
#include "bus/alarm/alarm_list_control/src/ConstDefine.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/IDL/src/CommonExceptionsCorbadef.h"

#include "bus/alarm/alarm_list_control/pmodel/src/PreConflictAckDlgModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"
#include <QDateTime>

namespace TA_Base_Bus
{
	PreConflickDialog::PreConflickDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_PreConflict)
	{
		 
		ui.setupUi(this);

		this->m_pModel = new PreConflictAckDlgModel(app->getApplicationController(), PMODEL_PRE_CONFLICT_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);

		// Setup Connect Slots
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	}

	PreConflickDialog::~PreConflickDialog()
	{

	}

	std::string  PreConflickDialog::getMyPModelName(void)
	{
		return  "";
	}
	void PreConflickDialog::setupPModel(void)
	{
		//m_realPModel = dynamic_cast<AlarmGUIDlgModel*> (m_pModel);
	}

	// For dynamic language switching
	void PreConflickDialog::translateUI()
	{
		ui.retranslateUi(this);
	}

	void PreConflickDialog::setAlarmDetail(TA_Base_Bus::AlarmDataItem* details)
	{
		m_details = details;
		//return this->exec();
	}

	void PreConflickDialog::OnInitDialog()
	{
		FUNCTION_ENTRY("OnInitDialog");
		//	std::string parameters = m_details->alarmParameterList;
		PreConflictAckDlgModel* pTempModel = dynamic_cast<PreConflictAckDlgModel*>(m_pModel);
		ConflictingTrains trains = pTempModel->GetAlarmParameters(m_details);
		
		TAUtfString radio1Text = TAUtfString(tr("Train: ")) + TAUtfString::createFromMultitByteString(trains.offendingTrainString);
		TAUtfString radio2Text = TAUtfString(tr("Train: ")) + TAUtfString::createFromMultitByteString(trains.owningTrainString);
		
		m_train1 = trains.offendingTrain;
		m_train2 = trains.owningTrain;

		ui.rdoTrain1->setText(radio1Text.std_str());
		ui.rdoTrain2->setText(radio2Text.std_str());

		ui.rdoTrain2->setChecked(true);
		ui.rdoTrain2->setChecked(false);

		ui.rdoTrain2->setEnabled(true);
		ui.rdoTrain2->setEnabled(true);

		ui.btnOk->setEnabled(true);
		ui.btnCancel->setEnabled(true);

		FUNCTION_EXIT;
	}

	void PreConflickDialog::OnOK()
	{
		// Get the train to deactivate from the checkboxes
		int trainToDeactivate = 0;

		if (ui.rdoTrain2->isChecked() == 1)
		{
			trainToDeactivate = m_train1;
			//deactivate train 1;
		}
		else
		{
			trainToDeactivate = m_train2;
			//deactivate train 2;
		}
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "trainToDeactivate =%d", trainToDeactivate);//CL-20990++ TestLog
		bool bSuccess = false;
		try
		{

			PreConflictAckDlgModel* pTempModel = dynamic_cast<PreConflictAckDlgModel*>(m_pModel);
			pTempModel->deactivateTrain(trainToDeactivate);
			bSuccess = true;
		}
		// catch any exceptions and display the error to the operator
		catch (TA_Base_Core::TransactiveException& e)
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			TAUtfString description = tr("Deactivate Train: ") + e.what();
			userMsg << description.std_str();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_140037);
		}
		catch (TA_Base_Core::AccessDeniedException&)
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			TAUtfString description = tr("Deactivate Train: Access Denied");
			userMsg << description.std_str();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_140037);
		}
		catch (...)
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			TAUtfString description = tr("Deactivate Train");
			userMsg << description.std_str();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_140037);
		}

		if (bSuccess)
			return; // Do not close the window if error happends

		this->setResult(QDialog::Accepted);
		QDialog::close();
	}

	void PreConflickDialog::OnCancel()
	{
		// TODO: Add extra cleanup here
		this->setResult(QDialog::Rejected);
		QDialog::close();
	}

}

