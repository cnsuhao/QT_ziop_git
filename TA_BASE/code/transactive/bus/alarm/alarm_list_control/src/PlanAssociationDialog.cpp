#include "PlanAssociationDialog.h"
#include "ConstDefine.h"
#include <QString>
#include <QListWidgetItem>

#include "bus/alarm/alarm_list_control/pmodel/src/PlanAssociationDlgPModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

namespace TA_Base_Bus
{
	PlanAssociationDialog::PlanAssociationDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_PlanAssociation)
	{
		ui.setupUi(this);

		this->m_pModel = new PlanAssociationDlgPModel(app->getApplicationController(), PMODEL_PLAN_ASSOC_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);

		// Setup Connect Slots
		connect(ui.planList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onDblclkAlarmplanlistbox(QListWidgetItem*)));
		connect(ui.planList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onSelchangeAlarmplanlistbox(QListWidgetItem *, QListWidgetItem *)));
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(onPlanlistboxOk()));
		connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(onPlanlistboxCancel()));
	}

	PlanAssociationDialog::~PlanAssociationDialog()
	{

	}

	//ITransActiveView interface
	std::string PlanAssociationDialog::getMyPModelName(void)
	{
		return 	PMODEL_PLAN_ASSOC_DLG;
	}
	
	void PlanAssociationDialog::setupPModel(void)
	{
		m_pTheRealModel = dynamic_cast<PlanAssociationDlgPModel*>(m_pModel);
	}

	// For dynamic language switching
	void PlanAssociationDialog::translateUI()
	{
		ui.retranslateUi(this);
	}

	void PlanAssociationDialog::initUIControl()
	{	// Initialize UI Controls
		TA_Base_Core::PlanDetailsList& objPlan = m_pTheRealModel->getPlanDetailList();
		for (TA_Base_Core::PlanDetailsList::iterator iter = objPlan.begin(); iter != objPlan.end(); ++iter)
		{
			ui.planList->addItem(*iter);
		}

	}

	void PlanAssociationDialog::onPlanlistboxOk()
	{
		FUNCTION_ENTRY("onPlanlistboxOk");

		QListWidgetItem * selected = ui.planList->currentItem();
		if (selected == NULL)
			return;
		
		TAUtfString actionName = tr("run selected plan");
		//actionName.LoadString(IDS_RUN_SELECTED_PLAN);
		TA_Base_Bus::TransActiveMessage userMsg;
		userMsg << actionName.std_str();
		UINT selectedButton = userMsg.showMsgBox(IDS_UW_010003);
		if (selectedButton == IDNO)
		{
			return;
		}

		std::string planPath = selected->text().toStdString();
		m_pTheRealModel->setSelectedPlanPath(planPath);

		QDialog::accept();
	}

	void PlanAssociationDialog::onPlanlistboxCancel()
	{
		QDialog::reject();
		this->closeView();
	}

	void PlanAssociationDialog::onDblclkAlarmplanlistbox(QListWidgetItem* pItem)
	{
		onPlanlistboxOk();
		this->closeView();
	}

	void PlanAssociationDialog::onSelchangeAlarmplanlistbox(QListWidgetItem * current, QListWidgetItem * previous)
	{	
		if (current == NULL)
		{
			ui.btnOk->setEnabled(false);
		}
		else
		{
			ui.btnOk->setEnabled(true);
		}
	}

	TAUtfString PlanAssociationDialog::getSelectedPlanPath()
	{
		TAUtfString retVal = TAUtfString::createFromMultitByteString(m_pTheRealModel->getSelectedPlanPath().c_str());
		return retVal;
	}
}
