/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/event/event_viewer/src/AvalancheNamedFilter.cpp $
  * @author:  Bradley Cleaver
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class implements the dynamic filtering ability required to filter
  * by the alarm ID of the currently selected event.
  *
  */

//#include "stdafx.h"

#include <string>
#include <vector>

#include "app/event/event_viewer/pmodel/src/Filter.h"
#include "app/event/event_viewer/pmodel/src/AvalancheNamedFilter.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"

namespace
{
    //const std::string TEXT_NO_ALARMID( "No Alarm Filter" );
}

//const std::string AvalancheNamedFilter::NAMEDFILTER_AVALANCHE( "Avalanche Child Events" );

AvalancheNamedFilter::AvalancheNamedFilter( Filter* filter ) : m_filter(filter)
{
}


AvalancheNamedFilter::~AvalancheNamedFilter()
{
}


TA_Base_Core::NamedFilter* AvalancheNamedFilter::clone()
{
    // Prepare the state of the filter.
    removeStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMID );

    // Set the default value for the filter.
    std::string alarmIdFilter = EVDisplayStrRes::GetInstance()->ALARM_FILTER_MESSAGE;

    // Now get the alarm ID of the first event.
    const std::vector<unsigned long>& selected = m_filter->getSelectedItems();
    if ( 0 < selected.size() )
    {
        // Get the first selected item.
        std::string alarmId = m_filter->getItemData( selected[0], EVDisplayStrRes::GetInstance()->FILTER_ALARMID );

        // If the alarm ID exists, use it.
        if ( 0 < alarmId.size() )
        {
            alarmIdFilter = alarmId;
        }
    }

    // Add the alarm id to the filter.
    addStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMID, alarmIdFilter );

    // Reapply the filter name.
    setName( EVDisplayStrRes::GetInstance()->NAMEDFILTER_AVALANCHE );

    // Finally, clone the filter.
    return NamedFilter::clone();
}
