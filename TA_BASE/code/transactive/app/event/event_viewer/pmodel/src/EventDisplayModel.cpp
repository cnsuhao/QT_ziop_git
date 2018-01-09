/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EventDisplayModel.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * This is the basic Model which will be used by many views.
  *
  */

#include "app/event/event_viewer/pmodel/src/EventDisplayModel.h"
#include "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/ActionThread.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DateFormat.h"

using TA_Base_Core::DebugUtil;

namespace TA_Base_App
{

    EventDisplayModel::EventDisplayModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName),m_currentSortColumn(-1),m_sortAscending(false),m_printoutColWidths(NULL)
    {
		m_filter = NULL;
    }

    EventDisplayModel::~EventDisplayModel(void)
    {

    }

	void EventDisplayModel::setFilter(Filter* filter)
	{
		m_filter = filter;
		if(m_printoutColWidths == NULL)
		{
			// TD 12763 since this is the first time, establish the default printout width for each columns
			std::vector<Filter::Column> filterColumns = m_filter->getColumns();
			m_printoutColWidths = new int [filterColumns.size() + 1];
			for (unsigned int i=0 ; i<filterColumns.size(); ++i )
			{
				Filter::Column col = filterColumns[i];
				m_printoutColWidths[i+1] = col.m_width;
			}
		}
	}
	
	std::vector<Filter::Column> EventDisplayModel::getFilterColumns()
	{
		return m_filter->getColumns();	  
	}


	std::string EventDisplayModel::getFilterItemData(const unsigned long index,const std::string& columnName)
	{
		if(m_filter != NULL )
		{
			return m_filter->getItemData(index,columnName);
		}
		return "";
	}

	int EventDisplayModel::getCurrentSortColumn()
	{
		return m_currentSortColumn;
	}

	bool EventDisplayModel::getCurrentSortOrder()
	{
		return m_sortAscending;
	}


	void EventDisplayModel::sort(const std::string& columnName, const int columnId)
	{
		// Toggle order or change to new column
		bool newSortAscending = true;
		if(m_currentSortColumn == columnId)
		{
			newSortAscending = !m_sortAscending;
		}
		m_sortAscending = newSortAscending;

		if( m_currentSortColumn == columnId )
		{
			m_filter->reverse(columnName, m_sortAscending );
		}
		else
		{
			// Save the column ID for processing future sorting.
			m_currentSortColumn = columnId;

			// Need to use the "headerNameStr" here, as the headerName char array has been replaced with the last one selected.
			m_filter->sort(columnName,m_sortAscending);
		}
	}
	
	void EventDisplayModel::setSelectedItems( std::vector<unsigned long>& selectedItems )
	{
		 m_filter->setSelectedItems( selectedItems );
	}

	const std::vector<unsigned long>& EventDisplayModel::getSelectedItems()
	{
		std::vector<unsigned long> selectedItems;
		if(m_filter != NULL)
		{
			return  m_filter->getSelectedItems();
		}
		return selectedItems;
	}

	void EventDisplayModel::executeColumnAction(int columnID, std::string columnName)
	{
		// Retrieve the list of columns from the filter.
		const std::vector<Filter::Column>& columns = m_filter->getColumns();
		if ( columnID < 0 || columnID >= static_cast<int>(columns.size()) )
		{
			return;
		}
		
		// Now find its associated Column object.
		std::vector<Filter::Column>::const_iterator it;
		for ( it=columns.begin(); it!=columns.end(); it++ )
		{
			// Execute the action if there is an associated action and the name of
			// the column is correct.
			if ( ( 0 != it->m_action ) && ( it->m_name == columnName ) )
			{
				executeAction(it->m_action);
				break;
			}
		}
		return;
	}

	void EventDisplayModel::executeAction( const unsigned int actionId )
	{
		// Find the associated action.
		IEventAction* action = m_filter->getActionWithId( actionId );
		if ( NULL != action )
		{
			// Execute it.
			ActionThread  *actionthread = new ActionThread(m_filter,action);			

			//todo delete this pointer in Actionthread Terminate method
		}
		else
		{
			LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Failed to execute action %i. No matching action found.", actionId);
		}
	}


	void EventDisplayModel::getFilterRowColour( const unsigned long index, COLORREF& foreground, COLORREF& background )
	{
		return m_filter->getRowColour( index, foreground, background );
	}
	
	COLORREF EventDisplayModel::getFilterItemColour(const unsigned long index, const std::string& columnName)
	{
		 return m_filter->getItemColour(index,columnName);
	}

	COLORREF EventDisplayModel::getBackgroundColour()
	{
		return m_filter->getBackgroundColour();
	}

	int* EventDisplayModel::getPrintColWidth()
	{
		return m_printoutColWidths;
	}

	void EventDisplayModel::clearSortMark()
	{
		//todo check
		notifyViews(CLEAR_COLUMN_BITMAPMASK.c_str());
		m_currentSortColumn = -1;
		m_sortAscending = false;
	}

	


	

	void EventDisplayModel::previousPage()
	{
		m_filter->previousPage();
	}

	void EventDisplayModel::nextPage()
	{
		m_filter->nextPage();
	}


}