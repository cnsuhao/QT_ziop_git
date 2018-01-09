#include "StdAfx.h"
#include "IDMap.h"
#include "Utility.h"
#include "RegexFactory.h"


std::map<std::string, ID> IDMap::m_common_map;
boost::mutex IDMap::m_mutex;


IDMap::IDMap()
{
    static bool once = load_windows_definition();
}


ID& IDMap::look_up( const std::string& name)
{
    std::map<std::string, ID>::iterator it = m_map.find( name );

    if ( it != m_map.end() )
    {
        return it->second;
    }

    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        std::map<std::string, ID>::iterator it = m_common_map.find( name );

        if ( it != m_common_map.end() )
        {
            return it->second;
        }
    }

    std::cout << "cannot find id: " << name << std::endl;
    static ID default_id;
    default_id.m_name = name;
    return default_id;
}


void IDMap::load_id_definition_from_file( const std::string& rc_h )
{
    std::string s = Utility::get_string_from_file( rc_h );

    if ( s.empty() )
    {
        std::cout << "open failed: " << rc_h << std::endl;
        return;
    }

    const char* id_regex_str =
        "(?x)"
        "^ \\#define \\s+ (\\w+) \\s+ ( [0-9a-fA-FxXlL]+ | \\d+ )"; //$1: name, $2: value

    const boost::regex& e = RegexFactory::instance().create_regex( id_regex_str );
    boost::sregex_iterator it( s.begin(), s.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        const std::string& name = it->str(1);
        const std::string& value = it->str(2);

        if ( boost::istarts_with(value, "0x") )
        {
            size_t decimal = strtol( value.c_str(), NULL, 16 );
            m_map[ name ] = ID( name, boost::lexical_cast<std::string>( decimal ) );
            continue;
        }

        m_map[ name ] = ID( name, value );
    }
}


bool IDMap::load_windows_definition()
{
    // const char* afxres_h = "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\VC\\atlmfc\\include\\afxres.h";
    // load_id_definition_from_file( afxres_h );

    boost::lock_guard<boost::mutex> lock(m_mutex);

    if ( false == m_common_map.empty() )
    {
        return true;
    }

    #define add_id_to_map(n,v) m_common_map[ n ] = ID( n, v )

    // winuser.h
    add_id_to_map( "IDOK", "1" );
    add_id_to_map( "IDCANCEL", "2" );
    add_id_to_map( "IDABORT", "3" );
    add_id_to_map( "IDRETRY", "4" );
    add_id_to_map( "IDIGNORE", "5" );
    add_id_to_map( "IDYES", "6" );
    add_id_to_map( "IDNO", "7" );
    add_id_to_map( "IDCLOSE", "8" );
    add_id_to_map( "IDHELP", "9" );
    add_id_to_map( "IDTRYAGAIN", "10" );
    add_id_to_map( "IDCONTINUE", "11" );
    add_id_to_map( "IDC_STATIC", "-1" );

    // afxres.h
    add_id_to_map( "CBRS_ALIGN_LEFT", "4096" );
    add_id_to_map( "CBRS_ALIGN_TOP", "8192" );
    add_id_to_map( "CBRS_ALIGN_RIGHT", "16384" );
    add_id_to_map( "CBRS_ALIGN_BOTTOM", "32768" );
    add_id_to_map( "CBRS_ALIGN_ANY", "61440" );
    add_id_to_map( "CBRS_BORDER_LEFT", "256" );
    add_id_to_map( "CBRS_BORDER_TOP", "512" );
    add_id_to_map( "CBRS_BORDER_RIGHT", "1024" );
    add_id_to_map( "CBRS_BORDER_BOTTOM", "2048" );
    add_id_to_map( "CBRS_BORDER_ANY", "3840" );
    add_id_to_map( "CBRS_TOOLTIPS", "16" );
    add_id_to_map( "CBRS_FLYBY", "32" );
    add_id_to_map( "CBRS_FLOAT_MULTI", "64" );
    add_id_to_map( "CBRS_BORDER_3D", "128" );
    add_id_to_map( "CBRS_HIDE_INPLACE", "8" );
    add_id_to_map( "CBRS_SIZE_DYNAMIC", "4" );
    add_id_to_map( "CBRS_SIZE_FIXED", "2" );
    add_id_to_map( "CBRS_FLOATING", "1" );
    add_id_to_map( "CBRS_GRIPPER", "4194304" );
    add_id_to_map( "CBRS_ALL", "4259839" );
    add_id_to_map( "CBRS_NOALIGN", "0" );
    add_id_to_map( "ID_MFCLOC_MANIFEST", "1000" );
    add_id_to_map( "ID_INDICATOR_EXT", "59136" );
    add_id_to_map( "ID_INDICATOR_CAPS", "59137" );
    add_id_to_map( "ID_INDICATOR_NUM", "59138" );
    add_id_to_map( "ID_INDICATOR_SCRL", "59139" );
    add_id_to_map( "ID_INDICATOR_OVR", "59140" );
    add_id_to_map( "ID_INDICATOR_REC", "59141" );
    add_id_to_map( "ID_INDICATOR_KANA", "59142" );
    add_id_to_map( "ID_SEPARATOR", "0" );
    add_id_to_map( "AFX_IDW_CONTROLBAR_FIRST", "59392" );
    add_id_to_map( "AFX_IDW_CONTROLBAR_LAST", "59647" );
    add_id_to_map( "AFX_IDW_TOOLBAR", "59392" );
    add_id_to_map( "AFX_IDW_STATUS_BAR", "59393" );
    add_id_to_map( "AFX_IDW_PREVIEW_BAR", "59394" );
    add_id_to_map( "AFX_IDW_RESIZE_BAR", "59395" );
    add_id_to_map( "AFX_IDW_REBAR", "59396" );
    add_id_to_map( "AFX_IDW_DIALOGBAR", "59397" );
    add_id_to_map( "AFX_IDW_DOCKBAR_TOP", "59419" );
    add_id_to_map( "AFX_IDW_DOCKBAR_LEFT", "59420" );
    add_id_to_map( "AFX_IDW_DOCKBAR_RIGHT", "59421" );
    add_id_to_map( "AFX_IDW_DOCKBAR_BOTTOM", "59422" );
    add_id_to_map( "AFX_IDW_DOCKBAR_FLOAT", "59423" );
    add_id_to_map( "AFX_IDW_PANE_FIRST", "59648" );
    add_id_to_map( "AFX_IDW_PANE_LAST", "59903" );
    add_id_to_map( "AFX_IDW_HSCROLL_FIRST", "59904" );
    add_id_to_map( "AFX_IDW_VSCROLL_FIRST", "59920" );
    add_id_to_map( "AFX_IDW_SIZE_BOX", "59936" );
    add_id_to_map( "AFX_IDW_PANE_SAVE", "59937" );
    add_id_to_map( "AFX_IDS_APP_TITLE", "57344" );
    add_id_to_map( "AFX_IDS_IDLEMESSAGE", "57345" );
    add_id_to_map( "AFX_IDS_HELPMODEMESSAGE", "57346" );
    add_id_to_map( "AFX_IDS_APP_TITLE_EMBEDDING", "57347" );
    add_id_to_map( "AFX_IDS_COMPANY_NAME", "57348" );
    add_id_to_map( "AFX_IDS_OBJ_TITLE_INPLACE", "57349" );
    add_id_to_map( "ID_FILE_NEW", "57600" );
    add_id_to_map( "ID_FILE_OPEN", "57601" );
    add_id_to_map( "ID_FILE_CLOSE", "57602" );
    add_id_to_map( "ID_FILE_SAVE", "57603" );
    add_id_to_map( "ID_FILE_SAVE_AS", "57604" );
    add_id_to_map( "ID_FILE_PAGE_SETUP", "57605" );
    add_id_to_map( "ID_FILE_PRINT_SETUP", "57606" );
    add_id_to_map( "ID_FILE_PRINT", "57607" );
    add_id_to_map( "ID_FILE_PRINT_DIRECT", "57608" );
    add_id_to_map( "ID_FILE_PRINT_PREVIEW", "57609" );
    add_id_to_map( "ID_FILE_UPDATE", "57610" );
    add_id_to_map( "ID_FILE_SAVE_COPY_AS", "57611" );
    add_id_to_map( "ID_FILE_SEND_MAIL", "57612" );
    add_id_to_map( "ID_FILE_NEW_FRAME", "57613" );
    add_id_to_map( "ID_FILE_MRU_FIRST", "57616" );
    add_id_to_map( "ID_FILE_MRU_FILE1", "57616" );
    add_id_to_map( "ID_FILE_MRU_FILE2", "57617" );
    add_id_to_map( "ID_FILE_MRU_FILE3", "57618" );
    add_id_to_map( "ID_FILE_MRU_FILE4", "57619" );
    add_id_to_map( "ID_FILE_MRU_FILE5", "57620" );
    add_id_to_map( "ID_FILE_MRU_FILE6", "57621" );
    add_id_to_map( "ID_FILE_MRU_FILE7", "57622" );
    add_id_to_map( "ID_FILE_MRU_FILE8", "57623" );
    add_id_to_map( "ID_FILE_MRU_FILE9", "57624" );
    add_id_to_map( "ID_FILE_MRU_FILE10", "57625" );
    add_id_to_map( "ID_FILE_MRU_FILE11", "57626" );
    add_id_to_map( "ID_FILE_MRU_FILE12", "57627" );
    add_id_to_map( "ID_FILE_MRU_FILE13", "57628" );
    add_id_to_map( "ID_FILE_MRU_FILE14", "57629" );
    add_id_to_map( "ID_FILE_MRU_FILE15", "57630" );
    add_id_to_map( "ID_FILE_MRU_FILE16", "57631" );
    add_id_to_map( "ID_FILE_MRU_LAST", "57631" );
    add_id_to_map( "ID_EDIT_CLEAR", "57632" );
    add_id_to_map( "ID_EDIT_CLEAR_ALL", "57633" );
    add_id_to_map( "ID_EDIT_COPY", "57634" );
    add_id_to_map( "ID_EDIT_CUT", "57635" );
    add_id_to_map( "ID_EDIT_FIND", "57636" );
    add_id_to_map( "ID_EDIT_PASTE", "57637" );
    add_id_to_map( "ID_EDIT_PASTE_LINK", "57638" );
    add_id_to_map( "ID_EDIT_PASTE_SPECIAL", "57639" );
    add_id_to_map( "ID_EDIT_REPEAT", "57640" );
    add_id_to_map( "ID_EDIT_REPLACE", "57641" );
    add_id_to_map( "ID_EDIT_SELECT_ALL", "57642" );
    add_id_to_map( "ID_EDIT_UNDO", "57643" );
    add_id_to_map( "ID_EDIT_REDO", "57644" );
    add_id_to_map( "ID_WINDOW_NEW", "57648" );
    add_id_to_map( "ID_WINDOW_ARRANGE", "57649" );
    add_id_to_map( "ID_WINDOW_CASCADE", "57650" );
    add_id_to_map( "ID_WINDOW_TILE_HORZ", "57651" );
    add_id_to_map( "ID_WINDOW_TILE_VERT", "57652" );
    add_id_to_map( "ID_WINDOW_SPLIT", "57653" );
    add_id_to_map( "AFX_IDM_WINDOW_FIRST", "57648" );
    add_id_to_map( "AFX_IDM_WINDOW_LAST", "57663" );
    add_id_to_map( "AFX_IDM_FIRST_MDICHILD", "65280" );
    add_id_to_map( "ID_APP_ABOUT", "57664" );
    add_id_to_map( "ID_APP_EXIT", "57665" );
    add_id_to_map( "ID_HELP_INDEX", "57666" );
    add_id_to_map( "ID_HELP_FINDER", "57667" );
    add_id_to_map( "ID_HELP_USING", "57668" );
    add_id_to_map( "ID_CONTEXT_HELP", "57669" );
    add_id_to_map( "ID_HELP", "57670" );
    add_id_to_map( "ID_DEFAULT_HELP", "57671" );
    add_id_to_map( "ID_NEXT_PANE", "57680" );
    add_id_to_map( "ID_PREV_PANE", "57681" );
    add_id_to_map( "ID_FORMAT_FONT", "57696" );
    add_id_to_map( "ID_OLE_INSERT_NEW", "57856" );
    add_id_to_map( "ID_OLE_EDIT_LINKS", "57857" );
    add_id_to_map( "ID_OLE_EDIT_CONVERT", "57858" );
    add_id_to_map( "ID_OLE_EDIT_CHANGE_ICON", "57859" );
    add_id_to_map( "ID_OLE_EDIT_PROPERTIES", "57860" );
    add_id_to_map( "ID_OLE_VERB_FIRST", "57872" );
    add_id_to_map( "ID_OLE_VERB_LAST", "57887" );
    add_id_to_map( "AFX_ID_PREVIEW_CLOSE", "58112" );
    add_id_to_map( "AFX_ID_PREVIEW_NUMPAGE", "58113" );
    add_id_to_map( "AFX_ID_PREVIEW_NEXT", "58114" );
    add_id_to_map( "AFX_ID_PREVIEW_PREV", "58115" );
    add_id_to_map( "AFX_ID_PREVIEW_PRINT", "58116" );
    add_id_to_map( "AFX_ID_PREVIEW_ZOOMIN", "58117" );
    add_id_to_map( "AFX_ID_PREVIEW_ZOOMOUT", "58118" );
    add_id_to_map( "ID_VIEW_TOOLBAR", "59392" );
    add_id_to_map( "ID_VIEW_STATUS_BAR", "59393" );
    add_id_to_map( "ID_VIEW_REBAR", "59396" );
    add_id_to_map( "ID_VIEW_AUTOARRANGE", "59397" );
    add_id_to_map( "ID_VIEW_SMALLICON", "59408" );
    add_id_to_map( "ID_VIEW_LARGEICON", "59409" );
    add_id_to_map( "ID_VIEW_LIST", "59410" );
    add_id_to_map( "ID_VIEW_DETAILS", "59411" );
    add_id_to_map( "ID_VIEW_LINEUP", "59412" );
    add_id_to_map( "ID_VIEW_BYNAME", "59413" );
    add_id_to_map( "ID_RECORD_FIRST", "59648" );
    add_id_to_map( "ID_RECORD_LAST", "59649" );
    add_id_to_map( "ID_RECORD_NEXT", "59650" );
    add_id_to_map( "ID_RECORD_PREV", "59651" );
    add_id_to_map( "AFX_IDS_SCFIRST", "61184" );
    add_id_to_map( "AFX_IDS_SCSIZE", "61184" );
    add_id_to_map( "AFX_IDS_SCMOVE", "61185" );
    add_id_to_map( "AFX_IDS_SCMINIMIZE", "61186" );
    add_id_to_map( "AFX_IDS_SCMAXIMIZE", "61187" );
    add_id_to_map( "AFX_IDS_SCNEXTWINDOW", "61188" );
    add_id_to_map( "AFX_IDS_SCPREVWINDOW", "61189" );
    add_id_to_map( "AFX_IDS_SCCLOSE", "61190" );
    add_id_to_map( "AFX_IDS_SCRESTORE", "61202" );
    add_id_to_map( "AFX_IDS_SCTASKLIST", "61203" );
    add_id_to_map( "AFX_IDS_MDICHILD", "61215" );
    add_id_to_map( "AFX_IDS_DESKACCESSORY", "61402" );
    add_id_to_map( "AFX_IDS_OPENFILE", "61440" );
    add_id_to_map( "AFX_IDS_SAVEFILE", "61441" );
    add_id_to_map( "AFX_IDS_ALLFILTER", "61442" );
    add_id_to_map( "AFX_IDS_UNTITLED", "61443" );
    add_id_to_map( "AFX_IDS_SAVEFILECOPY", "61444" );
    add_id_to_map( "AFX_IDS_PREVIEW_CLOSE", "61445" );
    add_id_to_map( "AFX_IDS_UNNAMED_FILE", "61446" );
    add_id_to_map( "AFX_IDS_HIDE", "61457" );
    add_id_to_map( "AFX_IDP_NO_ERROR_AVAILABLE", "61472" );
    add_id_to_map( "AFX_IDS_NOT_SUPPORTED_EXCEPTION", "61473" );
    add_id_to_map( "AFX_IDS_RESOURCE_EXCEPTION", "61474" );
    add_id_to_map( "AFX_IDS_MEMORY_EXCEPTION", "61475" );
    add_id_to_map( "AFX_IDS_USER_EXCEPTION", "61476" );
    add_id_to_map( "AFX_IDS_INVALID_ARG_EXCEPTION", "61477" );
    add_id_to_map( "AFX_IDS_PRINTONPORT", "61504" );
    add_id_to_map( "AFX_IDS_ONEPAGE", "61505" );
    add_id_to_map( "AFX_IDS_TWOPAGE", "61506" );
    add_id_to_map( "AFX_IDS_PRINTPAGENUM", "61507" );
    add_id_to_map( "AFX_IDS_PREVIEWPAGEDESC", "61508" );
    add_id_to_map( "AFX_IDS_PRINTDEFAULTEXT", "61509" );
    add_id_to_map( "AFX_IDS_PRINTDEFAULT", "61510" );
    add_id_to_map( "AFX_IDS_PRINTFILTER", "61511" );
    add_id_to_map( "AFX_IDS_PRINTCAPTION", "61512" );
    add_id_to_map( "AFX_IDS_PRINTTOFILE", "61513" );
    add_id_to_map( "AFX_IDS_OBJECT_MENUITEM", "61568" );
    add_id_to_map( "AFX_IDS_EDIT_VERB", "61569" );
    add_id_to_map( "AFX_IDS_ACTIVATE_VERB", "61570" );
    add_id_to_map( "AFX_IDS_CHANGE_LINK", "61571" );
    add_id_to_map( "AFX_IDS_AUTO", "61572" );
    add_id_to_map( "AFX_IDS_MANUAL", "61573" );
    add_id_to_map( "AFX_IDS_FROZEN", "61574" );
    add_id_to_map( "AFX_IDS_ALL_FILES", "61575" );
    add_id_to_map( "AFX_IDS_SAVE_MENU", "61576" );
    add_id_to_map( "AFX_IDS_UPDATE_MENU", "61577" );
    add_id_to_map( "AFX_IDS_SAVE_AS_MENU", "61578" );
    add_id_to_map( "AFX_IDS_SAVE_COPY_AS_MENU", "61579" );
    add_id_to_map( "AFX_IDS_EXIT_MENU", "61580" );
    add_id_to_map( "AFX_IDS_UPDATING_ITEMS", "61581" );
    add_id_to_map( "AFX_IDS_METAFILE_FORMAT", "61582" );
    add_id_to_map( "AFX_IDS_DIB_FORMAT", "61583" );
    add_id_to_map( "AFX_IDS_BITMAP_FORMAT", "61584" );
    add_id_to_map( "AFX_IDS_LINKSOURCE_FORMAT", "61585" );
    add_id_to_map( "AFX_IDS_EMBED_FORMAT", "61586" );
    add_id_to_map( "AFX_IDS_PASTELINKEDTYPE", "61588" );
    add_id_to_map( "AFX_IDS_UNKNOWNTYPE", "61589" );
    add_id_to_map( "AFX_IDS_RTF_FORMAT", "61590" );
    add_id_to_map( "AFX_IDS_TEXT_FORMAT", "61591" );
    add_id_to_map( "AFX_IDS_INVALID_CURRENCY", "61592" );
    add_id_to_map( "AFX_IDS_INVALID_DATETIME", "61593" );
    add_id_to_map( "AFX_IDS_INVALID_DATETIMESPAN", "61594" );
    add_id_to_map( "AFX_IDP_INVALID_FILENAME", "61696" );
    add_id_to_map( "AFX_IDP_FAILED_TO_OPEN_DOC", "61697" );
    add_id_to_map( "AFX_IDP_FAILED_TO_SAVE_DOC", "61698" );
    add_id_to_map( "AFX_IDP_ASK_TO_SAVE", "61699" );
    add_id_to_map( "AFX_IDP_FAILED_TO_CREATE_DOC", "61700" );
    add_id_to_map( "AFX_IDP_FILE_TOO_LARGE", "61701" );
    add_id_to_map( "AFX_IDP_FAILED_TO_START_PRINT", "61702" );
    add_id_to_map( "AFX_IDP_FAILED_TO_LAUNCH_HELP", "61703" );
    add_id_to_map( "AFX_IDP_INTERNAL_FAILURE", "61704" );
    add_id_to_map( "AFX_IDP_COMMAND_FAILURE", "61705" );
    add_id_to_map( "AFX_IDP_FAILED_MEMORY_ALLOC", "61706" );
    add_id_to_map( "AFX_IDP_UNREG_DONE", "61707" );
    add_id_to_map( "AFX_IDP_UNREG_FAILURE", "61708" );
    add_id_to_map( "AFX_IDP_DLL_LOAD_FAILED", "61709" );
    add_id_to_map( "AFX_IDP_DLL_BAD_VERSION", "61710" );
    add_id_to_map( "AFX_IDP_PARSE_INT", "61712" );
    add_id_to_map( "AFX_IDP_PARSE_REAL", "61713" );
    add_id_to_map( "AFX_IDP_PARSE_INT_RANGE", "61714" );
    add_id_to_map( "AFX_IDP_PARSE_REAL_RANGE", "61715" );
    add_id_to_map( "AFX_IDP_PARSE_STRING_SIZE", "61716" );
    add_id_to_map( "AFX_IDP_PARSE_RADIO_BUTTON", "61717" );
    add_id_to_map( "AFX_IDP_PARSE_BYTE", "61718" );
    add_id_to_map( "AFX_IDP_PARSE_UINT", "61719" );
    add_id_to_map( "AFX_IDP_PARSE_DATETIME", "61720" );
    add_id_to_map( "AFX_IDP_PARSE_CURRENCY", "61721" );
    add_id_to_map( "AFX_IDP_PARSE_GUID", "61722" );
    add_id_to_map( "AFX_IDP_PARSE_TIME", "61723" );
    add_id_to_map( "AFX_IDP_PARSE_DATE", "61724" );
    add_id_to_map( "AFX_IDP_FAILED_INVALID_FORMAT", "61728" );
    add_id_to_map( "AFX_IDP_FAILED_INVALID_PATH", "61729" );
    add_id_to_map( "AFX_IDP_FAILED_DISK_FULL", "61730" );
    add_id_to_map( "AFX_IDP_FAILED_ACCESS_READ", "61731" );
    add_id_to_map( "AFX_IDP_FAILED_ACCESS_WRITE", "61732" );
    add_id_to_map( "AFX_IDP_FAILED_IO_ERROR_READ", "61733" );
    add_id_to_map( "AFX_IDP_FAILED_IO_ERROR_WRITE", "61734" );
    add_id_to_map( "AFX_IDP_SCRIPT_ERROR", "61744" );
    add_id_to_map( "AFX_IDP_SCRIPT_DISPATCH_EXCEPTION", "61745" );
    add_id_to_map( "AFX_IDP_STATIC_OBJECT", "61824" );
    add_id_to_map( "AFX_IDP_FAILED_TO_CONNECT", "61825" );
    add_id_to_map( "AFX_IDP_SERVER_BUSY", "61826" );
    add_id_to_map( "AFX_IDP_BAD_VERB", "61827" );
    add_id_to_map( "AFX_IDS_NOT_DOCOBJECT", "61828" );
    add_id_to_map( "AFX_IDP_FAILED_TO_NOTIFY", "61829" );
    add_id_to_map( "AFX_IDP_FAILED_TO_LAUNCH", "61830" );
    add_id_to_map( "AFX_IDP_ASK_TO_UPDATE", "61831" );
    add_id_to_map( "AFX_IDP_FAILED_TO_UPDATE", "61832" );
    add_id_to_map( "AFX_IDP_FAILED_TO_REGISTER", "61833" );
    add_id_to_map( "AFX_IDP_FAILED_TO_AUTO_REGISTER", "61834" );
    add_id_to_map( "AFX_IDP_FAILED_TO_CONVERT", "61835" );
    add_id_to_map( "AFX_IDP_GET_NOT_SUPPORTED", "61836" );
    add_id_to_map( "AFX_IDP_SET_NOT_SUPPORTED", "61837" );
    add_id_to_map( "AFX_IDP_ASK_TO_DISCARD", "61838" );
    add_id_to_map( "AFX_IDP_FAILED_TO_CREATE", "61839" );
    add_id_to_map( "AFX_IDP_FAILED_MAPI_LOAD", "61840" );
    add_id_to_map( "AFX_IDP_INVALID_MAPI_DLL", "61841" );
    add_id_to_map( "AFX_IDP_FAILED_MAPI_SEND", "61842" );
    add_id_to_map( "AFX_IDP_FILE_NONE", "61856" );
    add_id_to_map( "AFX_IDP_FILE_GENERIC", "61857" );
    add_id_to_map( "AFX_IDP_FILE_NOT_FOUND", "61858" );
    add_id_to_map( "AFX_IDP_FILE_BAD_PATH", "61859" );
    add_id_to_map( "AFX_IDP_FILE_TOO_MANY_OPEN", "61860" );
    add_id_to_map( "AFX_IDP_FILE_ACCESS_DENIED", "61861" );
    add_id_to_map( "AFX_IDP_FILE_INVALID_FILE", "61862" );
    add_id_to_map( "AFX_IDP_FILE_REMOVE_CURRENT", "61863" );
    add_id_to_map( "AFX_IDP_FILE_DIR_FULL", "61864" );
    add_id_to_map( "AFX_IDP_FILE_BAD_SEEK", "61865" );
    add_id_to_map( "AFX_IDP_FILE_HARD_IO", "61866" );
    add_id_to_map( "AFX_IDP_FILE_SHARING", "61867" );
    add_id_to_map( "AFX_IDP_FILE_LOCKING", "61868" );
    add_id_to_map( "AFX_IDP_FILE_DISKFULL", "61869" );
    add_id_to_map( "AFX_IDP_FILE_EOF", "61870" );
    add_id_to_map( "AFX_IDP_ARCH_NONE", "61872" );
    add_id_to_map( "AFX_IDP_ARCH_GENERIC", "61873" );
    add_id_to_map( "AFX_IDP_ARCH_READONLY", "61874" );
    add_id_to_map( "AFX_IDP_ARCH_ENDOFFILE", "61875" );
    add_id_to_map( "AFX_IDP_ARCH_WRITEONLY", "61876" );
    add_id_to_map( "AFX_IDP_ARCH_BADINDEX", "61877" );
    add_id_to_map( "AFX_IDP_ARCH_BADCLASS", "61878" );
    add_id_to_map( "AFX_IDP_ARCH_BADSCHEMA", "61879" );
    add_id_to_map( "AFX_IDS_OCC_SCALEUNITS_PIXELS", "61888" );
    add_id_to_map( "AFX_IDS_STATUS_FONT", "62000" );
    add_id_to_map( "AFX_IDS_TOOLTIP_FONT", "62001" );
    add_id_to_map( "AFX_IDS_UNICODE_FONT", "62002" );
    add_id_to_map( "AFX_IDS_MINI_FONT", "62003" );
    add_id_to_map( "AFX_IDP_SQL_FIRST", "62080" );
    add_id_to_map( "AFX_IDP_SQL_CONNECT_FAIL", "62081" );
    add_id_to_map( "AFX_IDP_SQL_RECORDSET_FORWARD_ONLY", "62082" );
    add_id_to_map( "AFX_IDP_SQL_EMPTY_COLUMN_LIST", "62083" );
    add_id_to_map( "AFX_IDP_SQL_FIELD_SCHEMA_MISMATCH", "62084" );
    add_id_to_map( "AFX_IDP_SQL_ILLEGAL_MODE", "62085" );
    add_id_to_map( "AFX_IDP_SQL_MULTIPLE_ROWS_AFFECTED", "62086" );
    add_id_to_map( "AFX_IDP_SQL_NO_CURRENT_RECORD", "62087" );
    add_id_to_map( "AFX_IDP_SQL_NO_ROWS_AFFECTED", "62088" );
    add_id_to_map( "AFX_IDP_SQL_RECORDSET_READONLY", "62089" );
    add_id_to_map( "AFX_IDP_SQL_SQL_NO_TOTAL", "62090" );
    add_id_to_map( "AFX_IDP_SQL_ODBC_LOAD_FAILED", "62091" );
    add_id_to_map( "AFX_IDP_SQL_DYNASET_NOT_SUPPORTED", "62092" );
    add_id_to_map( "AFX_IDP_SQL_SNAPSHOT_NOT_SUPPORTED", "62093" );
    add_id_to_map( "AFX_IDP_SQL_API_CONFORMANCE", "62094" );
    add_id_to_map( "AFX_IDP_SQL_SQL_CONFORMANCE", "62095" );
    add_id_to_map( "AFX_IDP_SQL_NO_DATA_FOUND", "62096" );
    add_id_to_map( "AFX_IDP_SQL_ROW_UPDATE_NOT_SUPPORTED", "62097" );
    add_id_to_map( "AFX_IDP_SQL_ODBC_V2_REQUIRED", "62098" );
    add_id_to_map( "AFX_IDP_SQL_NO_POSITIONED_UPDATES", "62099" );
    add_id_to_map( "AFX_IDP_SQL_LOCK_MODE_NOT_SUPPORTED", "62100" );
    add_id_to_map( "AFX_IDP_SQL_DATA_TRUNCATED", "62101" );
    add_id_to_map( "AFX_IDP_SQL_ROW_FETCH", "62102" );
    add_id_to_map( "AFX_IDP_SQL_INCORRECT_ODBC", "62103" );
    add_id_to_map( "AFX_IDP_SQL_UPDATE_DELETE_FAILED", "62104" );
    add_id_to_map( "AFX_IDP_SQL_DYNAMIC_CURSOR_NOT_SUPPORTED", "62105" );
    add_id_to_map( "AFX_IDP_SQL_FIELD_NOT_FOUND", "62106" );
    add_id_to_map( "AFX_IDP_SQL_BOOKMARKS_NOT_SUPPORTED", "62107" );
    add_id_to_map( "AFX_IDP_SQL_BOOKMARKS_NOT_ENABLED", "62108" );
    add_id_to_map( "AFX_IDS_DELETED", "62109" );
    add_id_to_map( "AFX_IDP_DAO_FIRST", "62128" );
    add_id_to_map( "AFX_IDP_DAO_ENGINE_INITIALIZATION", "62128" );
    add_id_to_map( "AFX_IDP_DAO_DFX_BIND", "62129" );
    add_id_to_map( "AFX_IDP_DAO_OBJECT_NOT_OPEN", "62130" );
    add_id_to_map( "AFX_IDP_DAO_ROWTOOSHORT", "62131" );
    add_id_to_map( "AFX_IDP_DAO_BADBINDINFO", "62132" );
    add_id_to_map( "AFX_IDP_DAO_COLUMNUNAVAILABLE", "62133" );
    add_id_to_map( "AFX_IDS_HTTP_TITLE", "62161" );
    add_id_to_map( "AFX_IDS_HTTP_NO_TEXT", "62162" );
    add_id_to_map( "AFX_IDS_HTTP_BAD_REQUEST", "62163" );
    add_id_to_map( "AFX_IDS_HTTP_AUTH_REQUIRED", "62164" );
    add_id_to_map( "AFX_IDS_HTTP_FORBIDDEN", "62165" );
    add_id_to_map( "AFX_IDS_HTTP_NOT_FOUND", "62166" );
    add_id_to_map( "AFX_IDS_HTTP_SERVER_ERROR", "62167" );
    add_id_to_map( "AFX_IDS_HTTP_NOT_IMPLEMENTED", "62168" );
    add_id_to_map( "AFX_IDS_CHECKLISTBOX_UNCHECK", "62177" );
    add_id_to_map( "AFX_IDS_CHECKLISTBOX_CHECK", "62178" );
    add_id_to_map( "AFX_IDS_CHECKLISTBOX_MIXED", "62179" );
    add_id_to_map( "AFX_IDC_LISTBOX", "100" );
    add_id_to_map( "AFX_IDC_CHANGE", "101" );
    add_id_to_map( "AFX_IDC_BROWSER", "102" );
    add_id_to_map( "AFX_IDC_PRINT_DOCNAME", "201" );
    add_id_to_map( "AFX_IDC_PRINT_PRINTERNAME", "202" );
    add_id_to_map( "AFX_IDC_PRINT_PORTNAME", "203" );
    add_id_to_map( "AFX_IDC_PRINT_PAGENUM", "204" );
    add_id_to_map( "ID_APPLY_NOW", "12321" );
    add_id_to_map( "ID_WIZBACK", "12323" );
    add_id_to_map( "ID_WIZNEXT", "12324" );
    add_id_to_map( "ID_WIZFINISH", "12325" );
    add_id_to_map( "AFX_IDC_TAB_CONTROL", "12320" );
    add_id_to_map( "AFX_IDD_FILEOPEN", "28676" );
    add_id_to_map( "AFX_IDD_FILESAVE", "28677" );
    add_id_to_map( "AFX_IDD_FONT", "28678" );
    add_id_to_map( "AFX_IDD_COLOR", "28679" );
    add_id_to_map( "AFX_IDD_PRINT", "28680" );
    add_id_to_map( "AFX_IDD_PRINTSETUP", "28681" );
    add_id_to_map( "AFX_IDD_FIND", "28682" );
    add_id_to_map( "AFX_IDD_REPLACE", "28683" );
    add_id_to_map( "AFX_IDD_NEWTYPEDLG", "30721" );
    add_id_to_map( "AFX_IDD_PRINTDLG", "30722" );
    add_id_to_map( "AFX_IDD_PREVIEW_TOOLBAR", "30723" );
    add_id_to_map( "AFX_IDD_INSERTOBJECT", "30724" );
    add_id_to_map( "AFX_IDD_CHANGEICON", "30725" );
    add_id_to_map( "AFX_IDD_CONVERT", "30726" );
    add_id_to_map( "AFX_IDD_PASTESPECIAL", "30727" );
    add_id_to_map( "AFX_IDD_EDITLINKS", "30728" );
    add_id_to_map( "AFX_IDD_FILEBROWSE", "30729" );
    add_id_to_map( "AFX_IDD_BUSY", "30730" );
    add_id_to_map( "AFX_IDD_OBJECTPROPERTIES", "30732" );
    add_id_to_map( "AFX_IDD_CHANGESOURCE", "30733" );
    add_id_to_map( "AFX_IDD_EMPTYDIALOG", "30734" );
    add_id_to_map( "AFX_IDC_CONTEXTHELP", "30977" );
    add_id_to_map( "AFX_IDC_MAGNIFY", "30978" );
    add_id_to_map( "AFX_IDC_SMALLARROWS", "30979" );
    add_id_to_map( "AFX_IDC_HSPLITBAR", "30980" );
    add_id_to_map( "AFX_IDC_VSPLITBAR", "30981" );
    add_id_to_map( "AFX_IDC_NODROPCRSR", "30982" );
    add_id_to_map( "AFX_IDC_TRACKNWSE", "30983" );
    add_id_to_map( "AFX_IDC_TRACKNESW", "30984" );
    add_id_to_map( "AFX_IDC_TRACKNS", "30985" );
    add_id_to_map( "AFX_IDC_TRACKWE", "30986" );
    add_id_to_map( "AFX_IDC_TRACK4WAY", "30987" );
    add_id_to_map( "AFX_IDC_MOVE4WAY", "30988" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_NW", "30998" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_N", "30999" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_NE", "31000" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_W", "31001" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_HV", "31002" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_E", "31003" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_SW", "31004" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_S", "31005" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_SE", "31006" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_HORZ", "31007" );
    add_id_to_map( "AFX_IDC_MOUSE_PAN_VERT", "31008" );
    add_id_to_map( "AFX_IDC_MOUSE_ORG_HORZ", "31009" );
    add_id_to_map( "AFX_IDC_MOUSE_ORG_VERT", "31010" );
    add_id_to_map( "AFX_IDC_MOUSE_ORG_HV", "31011" );
    add_id_to_map( "AFX_IDC_MOUSE_MASK", "31012" );
    add_id_to_map( "AFX_IDB_MINIFRAME_MENU", "30994" );
    add_id_to_map( "AFX_IDB_CHECKLISTBOX_95", "30996" );
    add_id_to_map( "AFX_IDR_PREVIEW_ACCEL", "30997" );
    add_id_to_map( "AFX_IDI_STD_MDIFRAME", "31233" );
    add_id_to_map( "AFX_IDI_STD_FRAME", "31234" );
    add_id_to_map( "AFX_IDC_FONTPROP", "1000" );
    add_id_to_map( "AFX_IDC_FONTNAMES", "1001" );
    add_id_to_map( "AFX_IDC_FONTSTYLES", "1002" );
    add_id_to_map( "AFX_IDC_FONTSIZES", "1003" );
    add_id_to_map( "AFX_IDC_STRIKEOUT", "1004" );
    add_id_to_map( "AFX_IDC_UNDERLINE", "1005" );
    add_id_to_map( "AFX_IDC_SAMPLEBOX", "1006" );
    add_id_to_map( "AFX_IDC_COLOR_BLACK", "1100" );
    add_id_to_map( "AFX_IDC_COLOR_WHITE", "1101" );
    add_id_to_map( "AFX_IDC_COLOR_RED", "1102" );
    add_id_to_map( "AFX_IDC_COLOR_GREEN", "1103" );
    add_id_to_map( "AFX_IDC_COLOR_BLUE", "1104" );
    add_id_to_map( "AFX_IDC_COLOR_YELLOW", "1105" );
    add_id_to_map( "AFX_IDC_COLOR_MAGENTA", "1106" );
    add_id_to_map( "AFX_IDC_COLOR_CYAN", "1107" );
    add_id_to_map( "AFX_IDC_COLOR_GRAY", "1108" );
    add_id_to_map( "AFX_IDC_COLOR_LIGHTGRAY", "1109" );
    add_id_to_map( "AFX_IDC_COLOR_DARKRED", "1110" );
    add_id_to_map( "AFX_IDC_COLOR_DARKGREEN", "1111" );
    add_id_to_map( "AFX_IDC_COLOR_DARKBLUE", "1112" );
    add_id_to_map( "AFX_IDC_COLOR_LIGHTBROWN", "1113" );
    add_id_to_map( "AFX_IDC_COLOR_DARKMAGENTA", "1114" );
    add_id_to_map( "AFX_IDC_COLOR_DARKCYAN", "1115" );
    add_id_to_map( "AFX_IDC_COLORPROP", "1116" );
    add_id_to_map( "AFX_IDC_SYSTEMCOLORS", "1117" );
    add_id_to_map( "AFX_IDC_PROPNAME", "1201" );
    add_id_to_map( "AFX_IDC_PICTURE", "1202" );
    add_id_to_map( "AFX_IDC_BROWSE", "1203" );
    add_id_to_map( "AFX_IDC_CLEAR", "1204" );
    add_id_to_map( "AFX_IDD_PROPPAGE_COLOR", "32257" );
    add_id_to_map( "AFX_IDD_PROPPAGE_FONT", "32258" );
    add_id_to_map( "AFX_IDD_PROPPAGE_PICTURE", "32259" );
    add_id_to_map( "AFX_IDB_TRUETYPE", "32384" );
    add_id_to_map( "AFX_IDS_PROPPAGE_UNKNOWN", "65025" );
    add_id_to_map( "AFX_IDS_COLOR_DESKTOP", "65028" );
    add_id_to_map( "AFX_IDS_COLOR_APPWORKSPACE", "65029" );
    add_id_to_map( "AFX_IDS_COLOR_WNDBACKGND", "65030" );
    add_id_to_map( "AFX_IDS_COLOR_WNDTEXT", "65031" );
    add_id_to_map( "AFX_IDS_COLOR_MENUBAR", "65032" );
    add_id_to_map( "AFX_IDS_COLOR_MENUTEXT", "65033" );
    add_id_to_map( "AFX_IDS_COLOR_ACTIVEBAR", "65034" );
    add_id_to_map( "AFX_IDS_COLOR_INACTIVEBAR", "65035" );
    add_id_to_map( "AFX_IDS_COLOR_ACTIVETEXT", "65036" );
    add_id_to_map( "AFX_IDS_COLOR_INACTIVETEXT", "65037" );
    add_id_to_map( "AFX_IDS_COLOR_ACTIVEBORDER", "65038" );
    add_id_to_map( "AFX_IDS_COLOR_INACTIVEBORDER", "65039" );
    add_id_to_map( "AFX_IDS_COLOR_WNDFRAME", "65040" );
    add_id_to_map( "AFX_IDS_COLOR_SCROLLBARS", "65041" );
    add_id_to_map( "AFX_IDS_COLOR_BTNFACE", "65042" );
    add_id_to_map( "AFX_IDS_COLOR_BTNSHADOW", "65043" );
    add_id_to_map( "AFX_IDS_COLOR_BTNTEXT", "65044" );
    add_id_to_map( "AFX_IDS_COLOR_BTNHIGHLIGHT", "65045" );
    add_id_to_map( "AFX_IDS_COLOR_DISABLEDTEXT", "65046" );
    add_id_to_map( "AFX_IDS_COLOR_HIGHLIGHT", "65047" );
    add_id_to_map( "AFX_IDS_COLOR_HIGHLIGHTTEXT", "65048" );
    add_id_to_map( "AFX_IDS_REGULAR", "65049" );
    add_id_to_map( "AFX_IDS_BOLD", "65050" );
    add_id_to_map( "AFX_IDS_ITALIC", "65051" );
    add_id_to_map( "AFX_IDS_BOLDITALIC", "65052" );
    add_id_to_map( "AFX_IDS_SAMPLETEXT", "65053" );
    add_id_to_map( "AFX_IDS_DISPLAYSTRING_FONT", "65054" );
    add_id_to_map( "AFX_IDS_DISPLAYSTRING_COLOR", "65055" );
    add_id_to_map( "AFX_IDS_DISPLAYSTRING_PICTURE", "65056" );
    add_id_to_map( "AFX_IDS_PICTUREFILTER", "65057" );
    add_id_to_map( "AFX_IDS_PICTYPE_UNKNOWN", "65058" );
    add_id_to_map( "AFX_IDS_PICTYPE_NONE", "65059" );
    add_id_to_map( "AFX_IDS_PICTYPE_BITMAP", "65060" );
    add_id_to_map( "AFX_IDS_PICTYPE_METAFILE", "65061" );
    add_id_to_map( "AFX_IDS_PICTYPE_ICON", "65062" );
    add_id_to_map( "AFX_IDS_COLOR_PPG", "65064" );
    add_id_to_map( "AFX_IDS_COLOR_PPG_CAPTION", "65065" );
    add_id_to_map( "AFX_IDS_FONT_PPG", "65066" );
    add_id_to_map( "AFX_IDS_FONT_PPG_CAPTION", "65067" );
    add_id_to_map( "AFX_IDS_PICTURE_PPG", "65068" );
    add_id_to_map( "AFX_IDS_PICTURE_PPG_CAPTION", "65069" );
    add_id_to_map( "AFX_IDS_PICTUREBROWSETITLE", "65072" );
    add_id_to_map( "AFX_IDS_BORDERSTYLE_0", "65073" );
    add_id_to_map( "AFX_IDS_BORDERSTYLE_1", "65074" );
    add_id_to_map( "AFX_IDS_VERB_EDIT", "65088" );
    add_id_to_map( "AFX_IDS_VERB_PROPERTIES", "65089" );
    add_id_to_map( "AFX_IDP_PICTURECANTOPEN", "65155" );
    add_id_to_map( "AFX_IDP_PICTURECANTLOAD", "65156" );
    add_id_to_map( "AFX_IDP_PICTURETOOLARGE", "65157" );
    add_id_to_map( "AFX_IDP_PICTUREREADFAILED", "65158" );
    add_id_to_map( "AFX_IDP_E_ILLEGALFUNCTIONCALL", "65184" );
    add_id_to_map( "AFX_IDP_E_OVERFLOW", "65185" );
    add_id_to_map( "AFX_IDP_E_OUTOFMEMORY", "65186" );
    add_id_to_map( "AFX_IDP_E_DIVISIONBYZERO", "65187" );
    add_id_to_map( "AFX_IDP_E_OUTOFSTRINGSPACE", "65188" );
    add_id_to_map( "AFX_IDP_E_OUTOFSTACKSPACE", "65189" );
    add_id_to_map( "AFX_IDP_E_BADFILENAMEORNUMBER", "65190" );
    add_id_to_map( "AFX_IDP_E_FILENOTFOUND", "65191" );
    add_id_to_map( "AFX_IDP_E_BADFILEMODE", "65192" );
    add_id_to_map( "AFX_IDP_E_FILEALREADYOPEN", "65193" );
    add_id_to_map( "AFX_IDP_E_DEVICEIOERROR", "65194" );
    add_id_to_map( "AFX_IDP_E_FILEALREADYEXISTS", "65195" );
    add_id_to_map( "AFX_IDP_E_BADRECORDLENGTH", "65196" );
    add_id_to_map( "AFX_IDP_E_DISKFULL", "65197" );
    add_id_to_map( "AFX_IDP_E_BADRECORDNUMBER", "65198" );
    add_id_to_map( "AFX_IDP_E_BADFILENAME", "65199" );
    add_id_to_map( "AFX_IDP_E_TOOMANYFILES", "65200" );
    add_id_to_map( "AFX_IDP_E_DEVICEUNAVAILABLE", "65201" );
    add_id_to_map( "AFX_IDP_E_PERMISSIONDENIED", "65202" );
    add_id_to_map( "AFX_IDP_E_DISKNOTREADY", "65203" );
    add_id_to_map( "AFX_IDP_E_PATHFILEACCESSERROR", "65204" );
    add_id_to_map( "AFX_IDP_E_PATHNOTFOUND", "65205" );
    add_id_to_map( "AFX_IDP_E_INVALIDPATTERNSTRING", "65206" );
    add_id_to_map( "AFX_IDP_E_INVALIDUSEOFNULL", "65207" );
    add_id_to_map( "AFX_IDP_E_INVALIDFILEFORMAT", "65208" );
    add_id_to_map( "AFX_IDP_E_INVALIDPROPERTYVALUE", "65209" );
    add_id_to_map( "AFX_IDP_E_INVALIDPROPERTYARRAYINDEX", "65210" );
    add_id_to_map( "AFX_IDP_E_SETNOTSUPPORTEDATRUNTIME", "65211" );
    add_id_to_map( "AFX_IDP_E_SETNOTSUPPORTED", "65212" );
    add_id_to_map( "AFX_IDP_E_NEEDPROPERTYARRAYINDEX", "65213" );
    add_id_to_map( "AFX_IDP_E_SETNOTPERMITTED", "65214" );
    add_id_to_map( "AFX_IDP_E_GETNOTSUPPORTEDATRUNTIME", "65215" );
    add_id_to_map( "AFX_IDP_E_GETNOTSUPPORTED", "65216" );
    add_id_to_map( "AFX_IDP_E_PROPERTYNOTFOUND", "65217" );
    add_id_to_map( "AFX_IDP_E_INVALIDCLIPBOARDFORMAT", "65218" );
    add_id_to_map( "AFX_IDP_E_INVALIDPICTURE", "65219" );
    add_id_to_map( "AFX_IDP_E_PRINTERERROR", "65220" );
    add_id_to_map( "AFX_IDP_E_CANTSAVEFILETOTEMP", "65221" );
    add_id_to_map( "AFX_IDP_E_SEARCHTEXTNOTFOUND", "65222" );
    add_id_to_map( "AFX_IDP_E_REPLACEMENTSTOOLONG", "65223" );

    return true;
}
