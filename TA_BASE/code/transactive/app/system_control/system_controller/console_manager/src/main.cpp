/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_controller/console_manager/src/ConsoleManager.cpp $
  * @author:  Ripple
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/02/10 17:45:28 $
  * Last modified by:  $Author: haijun.li $
  *
  */
// ConsoleManager.cpp
//
// This program is designed to run as a console application, not as a GUI.
// User interface added and ability to have a different log file for each application
//
// Author:      Bruce Fountain 08-Feb-2001
// Modified by: $Author: haijun.li $
// Modified on: $DateTime: 2012/02/10 17:45:28 $
// Version:     $Revision: #3 $
//
#include "StdAfx.h"
#include "SystemControllerAdmin.h"
#include "ProcessManager.h"
#include "ControlStationCorbaImpl.h"
#include "SystemControllerMonitor.h"
#include "DummyAuthenticationAgent.h"
#include "DummyRightsAgent.h"
#include "SignalHandler.h"
#include "Utilities.h"
#include "InteractiveLoop.h"
#include "DatabaseStatusMonitor.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/threads/src/Thread.h"
#include "core/utilities/src/DebugSETranslator.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtilInit.h"
#include "core/process_management/src/UtilityInitialiser.h"
#include "core/exceptions/src/InvalidCommandLineException.h"

using namespace TA_Base_Core;
using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::CorbaUtil;

// Forward declaration
bool loadParamFromConfigFile();
bool init(int argc, char* argv[]);
bool initRunParams(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    ACE::init();

    try
    {
        init(argc, argv);
    }
    catch (...)
    {
        std::cout << "could not load initialize parameter correctly, please check it" << std::endl;
        return -1;
    }

    // If the sessionId parameter is not set, try retrieving a valid session id
    // with the given parameters

    if (!RunParams::getInstance().isSet(RPARAM_SESSIONID))
    {
        if (RunParams::getInstance().isSet("RequestSessionId"))
        {
            Utilities::retrieveSessionId();
        }
        else
        {
            RunParams::getInstance().set(RPARAM_SESSIONID, "TransActive Super Session ID");
            std::cout << boost::format("RunParam '%s' not set, set to default: %s") % RPARAM_SESSIONID % "TransActive Super Session ID" << std::endl;
        }
    }

    ProcessManager::getInstance();
    ControlStationCorbaImpl::getInstance();
    SystemControllerAdmin::getInstance();
    SystemControllerMonitor::getInstance();
    DummyAuthenticationAgent::getInstance();
    DummyRightsAgent::getInstance();
    SignalHandler::getInstance();
    DatabaseStatusMonitor::getInstance();

    InteractiveLoop::getInstance().run();

    ProcessManager::getInstance().terminateAndWaitForAll();
    ACE::fini();
    return 1;
}

bool init(int argc, char* argv[])
{
    if (false == initRunParams(argc, argv))
    {
        return false;
    }

    int port = TA_Base_Core::getRunParamValue(RPARAM_MGRPORT, TA_Base_Core::DEFAULT_MGR_PORT);
    Utilities::checkConsoleManagerInstance(port);
    TA_Base_Core::gSetDebugUtilFromRunParams();
    TA_Base_Core::gInitStructuredExceptionHandler();
    CorbaUtil::getInstance().initialise(port);
    CorbaUtil::getInstance().activate();
    return true;
}

bool initRunParams(int argc, char* argv[])
{
    if (false == RunParams::getInstance().parseCmdLine(argc, argv))
    {
        if (RunParams::getInstance().isSet(RPARAM_VERSION))
        {
            return false;
        }

        TA_THROW(TA_Base_Core::InvalidCommandLineException("Command line arguments are in invalid format"));
    }

    if (!loadParamFromConfigFile())
    {
        return false;
    }

    // For Naming
    RunParams::getInstance().set("NamingUseMessaging");
    RunParams::getInstance().set(RPARAM_OPERATIONMODE, RPARAM_CONTROL);
    RunParams::getInstance().set(RPARAM_PROCESSSTATUS, RPARAM_RUNNINGCONTROL);

    if (!RunParams::getInstance().isSet(RPARAM_LOCATIONKEY))
    {
        RunParams::getInstance().set(RPARAM_LOCATIONKEY, "1");
        std::cout << boost::format("RunParam '%s' not set, set to default: %d") % RPARAM_LOCATIONKEY % 1 << std::endl;
    }

    if (!RunParams::getInstance().isSet(RPARAM_NOTIFYHOSTS))
    {
        RunParams::getInstance().set(RPARAM_NOTIFYHOSTS, "localhost:6667");
        std::cout << boost::format("RunParam '%s' not set, set to default: %s") % RPARAM_NOTIFYHOSTS % "localhost:6667" << std::endl;
    }

    if (!RunParams::getInstance().isSet(RPARAM_ENTITYNAME))
    {
        RunParams::getInstance().set(RPARAM_ENTITYNAME, Utilities::getLocationName() + "ConsoleManager");
    }

    if (!RunParams::getInstance().isSet(RPARAM_DEBUGFILE))
    {
        boost::filesystem::path debugLogDir = Utilities::getDebugLogDir();
        boost::filesystem::path debugFile = debugLogDir / (RunParams::getInstance().get(RPARAM_ENTITYNAME) + ".log");
        RunParams::getInstance().set(RPARAM_DEBUGFILE, debugFile.string());
    }

    return true;
}

bool loadParamFromConfigFile()
{
    std::string configFile;

    if (boost::filesystem::exists("ConsoleManager.ini"))
    {
        configFile = "ConsoleManager.ini";
    }
    else if (boost::filesystem::exists("ConsoleManager.cfg"))
    {
        configFile = "ConsoleManager.cfg";
    }
    else if (RunParams::getInstance().isSet("ConfigFile"))
    {
        configFile = RunParams::getInstance().get("ConfigFile");
    }

    if (configFile.empty())
    {
        return true;
    }

    std::ifstream ifs(configFile.c_str());

    if (!ifs)
    {
        std::cout << "Unable to open ConfigFile: " << configFile << std::endl;
        return false;
    }

    for (std::string line; std::getline(ifs, line);)
    {
        boost::trim(line);

        if (line.empty() || boost::starts_with(line, "#") || boost::starts_with(line, "//"))
        {
            continue;
        }

        std::vector<std::string> args;
        boost::split(args, line, boost::is_any_of("="), boost::token_compress_on);
        std::string name = Utilities::makeRunParameterName(boost::trim_copy(args[0]));
        std::string value;

        if (1 < args.size())
        {
            value = boost::trim_copy(args[1]);
        }

        if (name.size())
        {
            RunParams::getInstance().set(name, value);
        }
    }

    return true;
}
