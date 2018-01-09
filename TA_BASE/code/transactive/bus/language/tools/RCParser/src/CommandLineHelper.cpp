#include "StdAfx.h"
#include "CommandLineHelper.h"


CommandLineHelper::CommandLineHelper( int argc, char* argv[] )
{
    build_application_id_map();
    parse_command_line( argc, argv );
}


void CommandLineHelper::parse_command_line(int argc, char* argv[])
{
    // rcparser
    if ( 1 == argc )
    {
        std::string rc = search_file_by_extension_in_current_directory();

        if ( rc.empty() )
        {
            display_usage();
            ::exit(0);
        }

        std::string application_id = get_application_id_by_rc( rc );
        std::string rc_h = get_rc_h_by_rc( rc );
        std::string language = get_language_by_rc( rc );
        m_parameter_list.push_back( ParameterPtr( new Parameter( rc, rc_h, language, application_id ) ) );
    }
    // rcparser .rc
    else if ( 2 == argc )
    {
        std::string rc = argv[1];
        std::string application_id = get_application_id_by_rc( rc );
        std::string rc_h = get_rc_h_by_rc( rc );
        std::string language = get_language_by_rc( rc );
        m_parameter_list.push_back( ParameterPtr( new Parameter( rc, rc_h, language, application_id ) ) );
    }
    // rcparser id .rc
    if ( 3 == argc )
    {
        std::string application_id = argv[1];
        std::string rc = argv[2];
        std::string rc_h = get_rc_h_by_rc( rc );
        std::string language = get_language_by_rc( rc );
        m_parameter_list.push_back( ParameterPtr( new Parameter( rc, rc_h, language, application_id ) ) );
    }
    // rcparser 1.rc 2.rc 3.rc...
    else
    {
        for ( int i = 1; i < argc; ++i )
        {
            std::string rc = argv[i];
            std::string application_id = get_application_id_by_rc( rc );
            std::string rc_h = get_rc_h_by_rc( rc );
            std::string language = get_language_by_rc( rc );
            m_parameter_list.push_back( ParameterPtr( new Parameter( rc, rc_h, language, application_id ) ) );
        }
    }
}


void CommandLineHelper::display_usage()
{
    std::cout << "usage:\n";
    std::cout << "rcparser [id] <1.rc> [2.rc 3.rc...]";
    std::cout << std::endl;
}


std::string CommandLineHelper::search_file_by_extension_in_current_directory( const std::string& extension )
{
    boost::filesystem::path cur_path = boost::filesystem::current_path();

    for ( boost::filesystem::directory_iterator it( cur_path ), end; it != end; ++it )
    {
        const boost::filesystem::path& ext = it->path().extension();

        if ( extension == ext.string() )
        {
            return it->path().string();
        }
    }

    return "";
}


std::string CommandLineHelper::get_application_id_by_rc( const std::string& rc )
{
    boost::filesystem::path rc_path( rc );
    std::string rc_file_name = rc_path.stem().string();
    std::string& application_id = m_application_id_map[rc_file_name];

    if ( false == application_id.empty() )
    {
        return application_id;
    }

    return "0";
}


std::string CommandLineHelper::get_rc_h_by_rc( const std::string& rc )
{
    boost::filesystem::path rc_path( rc );
    return( rc_path.branch_path() / "Resource.h").string();
}


std::string CommandLineHelper::get_language_by_rc( const std::string& rc )
{
    boost::filesystem::path rc_path( rc );
    return rc_path.stem().string() + ".txt";
}


void CommandLineHelper::build_application_id_map()
{
    // rc -- id
    m_application_id_map[ "AlarmsGui" ]                     = "100";
    m_application_id_map[ "AlarmGenerator" ]                = "200";
    m_application_id_map[ "Iscs_Alarm_Display" ]            = "300";
    m_application_id_map[ "ISCSAlarmActionBridge" ]         = "400";
    m_application_id_map[ "ATSAlarmBridgeUtility" ]         = "500";
    m_application_id_map[ "AtsAlarmGenerator" ]             = "600";
    m_application_id_map[ "AtsIscsAlarmViewer" ]            = "700";
    m_application_id_map[ "EventExtractor" ]                = "800";
    m_application_id_map[ "EventViewer" ]                   = "900";
    m_application_id_map[ "crutil" ]                        = "1000";
    m_application_id_map[ "HistoryViewer" ]                 = "1100"; // TODO: new_trend_viewer
    m_application_id_map[ "HistoryViewer" ]                 = "1200"; // TODO: trend_viewer\src\HistoryViewer.rc
    m_application_id_map[ "HistoryViewer" ]                 = "1300"; // TODO:
    m_application_id_map[ "PlanManager" ]                   = "1400";
    m_application_id_map[ "SchedulingManager" ]             = "1500";
    m_application_id_map[ "ECSManager" ]                    = "1600";
    m_application_id_map[ "IncidentManager" ]               = "1700";
    m_application_id_map[ "JobRequestMgr" ]                 = "1800";
    m_application_id_map[ "MmsController" ]                 = "1900";
    m_application_id_map[ "DataNodeController" ]            = "2000";
    m_application_id_map[ "EquipmentStatusViewer" ]         = "2100";
    m_application_id_map[ "InspectorPanel" ]                = "2200";
    m_application_id_map[ "PowerDemandEditor" ]             = "2300";
    m_application_id_map[ "DutyManager" ]                   = "2400";
    m_application_id_map[ "RegionManager" ]                 = "2500";
    m_application_id_map[ "SystemManager" ]                 = "2600";
    m_application_id_map[ "ControlStation" ]                = "2700";
    m_application_id_map[ "SoundManager" ]                  = "2800";
    m_application_id_map[ "banner" ]                        = "2900";
    m_application_id_map[ "IscsBannerPage" ]                = "3000";
    m_application_id_map[ "PlanBannerPage" ]                = "3100";
    m_application_id_map[ "AtsBannerPage" ]                 = "3200";
    m_application_id_map[ "CallBannerPage" ]                = "3300";
    m_application_id_map[ "ConfigurationEditor" ]           = "3400";
    m_application_id_map[ "VideoDisplayManager" ]           = "3600";
    m_application_id_map[ "PAManager" ]                     = "3700";
    m_application_id_map[ "Authorise" ]                     = "3800";
    m_application_id_map[ "RadioManager" ]                  = "3900";
    m_application_id_map[ "RadioManager" ]                  = "4000"; // TODO: radio_manager_gui_icdv4\src\RadioManager.rc
    m_application_id_map[ "RadioMonitor" ]                  = "4100";
    m_application_id_map[ "PECAcceptance" ]                 = "4200";
    m_application_id_map[ "STISManager" ]                   = "4300";
    m_application_id_map[ "TisLogViewer" ]                  = "4400";
    m_application_id_map[ "TTISManager" ]                   = "4500";
    m_application_id_map[ "telephone_manager" ]             = "4600";
    m_application_id_map[ "TrainBorne_DisplayManager" ]     = "4700";
    m_application_id_map[ "TrainBorneManager" ]             = "4800";
    m_application_id_map[ "TrainControlManager" ]           = "4900";
}
