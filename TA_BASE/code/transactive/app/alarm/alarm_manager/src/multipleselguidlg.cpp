/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/alarm/alarm_manager/src/MultipleSelectionDlg.cpp $
* @author:  Ripple
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
*/
// MultipleSelectionDlg.cpp : implementation file
//

#include "multipleselguidlg.h"
#include "globals.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"
#include "core/utilities/src/DebugUtil.h"
#include <QtWidgets\QListWidgetItem>
#include "bus\generic_gui_view\src\HelpLauncher.h"


/////////////////////////////////////////////////////////////////////////////
// MultipleSelectionDlg dialog
/////////////////////////////////////////////////////////////////////////////

namespace TA_Base_App
{

	multipleSelGuiDlg::multipleSelGuiDlg(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_MultiSel)
		, m_pMyPModel(0)
	{
		FUNCTION_ENTRY("Constructor");
		 
		ui.setupUi(this);

		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnMultiok()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
		connect(ui.listItems, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
		FUNCTION_EXIT;
	}

	//ITransActiveView interface
	std::string multipleSelGuiDlg::getMyPModelName(void)
	{
		FUNCTION_ENTRY("getMyPModelName");
		FUNCTION_EXIT;
		return PMODEL_MultiSelDlg;
	}

	void multipleSelGuiDlg::setupPModel(void)
	{
		FUNCTION_ENTRY("setupPModel");
		m_pMyPModel = dynamic_cast<multipleSelDlgPModel*> (m_pModel);

		FUNCTION_EXIT;
	}

	void multipleSelGuiDlg::translateUI()
	{
		ui.retranslateUi(this);
	}

	void multipleSelGuiDlg::setDialogTitle(const TA_Base_Bus::TAUtfString& title)
	{
		FUNCTION_ENTRY("setDialogTitle");
		this->setWindowTitle(title.q_str());
		FUNCTION_EXIT;
	}

	void multipleSelGuiDlg::setItems(ListItems& items, bool includeAllItem)
	{
		FUNCTION_ENTRY("setItems");
		m_pMyPModel->setItems(items, includeAllItem);
		FUNCTION_EXIT;
	}

	void multipleSelGuiDlg::setSelection(ListItems& selItems)
	{
		FUNCTION_ENTRY("setSelection");
		m_pMyPModel->setSelection(selItems);
		FUNCTION_EXIT;
	}

	ListItems multipleSelGuiDlg::getSelection() const
	{
		FUNCTION_ENTRY("getSelection");
		FUNCTION_EXIT;
		return m_pMyPModel->getSelection();
	}

	/////////////////////////////////////////////////////////////////////////////
	// MultipleSelectionDlg message handlers
	/////////////////////////////////////////////////////////////////////////////
	void multipleSelGuiDlg::initUIControl()
	{	
	}

	void multipleSelGuiDlg::OnInitDialog()
	{
		// Set the window title
		this->setWindowTitle(m_pMyPModel->getWindowTitle().c_str());
		//CDialog::SetWindowText(m_windowTitle);

		// Clear the items
		ui.listItems->clear();

		// Add the ALL item to the list if specified.
		if (m_pMyPModel->isIncludeAll())
		{
			ui.listItems->addItem(multipleSelDlgPModel::LIST_ITEM_ALL_STRING.c_str());
			// Set this as selected if no items are selected
			if (m_pMyPModel->getSelection().size() == 0)
			{
				ui.listItems->item(multipleSelDlgPModel::LIST_ITEM_ALL_INDEX)->setSelected(TRUE);
			}
		}

		// Set the item list and the selected items
		ListItems tempListItems = m_pMyPModel->getListItems();
		ListItems::iterator iter;
		for (iter = tempListItems.begin(); iter != tempListItems.end(); iter++)
		{
			int index = 0;

			// Use a different method for inserting the string depending on whether sorting is
			// required. Ideally the style should be changed so that AddString can always be
			// used, however the style can't be changed once the list control is created
			// unless it is completely reconstructed.
			if (m_pMyPModel->isSortItems())
			{
				// Sort items, so use AddString as it will automatically
				// sort the items in alphabetical order.
				ui.listItems->addItem(iter->str.c_str());
				index = ui.listItems->count() - 1;
			}
			else
			{
				// Dont sort items, so use InsertString as it will not
				// apply any form of automatic sort.
				//index = m_multiList.InsertString(-1, iter->str.c_str());
				ui.listItems->addItem(iter->str.c_str());
				index = ui.listItems->count() - 1;
			}
			//m_multiList.SetItemData(index, iter->pkey);
			// Set this item as selected if necessary
			ListItems selItems = m_pMyPModel->getSelection();
			for (ListItems::iterator selIter = selItems.begin(); selIter != selItems.end(); ++selIter)
			{
				if (iter->pkey == selIter->pkey)
				{
					ui.listItems->item(index)->setSelected(TRUE);
					break;
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void multipleSelGuiDlg::OnMultiok()
	{
		m_pMyPModel->clearSelectedItem();

		// Check if the ALL item is selected (if it is we return an empty selection)
		if (!IsAllItemSelected())
		{
			// Return the selected items
			//unsigned int selCount = ui.listItems->selectedItems().count();
			QList<QListWidgetItem*> selItems = ui.listItems->selectedItems();
			//m_multiList.GetSelItems(selCount, selIndex);
			for (unsigned int i = 0; i < selItems.count(); i++)
			{
				QListWidgetItem* pItem = selItems[i];
				//unsigned long key = m_multiList.GetItemData(selIndex[i]);
				m_pMyPModel->addSelectedItem(pItem->text().toStdString());
			}
		}
		ListItems temp = m_pMyPModel->getSelection();
		m_resultData.get()->swap(temp);
		QDialog::accept();
	}

	/////////////////////////////////////////////////////////////////////////////
	void multipleSelGuiDlg::OnCancel()
	{
		QDialog::reject();
	}

	void multipleSelGuiDlg::itemSelectionChanged()
	{
		QModelIndexList indexes = ui.listItems->selectionModel()->selectedIndexes();
		foreach(QModelIndex index, indexes)
		{
			if (index.row() != 0 && ui.listItems->item(0)->isSelected() == true)
			{
				ui.listItems->item(0)->setSelected(false);
				break;
			}
		}

		
	}

	/////////////////////////////////////////////////////////////////////////////
	// MultipleSelectionDlg Private helper functions
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	bool multipleSelGuiDlg::IsAllItemSelected() const
	{
		if (m_pMyPModel->isIncludeAll())
		{
			// Check if all item is selected
			//unsigned int selCount = ui.listItems->selectedItems().count();
			QList<QListWidgetItem*> selItems = ui.listItems->selectedItems();
			//m_multiList.GetSelItems(selCount, selIndex);
			for (unsigned int i = 0; i < selItems.count(); i++)
			{
				if (multipleSelDlgPModel::LIST_ITEM_ALL_STRING.compare(selItems[i]->text().toStdString()) == 0)
					return true;
			}
		}
		return false;
	}

	void multipleSelGuiDlg::setSortItems(const bool sortItems)
	{
		m_pMyPModel->setSortItems(sortItems);
	}

	void multipleSelGuiDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args /*= NULL*/)
	{

	}
}
