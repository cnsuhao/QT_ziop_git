#include "AlarmFilterDlgPModel.h"

#include "app\alarm\alarm_manager\pmodel\src\SubsytemDataCache.h"
#include "app\alarm\alarm_manager\pmodel\src\SeverityCreateEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\StringEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\FilterEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\FilterChangeEventArgs.h"
#include "app/alarm/alarm_manager/pmodel/src/FilterDlgBtnStateUpdate.h"
#include "app/alarm/alarm_manager/pmodel/src/PopulateNamedFilterEventArgs.h"
#include "app/alarm/alarm_manager/pmodel/src/PopulateFilterSelEventArgs.h"

#include "bus/TA_MessageResource/resource.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"

#include "core/exceptions/src/DataException.h"
#include "core\exceptions\src\AlarmListCtrlException.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\data_access_interface\src\NamedFilterAccessFactory.h"

#include <boost/algorithm/string.hpp>

using TA_Base_Core::NamedFilter;
using namespace TA_Base_Bus;

namespace
{
	const std::string sSave("save");
	const std::string sSaved("saved");

	const std::string sLoad("load");
	const std::string sLoaded("loaded");

	const std::string sDelete("delete");
	const std::string sDeleted("deleted");

	/////////////////////////////////////////////////////////////////////////////
	// This is to get the selected item from a list of items, 
	// e.g. the operator list dlg, the location list dlg, alarm list dlg, etc.
	std::string getSelectedItemsString(const TA_Base_App::ListItems& selectedItems)
	{
		std::string text;
		if (selectedItems.empty())
		{
			text = "No Filter";
		}
		else
		{
			for (TA_Base_App::ListItems::const_iterator itr = selectedItems.begin(); itr != selectedItems.end(); ++itr)
			{
				if (itr != selectedItems.begin())
				{
					text += ", ";
				}
				text += std::string(itr->str).c_str();
			}
		}
		return text;
	}

	/////////////////////////////////////////////////////////////////////////////
	// This function constructs a new time_t from a date and a time time_t.
	time_t getCombinedTime(const time_t& date, const time_t& time)
	{
		time_t retVal;
		struct tm tempRes = { 0 };
		struct tm *tmpDate = gmtime(&date);
		struct tm *tmpTime = gmtime(&time);

		if (NULL != tmpDate)
		{
			tempRes.tm_year = tmpDate->tm_year;
			tempRes.tm_mon = tmpDate->tm_mon;
			tempRes.tm_mday = tmpDate->tm_mday;
		}

		if (NULL != tmpTime)
		{
			tempRes.tm_hour = tmpTime->tm_hour;
			tempRes.tm_min = tmpTime->tm_min;
			tempRes.tm_sec = tmpTime->tm_sec;
		}

		retVal = mktime(&tempRes);

		return retVal;
	}
}

namespace TA_Base_App
{

	AlarmFilterDlgPModel::AlarmFilterDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
		, m_chkFromDateTime(false)
		, m_chkToDateTime(false)
	{
		AlarmFilterService::getInstance().registerObserver(this);
	}


	AlarmFilterDlgPModel::~AlarmFilterDlgPModel()
	{
		AlarmFilterService::getInstance().unregisterObserver(this);
	}

	void AlarmFilterDlgPModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args/* = NULL*/)
	{
		// For now just forward the message
		notifyViews(updateType, args);
	}

	std::string AlarmFilterDlgPModel::getCurrentFilterString()
	{
		return m_NamedFilter;
	}

	TA_Base_Bus::Filter* AlarmFilterDlgPModel::getFilterObject()
	{
		return &m_filter;
	}

	TA_Base_Bus::Filter* AlarmFilterDlgPModel::getUserDefinedFilterObject()
	{
		return &AlarmFilterService::getInstance().getUserDefFilter();
	}

	TA_Base_Core::NamedFilter* AlarmFilterDlgPModel::getNamedFilterObject()
	{
		return &m_nmFilter;
	}

	void AlarmFilterDlgPModel::retrieveListItems()
	{
		AlarmFilterService::getInstance().retrieveSystems(m_systemTypes);
		AlarmFilterService::getInstance().retrieveSubsystemTypes(m_subsystemTypes, m_systemTypes);
		AlarmFilterService::getInstance().retrieveLocations(m_locations);
		AlarmFilterService::getInstance().retrieveOperators(m_operators);
		AlarmFilterService::getInstance().retrieveAlarmTypes(m_alarmTypes);
		AlarmFilterService::getInstance().retrieveSeverities(m_severityList);

	}

	unsigned long AlarmFilterDlgPModel::getSeverityColour(TA_Base_Core::IAlarmSeverityData* severityData)
	{
		return AlarmFilterService::getInstance().getSeverityColour(severityData);
	}

	void AlarmFilterDlgPModel::selectUserFilter()
	{
		m_NamedFilter = NamedFilter::UNNAMED.c_str();
		FilterChangeEventArgs* ev = new FilterChangeEventArgs(USERDEF, NamedFilter::UNNAMED.c_str());
		notifyUpdate(EV_FILTER_CHANGE, ev);

		FilterDlgBtnStateEventArgs* pBtnStateEv = new FilterDlgBtnStateEventArgs();
		pBtnStateEv->addButtonUpdate(BTN_SAVE, true);
		pBtnStateEv->addButtonUpdate(BTN_DELETE, false);

		notifyUpdate(EV_FILTERDLG_BTN_UPDATE, pBtnStateEv);
	}

	// Button Function
	void AlarmFilterDlgPModel::applyFilter()
	{
		FUNCTION_ENTRY("OnApply");

		bool isDefault = false;
		if (m_updateNeeded) //check if anything is updated
		{
			// Clear the filter object of all old filters
			m_filter.removeAll();
			AlarmFilterService::getInstance().setUserDefFilter(m_filter); // Clear User define filter
			//m_filterUserDef.removeAll();

			// No need to create filter if there is no filter ie. Default is set
			// Create the filter object, then tell the list control to filter on it.
			std::string curName = m_NamedFilter;
			std::string fName(curName);
			char* pszFilterName = new char[m_NamedFilter.size() + 1];
			sprintf(pszFilterName, "%s", m_NamedFilter.c_str());
			
			bool inFilterDB = AlarmFilterService::getInstance().checkInNamedFilterDatabase(fName);
			bool savedFilter = false;

			if (curName != AlarmFilter::FILTER_DEFAULT.c_str())
			{
				if (inFilterDB == true && m_savedFilter == true)
				{
					//Todo: This logic needs to be reviewed --
					//This is a case that the current filter changes is saved
					savedFilter = true;
					//index = m_namedFilter.GetCurSel();
					//if (index >= 0)
					//	m_namedFilter.GetLBText(index, curName);
				}
				else if (inFilterDB == true && m_savedFilter == false)
				{
					//This is a case that the current filter is changed and not saved
					//Display a message to remind user to save the filter
					savedFilter = false;

					// TD #11238
					// Revert the name back to "User Defined"
					curName = TA_Base_Core::NamedFilter::UNNAMED.c_str();
					m_NamedFilter = curName; //Todo: Notify update
					//m_namedFilter.SetWindowText(curName);
					//m_saveButton.EnableWindow(false);
					//m_deleteButton.EnableWindow(false);
				}
			}

			// This is for saved filter which is not Default and User Defined
			if (curName != AlarmFilter::FILTER_DEFAULT.c_str() && curName != TA_Base_Core::NamedFilter::UNNAMED.c_str() && savedFilter == true)
			{
				m_filter = createFilter();
				m_filter.setFilterName(fName);
				filterIndex = AlarmFilter::INDEX_LIST;
			}
			// Set the name if it is a user defined one which is not saved
			else if (curName == TA_Base_Core::NamedFilter::UNNAMED.c_str())
			{
				m_filter = createFilter();
				m_filter.setFilterName(NamedFilter::UNNAMED);
				filterIndex = AlarmFilter::INDEX_USER_DEFINED;
				//m_filterUserDef = m_filter;
				AlarmFilterService::getInstance().setUserDefFilter(m_filter);
			}
			// it must be a default filter name but check anyway
			else if (curName == AlarmFilter::FILTER_DEFAULT.c_str())
			{
				// No need to create filter as no filter is need for default
				filterIndex = AlarmFilter::INDEX_DEFAULT;
				isDefault = true;
			}
			// This is a new filter that is not saved
			else if (curName != AlarmFilter::FILTER_DEFAULT.c_str() && curName != TA_Base_Core::NamedFilter::UNNAMED.c_str() && savedFilter == false)
			{
				m_filter = createFilter();
				m_filter.setFilterName(fName);
				filterIndex = AlarmFilter::INDEX_USER_DEFINED; // User defined name will be refected in the parent dialog
			}

			try
			{
				//clear all the fitlers in the alarm list
				FilterEventArgs* pEvFilter = new FilterEventArgs(isDefault);
				pEvFilter->setFilter(&m_filter);
				this->getWinApp()->getViewManager()->sendBoardcastMessage(WM_SET_ALARM_FILTER, (TRA_WPARAM)(pEvFilter), 0);

				//notifyUpdate(EV_SET_ALARM_FILTER, pEvFilter);

				setUpdateNeeded(false);
				//m_saveButton.EnableWindow(!m_savedFilter);
				m_nCurFilterState = 0;
				m_strCurDlgFilter = m_filter.getFilterName();

				//Todo: Then notify the main dialog the filters have changed depending on the filter name type
				this->getWinApp()->getViewManager()->postBoardcastMessage(WM_APPLY_FILTER, (TRA_WPARAM)(filterIndex), (TRA_LPARAM)pszFilterName);
				//AfxGetMainWnd()->PostMessage(WM_ON_FILTER_DLG_APPLY, filterIndex, index);
			}
			catch (TA_Base_Core::AlarmListCtrlException)
			{
				//TA_Base_Bus::TransActiveMessage userMsg;
				//UINT selectedButton = userMsg.showMsgBox(IDS_UI_040022);
				throw; // Todo: Catch the exception from the caller and show message box
			}
		}

		//m_applyButton.EnableWindow(false);
		// Nothing to be done if not updated

		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::clearFilter()
	{
		m_selectedSubsystems.clear();
		m_selectedSubsystemTypes.clear();
		m_selectedSystems.clear();
		m_selectedLocations.clear();
		m_selectedOperators.clear();
		m_selectedAlarmTypes.clear();
		m_selectedServerity.clear();

		m_EquipmentID = "";
		m_Description = "";
		m_NamedFilter = AlarmFilter::FILTER_DEFAULT;
		m_FromDate = 0;
		m_FromTime = 0;
		m_ToDate = 0;
		m_ToTime = 0;
		m_OperationalMode = OMFilter::ALL_OM;
		m_AckState = AckFilter::ALL_ACK;
		m_AlarmState = StateFilter::ALL_STATE;

		//Severity are all check by default
		for (ListSeverityItems::iterator itr = m_severityList.begin(); itr != m_severityList.end(); itr++)
		{
			m_selectedServerity.push_back(*itr);
		}

		std::string filterName(m_filter.getFilterName());

		// Check whether it is already in default filter
		if (filterName != AlarmFilter::FILTER_DEFAULT && filterName != "")
		{
			// Clear the filter
			m_filter.removeAll();
			m_filter.setFilterName(AlarmFilter::FILTER_DEFAULT);

			// Reset the filter combo box
			FilterChangeEventArgs* ev = new FilterChangeEventArgs(DEFAULT, AlarmFilter::FILTER_DEFAULT);
			notifyUpdate(EV_FILTER_CHANGE, ev);
			//populateNamedFilters(AlarmFilterService::FILTER_DEFAULT);

			m_updateNeeded = true;
		}
		// TD 11033: A case of no filter is set when new  or on initialisation, set it to default
		else if (filterName == "")
		{
			// Clear the filter
			m_filter.setFilterName(AlarmFilter::FILTER_DEFAULT);

			// Reset the filter combo box
			//populateNamedFilters(AlarmFilterService::FILTER_DEFAULT);
			FilterChangeEventArgs* ev = new FilterChangeEventArgs(DEFAULT, AlarmFilter::FILTER_DEFAULT);
			notifyUpdate(EV_FILTER_CHANGE, ev);

			m_updateNeeded = false;
		}
		// TD 11033: A case of no filter is DEFAULT, set it to the filter to default
		// This step may not be necessary, but do it anyway. Don't change the m_updateNeeded, leave it alone!!
		// to whatever is set previous
		else if (filterName == AlarmFilter::FILTER_DEFAULT)
		{
			// Clear the filter
			m_filter.setFilterName(AlarmFilter::FILTER_DEFAULT);

			// Reset the filter combo box
			//populateNamedFilters(AlarmFilterService::FILTER_DEFAULT);
			FilterChangeEventArgs* ev = new FilterChangeEventArgs(DEFAULT, AlarmFilter::FILTER_DEFAULT);
			notifyUpdate(EV_FILTER_CHANGE, ev);
		}

		m_savedFilter = true; // Default filter need not be saved
	}
	
	void AlarmFilterDlgPModel::saveFilter()
	{
		FUNCTION_ENTRY("saveFilter");
		// Get the name to use.
		std::string fName = m_NamedFilter;
		std::string filterName = m_NamedFilter;

		// Convert to upper case for comparison
		boost::to_upper(fName);

		bool isUserDefined = (fName.compare("USER DEFINED") >= 0) ? true : false;
		bool isDefault = (fName.compare("DEFAULT") >= 0) ? true : false;
		bool isInComboList = AlarmFilterService::getInstance().checkInNamedFilterDatabase(fName);

		if (isUserDefined || isDefault) // Can't save a Default or User Defined file
		{
			// Display a message box to disallow these keywords as name
			const std::string saveEr("save an invalid name for");
			const std::string fileEr("saved as " + filterName);

			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(saveEr);
			dataContextPtr->pushParamValue(fileEr);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_SAVE_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			m_NamedFilter = NamedFilter::UNNAMED; //Todo: Notify change filter name
			return;
		}

		// This part is to delete old NamedFilter from database and create a new one
		if (/* (m_savedFilter) &&*/ (isInComboList == true))
		{
			// get the old filter name, in the event that this is a rename operation..
			// the old filter is the one we need to delete
			std::string oldFilterName(m_filter.getFilterName());

			// if the filter name we want to save to is in the list, then that
			// is the one that needs to be deleted
			if (isInComboList)
			{
				oldFilterName = m_NamedFilter;
				//CString strOldFilterName;
				//int nIndex = m_namedFilter.FindStringExact(-1, fName);
				//m_namedFilter.GetLBText(nIndex, strOldFilterName);
				//oldFilterName = strOldFilterName;
			}

			if (oldFilterName != TA_Base_Core::NamedFilter::UNNAMED)
			{
				// TD #11235
				// make the user confirm the overwrite
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UW_010003));
				dataContextPtr->pushParamValue(std::string("overwrite '" + oldFilterName + "' with the current filter settings"));

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_CONFIRM_SAVE));
				m_appController->getWinApp()->requestUserReply(eventArgs);
				//TA_Base_Bus::TransActiveMessage userMsg;
				//userMsg << "overwrite '" + oldFilterName + "' with the current filter settings";
				if (TA_Base_Bus::UIR_IDNO == eventArgs->getUIReplyType())
				{
					return;
				}

				std::auto_ptr<NamedFilter> loadedFilter;

				try
				{
					// Load the filter.
					loadedFilter = TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter(oldFilterName);
					AlarmFilterService::getInstance().deleteNamedFilter(*loadedFilter.get());
				}
				catch (...)
				{
				}
			}
		}

		// Clear current filter and create a new one
		m_filter.removeAll();
		m_filter = createFilter();
		m_filter.setFilterName(filterName);

		m_nmFilter.clear();
		m_nmFilter = AlarmFilterService::getInstance().convertFilterToNamedFilter(m_filter);

		try
		{
			// Save the filter.
			m_nmFilter.saveNamedFilter();

			if (stricmp(m_strCurDlgFilter.c_str(), m_filter.getFilterName().c_str()) == 0)
				m_nCurFilterState = 1;
		}
		catch (TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.getWhichData());

			// Display the save filter error message.
			//TA_Base_Bus::TransActiveMessage userMsg;
			//userMsg << sSave << sSaved;
			//UINT selectedButton = userMsg.showMsgBox(IDS_UE_040031);

			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(sSave);
			dataContextPtr->pushParamValue(sSaved);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_SAVE_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			// Only an unnamed filter can be saved, so rename it back.
			m_nmFilter.setName(NamedFilter::UNNAMED);
			return;
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Failed to create and save filter");
			// Display the save filter error message.
			//TA_Base_Bus::TransActiveMessage userMsg;
			//userMsg << sSave << sSaved;
			//UINT selectedButton = userMsg.showMsgBox(IDS_UE_040031);
			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(sSave);
			dataContextPtr->pushParamValue(sSaved);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_SAVE_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			// Only an unnamed filter can be saved, so rename it back.
			m_nmFilter.setName(NamedFilter::UNNAMED);
			return;
		}

		// Now update the combo box.
		// populateNamedFilters(m_nmFilter.getName()); // Todo: notify filter change

		setUpdateNeeded(true);
		m_savedFilter = true;
		m_bNMFilterUpdated = true;
		// TD12755 Reset the save button

		//m_saveButton.EnableWindow(false); // Todo: Notify button disable
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::deleteFilter()
	{
		FUNCTION_ENTRY("deleteFilter");
		// Get the name to use.
		std::string fName = m_NamedFilter;
		std::string filterName(fName);

		bool isUserDefined = filterName.compare(TA_Base_Core::NamedFilter::UNNAMED) ? false : true;
		bool isDefault = filterName.compare(AlarmFilter::FILTER_DEFAULT) ? false : true;

		// User can't delete a not existing Default or User Defined filter
		if (isUserDefined || isDefault)
		{
			// Display a message box to disallow these keywords as name
			//TA_Base_Bus::TransActiveMessage userMsg;
			const std::string delEr("delete an invalid name for ");
			const std::string fileEr("deleted as " + filterName);
			//userMsg << delEr;
			//userMsg << fileEr;
			//userMsg.showMsgBox(IDS_UE_040031);
			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(delEr);
			dataContextPtr->pushParamValue(fileEr);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			return;
		}

		if (AlarmFilterService::getInstance().checkInNamedFilterDatabase(filterName)) // Check that it is a saved filter
		{
			//int index = m_namedFilter.GetCurSel(); // Todo: not sure for what is this logic
			//m_namedFilter.GetLBText(index, fName);
		}
		else // The name is not in the database and cannot be deleted
		{
			// Display a message box to disallow the delete function
			//TA_Base_Bus::TransActiveMessage userMsg;
			const std::string delEr("delete ");
			const std::string fileEr("deleted as " + filterName + " not in the database");
			//userMsg << delEr;
			//userMsg << fileEr;
			//userMsg.showMsgBox(IDS_UE_040031);
			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(delEr);
			dataContextPtr->pushParamValue(fileEr);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			return;
		}

		bool isRightFile = m_nmFilter.getName() == filterName ? true : false; // Ensure that the filter is correctly selected
		// Delete an exiting NamedFilter in database
		if (isRightFile == true)
		{
			// Confirm the deletion.
			//TA_Base_Bus::TransActiveMessage userMsg;
			//userMsg << "delete '" + m_nmFilter.getName() + "'";
			//if (IDNO == userMsg.showMsgBox(IDS_UW_010003))
			//{
			//	return;
			//}

			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UW_010003));
			dataContextPtr->pushParamValue(std::string("delete '" + m_nmFilter.getName() + "'"));

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_CONFIRM_DELETE));
			m_appController->getWinApp()->requestUserReply(eventArgs);
			//TA_Base_Bus::TransActiveMessage userMsg;
			//userMsg << "overwrite '" + oldFilterName + "' with the current filter settings";
			if (TA_Base_Bus::UIR_IDNO == eventArgs->getUIReplyType())
			{
				return;
			}

			bool isDeleteOk = AlarmFilterService::getInstance().deleteNamedFilter(m_nmFilter);

			if (isDeleteOk == false)
			{
				// Display not successful message
				//TA_Base_Bus::TransActiveMessage userMsg;
				const std::string delEr("delete ");
				const std::string fileEr("delete " + m_filter.getFilterName() + " due to database error");
				//userMsg << delEr;
				//userMsg << fileEr;
				//userMsg.showMsgBox(IDS_UE_040031);

				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(delEr);
				dataContextPtr->pushParamValue(fileEr);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);

				PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(filterName);
				notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);
				return;
			}
			else if (stricmp(m_strCurDlgFilter.c_str(), filterName.c_str()) == 0)
				m_nCurFilterState = -1;

			m_bNMFilterUpdated = true;
			// Go back to default setting when old filter is deleted
			// And recreate all settings
			//OnClearFiltersButton();
			clearFilter();
			//marvin++
			//TD14756
			setUpdateNeeded(true);
			//++marvin
			//TD14756

			//TD15032
			PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(AlarmFilter::FILTER_DEFAULT);
			notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);
			//TD15032
			// Upon successful delete, notify the parent dialog to update the named filter list
			filterIndex = AlarmFilter::INDEX_LIST;
			//AfxGetMainWnd()->PostMessage(WM_ON_FILTER_DLG_DELETE, filterIndex, 0); //TD15032 azenitha
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TD15032: No need to refresh main dialog");
		}
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::cancel()
	{
		FUNCTION_ENTRY("Cancel");
		if (m_bNMFilterUpdated)
		{
			if (m_nCurFilterState == -1) //current deleted
			{
				// Reset the filter combo box
				PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(AlarmFilter::FILTER_DEFAULT);
				notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);
				applyFilter();
			}
			else if (m_nCurFilterState == 1) //current modified
			{
				PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(m_strCurDlgFilter);
				notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);
				applyFilter();
			}
			else
			{
				//Todo: this->getWinApp()->getViewManager()->postMessage(WM_ON_FILTER_DLG_UPDATE, 0, 0);
			}
		}

		FUNCTION_EXIT;
	}

	/////////////////////////////////////////////////////////////////////////////
	TA_Base_Bus::Filter AlarmFilterDlgPModel::createFilter()
	{
		FUNCTION_ENTRY("createFilter");
		// Create a filter object
		TA_Base_Bus::Filter filter;
		ListItems::iterator iter;

		// Filter on subsystem
		for (iter = m_selectedSubsystems.begin(); iter != m_selectedSubsystems.end(); iter++)
		{
			// Add a filter for this KEY
			filter.addFilter(TA_Base_Bus::Filter::FILTER_SUBSYSTEM, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->pkey);
		}

		// Filter on subsystemType
		for (iter = m_selectedSubsystemTypes.begin(); iter != m_selectedSubsystemTypes.end(); iter++)
		{
			// Add a filter for this tring
			filter.addFilter(TA_Base_Bus::Filter::FILTER_SUBSYSTEMTYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->str);
		}

		// Filter on system
		for (iter = m_selectedSystems.begin(); iter != m_selectedSystems.end(); iter++)
		{
			// Add a filter for this tring
			//filter.addFilter(TA_Base_Bus::Filter::FILTER_SUBSYSTEM, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->pkey);			
			filter.addFilter(TA_Base_Bus::Filter::FILTER_SYSTEMTYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->str);
		}
		// Filter on location
		for (iter = m_selectedLocations.begin(); iter != m_selectedLocations.end(); iter++)
		{
			// Add a filter for this KEY
			filter.addFilter(TA_Base_Bus::Filter::FILTER_LOCATION, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->pkey);
		}

		// Filter on operator
		for (iter = m_selectedOperators.begin(); iter != m_selectedOperators.end(); iter++)
		{
			// Add a filter for this STRING
			filter.addFilter(TA_Base_Bus::Filter::FILTER_OPERATOR, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->str);
		}

		// Filter on alarm type
		for (iter = m_selectedAlarmTypes.begin(); iter != m_selectedAlarmTypes.end(); iter++)
		{
			// Add a filter for this STRING
			filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, iter->pkey);
		}

		// Filter on asset
		if (!m_EquipmentID.empty())
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_ASSET_NAME, TA_Base_Bus::Filter::COMPARE_CONTAINS, m_EquipmentID);
		}

		// Filter on description
		if (!m_Description.empty())
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_DESCRIPTION, TA_Base_Bus::Filter::COMPARE_CONTAINS, m_Description);
		}

		// Filter on severity
		unsigned int numSeverityFilters = 0;
		ListSeverityItems::iterator itr = m_selectedServerity.begin();
		for (itr; itr != m_selectedServerity.end(); itr++)
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_SEVERITY, TA_Base_Bus::Filter::COMPARE_EQUAL, itr->pkey);
			++numSeverityFilters;
		}

		if (numSeverityFilters == 0)
		{
			// Create a filter that filters out all severities
			// Severities cannot be negative, so the following filter will do
			// TD #11265
			// Use severity=-1 filter rather than severity<0
			filter.addFilter(TA_Base_Bus::Filter::FILTER_SEVERITY, TA_Base_Bus::Filter::COMPARE_EQUAL, -1);
		}

		// Filter on state
		addStateFilters(filter);

		// Filter on OMType
		addOMTypeFilters(filter);

		// Filter on time
		addTimeFilters(filter);

		FUNCTION_EXIT;
		return filter;
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlgPModel::addTimeFilters(TA_Base_Bus::Filter& filter)
	{
		bool filterOnFromTime = m_chkFromDateTime; // -> == 1) ? true : false;
		bool filterOnToTime = m_chkToDateTime; // ->GetCheck() == 1 ? true : false;

		time_t fromResult, toResult;

		if (filterOnFromTime) //Only get From time if there is request
		{
			fromResult = getCombinedTime(m_FromDate, m_FromTime);
			m_bakfromDateTime = fromResult;
		}

		if (filterOnToTime) //Only get To time if there is request
		{
			toResult = getCombinedTime(m_ToDate, m_ToTime);
			m_bakToDateTime = toResult;
		}

		if (filterOnFromTime && !filterOnToTime)
		{
			// Filter on from time
			filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME,
				TA_Base_Bus::Filter::COMPARE_GREATER_THAN_EQUAL, fromResult);
		}
		else if (!filterOnFromTime && filterOnToTime)
		{
			// Filter on to time
			filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME,
				TA_Base_Bus::Filter::COMPARE_LESS_THAN_EQUAL, toResult);
		}
		else if (filterOnFromTime && filterOnToTime)
		{
			// Filter on both from time and to time
			TA_Base_Bus::Filter::SDateRange range;
			range.startTime = fromResult;
			range.endTime = toResult;

			filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME_RANGE,
				TA_Base_Bus::Filter::COMPARE_RANGE_INCLUSIVE, range);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlgPModel::addOMTypeFilters(TA_Base_Bus::Filter& filter)
	{
		if (m_OperationalMode == OMFilter::OPERATION)
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
				TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_OPERATION);
		}
		else if (m_OperationalMode == OMFilter::MAINTENANCE)
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
				TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_MAINTENANCE);
		}
		else if (m_OperationalMode == OMFilter::BOTH)
		{
			filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
				TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_BOTH);
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlgPModel::addStateFilters(TA_Base_Bus::Filter& filter)
	{
		if (m_AckState == AckFilter::ACKED)
		{
			if (m_AlarmState == StateFilter::OPEN)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_ACKED);
			}
			else if (m_AlarmState == StateFilter::CLOSED)
			{
				// Should never get here.
				TA_ASSERT(false, "Cannot filter on alarms that are acked AND closed");
			}
			else
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_ACKED);
			}
		}
		else if (m_AckState == AckFilter::UNACKED)
		{
			if (m_AlarmState == StateFilter::OPEN)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_UNACKED);
			}
			else if (m_AlarmState == StateFilter::CLOSED)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_CLOSED);
			}
			else
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_ALL_UNACKED);
			}
		}
		else
		{
			if (m_AlarmState == StateFilter::OPEN)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_ALL_OPEN);
			}
			else if (m_AlarmState == StateFilter::CLOSED)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_CLOSED);
			}
			else
			{
				// Do nothing
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Call this when the controls have changed
	//void AlarmFilterService::setUpdateNeeded(bool setUpdateNeeded)
	//{
	//	m_updateNeeded = setUpdateNeeded;
	//	//enableButtons(); //enable the buttons to apply change

	//	// Get the name of selected filter only if the m_namedFilter is not empty
	//	std::string filterName(m_filter.getFilterName());
	//	bool isDefaultFilter = (m_filter.getFilterName() == AlarmFilterService::FILTER_DEFAULT) ? true : false;

	//	// Check whether there is need to change the combox name for filter
	//	// Remain unchanged if the display is reflective of current filter
	//	// Change default to user defined
	//	if (setUpdateNeeded && isDefaultFilter)
	//	{
	//		//selectUserFilter();
	//		//m_saveButton.EnableWindow(false);
	//		m_NamedFilter = NamedFilter::UNNAMED.c_str();
	//	}
	//	//Todo: We need a way to notify GUI to update button states
	//	m_savedFilter = !setUpdateNeeded; //changes made but filter is not saved
	//}

	// Data setters and getters
	
	// Selected Items
	void AlarmFilterDlgPModel::addSelectedSystem(const std::string& system)
	{
		FUNCTION_ENTRY("addSelectedSystem");
		ListItems::iterator it;
		for (it = m_systemTypes.begin(); it != m_systemTypes.end(); ++it)
		{
			if (system.compare((*it).str) == 0)
			{
				m_selectedSystems.push_back(*it);
				break;
			}
		}
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setSelSystemList(const ListItems& systemList)
	{
		FUNCTION_ENTRY("setSystemList");
		m_selectedSystems = systemList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelSystemList()
	{
		FUNCTION_ENTRY("setSystemList");
		FUNCTION_EXIT;
		return m_selectedSystems;
	}

	void AlarmFilterDlgPModel::addSelectedSubsystem(unsigned long subSystemKey)
	{
		FUNCTION_ENTRY("addSelectedSubsystem");
		ListItems::iterator it;
		for (it = m_subsystems.begin(); it != m_subsystems.end(); ++it)
		{
			if (it->pkey == subSystemKey)
			{
				m_selectedSubsystems.push_back(*it);
				break;
			}
		}
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setSelSubSystemList(const ListItems& subSystemList)
	{
		FUNCTION_ENTRY("setSubSystemList");
		m_selectedSubsystems = subSystemList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelSubSystemList()
	{
		FUNCTION_ENTRY("getSubSystemList");
		FUNCTION_EXIT;
		return m_selectedSubsystems;
	}

	void AlarmFilterDlgPModel::addSelectedSubsystemType(const std::string& subSystemType)
	{
		FUNCTION_ENTRY("addSelectedSubsystemType");
		// Find stringValue in m_subsystemTypes and then add the item to m_selectedSubsystemTypes
		for (ListItems::iterator it = m_subsystemTypes.begin(); it != m_subsystemTypes.end(); ++it)
		{
			if (subSystemType.compare((*it).str) == 0)
			{
				m_selectedSubsystemTypes.push_back(*it);
				break; // break out of the for loop
			}
		}
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setSelSubSystemTypeList(const ListItems& subSystemTypeList)
	{
		FUNCTION_ENTRY("setSelSubSystemTypeList");
		m_selectedSubsystemTypes = subSystemTypeList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelSubSystemTypeList()
	{
		FUNCTION_ENTRY("getSelSubSystemTypeList");
		FUNCTION_EXIT;
		return m_selectedSubsystemTypes;
	}

	void AlarmFilterDlgPModel::addSelectedOperator(const std::string& operatorName)
	{
		FUNCTION_ENTRY("addSelectedOperator");
		// Find stringValue in m_operators and then add the item to m_selectedOperators
		for (ListItems::iterator it = m_operators.begin(); it != m_operators.end(); ++it)
		{
			if (operatorName.compare((*it).str) == 0)
			{
				m_selectedOperators.push_back(*it);
				break; // break out of the for loop
			}
		}
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setSelOperatorList(const ListItems& operatorList)
	{
		FUNCTION_ENTRY("setOperatorList");
		m_selectedOperators = operatorList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelOperatorList()
	{
		FUNCTION_ENTRY("getOperatorList");
		FUNCTION_EXIT;
		return m_selectedOperators;
	}

	void AlarmFilterDlgPModel::setSelLocationList(const ListItems& locationList)
	{
		FUNCTION_ENTRY("setLocationList");
		m_selectedLocations = locationList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::addSelectedLocation(unsigned long locationKey)
	{
		FUNCTION_ENTRY("addSelectedLocation");
		ListItems::iterator it;
		for (it = m_locations.begin(); it != m_locations.end(); ++it)
		{
			if (it->pkey == locationKey)
			{
				m_selectedLocations.push_back(*it);
				break;
			}
		}
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelLocationList()
	{
		FUNCTION_ENTRY("getLocationList");
		FUNCTION_EXIT;
		return m_selectedLocations;
	}

	void AlarmFilterDlgPModel::setSelAlarmTypeList(const ListItems& alarmTypeList)
	{
		FUNCTION_ENTRY("setAlarmTypeList");
		m_selectedAlarmTypes = alarmTypeList;
		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::addSelectedAlarmType(unsigned long alarmTypeKey)
	{
		FUNCTION_ENTRY("addSelectedAlarmType");
		// Find stringValue in m_alarmTypes and then add the item to m_selectedAlarmTypes
		for (ListItems::iterator it = m_alarmTypes.begin(); it != m_alarmTypes.end(); ++it)
		{
			if (it->pkey == alarmTypeKey)
			{
				m_selectedAlarmTypes.push_back(*it);
				break; // break out of the for loop
			}

		}
		FUNCTION_EXIT;
	}

	ListItems AlarmFilterDlgPModel::getSelAlarmTypeList()
	{
		FUNCTION_ENTRY("getAlarmTypeList");
		FUNCTION_EXIT;
		return m_selectedAlarmTypes;
	}

	void AlarmFilterDlgPModel::addSelectedSeverity(unsigned long severity)
	{
		FUNCTION_ENTRY("addSelectedSeverity");
		ListSeverityItems::iterator itr = m_severityList.begin();
		for (itr; itr != m_severityList.end(); itr++)
		{
			if (itr->pkey == severity)
			{
				m_selectedServerity.push_back((*itr));
				break;
			}
		}

		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::removeSelectedSeverity(unsigned long severity)
	{
		FUNCTION_ENTRY("addSelectedSeverity");
		ListSeverityItems::iterator itr = m_selectedServerity.begin();
		for (itr; itr != m_selectedServerity.end(); itr++)
		{
			if (itr->pkey == severity)
			{
				m_selectedServerity.erase(itr);
				break;
			}
		}

		FUNCTION_EXIT;
	}
	
	ListSeverityItems AlarmFilterDlgPModel::getSelSeverityList()
	{
		FUNCTION_ENTRY("getSelSeverityList");
		FUNCTION_EXIT;
		return m_selectedServerity;
	}

	void AlarmFilterDlgPModel::changeNamedFilter(const std::string& namedFilter)
	{
		m_NamedFilter = namedFilter;
		bool isCurFilter = namedFilter.compare(m_filter.getFilterName()) ? false : true;
		FilterDlgBtnStateEventArgs* pBtnStateEv = new FilterDlgBtnStateEventArgs();

		bool isDefault = namedFilter.compare(AlarmFilter::FILTER_DEFAULT) ? false : true;
		if (isDefault)
		{
			clearFilter();
			return;
		}

		// Make sure the same filter wasn't selected.
		if (isCurFilter == false)
		{
			std::auto_ptr<NamedFilter> loadedFilter;

			try
			{
				// Load the filter.
				loadedFilter = TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter(namedFilter);
			}
			catch (TA_Base_Core::DataException& ex)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.getWhichData());

				// Display the load filter error message.
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(
						new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));

				dataContextPtr->pushParamValue(sLoad);
				dataContextPtr->pushParamValue(sLoaded);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(
					new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));

				m_appController->getWinApp()->requestUserReply(eventArgs);

				// Reselect the current named filter.
				// Reset the filter combo box
				PopulateFilterSelEventArgs* ev = new PopulateFilterSelEventArgs(&m_filter);
				notifyUpdate(EV_POPULATE_FILTER_SEL, ev);
				return;
			}
			catch (...)
			{
				// Display the load filter error message.
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(sLoad);
				dataContextPtr->pushParamValue(sLoaded);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);

				// Reselect the current named filter.
				PopulateFilterSelEventArgs* ev = new PopulateFilterSelEventArgs(&m_filter);
				notifyUpdate(EV_POPULATE_FILTER_SEL, ev);


				/*TA_Base_Bus::UINotificationEventArgsPtr notifyArgs(
					new TA_Base_Bus::UINotificationEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));

				m_appController->getWinApp()->processUserNotification(eventArgs);*/

				return;
			}

			// Copy the new filter to our internal filter.
			m_nmFilter.clear();
			m_nmFilter = *(loadedFilter.get());

			// Convert the nmFilter to Filter class
			std::vector<std::string> stringList = m_nmFilter.getStringFilterList();
			std::vector<std::string> numericList = m_nmFilter.getNumericFilterList();
			if (stringList.size() > 0 || numericList.size() > 0) 
			{
				m_filter.removeAll();
				m_filter = AlarmFilterService::getInstance().convertNamedFilterToFilter(m_nmFilter);

				PopulateFilterSelEventArgs* ev = new PopulateFilterSelEventArgs(&m_filter);
				notifyUpdate(EV_POPULATE_FILTER_SEL, ev);
			}
			else
			{
				//Warn user of null named filter
				const std::string retEr("retrieve ");
				const std::string fileEr("load using " + namedFilter);

				// Display the load filter error message.
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(retEr);
				dataContextPtr->pushParamValue(fileEr);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);
				return;
			}

			// Set the state of the save and delete buttons.
			//m_namedFilter.SetWindowText(filterName_c); Todo:

			setUpdateNeeded(true); // A new filter selected => need updating
			pBtnStateEv->addButtonUpdate(BTN_SAVE, false);
			pBtnStateEv->addButtonUpdate(BTN_DELETE, true);

			m_savedFilter = true; // A newly loaded filter need not be saved again unless change is made
		}
		else
		{
			setUpdateNeeded(false); // Filter is current => no need to update
			pBtnStateEv->addButtonUpdate(BTN_SAVE, false);
			m_savedFilter = true;
		}

		notifyUpdate(EV_FILTERDLG_BTN_UPDATE, pBtnStateEv);
	}

	void AlarmFilterDlgPModel::setChkFromDateTime(bool bChecked)
	{
		FUNCTION_ENTRY("setChkFromDateTime");
		m_chkFromDateTime = bChecked;
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setFromDate(time_t date)
	{
		FUNCTION_ENTRY("setFromDate");
		m_FromDate = date;

		time_t fromDateTime = getCombinedTime(m_FromDate, m_FromTime);
		time_t toDateTime = getCombinedTime(m_ToDate, m_ToTime);

		m_FromTime = fromDateTime;

		if (toDateTime < fromDateTime)
		{
			m_FromTime = toDateTime; //m_fromTime.SetTime(&toDateTime);
			//Todo: Notify time filter change
		}

		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setFromTime(time_t ptime)
	{
		FUNCTION_ENTRY("setFromTime");
		m_FromTime = ptime;

		time_t currentTime = time(0);

		time_t fromDateTime = getCombinedTime(m_FromDate, m_FromTime);
		time_t toDateTime = getCombinedTime(m_ToDate, m_ToTime);

		if (toDateTime < fromDateTime)
		{
			m_ToTime = fromDateTime;
			//Todo: Notify Time filter change
		}
		setUpdateNeeded(true);

		FUNCTION_EXIT;
	}

	time_t AlarmFilterDlgPModel::getFromDateTime()
	{
		return getCombinedTime(m_FromDate, m_FromTime);
	}

	time_t AlarmFilterDlgPModel::getToDateTime()
	{
		return getCombinedTime(m_ToDate, m_ToTime);
	}

	void AlarmFilterDlgPModel::setChkToDateTime(bool bChecked)
	{
		FUNCTION_ENTRY("setChkToDateTime");
		m_chkToDateTime = bChecked;
		FUNCTION_EXIT;
	}
		
	void AlarmFilterDlgPModel::setToDate(time_t date)
	{
		FUNCTION_ENTRY("setToDate");
		m_ToDate = date;

		time_t fromDateTime = getCombinedTime(m_FromDate, m_FromTime);
		time_t toDateTime = getCombinedTime(m_ToDate, m_ToTime);

		m_ToTime = toDateTime;

		if (toDateTime < fromDateTime)
		{
			m_ToTime = fromDateTime; //m_fromTime.SetTime(&toDateTime);
			//Todo: Notify time filter change
		}

		setUpdateNeeded(true);
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setToTime(time_t time)
	{
		FUNCTION_ENTRY("setToTime");
		m_ToTime = time;

		time_t fromDateTime = getCombinedTime(m_FromDate, m_FromTime);
		time_t toDateTime = getCombinedTime(m_ToDate, m_ToTime);

		if (toDateTime < fromDateTime)
		{
			m_FromTime = toDateTime;
			//Todo: Notify Time filter change
		}
		setUpdateNeeded(true);
		FUNCTION_EXIT
	}

	std::list<std::string> AlarmFilterDlgPModel::getFilterNameList()
	{
		return AlarmFilterService::getInstance().getFilterNameList();
	}

	void AlarmFilterDlgPModel::setUpdateNeeded(bool updateNeeded)
	{
		FUNCTION_ENTRY("setUpdateNeeded");

		m_updateNeeded = updateNeeded;
		FilterDlgBtnStateEventArgs* pBtnStateEv = new FilterDlgBtnStateEventArgs();

		pBtnStateEv->addButtonUpdate(BTN_OK, true);
		pBtnStateEv->addButtonUpdate(BTN_APPLY, true);
		pBtnStateEv->addButtonUpdate(BTN_CANCEL, true);

		// Get the name of selected filter only if the m_namedFilter is not empty
		std::string filterName(m_filter.getFilterName());
		bool isDefaultFilter = (m_filter.getFilterName() == AlarmFilter::FILTER_DEFAULT) ? true : false;

		// Check whether there is need to change the combox name for filter
		// Remain unchanged if the display is reflective of current filter
		// Change default to user defined
		if (updateNeeded && isDefaultFilter)
		{
			selectUserFilter();
			pBtnStateEv->addButtonUpdate(BTN_SAVE, false);
		}
		else
		{
			pBtnStateEv->addButtonUpdate(BTN_SAVE, true);
		}

		m_savedFilter = !updateNeeded; //changes made but filter is not saved

		notifyUpdate(EV_FILTERDLG_BTN_UPDATE, pBtnStateEv);
		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setNamedFilter(std::string filterName)
	{
		FUNCTION_ENTRY("setNamedFilter");

		bool isDefault = (filterName == AlarmFilter::FILTER_DEFAULT) ? true : false;
		
		if (filterName != "" && isDefault == false && AlarmFilterService::getInstance().checkInNamedFilterDatabase(filterName) == true) // This filter can be found in database
		{
			// Load the named filter from database	
			std::auto_ptr<NamedFilter> loadedFilter;

			try
			{
				// Load the filter.
				loadedFilter = TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter(filterName);
			}
			catch (TA_Base_Core::DataException& ex)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.getWhichData());

				// Display the load filter error message.
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(sLoad);
				dataContextPtr->pushParamValue(sLoaded);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);


				return;
			}

			// Copy the new filter to our internal filter.
			m_nmFilter.clear();
			m_nmFilter = *(loadedFilter.get());

			// Convert the nmFilter to Filter class
			std::vector<std::string> stringList = m_nmFilter.getStringFilterList();
			std::vector<std::string> numericList = m_nmFilter.getNumericFilterList();

			if (stringList.size() > 0 || numericList.size() > 0) {
				// Clear the current filter and set it the to named filter
				m_filter.removeAll();
				m_filter = AlarmFilterService::getInstance().convertNamedFilterToFilter(m_nmFilter);
			}
			else
			{
				//Warn user of null named filter
				const std::string retEr("retrieve ");
				const std::string fileEr("load using " + filterName);

				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(retEr);
				dataContextPtr->pushParamValue(fileEr);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);

				return;
			}

		}
		else if (isDefault == true) // This is for default setting
		{
			m_filter.removeAll();
			m_filter.setFilterName(AlarmFilter::FILTER_DEFAULT);
		}
		else // The filter name is undefined or not present
		{
			// Display a error message to notify an invalid filter name
			const std::string retEr("load ");
			std::string fileEr;
			if (filterName == "")
				fileEr = "load using empty string. Please provide a valid name";
			else
				fileEr = "load using " + filterName + " as it is not found in database";

			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(retEr);
			dataContextPtr->pushParamValue(fileEr);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			return;
		}

		// set the alarm control list to this filter
		try
		{
			//clear all the fitlers in the alarm list
			//m_alarmList.setFilter(m_filter, isDefault); // Todo:
			FilterEventArgs* pEvFilter = new FilterEventArgs(isDefault);
			pEvFilter->setFilter(&m_filter);
			//notifyUpdate(EV_SET_ALARM_FILTER, pEvFilter);
			this->getWinApp()->getViewManager()->postBoardcastMessage(WM_SET_ALARM_FILTER, (TRA_WPARAM)(pEvFilter), 0);
		}
		catch (TA_Base_Core::AlarmListCtrlException)
		{
			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UI_040022));
			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);
		}

		FUNCTION_EXIT;
	}

	void AlarmFilterDlgPModel::setNewFilter(std::string filterName)
	{
		FUNCTION_ENTRY("setNamedFilter");

		bool isDefault = (filterName == AlarmFilter::FILTER_DEFAULT) ? true : false;
		bool isUserDefined = (filterName == TA_Base_Core::NamedFilter::UNNAMED) ? true : false;
		//bool isCurFilter = (filterName == m_filter.getFilterName()) ? true : false;
		//if (isCurFilter) return; // No need to reapply current filter

		if (filterName != "" && isDefault == false && isUserDefined == false && AlarmFilterService::getInstance().checkInNamedFilterDatabase(filterName) == true) // This filter can be found in database
		{
			// Load the named filter from database	
			std::auto_ptr<NamedFilter> loadedFilter;

			try
			{ 
				// Load the filter.
				loadedFilter = TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter(filterName);
			}
			catch (TA_Base_Core::DataException& ex)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.getWhichData());

				// Display the load filter error message.
				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(sLoad);
				dataContextPtr->pushParamValue(sLoaded);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);


				return;
			}

			// Copy the new filter to our internal filter.
			m_nmFilter.clear();
			m_nmFilter = *(loadedFilter.get());

			// Convert the nmFilter to Filter class
			std::vector<std::string> stringList = m_nmFilter.getStringFilterList();
			std::vector<std::string> numericList = m_nmFilter.getNumericFilterList();

			if (stringList.size() > 0 || numericList.size() > 0) {
				// Clear the current filter and set it the to named filter
				m_filter.removeAll();
				m_filter = AlarmFilterService::getInstance().convertNamedFilterToFilter(m_nmFilter);
				// Notify Filter change
				PopulateFilterSelEventArgs* pFilterEvent = new PopulateFilterSelEventArgs(&m_filter);
				notifyUpdate(EV_POPULATE_FILTER_SEL, pFilterEvent);

				PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(filterName);
				notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);
			}
			else
			{
				//Warn user of null named filter
				const std::string retEr("retrieve ");
				const std::string fileEr("load using " + filterName);

				TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
				dataContextPtr->pushParamValue(retEr);
				dataContextPtr->pushParamValue(fileEr);

				TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
				m_appController->getWinApp()->requestUserReply(eventArgs);

				return;
			}

			// Set the state of the save and delete buttons.
			FilterDlgBtnStateEventArgs* pBtnStateEv = new FilterDlgBtnStateEventArgs();

			pBtnStateEv->addButtonUpdate(BTN_SAVE, true);
			pBtnStateEv->addButtonUpdate(BTN_DELETE, true);
			notifyUpdate(EV_FILTERDLG_BTN_UPDATE, pBtnStateEv);

			setUpdateNeeded(false);

		}
		else if (isDefault == true) // This is for default setting
		{
			clearFilter();
		}
		else if (isUserDefined == true)
		{
			m_filter.removeAll();
			m_filter = AlarmFilterService::getInstance().getUserDefFilter(); //m_filterUserDef;
			PopulateFilterSelEventArgs* pFilterEvent = new PopulateFilterSelEventArgs(&m_filter);
			notifyUpdate(EV_POPULATE_FILTER_SEL, pFilterEvent);

			PopulateNamedFilterEventArgs* ev = new PopulateNamedFilterEventArgs(filterName);
			notifyUpdate(EV_POPULATE_NAMED_FILTER, ev);

			m_updateNeeded = true;
			//enableButtons();

			//bool filterOnFromTime = (static_cast<CButton*>(GetDlgItem(IDC_FROM_TIME))->GetCheck() == 1) ? true : false;
			//bool filterOnToTime = (static_cast<CButton*>(GetDlgItem(IDC_TO_TIME))->GetCheck() == 1) ? true : false;


			//if (filterOnFromTime) //Only get From time if there is request
			//{
			//	m_fromDate.SetTime(&m_bakfromDateTime);
			//	m_fromTime.SetTime(&m_bakfromDateTime);
			//}
			//if (filterOnToTime) //Only get From time if there is request
			//{
			//	m_toDate.SetTime(&m_bakToDateTime);
			//	m_toTime.SetTime(&m_bakToDateTime);
			//}
		}
		else // The filter name is undefined or not present
		{
			// Display a error message to notify an invalid filter name
			const std::string retEr("load ");
			std::string fileEr;
			if (filterName == "")
				fileEr = "load using empty string. Please provide a valid name";
			else
				fileEr = "load using " + filterName + " as it is not found in database";

			TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_040031));
			dataContextPtr->pushParamValue(retEr);
			dataContextPtr->pushParamValue(fileEr);

			TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_FILTER_DELETED_ERROR));
			m_appController->getWinApp()->requestUserReply(eventArgs);

			return;
		}

		//Apply button is always to be disabled upon start up
		// Set the state of the save and delete buttons.
		FilterDlgBtnStateEventArgs* pBtnStateEv = new FilterDlgBtnStateEventArgs();
		pBtnStateEv->addButtonUpdate(BTN_APPLY, false);
		notifyUpdate(EV_FILTERDLG_BTN_UPDATE, pBtnStateEv);

		FUNCTION_EXIT;
	}
}
