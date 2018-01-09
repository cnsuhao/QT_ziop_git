#ifndef ASSOCIATEDPLANS_H
#define ASSOCIATEDPLANS_H

#include <string>
#include <QDialog>
#include "ui_PlanAssociationDialog.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

namespace TA_Base_Bus
{
	// Forward declaration
	class PlanAssociationDlgPModel;

	class PlanAssociationDialog : public AbstractDialog
	{
		Q_OBJECT
	public:
		PlanAssociationDialog(TA_Base_Bus::ITransActiveApp* app);
		~PlanAssociationDialog();

		TAUtfString getSelectedPlanPath();

		//ITransActiveView interface
		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();
	protected:
		virtual void initUIControl();

	public slots:
		void onPlanlistboxOk();
		void onPlanlistboxCancel();
		void onDblclkAlarmplanlistbox(QListWidgetItem* pItem);
		void onSelchangeAlarmplanlistbox(QListWidgetItem * current, QListWidgetItem * previous);

	private:
		Ui::AssociatedPlans ui;
		PlanAssociationDlgPModel* m_pTheRealModel;
	};

}

#endif // ASSOCIATEDPLANS_H
