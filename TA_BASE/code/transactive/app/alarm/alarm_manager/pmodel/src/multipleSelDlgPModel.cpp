
#include "multipleSelDlgPModel.h"
#include "core/utilities/src/DebugUtil.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmFilterService.h"

namespace TA_Base_App
{
	const int multipleSelDlgPModel::LIST_ITEM_ALL_INDEX = 0;
	const std::string multipleSelDlgPModel::LIST_ITEM_ALL_STRING = "No Filter";
	const std::string multipleSelDlgPModel::EV_SORTCHANGE = "Sort Change";

	class Equal : public std::binary_function<ListItem, std::string, bool>
	{
	public:
		bool operator()(const ListItem& _Left, const std::string& _Right) const
		{
			if (_Left.str.compare(_Right) == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	multipleSelDlgPModel::multipleSelDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
		, m_includeAllItem(false)
		, m_sortItems(true)
	{
		FUNCTION_ENTRY("Constructor");
		FUNCTION_EXIT;
	}

	multipleSelDlgPModel::~multipleSelDlgPModel()
	{
		FUNCTION_ENTRY("Destructor");
		FUNCTION_EXIT;
	}

	ListItems multipleSelDlgPModel::getListItems() const
	{
		return m_listItems;
	}

	ListItems multipleSelDlgPModel::getSelection() const
	{
		FUNCTION_ENTRY("getSelection");
		FUNCTION_EXIT;
		return m_selectedItems;
	}

	void multipleSelDlgPModel::setSelection(const ListItems& selection)
	{
		FUNCTION_ENTRY("setSelection");
		m_selectedItems.clear();
		m_selectedItems = selection;
		FUNCTION_EXIT;
	}

	void multipleSelDlgPModel::addSelectedItem(const std::string& selItem)
	{
		FUNCTION_ENTRY("getSelection");
		ListItems::iterator itr = m_listItems.begin();
		for (itr; itr != m_listItems.end(); itr++)
		{
			ListItem tempItem = *itr;
			if (tempItem.str.compare(selItem) == 0)
			{ // We found it then add it to selected items
				m_selectedItems.push_back(tempItem);
				break;
			}
		}
		FUNCTION_EXIT;
	}

	void multipleSelDlgPModel::clearSelectedItem()
	{
		FUNCTION_ENTRY("clearSelectedItem");
		m_selectedItems.clear();
		FUNCTION_EXIT;
	}

	void multipleSelDlgPModel::setItems(const ListItems& items, bool includeAllItem)
	{
		FUNCTION_ENTRY("setItems");
		m_includeAllItem = includeAllItem;
		m_listItems = items;
		FUNCTION_EXIT;
	}

	void multipleSelDlgPModel::setSortItems(const bool sortItems)
	{
		FUNCTION_ENTRY("setSortItems");
		m_sortItems = sortItems;
		FUNCTION_EXIT;
	}

	/////////////////////////////////////////////////////////////////////////////
	//bool multipleSelDlgPModel::IsAllItemSelected() const
	//{
	//	if (m_includeAllItem)
	//	{
	//		// Check if all item is selected
	//		unsigned int selCount = m_multiList.GetSelCount();
	//		int* selIndex = new int[selCount];

	//		m_multiList.GetSelItems(selCount, selIndex);
	//		for (unsigned int i = 0; i < selCount; i++)
	//		{
	//			if (selIndex[i] == LIST_ITEM_ALL_INDEX)
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//	return false;
	//}
}