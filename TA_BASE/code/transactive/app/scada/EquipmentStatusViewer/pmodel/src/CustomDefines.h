/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/CustomDefines.h $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Custom defines used in the entire application
 *
 */

#if !defined(EDSV_CUSTOMDEFINES_H)
#define EDSV_CUSTOMDEFINES_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "core/data_access_interface/entity_access/src/DataNodeEntityData.h"

namespace TA_Base_App
{
	/**
	 *	Identifies the column indices in the Equipment Status List control
	 *
	 */
	enum EslColumns
	{
		DUMMY_COLUMN        = 0,      
		ESL_COLUMN_MIN      = 1,
		ASSET_COLUMN		= 1,
		DESCRIPTION_COLUMN,
		DATAPOINT_COLUMN,
		TYPE_COLUMN,
		VALUE_COLUMN,
		QUALITY_COLUMN,
		STATUS_COLUMN,
		MODIFIED_BY_COLUMN,
		//TIMESTAMP_COLUMN,
		ESL_COLUMN_MAX
	};

	/**
	 *	A FilterElement is just an "on/off" flag and the string to search for if "on"
	 *
	 */
    struct FilterElement
    {
        bool filterInUse;
        std::string filterString;

        //  Default "empty" constructor
        FilterElement() :
            filterInUse(false),
            filterString("") {};

        FilterElement( bool inUse, std::string text) :
            filterInUse( inUse),
            filterString( text) {};
    };

    typedef std::vector< FilterElement >	FilterFlagsAndStrings;


    /**
	 *	Identifies the filter indices for the Filter vectors
	 *
	 */
	enum FilterOffsets
	{
		ESL_FILTER_OFFSET_MIN = 0 ,
		REPORT_TYPE_FILTER_OFFSET = ESL_FILTER_OFFSET_MIN,
		ASSET_FILTER_OFFSET,					// 1
		DESCRIPTION_FILTER_OFFSET,				// 2
		DATAPOINT_NAME_FILTER_OFFSET,			// 3
		DATAPOINT_TYPE_FILTER_OFFSET,			// 4
		DATAPOINT_VALUE_FILTER_OFFSET,			// 5
		STATUS_FILTER_OFFSET,					// 6
		STATUS_SCAN_INHIBIT_FILTER_OFFSET,		// 7
		STATUS_ALARM_INHIBIT_FILTER_OFFSET,		// 8
		STATUS_CONTROL_INHIBIT_FILTER_OFFSET,	// 9
		STATUS_MMS_INHIBIT_FILTER_OFFSET,	    // 10
		STATUS_TAGGED_FILTER_OFFSET,			// 11
		STATUS_TAGGED_PTW_FILTER_OFFSET,		// 12
		STATUS_OVERRIDDEN_FILTER_OFFSET,		// 13
		MODIFIED_BY_FILTER_OFFSET,				// 14
		TIMESTAMP_FILTER_OFFSET,				// 15
        ESL_FILTER_OFFSET_MAX					// 16
	};

    /**
	 *	Special value for the following EslFilterMap.
	 *  This means that there is no equivalent column for this filter element,
     *  so that filter element comparison must be specialised by the derived classes.
	 */
    const int USE_DERIVED_CLASS = -1;

    /**
	 *	Array to map the FilterOffsets to their matching EslColumn
	 *
	 */
    const int EslFilterMap[ ESL_FILTER_OFFSET_MAX] = { 
        USE_DERIVED_CLASS,      // REPORT_TYPE_FILTER_OFFSET (DataNodes or DataPoints only?)
		ASSET_COLUMN,           // ASSET_FILTER_OFFSET
		DESCRIPTION_COLUMN,     // DESCRIPTION_FILTER_OFFSET
		DATAPOINT_COLUMN,       // DATAPOINT_NAME_FILTER_OFFSET
		TYPE_COLUMN,            // DATAPOINT_TYPE_FILTER_OFFSET
		VALUE_COLUMN,           // DATAPOINT_VALUE_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_SCAN_INHIBIT_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_ALARM_INHIBIT_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_CONTROL_INHIBIT_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_MMS_INHIBIT_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_TAGGED_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_TAGGED_PTW_FILTER_OFFSET
		STATUS_COLUMN,          // STATUS_OVERRIDDEN_FILTER_OFFSET
		MODIFIED_BY_COLUMN,     // MODIFIED_BY_FILTER_OFFSET
        //TIMESTAMP_COLUMN        // TIMESTAMP_FILTER_OFFSET
    };


    /**
	 *	Indices into the Predefined Filter strings array
	 *  The user can only tick/untick these filter elements; they can not enter nor select any filter text.
     *  The filter text is built using these strings so that multiple filter elements can examine the text
     *  from the single "STATUS" column.
	 *  The text values to search for must be the same as the text values that the datapoint
	 *  status text is built from. This is where they are centrally defined.
	 */
	enum PredefinedFilterStrings
	{
        SCAN_FILTERSTRING = 0,
        ALARM_FILTERSTRING,
        CONTROL_FILTERSTRING,
        MMS_FILTERSTRING,
        TAGGED_FILTERSTRING,
        TAGGED_PTW_FILTERSTRING,
        //  The following elements are used in constructing status or summary text, though not used to filter
        INHIBITED_FILTERSTRING,
        OVERRIDDEN_FILTERSTRING,
        SEPARATOR_FILTERSTRING,
        SUMMARY_FILTERSTRING,
        STATUS_SEPARATOR_FILTERSTRING,
        PREDEFINED_FILTERSTRING_MAX
    };

	const std::string FilterString[PREDEFINED_FILTERSTRING_MAX] = 
        { "Scan", "Alarm", "Control", "MMS", "Tagged", "Tagged with PTW",
          " Inhibited", "Overridden", ", ", "(Summary)", " / " };


    /**
	 *	Identifies the action to take when processing ESL Item updates
	 *
	 */
    enum ItemUpdateType
    {
        ADD_MODIFY_ITEM = 0,
        DELETE_ITEM
    };


	/**
	 *	Identifies the type of node, applies to both lists and trees
	 *
	 */
	enum Node_Item 
	{ 
		LOCATION	= 0, 
		SYSTEM, 
		SUBSYSTEM, 
		DATANODE, 
		DATAPOINT,
		DATANODE_SUMMARY	
	};

	/**
	 *	Data required to build an EquipmentStatusListItem
	 */
    struct EquipmentReportEntity
    {
        unsigned long entityKey;
        std::string reportItemKey;
        Node_Item itemType;
        boost::shared_ptr<TA_Base_Core::DataNodeEntityData> itemDnEntityData;
        
        EquipmentReportEntity( Node_Item type, unsigned long key, std::string itemKey="")
            : itemType( type), entityKey( key), reportItemKey( itemKey) {};
        EquipmentReportEntity( Node_Item type, boost::shared_ptr<TA_Base_Core::DataNodeEntityData> entitydata, std::string itemKey="")
            : itemType( type), entityKey( entitydata->getKey()), reportItemKey( itemKey), itemDnEntityData(entitydata) {};
    };

	const std::string ACTIVE_PROFILE_ALL("* Active Profile - All Tagged/Tagged with PTW/Inhibited/Overridden");
	const std::string ACTIVE_PROFILE_MMS("* Active Profile - MMS Inhibited");

    enum Active_Profile 
	{ 
		ACTIVE_PROFILE_NONE		= -1, 
		ACTIVE_PROFILES_FIRST	= 0,
		ACTIVE_PROFILE_ALL_DATA	= ACTIVE_PROFILES_FIRST, 
		ACTIVE_PROFILE_MMS_DATA = 1, 
		MAX_ACTIVE_PROFILES		= 2
	};

    enum EIcon
    {
        ESL_RED,
        ESL_GREEN,
        ESL_ORANGE,
        ESL_BLUE,
        ESL_ICON_MAX
    };

	struct Sort_Parameters
	{
		int		sortColumn;			            
		bool	ascending;	
		void*	realData;
		Sort_Parameters() : sortColumn( 0), ascending( true), realData(NULL){}
	};

	enum EDSV_AUDIT_MSG
	{
		EAM_CREATE_REPORT,
		EAM_DEL_REPORT,
		EAM_MOD_REPORT,
		EAM_PRINT_REPORT
	};

#define PROGRESS_BAR_COLOR		RGB(255,128,0)

#define WM_ENTITY_UPDATE		(WM_USER+1)
#define WM_LIST_ITEM_UPDATE		(WM_APP+3)
#define WM_SESSION_CHANGE       (WM_APP+4)
#define WM_SORT_LIST_CTRL		(WM_APP+5)
#define WM_REPOPULATE_ITEM		(WM_APP+6)

const std::string ACTIVE_PROFILE_ALL_REPORT	= "0";
const std::string ACTIVE_PROFILE_MMS_REPORT	= "1";

const std::string REPORT_TYPE_EQUIPMENT	= "Equipment Only";
const std::string REPORT_TYPE_DATAPOINT	= "DataPoints Only";
const std::string REPORT_TYPE_ALL = "Equipment and DataPoints";
//TD17888 marc_bugfix
const std::string RPARAM_EXPORTDIR = "ExportDirectory";

namespace EDSV
{
	const std::string PMODEL_MAIN_DIALOG  =	"EDSVMainDlg";
	const std::string DOMAIN_DATA_CHANGE_ITEM_UPDATE  = "Equipment Item Update";
	const std::string DOMAIN_DATA_CHANGE_SESSION_UPDATE = "Session update";
	const std::string DOMAIN_DATA_CHANGE_REPOPULATE_ITEM = "Repopulate equipment item";
	//Event Args type code
	const int EVENT_ARG_ITEM_UPDATE = 1;
}

} // TA_Base_App

#endif
