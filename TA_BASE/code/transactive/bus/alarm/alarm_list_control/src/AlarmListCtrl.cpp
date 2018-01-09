/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/AlarmListCtrl.cpp $
* @author:  Karen Graham
* @version: $Revision: #8 $
*
* Last modification: $DateTime: 2016/12/06 12:21:32 $
* Last modified by:  $Author: Noel $
* 
* This is a read-only list of alarms.
*/

#pragma warning(disable:4786)

#include <algorithm>
#include <set>
 

#include "bus/alarm/alarm_common/src/AlarmConstants.h"
 
#include "bus/alarm/alarm_list_control/src/AlarmListCtrl.h"
#include "bus/alarm/alarm_list_control/src/IAlarmListObserver.h"
#include "bus/alarm/alarm_list_control/src/AlarmActionFactory.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionDisplayComment.h"
#include "bus/alarm/alarm_list_control/src/resource.h"
#include "bus/alarm/alarm_list_control/src/IAlarmColumn.h"
#include "bus/alarm/alarm_list_control/src/ActionThread.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnIdentifiers.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSuppressAlarm.h"

#include "bus/alarm/alarm_list_control/src/AlarmListTableModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListControlController.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"

#include "bus/resources/resource.h"

#include "core/exceptions/src/AlarmListCtrlException.h"
#include "core/exceptions/src/AlarmActionException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

#include "bus/alarm/alarm_list_control/src/ConstDefine.h"
#include "bus/alarm/alarm_list_control/src/AlarmListViewFactory.h"
#include "bus/alarm/alarm_list_control/src/AlarmListDelegate.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Core::AlarmActionException;
using TA_Base_Core::ThreadGuard;


namespace TA_Base_Bus
{
	    
	 
    AlarmListCtrl::AlarmListCtrl(bool canColumnsBeSorted /*=true*/, bool canColumnsBeReordered /*=true*/)
        : TransActiveLibraryApp(), 
		AbstractTransActiveView(this, ViewType_AlarmListCtrl),
	    m_alarmActionFactory(NULL),	 
		m_actionThread(NULL),
		m_observer(NULL),
		NO_VALUE(""),
		NO_ALARM_STORE_MSG(tr("Alarms cannot currently be retrieved.")),    // UE-040038
		NO_ALARMS_MATCH_FILTER_MSG(tr("No alarms match current filter."))
    {
        FUNCTION_ENTRY("Constructor");		 	

		m_tableModel= new AlarmListTableModel(this);
		m_tableModel->setTableView(this);
		this->setModel(m_tableModel); // Set the model here to enable the selection model
		//QItemSelectionModel::ClearAndSelect
		this->setSelection(this->rect(), QItemSelectionModel::ClearAndSelect);
		this->setSelectionMode(QAbstractItemView::MultiSelection);
		this->setSelectionBehavior(QAbstractItemView::SelectRows);

		startApp();

		AlarmListViewFactory* vf=new AlarmListViewFactory(this);		 
		this->getViewManager()->setViewFactory(vf);

		attachPModel();
		m_tableModel->setPModel(m_alarmListModel);
		m_alarmListModel->attachView(this);
		m_popupMenu =new QMenu();

		connect(this, SIGNAL(refresh_signal()), this, SLOT(onAlarmStoreRefresh()));
		connect(this, SIGNAL(restart_signal()), this, SLOT(onAlarmStoreRestart()));
		connect(this, SIGNAL(remap_signal()), this, SLOT(onAlarmStoreRemap()));
		connect(this, SIGNAL(unmap_signal()), this, SLOT(onAlarmStoreUnmap()));

		//connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
		/*connect(this->selectionModel(),
			SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
			SLOT(onSelChanged(const QItemSelection &, const QItemSelection &)));*/

		connect(this->selectionModel(),
			SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this,
			SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

		connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClicked(const QModelIndex &)));
		//connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex & )));
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(valueChanged(int )));

		connect(m_tableModel, SIGNAL(updateAlarmObservers(bool, bool, TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo&,
			unsigned long)), this, SLOT(updateObserver(bool, bool, TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo&,
			unsigned long)));

		installEventFilter(this);
		FUNCTION_EXIT;
    }

	/*void AlarmListCtrl::testSlot()
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "testSlot");
	}*/
	
	void AlarmListCtrl::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{		
		if (_stricmp(updateType, DOMAIN_DATA_CHANGE_AlarmStore_Refresh) == 0)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace, "AlarmStore Refresh");
			//m_tableModel->setNewDataFlag();
			emit refresh_signal();
		}
		else if (_stricmp(updateType, DOMAIN_DATA_CHANGE_AlarmStore_Restart) == 0)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "AlarmStore Restart");
			//m_alarmListModel->reSetPreFilterLocationAndSession();
			emit restart_signal();
		}
		else if (_stricmp(updateType, DOMAIN_DATA_CHANGE_AlarmStore_Remap) == 0)
		{
			 LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "AlarmStore Remap");
			 emit remap_signal();
		}
		else if (_stricmp(updateType, DOMAIN_DATA_CHANGE_AlarmStore_Unmap) == 0)
		{
			 LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "AlarmStore Unmap");
			 emit unmap_signal();
		}

	}

	std::string AlarmListCtrl::getMyPModelName()
	{
		return PMODEL_ALARM_LIST_CONTROL;
	}

	 
	void AlarmListCtrl::setupPModel()
	{
		m_alarmListModel = dynamic_cast<AlarmListModel*> (m_pModel);	
		this->setItemDelegate(new AlarmListDelegate(m_alarmListModel, this));
	}

    AlarmListCtrl::~AlarmListCtrl()
    {
		m_alarmListModel->detachView(this);
      
    }


	IApplicationController * AlarmListCtrl::createApplicationController()
	{
		AlarmListControlController* pcontroller=new AlarmListControlController(this, "AlarmListControl");
		return pcontroller;
	}
		
	 
			
	/////////////////////////////////////////////////////////////////////////////
	// Public methods
	/////////////////////////////////////////////////////////////////////////////
 	
	void AlarmListCtrl::executeAction(unsigned long commandId)
    {

		FUNCTION_ENTRY("executeAction");

		TA_ASSERT(m_alarmActionFactory != NULL, "The alarm action factory has not been set. Has startAlarmList() been called");

		AlarmActionFactory::AlarmActionList::const_iterator iter = m_alarmActionFactory->getAlarmActions().begin();
		AlarmActionFactory::AlarmActionList::const_iterator iterEnd = m_alarmActionFactory->getAlarmActions().end();

		m_alarmRetriever.clearCache();
		while (iter != iterEnd)
		{
			if (commandId == INHIBIT_ALARM_BY_TYPE || commandId == INHIBIT_ALARM_BY_DESCRIPTION)
			{
				if ((*iter)->getActionControlId() == INHIBIT_ALARM)
				{
					ActionSuppressAlarm* suppressAction = dynamic_cast <ActionSuppressAlarm*> (*iter);
					suppressAction->setSubActionControlId(commandId);

					m_actionThread->executeAction(INHIBIT_ALARM);
					//Todo: m_hasSelectionChanged = true;
					break;
				}
			}
			else if ((*iter)->getActionControlId() == commandId)
			{
				// flag the last action if it is a ack or close alarm
				unsigned int actionId = (*iter)->getActionControlId();
				if (actionId == ACKNOWLEDGE_ALARM || actionId == CLOSE_AND_ACK_ALARM || actionId == ACKNOWLEDGE_ALL)
				{
					//m_isLastActionAckOrClose = true; // Todo:
				}
				else if (actionId == INHIBIT_ALARM)
				{
					// reset the sub action control id to zero ie. perform the main action
					// not the sub action
					ActionSuppressAlarm* suppressAction = dynamic_cast <ActionSuppressAlarm*> (*iter);
					suppressAction->setSubActionControlId(0); // reset the sub control
				}
				// Indicate that the available actions should be reevaluated.
				m_actionThread->executeAction(commandId);

				//Todo: m_hasSelectionChanged = true;
				break;
			}
			++iter;
		}

		
		FUNCTION_EXIT;
    }
	
    /////////////////////////////////////////////////////////////////////////////
    // Private helper methods
    /////////////////////////////////////////////////////////////////////////////
	
	   
	TAUtfString AlarmListCtrl::getAlarmComment(int item)
	{
		FUNCTION_ENTRY("getAlarmComment");
		TAUtfString comment("");
		if (m_tableModel == NULL)
		{
			FUNCTION_EXIT;
			return NO_VALUE;
		}

		if (item < 0 || item >= static_cast<int>(m_tableModel->rowCount()))
		{
			// Item not within range
			FUNCTION_EXIT;
			return NO_VALUE;
		}

		try
		{
			comment = TAUtfString::createFromMultitByteString(m_alarmListModel->getDisplayDataWrap()->getItemString(item, ALARM_COMMENTS));
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Alarm could not be retrieved. Details field will be set to an empty string");
		}

		FUNCTION_EXIT;
		return comment;
	} 
	
    void AlarmListCtrl::updateObserver(bool wereAlarmsModified, bool wereAlarmsAdded,
		TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo& details,
		unsigned long selectedAlarmCount)
    {
		FUNCTION_ENTRY("AlarmListCtrl::updateObserver");
		// Now update the alarm counts and the highest severity
		
		try
		{
			if (m_observer != NULL)
			{
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Updating the observer alarm counts");
				m_observer->updateAlarmCounts(details.totalAlarms, details.unackAlarms, details.exceededMaxDisplay, details.isLoadingAlarms);

				if (wereAlarmsModified)
				{

					if (details.highestSeverity != 0)
					{
						m_observer->updateAlarmSound(details.highestSeverity, wereAlarmsAdded);
					}
					else
					{
						m_observer->updateAlarmSound(0, wereAlarmsAdded);
					}

				}
			}
		}
		catch (const TA_Base_Core::AlarmListCtrlException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve alarm details");
		}

		
		updateObserverWithDescription(selectedAlarmCount, (details.totalAlarms > 0));
   
		FUNCTION_EXIT;
    }	
    
    void AlarmListCtrl::updateObserverWithActionAvailability()
    {
        LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace, "updateObserverWithActionAvailability");
		if (m_observer == NULL)
		{
			return;
		}
		
		// Now step through all actions and add their availability to the map
		IAlarmListObserver::ItemAvailability itemAvailabilityMap;

		AlarmActionFactory::AlarmActionList::const_iterator action = m_alarmActionFactory->getAlarmActions().begin();
		AlarmActionFactory::AlarmActionList::const_iterator actionEnd = m_alarmActionFactory->getAlarmActions().end();

		m_alarmRetriever.clearCache();
		while (action != actionEnd)
		{
			unsigned int controlId = (*action)->getActionControlId();

			if (controlId != SEPARATOR) // Don't add any separators obviously
			{
				IAlarmListObserver::Availability state = IAlarmListObserver::DISABLED;
				try
				{
					if ((*action)->isAvailable(m_alarmRetriever))
					{
						state = IAlarmListObserver::ENABLED;
					}
				}
				catch (const TA_Base_Core::AlarmListCtrlException& ale)
				{
					std::ostringstream desc;
					desc << "[AlarmListCtrl::updateObserverWithActionAvailability] Caught AlarmListCtrlException - " << ale.what();
					LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", desc.str().c_str());
				}
				catch (...)
				{
					LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "[AlarmListCtrl::updateObserverWithActionAvailability] Caught unknown exception; Could not determine if action is available");
				}

				itemAvailabilityMap[controlId] = state;
			}
			++action;
		}

		m_observer->updateAvailableAlarmActions(itemAvailabilityMap);
      
		LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace, "updateObserverWithActionAvailability out");
    }
		
    void AlarmListCtrl::updateObserverWithDescription(unsigned long selectedAlarmCount, bool valid)
    {
		FUNCTION_ENTRY("AlarmListCtrl::updateObserverWithDescription");
		if (m_observer == NULL)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "No observer");
			FUNCTION_EXIT;
			return;
		}

		// If there are no alarms in the list
		if (!valid)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Selected Alarm details is empty");
			m_observer->setCurrentDescriptionText(NO_ALARMS_MATCH_FILTER_MSG);
			FUNCTION_EXIT;
			return;
		}

		if (m_alarmRetriever.isActionAlreadyRunning())
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Getting from alarm retriever");
			//marvin++
			TAUtfString	strDesc = m_alarmRetriever.getCurrentAction();
			//++marvin
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "setting description text");
			m_observer->setCurrentDescriptionText(strDesc);
		}
		else if (selectedAlarmCount == 0)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Selected alarm count zero");
			m_observer->setCurrentDescriptionText(NO_VALUE);
		}
		else if (selectedAlarmCount == 1)
		{
			// Set with the selected alarm's description
			std::vector<std::string> selectedAlarms = m_alarmListModel->getAlarmDataSync()->getSelectionWrapper()->getSelectedAlarms();
			if (selectedAlarms[0].size() == 0)
			{
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "No value selected");
				m_observer->setCurrentDescriptionText(NO_VALUE);
				FUNCTION_EXIT;
				return;
			}

			try
			{
				TAUtfString strDesc;
				AlarmDataItem * pdataTemp = m_alarmRetriever.getAlarmItem(selectedAlarms[0]);
				if (pdataTemp != NULL)
				{
					std::auto_ptr<TA_Base_Bus::AlarmDataItem> alarmItem(m_alarmRetriever.getAlarmItem(selectedAlarms[0]));
					strDesc = TAUtfString::createFromMultitByteString(alarmItem->alarmDescription);
				}

				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Setting current description");
				m_observer->setCurrentDescriptionText(strDesc);

			}
			catch (const TA_Base_Core::AlarmListCtrlException&)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve the alarm from the alarm store to get a description from it");
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "No alarm store message");
				m_observer->setCurrentDescriptionText(NO_ALARM_STORE_MSG);
			}
			catch (...)
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Could not retrieve the alarm from the alarm store to get a description from it");
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "No alarm store message");
				m_observer->setCurrentDescriptionText(NO_ALARM_STORE_MSG);
			}
		}
		else
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Alarm selected description");
			// Set with the current selected alarm count
			TAUtfString	description;
			description = TAUtfString::number(selectedAlarmCount) + TAUtfString(tr(" alarms selected"));
			m_observer->setCurrentDescriptionText(description);
		}
		FUNCTION_EXIT
    }
		
	void AlarmListCtrl::doPreAction()
	{
		for (AlarmActionFactory::AlarmActionList::const_iterator itAlarmActions = m_alarmActionFactory->getAlarmActions().begin();
		itAlarmActions != m_alarmActionFactory->getAlarmActions().end();
		++itAlarmActions ) //this will take about 15 secs when db is not busy
        {
            try
            {
			//	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "!!!test name %s begin", (*itAlarmActions)->getMenuEntry().c_str());
                (*itAlarmActions)->preformPreAction(m_alarmRetriever);
			//	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "!!!test name %s end", (*itAlarmActions)->getMenuEntry().c_str());
            }
            catch( const TA_Base_Core::AlarmListCtrlException& ale )
            {
                std::ostringstream desc;
                desc << "[AlarmListCtrl::doPreAction] Caught AlarmListCtrlException - " << ale.what();
                LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", desc.str().c_str() );
            }
            catch( ... )
            {
                LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "[AlarmListCtrl::doPreAction] Caught unknown exception" );
            }
        }
		
	}

	//----End  TD15005

	bool AlarmListCtrl::isDeterminateColumn(const long columnID, TAUtfString& strColumnCaption)
	{
		FUNCTION_ENTRY("isDeterminateColumn");
		FUNCTION_EXIT;
		return m_tableModel->isDeterminateColumn(columnID, strColumnCaption.toMultiByteStdString());
	}	

    //Converted-------------------------------------------------------------------------------

	unsigned long AlarmListCtrl::getEarliestAlarmTime()
	{         
		try
		{

			return  m_tableModel->getEarliestAlarmTime();
		}
		catch( const TA_Base_Core::AlarmListCtrlException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve alarm details");
		}

		return 0;
	}

	unsigned long AlarmListCtrl::getLatestAlarmTime()
	{
		try
		{			 
			return  m_tableModel->getLatestAlarmTime();
		}
		catch( const TA_Base_Core::AlarmListCtrlException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve alarm details");
		}

		return 0;
	}
	 
	void AlarmListCtrl::setFilter(const Filter& filter,bool isDefault)
	{
		FUNCTION_ENTRY("setFilter");
		m_alarmListModel->setFilter(filter,isDefault);	
		FUNCTION_EXIT;
	}	 

	void AlarmListCtrl::defaultSort()
	{
		m_alarmListModel->defaultSort();
		if ( RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING))
		{
			this->sortByColumn(0,Qt::DescendingOrder);
		}
		else
		{
			//remove sorting flag
			this->setSortingEnabled(false);
			this->setSortingEnabled(true);
		}
	}

	void AlarmListCtrl::sectionClickedEvent(int column, Qt::SortOrder order)
	{
		bool ascending=true;
		if (order==Qt::DescendingOrder) ascending=false;
		m_tableModel->sortColumn(column,ascending);
	}


	void AlarmListCtrl::updatePreFilterLocationsAndSession( const Filter& preFilter,
		const std::vector<unsigned long>& locations,
		const std::string& sessionID)
	{
		FUNCTION_ENTRY("updatePreFilterLocationsAndSession");		
		// TD12019: Make the alarm store communication occur in a different thread so that communication
		//          issues with the alarm store do not give the appearance that a profile change is
		//          taking a significant amount of time (delays in this method block the control station).
		m_alarmListModel->updatePreFilterLocationsAndSession( preFilter,
			locations,
			sessionID );		
		FUNCTION_EXIT;
	}


	void AlarmListCtrl::popContextMenu(const QPoint& pos)
	{
		QPoint globalPos = this->mapToGlobal(pos);	 
		m_popupMenu->popup(globalPos);
	}

	void AlarmListCtrl::triggeredAction(QAction * action)
	{
		FUNCTION_ENTRY("triggeredAction");
		unsigned int nID=action->data().toUInt();
		try
		{
			executeAction(nID);
		}
		catch(const TA_Base_Core::TransactiveException& te)
		{
			std::ostringstream desc;
			desc << "Caught exception executing action " << nID << "; Reason: " << te.what();
			LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", desc.str().c_str() );
		}
		catch(...)
		{
			std::ostringstream desc;
			desc << "[AlarmListCtrl::onContextMenuItemSelected] Caught unknown exception; nID: " << nID;
			LOG_EXCEPTION_CATCH( SourceInfo, "Unknown", desc.str().c_str() );
		}
		FUNCTION_EXIT;
	}


	void AlarmListCtrl::startAlarmList( AlarmActionFactory& alarmActionFactory,
		const std::vector<unsigned long>& locations,
		const Filter& preFilter,                                  
		const Filter& filter,		
		long sortColumn,
		bool isAscendingSort,
		const std::string& sessionId ,
		bool isHighestSeverityNeeded)
	{
		FUNCTION_ENTRY("startAlarmList");

		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Setting up the alarm retriever and alarm action factory.");		

		m_alarmActionFactory = &alarmActionFactory;
		m_alarmRetriever.setAlarmListCtrl(*this);
		m_alarmRetriever.setSelectionRetriever(m_alarmListModel->getAlarmDataSync()->getSelectionWrapper());
		if (m_actionThread==NULL)
		{
			m_actionThread=new ActionThread(m_alarmActionFactory,&m_alarmRetriever);
		}
		// Create the alarm store so we can get all our alarms to add         
        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "connecting to the alarm store");	
		Sort sort;		 			  
		m_alarmListModel->connectToAlarmStore(preFilter,filter,locations,sessionId,sort,isAscendingSort,isHighestSeverityNeeded);
		if (isAscendingSort)
		{
			this->sortByColumn(sortColumn, Qt::AscendingOrder);				 
		}
		else
		{
			this->sortByColumn(sortColumn,Qt::DescendingOrder);
		}
		
		this->resizeRowsToContents();
		this->setWordWrap(false);				
		
		//set up pop up menu
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
			this, SLOT(popContextMenu(const QPoint&)));
		connect(m_popupMenu,SIGNAL(triggered(QAction *)), this,SLOT(triggeredAction(QAction *)));
		for (AlarmActionFactory::AlarmActionList::const_iterator itAlarmActions = m_alarmActionFactory->getAlarmActions().begin();
			itAlarmActions != m_alarmActionFactory->getAlarmActions().end();
			++itAlarmActions )
		{			 
			unsigned int controlId = (*itAlarmActions)->getActionControlId();
			(*itAlarmActions)->setPModel(m_alarmListModel);
			if (controlId == SEPARATOR)
			{
				//viewMenu->AppendMenu( MF_SEPARATOR );
				m_popupMenu->addSeparator();
			}		 
			else if( (*itAlarmActions)->isAvailableInContextMenu() )
			{
				QAction* action=m_popupMenu->addAction((*itAlarmActions)->getMenuEntry());	
				action->setData(controlId);
				std::map<unsigned int, QString> subControlIds = (*itAlarmActions)->getSubActionControlIds();
				if (subControlIds.size() > 0)
				{
					QMenu* subMenu=new QMenu();
					action->setMenu(subMenu);
					std::map<unsigned int, QString>::iterator subItr = subControlIds.begin();
					for (subItr; subItr!=subControlIds.end(); subItr++)
					{ 
						action=subMenu->addAction(subItr->second);
						action->setData(subItr->first);
					}
				}										
				 
			}
		}			 
		FUNCTION_EXIT;
	}
	
	void AlarmListCtrl::setupAlarmList(  IAlarmListObserver& observer, 
		const std::vector<IAlarmColumn*> &columns, 	 
		bool canColumnsBeReordered)
	{
		FUNCTION_ENTRY("setupAlarmList");
		m_observer = &observer;

		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Setting extended styles of list");

		if(canColumnsBeReordered)
		{
			this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);	
			this->setSortingEnabled(true);
		}
		else
		{
			this->setSortingEnabled(false);			
			this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		}  

		if (TA_Base_Core::RunParams::getInstance().isSet(AlarmConstants::RPARAM_NOGRIDLINES.c_str()) )
		{
			this->setShowGrid(false);
		}	

		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Adding the columns to the list");
		m_tableModel->setColumns(columns);       
		FUNCTION_EXIT;
	}

	RightsChecker& AlarmListCtrl::getRightsChecker()
	{
		FUNCTION_ENTRY("getRightsChecker");
		FUNCTION_EXIT;
		return m_alarmListModel->getRightsChecker();
	}

	DatabaseCache& AlarmListCtrl::getDatabaseCache()
	{
		FUNCTION_ENTRY("getDatabaseCache");
		FUNCTION_EXIT;
		return m_alarmListModel->getDatabaseCache();
	}

	void AlarmListCtrl::onAlarmStoreRefresh()
	{
		FUNCTION_ENTRY("onAlarmStoreRefresh");
		m_tableModel->setNewDataFlag();
		FUNCTION_EXIT;
	}

	void AlarmListCtrl::onAlarmStoreRestart()
	{
		FUNCTION_ENTRY("onAlarmStoreRestart");
		m_alarmListModel->reSetPreFilterLocationAndSession();
		FUNCTION_EXIT;
	}

	void AlarmListCtrl::onAlarmStoreRemap()
	{
		FUNCTION_ENTRY("onAlarmStoreRemap");

		FUNCTION_EXIT;
	}

	void AlarmListCtrl::onALarmStoreUnmap()
	{
		FUNCTION_ENTRY("onALarmStoreUnmap");
		m_tableModel->resetDataTable();
		FUNCTION_EXIT;
	}

	// Process the multiple selection using mouse event
	void AlarmListCtrl::multiselectAlarmItem(int nTargetIndex)
	{
		rangeSelInfo rangeInfo;
		// get updated position of our selected alarm Item

		try
		{
			long nTempPos = m_tableModel->findItemPos(m_strSelectedAlarmID);
			if (nTempPos != -1)
			{
				m_nCurSelPos = nTempPos;
			}

			if (m_nCurSelPos > nTargetIndex)
			{
				rangeInfo.lSelStart = nTargetIndex;
				rangeInfo.lSelEnd = m_nCurSelPos;

				m_nSelDirection = -1; // Selection is going up
			}
			else
			{
				rangeInfo.lSelStart = m_nCurSelPos;
				rangeInfo.lSelEnd = nTargetIndex;

				m_nSelDirection = 1; // Selection is going down
			}

			// m_alarmDataSync.selectRangeAlarms(rangeInfo.lSelStart, rangeInfo.lSelEnd);
			// retrieve the selected item
			if (rangeInfo.lSelStart == rangeInfo.lSelEnd)
				m_tableModel->selectAlarmItem(rangeInfo.lSelStart);
			else
				m_tableModel->selectRangeAlarms(rangeInfo.lSelStart, rangeInfo.lSelEnd);


			updateObserverWithDescription(m_tableModel->getSelectedAlarmsCount(), true);
			updateObserverWithActionAvailability();

			m_rangeSelInfo = rangeInfo; // reset range information
			m_isRangeSel = true; // selection is not ranged

		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Unknown exception occurred.");
		}
	}


	// Process the multiple selection using keyboard events
	void AlarmListCtrl::multiselectAlarmItem(int nTargetIndex, unsigned long nKeyPressed)
	{
		rangeSelInfo rangeInfo;

		try
		{
			// get updated position of our selected alarm Item
			int nTempPos = m_tableModel->findItemPos(m_strSelectedAlarmID);
			if (nTempPos != -1)
			{
				m_nCurSelPos = nTempPos;
			}

			switch (nKeyPressed)
			{	
			case Qt::Key::Key_Down:
			{
				// Selection should start at first selected item to end item.
				int nFocusedIndex = 0;
				rangeInfo.lSelStart = m_nCurSelPos;

				if (m_isRangeSel) // from range selection and arrow down is pressed
				{
					// re-calculate selected alarm position
					long nRange = 0;
					nRange = m_rangeSelInfo.lSelEnd - m_rangeSelInfo.lSelStart;
					//nRange--;
					if (nRange == 0)
					{
						m_nSelDirection = 1; // change the selection direction
					}

					if (m_nSelDirection == 1) // direction is going down
					{
						rangeInfo.lSelEnd = (m_nCurSelPos + (nRange + 1) >= (long)m_tableModel->rowCount()) ? (long)m_tableModel->rowCount() - 1 : m_nCurSelPos + (nRange + 1);
						nFocusedIndex = rangeInfo.lSelEnd;
					}
					else if (m_nSelDirection == -1) // direction is going up
					{
						rangeInfo.lSelStart = (m_nCurSelPos - (nRange - 1) < 0) ? 0 : (m_nCurSelPos - (nRange - 1));
						rangeInfo.lSelEnd = m_nCurSelPos;
						nFocusedIndex = rangeInfo.lSelStart;
					}

				}
				// arrow down is pressed and previous operation is not range selection
				else /*if(!m_isRangeSel) */
				{
					rangeInfo.lSelEnd = (m_nCurSelPos + 1 >= (long)m_tableModel->rowCount()) ? m_tableModel->rowCount() - 1 : (m_nCurSelPos + 1);
					nFocusedIndex = rangeInfo.lSelEnd;
					m_nSelDirection = 1;
				}

				m_bIgnoreEvent = true;
				//SetItemState(nFocusedIndex, LVIS_FOCUSED, LVIS_FOCUSED);
				QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Current | QItemSelectionModel::Rows;
				QModelIndex index = this->model()->index(nFocusedIndex, 0);
				this->selectionModel()->select(index, flags);
			}
			break;
			case Qt::Key::Key_Up:
			{
				int nFocusedIndex = 0;
				rangeInfo.lSelEnd = m_nCurSelPos;

				if (m_isRangeSel) // from range selection
				{
					// re-calculate selected alarm position
					long nRange = 0;
					nRange = m_rangeSelInfo.lSelEnd - m_rangeSelInfo.lSelStart;
					//nRange--;
					// Todo need to check with the current nRange with m_mapSelectedAlarms.size();

					if (nRange == 0)
					{
						m_nSelDirection = -1; // change the selection direction
					}

					// direction is going up
					if (m_nSelDirection == -1)
					{
						rangeInfo.lSelStart = (m_nCurSelPos - (nRange + 1) <= 0) ? 0 : m_nCurSelPos - (nRange + 1);
						nFocusedIndex = rangeInfo.lSelStart;
					}
					// direction is going down
					else if (m_nSelDirection == 1)
					{
						rangeInfo.lSelStart = m_nCurSelPos;
						rangeInfo.lSelEnd = (m_nCurSelPos + (nRange - 1) >= (long)m_tableModel->rowCount()) ? (long)m_tableModel->rowCount() - 1 : m_nCurSelPos + (nRange - 1);
						nFocusedIndex = rangeInfo.lSelEnd;
					}

				}
				// arrow up is pressed and previous operation is not range selection
				else /*if(!m_isRangeSel)*/
				{
					rangeInfo.lSelStart = (m_nCurSelPos - 1 < 0) ? 0 : m_nCurSelPos - 1;
					nFocusedIndex = rangeInfo.lSelStart;
					m_nSelDirection = -1;
				}

				m_bIgnoreEvent = true;
				
				QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Current | QItemSelectionModel::Rows;
				QModelIndex index = this->model()->index(nFocusedIndex, 0);
				this->selectionModel()->select(index, flags);

			}
			break;
			case Qt::Key::Key_PageDown:
			case Qt::Key::Key_End:
				if (m_nSelDirection == -1 && m_nCurSelPos > nTargetIndex)
				{
					rangeInfo.lSelStart = nTargetIndex;
					rangeInfo.lSelEnd = m_nCurSelPos;
				}
				else
				{
					rangeInfo.lSelStart = m_nCurSelPos;
					rangeInfo.lSelEnd = nTargetIndex;
					m_nSelDirection = 1;

				}
				break;
			case Qt::Key::Key_PageUp:
			case Qt::Key::Key_Home:
				if (m_nSelDirection == 1 && m_nCurSelPos < nTargetIndex)
				{
					rangeInfo.lSelStart = m_nCurSelPos;
					rangeInfo.lSelEnd = nTargetIndex;
				}
				else
				{
					rangeInfo.lSelEnd = m_nCurSelPos;
					// Selection should start at beginning to the first selected item.
					rangeInfo.lSelStart = nTargetIndex;
					m_nSelDirection = -1;

				}
				break;
			}

			// retrieve the selected item
			if (rangeInfo.lSelStart == rangeInfo.lSelEnd)
				m_tableModel->selectAlarmItem(rangeInfo.lSelStart);
			else
				m_tableModel->selectRangeAlarms(rangeInfo.lSelStart, rangeInfo.lSelEnd);

			m_rangeSelInfo = rangeInfo;
			m_isRangeSel = true;
			updateObserverWithDescription(m_tableModel->getSelectedAlarmsCount(), true);
			updateObserverWithActionAvailability();

		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Unknown exception occurred.");
		}
	}

	bool AlarmListCtrl::eventFilter(QObject* object, QEvent* event)
	{
		//Todo: if needed
		
		return QWidget::eventFilter(object, event);
	}

	void AlarmListCtrl::currentRowChanged(const QModelIndex & current, const QModelIndex & previous)
	{

		if (!current.isValid())
			return;

		Qt::MouseButtons objBtnState = QApplication::mouseButtons();
		if (objBtnState == Qt::RightButton)
		{	// Ignore right click
			return;
		}
		
		try
		{
			// check if its unselect
			std::string alarmID = m_tableModel->getAlarmIdAt(current.row());
			bool selected = m_tableModel->isAlarmItemSelected(alarmID);
			bool shiftKeyPressed = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
			bool ctrlKeyPressed = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
			
			int newIdx = current.row();
			if (shiftKeyPressed)
			{	// Multiple selection
				m_tableModel->unSelectAllAlarmItem(); // Remove existing selection
				if (!m_strSelectedAlarmID.empty())
				{
					multiselectAlarmItem(newIdx);
				}
				else
					m_tableModel->selectAlarmItem(newIdx);

			}
			else if (ctrlKeyPressed)
			{
				if (selected)
					m_tableModel->unselectAlarmItem(newIdx);
				else
					m_tableModel->selectAlarmItem(newIdx);
			}
			else
			{ // Single Selection
				qDebug() << "Single Selection";
				m_tableModel->unSelectAllAlarmItem(); // Remove existing selection

				if (selected)
					m_strSelectedAlarmID = "";
				else
				{
					m_strSelectedAlarmID = m_tableModel->getAlarmIdAt(newIdx);
					m_tableModel->selectAlarmItem(newIdx);
				}
			}
		
			emit m_tableModel->layoutChanged();
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "unknown exception occurred.");
		}
	}
	
	// Click event
	void AlarmListCtrl::onClicked(const QModelIndex & index)
	{
		//qDebug() << "onClicked called";
		//if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true)
		//{	// Multiple selection
		//	qDebug() << "Multiple selection shift";
		//	if (!m_strSelectedAlarmID.empty())
		//	{
		//		unsigned long ulStartIdx = m_tableModel->findItemPos(m_strSelectedAlarmID);
		//		multiselectAlarmItem(index.row());
		//	}
		//	else
		//		m_tableModel->selectAlarmItem(index.row());

		//}
		//else if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true)
		//{
		//	qDebug() << "Multiple Selection control";
		//	m_tableModel->selectAlarmItem(index.row());
		//}
		//else
		//{ // Single Selection
		//	qDebug() << "Single Selection";
		//	m_tableModel->unSelectAllAlarmItem(); // Remove existing selection

		//	m_strSelectedAlarmID = m_tableModel->getAlarmIdAt(index.row());
		//	m_tableModel->selectAlarmItem(index.row());
		//}
		//
		//emit m_tableModel->layoutChanged();
	}

	// Double click event
	void AlarmListCtrl::onDoubleClicked(const QModelIndex & index)
	{
		qDebug() << "doubleClicked: the cell selection has changed";
	}

	void AlarmListCtrl::valueChanged(int value)
	{
		qDebug() << "valueChanged: the scroll value changed";
		ULONG ulIdxFrom = value; //this->indexAt(this->rect().topLeft()).row();
		int nCountPerPage = 50; //this->indexAt(this->rect().bottomRight()).row() - ulIdxFrom + 1;
		ULONG ulIdxTo = ulIdxFrom + nCountPerPage;

		LOG2(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "AlarmListCtrl::setListDisplayInfo: from index: %d to %d", ulIdxFrom, ulIdxTo);
		m_tableModel->setListDisplayInfo(ulIdxFrom, ulIdxTo);
	}

	void AlarmListCtrl::translateUI()
	{
		FUNCTION_ENTRY("translateUI");
		// Update Context Menu
		// Search the context menu for the correct item
		QList<QAction*> objActions = m_popupMenu->actions();
		QList<QAction*>::iterator itr = objActions.begin();
		for (itr; itr != objActions.end(); itr++)
		{	
			if ((*itr)->isSeparator())
				continue;
		
			int menuID = (*itr)->data().toInt();
			for (AlarmActionFactory::AlarmActionList::const_iterator itAlarmActions = m_alarmActionFactory->getAlarmActions().begin();
				itAlarmActions != m_alarmActionFactory->getAlarmActions().end();
				++itAlarmActions)
			{
				unsigned int controlId = (*itAlarmActions)->getActionControlId();
				if (controlId == menuID)
				{
					(*itr)->setText((*itAlarmActions)->getMenuEntry());
					std::map<unsigned int, QString> subControlIds = (*itAlarmActions)->getSubActionControlIds();
					if (subControlIds.size() > 0)
					{
						QList<QAction*> objSubActions = (*itr)->menu()->actions();
						QList<QAction*>::iterator subitr = objSubActions.begin();
						for (subitr; subitr != objSubActions.end(); subitr++)
						{
							unsigned int subID = (*subitr)->data().toInt();
							std::map<unsigned int, QString>::iterator subItr = subControlIds.begin();
							for (subItr; subItr != subControlIds.end(); subItr++)
							{
								if (subID == subItr->first)
								{
									(*subitr)->setText(subItr->second);
									break;
								}
							}
						}
					}

					break;
				}
			}
		}

		this->horizontalHeader()->viewport()->update();
		this->switchLanguage("");
		FUNCTION_EXIT;
	}

}

