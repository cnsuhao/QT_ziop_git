#ifndef MULTIPLESELGUIDLG_H
#define MULTIPLESELGUIDLG_H

#include <QWidget>
#include "app\alarm\alarm_manager\GeneratedFiles\ui_multipleselguidlg.h"
#include "app\alarm\alarm_manager\pmodel\src\multipleseldlgpmodel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"

namespace TA_Base_App
{

	class multipleSelGuiDlg : public TA_Base_Bus::AbstractDialog
	{
		Q_OBJECT

	public:
		// Constructor
		multipleSelGuiDlg(TA_Base_Bus::ITransActiveApp* app);

		// Destructor
		virtual ~multipleSelGuiDlg() {};

		void setDialogTitle(const TA_Base_Bus::TAUtfString& title);
		void setItems(ListItems& items, bool includeAllItem);
		void setSelection(ListItems& selItems);
		ListItems getSelection() const;

		//ITransActiveView interface
		std::string  getMyPModelName(void);
		void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();

		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);
		
		void setSortItems(const bool sortItems);
	
		void setResult(boost::shared_ptr<ListItems> resultData)
				{ m_resultData = resultData; }

	private:
		// Helper function. Returns true IFF m_includeAllItem && the ALL item
		// is one of the selections
		bool IsAllItemSelected() const;

	protected:
		virtual void initUIControl();
		virtual void OnInitDialog();

	public slots:
		void OnMultiok();
		void OnCancel();
		void itemSelectionChanged();
	private:
		Ui::multipleSelGuiDlg ui;
		multipleSelDlgPModel * m_pMyPModel;
		boost::shared_ptr<ListItems> m_resultData;
	};

}

#endif // MULTIPLESELGUIDLG_H
