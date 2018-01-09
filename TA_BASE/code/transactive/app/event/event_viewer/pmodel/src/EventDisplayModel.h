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

#ifndef EVENT_DISPLAY_MODEL_1234567890_H
#define EVENT_DISPLAY_MODEL_1234567890_H

#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"


namespace TA_Base_App
{
    class EventDisplayModel : public TA_Base_Bus::AbstractPModel
    {
	public:
		EventDisplayModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~EventDisplayModel(void);

		void setFilter(Filter * filter);

		std::string getFilterItemData(const unsigned long index,const std::string& columnName);

		int getCurrentSortColumn();

		bool getCurrentSortOrder();

		void sort(const std::string& columnName, const int columnId);

		void setSelectedItems( std::vector<unsigned long>& selectedItems );

		const std::vector<unsigned long>& getSelectedItems();

		void executeColumnAction(int columnID, std::string columnName);

		void getFilterRowColour( const unsigned long index, COLORREF& foreground, COLORREF& background );

		COLORREF getFilterItemColour(const unsigned long index, const std::string& columnName);

		std::vector<Filter::Column> getFilterColumns();

		COLORREF getBackgroundColour();

		int* getPrintColWidth();
		
		void clearSortMark();
		
		void nextPage();
		
		void previousPage();


	private:
		void executeAction( const unsigned int actionId );

		Filter * m_filter;
		int m_currentSortColumn;
		bool m_sortAscending;
		int * m_printoutColWidths;	
	};
}

#endif // EVENT_DISPLAY_MODEL_1234567890_H