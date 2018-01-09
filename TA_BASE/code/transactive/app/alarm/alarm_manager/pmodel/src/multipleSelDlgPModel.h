
#ifndef __MULTIPLE_SELECTION_DLG_PMODEL_H_INCLUDED__
#define __MULTIPLE_SELECTION_DLG_PMODEL_H_INCLUDED__

#include "app/alarm/alarm_manager/pmodel/src/SubsytemDataCache.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
	class multipleSelDlgPModel
		: public TA_Base_Bus::AbstractPModel
	{
	public: 
		multipleSelDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~multipleSelDlgPModel();

		ListItems getListItems() const;
		ListItems getSelection() const;
		
		void setSelection(const ListItems& selection);
		void addSelectedItem(const std::string& selItem);
		void clearSelectedItem();

		void setItems(const ListItems& items, bool includeAllItem);
		void setSortItems(const bool sortItems);

		bool isSortItems() { return m_sortItems; }

		void setWindowTitle(const std::string& windowTitle) { m_windowTitle = windowTitle;  }
		std::string getWindowTitle() { return m_windowTitle; }

		bool isIncludeAll() { return m_includeAllItem; }

		//bool IsAllItemSelected() const;

	public:
		static const int LIST_ITEM_ALL_INDEX;
		static const std::string LIST_ITEM_ALL_STRING;

		// don't know if we really need this as we only use the sort during init dialog
		static const std::string EV_SORTCHANGE; 
	private:
		std::string m_windowTitle;

		// A map containing all items in the list
		ListItems m_listItems;

		// A map containing only the selected items (subset of m_listItems)
		ListItems m_selectedItems;

		// Does this control have an "<All>" item that returns an empty selection
		// when it is included in the selected items?
		// Note that the behaviour of the "<All>" item is slightly non-intuitive:
		// if the ALL item is selected then this dialog returns no selection.
		// This is because this dialog is used to filter out alarms and if ALL
		// is selected for a particular filter then no filter is required.
		bool m_includeAllItem;

		bool m_sortItems;
	};

}

#endif //__MULTIPLE_SELECTION_DLG_PMODEL_H_INCLUDED__