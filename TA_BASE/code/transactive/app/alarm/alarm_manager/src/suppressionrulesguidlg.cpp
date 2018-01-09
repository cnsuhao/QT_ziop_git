#include "suppressionrulesguidlg.h"
#include "globals.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

#include "core/data_access_interface/alarm_rule/src/SuppressionRuleData.h"
#include "core/data_access_interface/alarm_rule/src/IAlarmRuleData.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleData.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"

#include "core/utilities/src/DebugUtil.h"
#include "core\utilities\src\TAAssert.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include <QDateTime>
#include <QImage>
#include <QVariant>

namespace TA_Base_App
{
	SuppressionRulesGUIDlg::SuppressionRulesGUIDlg(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_SuppressionRule)
		//, m_pMyPModel(0)
	{
		ui.setupUi(this);


		connect(ui.rdoShowAll, SIGNAL(toggled(bool)), this, SLOT(OnRadioShowAll(bool)));
		connect(ui.rdoFilterLocation, SIGNAL(toggled(bool)), this, SLOT(OnRadioFilterLocation(bool)));
		connect(ui.tblAlarmRules, SIGNAL(itemSelectionChanged()), this, SLOT(OnClickRulesList()));
		connect(ui.cboLocation, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangeComboLocations(int)));

		connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(OnClose()));
		connect(ui.btnRefresh, SIGNAL(clicked()), this, SLOT(OnRefresh()));
		connect(ui.btnExportList, SIGNAL(clicked()), this, SLOT(OnExport()));
		connect(ui.btnPrintList, SIGNAL(clicked()), this, SLOT(OnPrint()));
		connect(ui.btnRemoveInhibition, SIGNAL(clicked()), this, SLOT(OnRemoveButton()));
		
	}

	SuppressionRulesGUIDlg::~SuppressionRulesGUIDlg()
	{
		FUNCTION_ENTRY("Destructor");
		FUNCTION_EXIT;
	}

	std::string  SuppressionRulesGUIDlg::getMyPModelName(void)
	{
		FUNCTION_ENTRY("getMyPModelName");
		FUNCTION_EXIT;
		return PMODEL_SuppressAlarmRule;
	}

	void SuppressionRulesGUIDlg::setupPModel(void)
	{
		FUNCTION_ENTRY("setupPModel");
		m_pTheRealPModel = dynamic_cast<SuppressionRulesDlgPModel*> (m_pModel);
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::translateUI()
	{
		FUNCTION_ENTRY("translateUI");
		//Todo:

		ui.retranslateUi(this);
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::initUIControl()
	{
		FUNCTION_ENTRY("initUIControl");

		//// Load Images
		///*QString imgBlankPath = ".\res\\closed_padlock.ico";
		//QImage *imgBlank = new QImage();
		//imgBlank->load(imgBlankPath);
		//*m_blankImg = QPixmap::fromImage(*imgBlank);

		//QString imgRemovablePath = ".\res\\open_padlock.ico";
		//QImage *imgRemovable = new QImage();
		//imgRemovable->load(imgRemovablePath);
		//*m_removableImg = QPixmap::fromImage(*imgRemovable);*/

		//m_blankImg = new QPixmap(":/res/closed_padlock.ico");
		//m_removableImg = new QPixmap(":/res/open_padlock.ico");

		//// set up and load the data
		//setUpHeadings();
		//// populate the location combo box and location name_key map
		//populateLocationCombo();

		//// Check for permission to show all rules and disable buttons accordingly
		//// Default to show all inhibitions if the operator has the rights
		//if (m_pTheRealPModel->hasPermissionToViewAllRules())
		//{
		//	// Check the SHOW_ALL radio button by default
		//	ui.rdoShowAll->setChecked(true);
		//	ui.rdoFilterLocation->setChecked(false);
		//	// Populate the list control
		//	loadAllRules();
		//	// Disable the location combo box
		//	ui.cboLocation->setEnabled(false);
		//}
		//else
		//{
		//	// Check the LOCATION radio button by default
		//	ui.rdoShowAll->setChecked(false);
		//	ui.rdoFilterLocation->setChecked(true);

		//	// populate the list control
		//	loadRulesByLocation(m_pTheRealPModel->getCurrentLocationKey());
		//	// Disable the show-all button and location combo box for selection
		//	ui.rdoShowAll->setEnabled(false);
		//	ui.cboLocation->setEnabled(false);
		//}

		//// Disable the Remove button
		//ui.btnRemoveInhibition->setEnabled(false);
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnInitDialog()
	{
		// Load Images
		/*QString imgBlankPath = ".\res\\closed_padlock.ico";
		QImage *imgBlank = new QImage();
		imgBlank->load(imgBlankPath);
		*m_blankImg = QPixmap::fromImage(*imgBlank);

		QString imgRemovablePath = ".\res\\open_padlock.ico";
		QImage *imgRemovable = new QImage();
		imgRemovable->load(imgRemovablePath);
		*m_removableImg = QPixmap::fromImage(*imgRemovable);*/

		m_blankImg = new QPixmap(":/res/closed_padlock.ico");
		m_removableImg = new QPixmap(":/res/open_padlock.ico");

		// set up and load the data
		setUpHeadings();
		// populate the location combo box and location name_key map
		populateLocationCombo();

		ui.rdoShowAll->blockSignals(true);
		ui.rdoFilterLocation->blockSignals(true);
		// Check for permission to show all rules and disable buttons accordingly
		// Default to show all inhibitions if the operator has the rights
		if (m_pTheRealPModel->hasPermissionToViewAllRules())
		{
			// Check the SHOW_ALL radio button by default
			ui.rdoShowAll->setChecked(true);
			ui.rdoFilterLocation->setChecked(false);
			// Populate the list control
			loadAllRules();
			// Disable the location combo box
			ui.cboLocation->setEnabled(false);
		}
		else
		{
			// Check the LOCATION radio button by default
			ui.rdoShowAll->setChecked(false);
			ui.rdoFilterLocation->setChecked(true);

			// populate the list control
			loadRulesByLocation(m_pTheRealPModel->getCurrentLocationKey());
			// Disable the show-all button and location combo box for selection
			ui.rdoShowAll->setEnabled(false);
			ui.cboLocation->setEnabled(false);
		}

		// Disable the Remove button
		ui.btnRemoveInhibition->setEnabled(false);

		ui.rdoShowAll->blockSignals(false);
		ui.rdoFilterLocation->blockSignals(false);
	}

	void SuppressionRulesGUIDlg::OnRemoveButton()
	{
		FUNCTION_ENTRY("OnRemoveButton");
		QItemSelectionModel *selectionModel = ui.tblAlarmRules->selectionModel();
		int selectedCount = selectionModel->selectedRows().count();
		if (selectedCount > 0)
		{
			std::vector<int> deleteIndexes;
			foreach(QModelIndex index, selectionModel->selectedRows())
			{
				int nItem = index.row();
				deleteIndexes.push_back(nItem);

				try
				{
					RuleInfo* ruleInfo = ui.tblAlarmRules->item(index.row(), 0)->data(Qt::UserRole).value<RuleInfo*>();
					TA_Base_Core::SuppressionRuleData* ruleDetails = ruleInfo->suppressionRule;

					ruleDetails->deleteThisObject();
					m_pTheRealPModel->sendConfigUpdate(ruleDetails);

				}
				catch (const TA_Base_Core::DatabaseException& e)
				{
					std::ostringstream error;
					error << "Remove Suppression Rule failed " << e.what();
					LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "%s", error.str().c_str());
					// TD14164 azenitha++
					TA_Base_Bus::TransActiveMessage userMsg;
					std::string reason = e.what();
					userMsg << reason;
					userMsg.showMsgBox(IDS_UE_040041);
					/*AfxMessageBox(error.str().c_str());*/
					// TD14164 ++azenitha
				}
			}
			
			// Delete the items from the list. Delete from the bottom because if we delete from the top, the
			// number of items in the list changes and hence all the items below the deleted item will have
			// different indexes.
			std::vector<int>::iterator it = deleteIndexes.end();
			--it;
			for (; it != deleteIndexes.begin(); --it)
			{
				// Delete item from the list
				ui.tblAlarmRules->removeRow(*it);
			}
			ui.tblAlarmRules->removeRow(*it);

			// Disable the Remove button once all the items have been removed
			//TA_ASSERT(GetDlgItem(IDC_REMOVE_BUTTON) != NULL, "Error creating Remove button.");
			//GetDlgItem(IDC_REMOVE_BUTTON)->EnableWindow(false);
			ui.btnRemoveInhibition->setEnabled(false);
		}

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnRadioFilterLocation(bool checked)
	{
		FUNCTION_ENTRY("OnRadioFilterLocation");
		if (checked)
		{
			if (m_pTheRealPModel->hasPermissionToViewAllRules())
			{
				// enable the combo box
				ui.cboLocation->setEnabled(true);
				// load the rules by current location
				loadRulesByLocation(m_pTheRealPModel->getCurrentLocationKey());
			}
		}
		

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnRadioShowAll(bool checked)
	{
		FUNCTION_ENTRY("OnRadioShowAll");
		if (checked)
		{
			ui.cboLocation->setEnabled(false);
			loadAllRules();
		}
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnClickRulesList()
	{
		FUNCTION_ENTRY("OnClickRulesList");
		QItemSelectionModel *selectionModel = ui.tblAlarmRules->selectionModel();
		int selectedCount = selectionModel->selectedRows().count();
		if (selectedCount > 0)
		{
			std::vector<unsigned long> regionKeys;
			m_pTheRealPModel->getCurrentOperatorsRegionAssignment(regionKeys);

			// Check that every rule selected has been created by the same profile. If not, the remove
			// button will be disabled. If so, then the remove button will only be enabled if the current
			// profile has permission to remove rules.

			bool cannotRemove = false;
			foreach(QModelIndex index, selectionModel->selectedRows())
			{
				int nItem = index.row();
				RuleInfo* ruleInfo = ui.tblAlarmRules->item(index.row(), 0)->data(Qt::UserRole).value<TA_Base_App::RuleInfo*>();
				TA_Base_Core::SuppressionRuleData* ruleDetails = ruleInfo->suppressionRule;

				if (!ruleInfo->isCreatedByCurrentProfile)
				{
					// There is at least one rule that can't be removed. Disable the Remove button.
					ui.btnRemoveInhibition->setEnabled(false);
					cannotRemove = true;
					break;
				}

				TA_Base_Core::SuppressionRuleData* ruleData = ruleInfo->suppressionRule;

				unsigned long regionKey = 0;
				if (m_pTheRealPModel->getRegionKeyByEntity(ruleData->getEntityKey(), regionKey))
				{
					std::vector<unsigned long>::iterator itRegionKey = std::find(regionKeys.begin(), regionKeys.end(), regionKey);
					if (itRegionKey == regionKeys.end())
					{
						// There is at least one rule that can't be removed. Disable the Remove button.
						ui.btnRemoveInhibition->setEnabled(false);
						cannotRemove = true;
						break;
					}
				}
			}

			if (!cannotRemove)
			{
				if (m_pTheRealPModel->hasPermissionToRemoveRules())
				{
					// Enable the Remove button
					ui.btnRemoveInhibition->setEnabled(true);
				}
			}
		}
		else
		{
			// Disable the Remove button
			ui.btnRemoveInhibition->setEnabled(false);
		}

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
	{
		FUNCTION_ENTRY("OnColumnclick");
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnClose()
	{
		FUNCTION_ENTRY("OnClose");
		clearList();

		QDialog::reject();
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnDestroy()
	{
		FUNCTION_ENTRY("OnDestroy");
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnRefresh()
	{
		FUNCTION_ENTRY("OnRefresh");

		//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "OnRefresh is called. Auto refresh is reset to %d secs.", m_refreshTime);

		if (ui.rdoShowAll->isChecked() == true)
		{
			loadAllRules();
		}
		else if (ui.rdoFilterLocation->isChecked() == true)
		{
			loadRulesByLocation(m_pTheRealPModel->getCurrentLocationKey());
		}

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnExport()
	{
		FUNCTION_ENTRY("OnExport");
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnPrint()
	{
		FUNCTION_ENTRY("OnPrint");
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnTimer(UINT nIDEvent)
	{
		FUNCTION_ENTRY("OnTimer");
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::OnSelchangeComboLocations(int index)
	{
		FUNCTION_ENTRY("OnSelchangeComboLocations");
		// Get the select location string
		
		std::string selLocationName = ui.cboLocation->itemText(index).toStdString();
		// Obtain the location key from map
		if (!selLocationName.empty())
		{
			m_pTheRealPModel->setCurrentLocation(selLocationName);
		}

		// reload the rule list
		loadRulesByLocation(m_pTheRealPModel->getCurrentLocationKey());

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::setUpHeadings()
	{
		FUNCTION_ENTRY("setUpHeadings");
		//m_rulesList.SetExtendedStyle(LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_INFOTIP);
		ui.tblAlarmRules->setSortingEnabled(true);
		ui.tblAlarmRules->setSelectionMode(QAbstractItemView::SingleSelection);
		ui.tblAlarmRules->setSelectionBehavior(QAbstractItemView::SelectRows);
		ui.tblAlarmRules->setColumnCount(7);
		/*QStringList objList;
		objList.push_back(SuppressionRulesDlgPModel::REMOVABLE_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::DATE_TIME_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::ASSET_NAME_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::DESCRIPTION_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::VALUE_COLUMN_NAME);
		objList.push_back(SuppressionRulesDlgPModel::OPERATOR_COLUMN_NAME);
		*/
		//ui.tblAlarmRules->setHorizontalHeaderLabels("");

		// Column 0
		QTableWidgetItem* pCol0 = new QTableWidgetItem(QString(SuppressionRulesDlgPModel::REMOVABLE_COLUMN_NAME), QTableWidgetItem::Type);
		pCol0->setTextAlignment(Qt::AlignCenter);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::REMOVABLE_COLUMN, pCol0);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::REMOVABLE_COLUMN, SuppressionRulesDlgPModel::REMOVABLE_COLUMN_WIDTH);
		
		// Column 1
		QTableWidgetItem* pCol1 = new QTableWidgetItem(SuppressionRulesDlgPModel::DATE_TIME_COLUMN_NAME, QTableWidgetItem::Type);
		pCol1->setText(SuppressionRulesDlgPModel::DATE_TIME_COLUMN_NAME);
		pCol1->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::DATE_TIME_COLUMN, pCol1);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::DATE_TIME_COLUMN, SuppressionRulesDlgPModel::DATE_TIME_COLUMN_WIDTH);

		// Column 2
		QTableWidgetItem* pCol2 = new QTableWidgetItem(SuppressionRulesDlgPModel::ASSET_NAME_COLUMN_NAME, QTableWidgetItem::Type);
		pCol2->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::ASSET_NAME_COLUMN, pCol2);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::ASSET_NAME_COLUMN, SuppressionRulesDlgPModel::ASSET_NAME_COLUMN_WIDTH);
		//pCol2->setData(Qt::UserRole, )

		// Column 3
		QTableWidgetItem* pCol3 = new QTableWidgetItem(SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN_NAME, QTableWidgetItem::Type);
		pCol3->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN, pCol3);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN, SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN_WIDTH);
		
		// Column 4
		QTableWidgetItem* pCol4 = new QTableWidgetItem(SuppressionRulesDlgPModel::DESCRIPTION_COLUMN_NAME, QTableWidgetItem::Type);
		pCol4->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::DESCRIPTION_COLUMN, pCol4);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::DESCRIPTION_COLUMN, SuppressionRulesDlgPModel::DESCRIPTION_COLUMN_WIDTH);
		
		// Column 5
		QTableWidgetItem* pCol5 = new QTableWidgetItem(SuppressionRulesDlgPModel::VALUE_COLUMN_NAME, QTableWidgetItem::Type);
		pCol5->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::VALUE_COLUMN, pCol5);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::VALUE_COLUMN, SuppressionRulesDlgPModel::VALUE_COLUMN_WIDTH);
		
		// Column 6
		QTableWidgetItem* pCol6 = new QTableWidgetItem(SuppressionRulesDlgPModel::OPERATOR_COLUMN_NAME, QTableWidgetItem::Type);
		pCol6->setTextAlignment(Qt::AlignLeft);
		ui.tblAlarmRules->setHorizontalHeaderItem(SuppressionRulesDlgPModel::OPERATOR_COLUMN, pCol6);
		ui.tblAlarmRules->setColumnWidth(SuppressionRulesDlgPModel::OPERATOR_COLUMN, SuppressionRulesDlgPModel::OPERATOR_COLUMN_WIDTH);
		
		// Set up image list // Todo: Noel
		//m_removableIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_RULE_REMOVABLE));
		//m_blankIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_UNREMOVABLE_RULE));
		//m_imageList.Create(16, 16, ILC_COLOR8, 4, 4);
		//m_imageList.Add(m_removableIcon);
		//m_imageList.Add(m_blankIcon);
		//// Set up icon vector
		//m_imageStringVector.push_back("Removable");
		//m_imageStringVector.push_back("Unremovable");


		//m_rulesList.SetImageList(&m_imageList, LVSIL_SMALL);

		//// Set the image in the header that the rules are sorted by default
		//CHeaderCtrl* headerCtrl = m_rulesList.GetHeaderCtrl();

		//HDITEM headerItem;
		//headerItem.mask = HDI_FORMAT | HDI_BITMAP;
		//headerCtrl->GetItem(m_sortColumn, &headerItem);
		//if (headerItem.hbm != 0)
		//{
		//	DeleteObject(headerItem.hbm);
		//	headerItem.hbm = 0;
		//}
		//headerItem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;

		//int resourceId = IDB_ASCENDING;
		//if (!m_sortAscending)
		//{
		//	resourceId = IDB_DESCENDING;
		//}

		//// Load the bitmap based on whether we are sorting ascending or descending
		//headerItem.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(resourceId), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

		//TA_ASSERT(headerItem.hbm != NULL, "The resource file bus/alarm/alarm_list_control/src/AlarmListCtrl.rc must be included in the application's rc file");

		//headerCtrl->SetItem(m_sortColumn, &headerItem);

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::populateLocationCombo()
	{
		FUNCTION_ENTRY("populateLocationCombo");
		ui.cboLocation->blockSignals(true);
		std::map<std::string, unsigned long> objLocations = m_pTheRealPModel->getAllLocationData();
		std::map<std::string, unsigned long>::iterator itr = objLocations.begin();
		int idx = 0;
		for (itr; itr != objLocations.end(); itr++)
		{
			ui.cboLocation->insertItem(idx, itr->first.c_str());
			if (m_pTheRealPModel->getCurrentLocationKey() == itr->second)
			{
				ui.cboLocation->setCurrentIndex(-1);
				ui.cboLocation->setCurrentIndex(idx);
			}
			idx++;
		}
		ui.cboLocation->blockSignals(false);
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::loadAllRules()
	{
		FUNCTION_ENTRY("loadAllRules");
		//ui.btnRemoveInhibition->setEnabled(false);
		//
		//// Clear the list first
		clearList();

		// Retrieve the rules
		std::vector<TA_Base_Core::SuppressionRuleData*> suppressionRules;
		if (!m_pTheRealPModel->getAllRules(suppressionRules))
		{
			displayErrorMessage();
		}
		else
		{
			// Sort list by the selected column
			sortList(suppressionRules);
			populateList(suppressionRules);
		}

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::loadRulesByLocation(unsigned long locationKey)
	{
		FUNCTION_ENTRY("loadRulesByLocation");
		ui.btnRemoveInhibition->setEnabled(false);

		// Clear the list first
		clearList();

		std::vector<TA_Base_Core::SuppressionRuleData*> suppressionRules;
		if (!m_pTheRealPModel->getRulesByLocation(locationKey, suppressionRules))
		{
			displayErrorMessage();
		}
		else
		{
			// Sort list by the selected column
			sortList(suppressionRules);
			populateList(suppressionRules);
		}

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::displayErrorMessage()
	{
		FUNCTION_ENTRY("displayErrorMessage");
		TA_Base_Bus::TransActiveMessage userMsg;
		std::string description = "retrieving suppression rules";
		userMsg << description;
		userMsg.showMsgBox(IDS_UW_632015);

		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::populateList(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		FUNCTION_ENTRY("populateList");
		// Get the current user's region assignment
		std::vector<unsigned long> regionKeys;
		m_pTheRealPModel->getCurrentOperatorsRegionAssignment(regionKeys);

		// Row Size
		ui.tblAlarmRules->setRowCount(suppressionRules.size());

		// Populate the list and delete the pointers
		int count = 0;
		for (std::vector<TA_Base_Core::SuppressionRuleData*>::iterator it = suppressionRules.begin();
			it != suppressionRules.end();
			++it)
		{
			if ((*it) != NULL)
			{
				time_t dateTime = (*it)->getDateModified();
				if (dateTime == 0) dateTime = (*it)->getDateCreated();

				std::string assetName = (*it)->getAssetName();
				if (assetName.empty())
				{
					assetName = "-";
				}

				std::string alarmType = (*it)->getAlarmTypeName();
				if (alarmType.empty())
				{
					alarmType = "-";
				}

				std::string description = (*it)->getAlarmDescription();
				if (description.empty())
				{
					description = "-";
				}

				std::string value = (*it)->getValue();
				if (value.empty())
				{
					value = "-";
				}

				std::string operatorName = (*it)->getOperatorName();
				if (operatorName.empty())
				{
					operatorName = "-";
				}

				// Store the rule and profile information as the item data to be retrieved later
				RuleInfo* ruleInfo = new RuleInfo;
				ruleInfo->isCreatedByCurrentProfile = false;

				// Insert an icon to indicate the rule is removable if the rule was created under the 
				// same profile and the entity related to the rule falls under the current user's region
				// assignment
				int image = SuppressionRulesDlgPModel::BLANK_IMAGE;
				unsigned long createdByProfile = (*it)->getCreatedByProfileKey();

				if (createdByProfile == m_pTheRealPModel->getProfileID())
				{
					ruleInfo->isCreatedByCurrentProfile = true;

					unsigned long regionKey = 0;
					if (m_pTheRealPModel->getRegionKeyByEntity((*it)->getEntityKey(), regionKey))
					{
						std::vector<unsigned long>::iterator itRegionKey = std::find(regionKeys.begin(), regionKeys.end(), regionKey);
						if (itRegionKey != regionKeys.end())
						{
							image = SuppressionRulesDlgPModel::REMOVABLE_IMAGE;
							(*it)->setRemovable(SuppressionRulesDlgPModel::REMOVABLE_IMAGE);
						}
						else
						{
							image = SuppressionRulesDlgPModel::BLANK_IMAGE;
							(*it)->setRemovable(SuppressionRulesDlgPModel::BLANK_IMAGE);
						}
					}
				}

				ruleInfo->suppressionRule = (*it);

				// Add row to list
				CtlTableWidgetItem* pItem = new CtlTableWidgetItem();
				if (image == SuppressionRulesDlgPModel::REMOVABLE_IMAGE)
					pItem->setData(Qt::DecorationRole, *m_blankImg);
				else
					pItem->setData(Qt::DecorationRole, *m_removableImg);

				QVariant temp;
				temp.setValue(ruleInfo);

				pItem->setData(Qt::UserRole, temp);
				ui.tblAlarmRules->setItem(count, 0, pItem);

			   TA_Base_Bus::TAUtfString displayTime("");
				if (dateTime > 0)
				{
					displayTime = QDateTime::fromTime_t(dateTime).toString(Qt::SystemLocaleShortDate);
				}
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::DATE_TIME_COLUMN, new CtlTableWidgetItem(displayTime.q_str(), UIDATETIME));
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::ASSET_NAME_COLUMN, new CtlTableWidgetItem(assetName));
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN, new CtlTableWidgetItem(alarmType));
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::DESCRIPTION_COLUMN, new CtlTableWidgetItem(description));
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::VALUE_COLUMN, new CtlTableWidgetItem(value));
				ui.tblAlarmRules->setItem(count, SuppressionRulesDlgPModel::OPERATOR_COLUMN, new CtlTableWidgetItem(operatorName));

				//DWORD itemData = reinterpret_cast <DWORD> (ruleInfo);
				//ui.tblAlarmRules->setUserData(count, itemData);
				++count;
				// Don't delete the pointers. They will be deleted when the list is cleared.
			}
		}

		FUNCTION_EXIT;
	}
	   
	void SuppressionRulesGUIDlg::clearList()
	{
		FUNCTION_ENTRY("clearList");
		// Clean up the pointers
		for (int i = 0; i < ui.tblAlarmRules->rowCount(); ++i)
		{
			RuleInfo* ruleInfo = ui.tblAlarmRules->item(i, 0)->data(Qt::UserRole).value<RuleInfo*>();
			if (ruleInfo != NULL)
			{
				TA_Base_Core::SuppressionRuleData* ruleData = ruleInfo->suppressionRule;

				if (ruleData != NULL)
				{
					delete ruleData;
					ruleData = NULL;
				}

				delete ruleInfo;
				ruleInfo = NULL;
			}
		}

		ui.tblAlarmRules->clearContents();
		FUNCTION_EXIT;
	}

	void SuppressionRulesGUIDlg::sortList(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		FUNCTION_ENTRY("sortList");
		FUNCTION_EXIT;
	}

}
