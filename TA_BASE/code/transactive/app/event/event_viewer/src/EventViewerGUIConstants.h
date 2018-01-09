/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source: $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/EventViewerGUIConstants.h $
* @author 
* @version $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by: 
* 
*
*/
#pragma once

namespace 
{
	const int STANDARD_MIN_HEIGHT = 205;
	const int STANDARD_MIN_WIDTH = 500;
	const int OFFSET_NO_STATUSBAR = 5;
	const int OFFSET_NO_TOOLBAR = 50;	
	const int STRETCH_PANE = 0;	
	const int FONT_SIZE = 100;
	const int NAMED_FILTER_LABEL_SEPARATOR_WIDTH = 60;
	const int NAMED_FILTER_LABEL_TOP = 6;
	const int NAMED_FILTER_LABEL_OFFSET = 0;
	const int NAMED_FILTER_COMBO_SEPARATOR_WIDTH = 200;
	const int NAMED_FILTER_COMBO_TOP = 10;
	const int NAMED_FILTER_COMBO_OFFSET = 15;
	const int NAMED_FILTER_COMBO_DROP_HEIGHT = 251;
	const int EVENT_LEVEL_SEPARATOR_ID = 99997;
	const int EVENT_LEVEL_LABEL_SEPARATOR_WIDTH = 50;
	const int EVENT_LEVEL_LABEL_TOP = 6;
	const int EVENT_LEVEL_LABEL_OFFSET = 0;
	const int EVENT_LEVEL_COMBO_SEPARATOR_WIDTH = 200;
	const int EVENT_LEVEL_COMBO_TOP = 10;
	const int EVENT_LEVEL_COMBO_OFFSET = 15;
	const int EVENT_LEVEL_COMBO_DROP_HEIGHT = 251;


	//const CString VIEW_MENU = "&View"; --- not used 
	//const CString COLUMNS_MENU = "Columns";
	//const TAUnicodeString FONT_NAME = _T("Arial Bold");
	//const CString IDLE_TEXT = "Ready";
	//const TAUnicodeString NAMED_FILTER_LABEL_TEXT = _T("Current Filter   ");
	//const TAUnicodeString EVENT_LEVEL_LABEL_TEXT = _T("Event Level");
	const TAUnicodeString PLUS_SYMBOL_STR = _T("+");
	const TAUnicodeString UNICODE_EMPTY_STR = _T("");
	 const TAUnicodeString UNTITLED_LBL_STR = _T("Untitled");
	 const TAUnicodeString  EXPORT_MSG_1_STR = _T("Set and save to Local Directory");

	const long    NAMED_FILTER_LABEL_SEPARATOR_ID = 99995;	
	const long    NAMED_FILTER_COMBO_SEPARATOR_ID = 99996;	
	const long   EVENT_LEVEL_LABEL_SEPARATOR_ID = 99998;	
	const long  EVENT_LEVEL_COMBO_SEPARATOR_ID = 99999;

	    // Number of seconds in 24 hours.
    const unsigned int TWENTY_FOUR_HOURS( 24*60*60 );

    // RunParam constants.
    /*const std::string RPARAM_FILTER_SUBSYSTEM( "FilterSubsystem" );
    const std::string RPARAM_FILTER_LOCATION( "FilterLocation" );
    const std::string RPARAM_FILTER_OPERATOR( "FilterOperator" );
    const std::string RPARAM_FILTER_EVENTTYPE( "FilterEventtype" );  
	const std::string RPARAM_FILTER_SYSTEM( "FilterSystem" ); 
    const std::string RPARAM_FILTER_ASSET( "FilterAsset" );
    const std::string RPARAM_FILTER_DESCRIPTION( "FilterDescription" );
    const std::string RPARAM_FILTER_FROM( "FilterFrom" );
    const std::string RPARAM_FILTER_TO( "FilterTo" );
    const std::string RPARAM_FILTER_SEVERITY( "FilterSeverity" );
    const std::string RPARAM_FILTER_ALARMTYPE( "FilterAlarmtype" );
    const std::string RPARAM_FILTER_ALARMID( "AlarmId" );
	const std::string RPARAM_FILTER_ALARMCHILD( "AlarmChild");
    const std::string RPARAM_FILTER_DEFAULT( "FilterDefault" );*/ //---- defined in PModel layer


	// This array is used by the status bar to set up the two panes it contains.
	UINT indicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_SIZE
	};

	// This array is used by the status bar to set up the two panes it contains.
	UINT threeIndicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAP,
		ID_INDICATOR_SIZE
	};


	const CTime EARLY_TIME_LIMIT(1);
   // const unsigned int TWENTY_FOUR_HOURS( 24*60*60 );

    //const std::string ERROR_DELETE( "delete" );
    //const std::string ERROR_DELETED( "deleted" );


	typedef enum
    {
        NF_USERDEFINED,
        NF_UNSAVED,
        NF_READONLY,
        NF_READWRITE
    } ENamedFilterState;


}

