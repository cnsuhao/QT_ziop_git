/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/src/RunParams.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * A singleton class that stores all the runtime parameters. Runtime parameters
  * can come from several different sources - command line, process manager,
  * entity configuration tables.
  *
  * Because the class is a singleton, runtime parameters can be accessed from
  * anywhere in the code instead of passing them around as parameters
  *
  */

#if defined( WIN32 )
    #pragma warning( disable : 4786 4244 4290 )
#endif

#include "RunParams.h"
#include "RunParamsImpl.h"
#include "DebugUtil.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace TA_Base_Core
{
    RunParams::ParamNameValue::ParamNameValue(const std::string& n, unsigned long v)
        : name(n)
    {
        try
        {
            value = boost::lexical_cast<std::string>(v);
        }
        catch (...)
        {
        }
    }

    RunParams& RunParams::getInstance()
    {
        static RunParams* instance = new RunParams;
        return *instance;
    }

    RunParams::RunParams()
    {
        m_impl = new RunParamsImpl;
    }

    void RunParams::cleanUp()
    {
        m_impl->cleanUp();
    }

    std::string RunParams::get(const char* name)
    {
        return m_impl->get(name);
    }

    std::string RunParams::promiseGet(const std::string& name, int timeoutMs)
    {
        return m_impl->promiseGet(name, timeoutMs);
    }

    void RunParams::set(const char* name, const char* value)
    {
        m_impl->set(name, value);
    }

    void RunParams::set(const std::string& name, unsigned long value)
    {
        m_impl->set(name, value);
    }

    void RunParams::forceSet(const std::string& name, const std::string& value)
    {
        m_impl->forceSet(name, value);
    }

    bool RunParams::isSet(const char* name)
    {
        return m_impl->isSet(name);
    }

    bool RunParams::promiseIsSet(const std::string& name, int timeoutMs)
    {
        return m_impl->promiseIsSet(name, timeoutMs);
    }

    void RunParams::getAll(ParamVector& params)
    {
        m_impl->getAll(params);
    }

    RunParams::ParamVector RunParams::getAll()
    {
        return m_impl->getAll();
    }

    void RunParams::registerRunParamUser(RunParamUser* user, const char* name)
    {
        m_impl->registerRunParamUser(user, name);
    }

    void RunParams::deregisterRunParamUser(RunParamUser* user)
    {
        m_impl->deregisterRunParamUser(user);
    }

    void RunParams::forwardUpdatesTo(RunParams* child)
    {
        m_impl->forwardUpdatesTo(child);
    }

    bool RunParams::parseCmdLine(int argc, char* argv[])
    {
        return m_impl->parseCmdLine(argc, argv);
    }

    bool RunParams::parseCmdLine(const char* cmdline)
    {
        return m_impl->parseCmdLine(cmdline);
    }

    bool RunParams::parseCmdLine(const std::vector<std::string>& args)
    {
        return m_impl->parseCmdLine(args);
    }

    bool RunParams::parseCmdLineAndReturnIt(const std::string& cmdline, std::map<std::string, std::string>& parameters)
    {
        return m_impl->parseCmdLineAndReturnIt(cmdline, parameters);
    }

    bool RunParams::checkUsage(const char* usage, std::string& error)
    {
        return m_impl->checkUsage(usage, error);
    }

    unsigned int getRunParamValue(const std::string& name, unsigned int defaultVal)
    {
        unsigned int value = defaultVal;

        if (RunParams::getInstance().isSet(name.c_str()))
        {
            // TODO LPP: strtoul() would be more robust, as it can indicate that the conversion failed
            // (whereas atoi simply returns 0).
            value = atoi(RunParams::getInstance().get(name.c_str()).c_str());
        }

        return value;
    }

    std::string getRunParamValue(const std::string& name, const std::string& defaultVal)
    {
        if (RunParams::getInstance().isSet(name.c_str()))
        {
            std::string value = RunParams::getInstance().get(name.c_str());

            if (value.size())
            {
                return value;
            }
        }

        return defaultVal;
    }

    void parseLocalConfigurationFile()
    {
        boost::filesystem::path configFile = boost::filesystem::system_complete(RunParams::getInstance().get(RPARAM_ENTITYNAME) + ".cfg");

        if (!boost::filesystem::exists(configFile))
        {
            configFile = boost::filesystem::system_complete(RunParams::getInstance().get(RPARAM_ENTITYNAME) + ".ini");
        }

        if (boost::filesystem::exists(configFile) && boost::filesystem::is_regular_file(configFile))
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "parseLocalConfigurationFile: %s", configFile.string().c_str());

            try
            {
                std::stringstream paramStrm;
                paramStrm << "--RunParamFile=" << configFile.string();
                RunParams::getInstance().parseCmdLine(paramStrm.str().c_str());
            }
            catch (std::exception& e)
            {
                LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "parseLocalConfigurationFile - error: %s", e.what());
            }
            catch (...)
            {
                LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "parseLocalConfigurationFile - unknown exception");
            }
        }
    }
}// Closes namespace TA_Base_Core
