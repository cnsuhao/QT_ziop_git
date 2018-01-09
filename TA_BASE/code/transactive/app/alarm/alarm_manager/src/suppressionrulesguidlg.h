#ifndef ALARMRULESGUIDLG_H
#define ALARMRULESGUIDLG_H

#include <QWidget>
#include <QPixmap>
#include "app\alarm\alarm_manager\GeneratedFiles\ui_suppressionrulesguidlg.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "app\alarm\alarm_manager\pmodel\src\SuppressionRulesDlgPModel.h"

//Q_DECLARE_METATYPE(TA_Base_App::RuleInfo);
Q_DECLARE_METATYPE(TA_Base_App::RuleInfo*);
//Q_DECLARE_OPAQUE_POINTER(TA_Base_App::RuleInfo*);

namespace TA_Base_App
{

	class SuppressionRulesGUIDlg 
		: public TA_Base_Bus::AbstractDialog
	{
		Q_OBJECT

	public:
		SuppressionRulesGUIDlg(TA_Base_Bus::ITransActiveApp* app);
		~SuppressionRulesGUIDlg();

		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();
	protected:
		virtual void initUIControl();
		virtual void OnInitDialog();
	public slots:
		void OnRemoveButton();
		void OnRadioFilterLocation(bool checked);
		void OnRadioShowAll(bool checked);
		void OnClickRulesList();
		void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
		void OnClose();
		void OnDestroy();
		void OnRefresh();
		void OnExport();
		void OnPrint();
		void OnTimer(UINT nIDEvent);
		void OnSelchangeComboLocations(int index);

	private:
		void setUpHeadings();
		void populateLocationCombo();
		void loadAllRules();
		void loadRulesByLocation(unsigned long locationKey);
		void displayErrorMessage();
		void populateList(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);
		void clearList();
		void sortList(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);

	private:
		Ui::suppressionRulesGUIDlg ui;
		SuppressionRulesDlgPModel * m_pTheRealPModel;

		QPixmap* m_blankImg;
		QPixmap* m_removableImg;
	};

}

#endif // ALARMRULESGUIDLG_H
