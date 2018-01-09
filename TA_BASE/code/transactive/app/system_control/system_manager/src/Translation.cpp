#include "Translation.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>


namespace TA_Base_App
{
    void Translation::empty()
    {
        QT_TR_NOOP("System Manager");
        QT_TR_NOOP("Standby");

        QT_TR_NOOP("N/A");

        QT_TR_NOOP("Control");
        QT_TR_NOOP("Monitor");

        QT_TR_NOOP("Unstarted");
        QT_TR_NOOP("Startup");
        QT_TR_NOOP("GoingToControl");
        QT_TR_NOOP("GoingToMonitor");
        QT_TR_NOOP("RunningControl");
        QT_TR_NOOP("RunningMonitor");
        QT_TR_NOOP("Terminating");
        QT_TR_NOOP("NotRunning");
        QT_TR_NOOP("Stopped");

        QT_TR_NOOP("FATAL");
        QT_TR_NOOP("ERROR");
        QT_TR_NOOP("WARNING");
        QT_TR_NOOP("NORMAL");
        QT_TR_NOOP("INFO");
        QT_TR_NOOP("DEBUG");
        QT_TR_NOOP("SQL");
        QT_TR_NOOP("CORBA");
        QT_TR_NOOP("MSG");
        QT_TR_NOOP("MSGPUB");
        QT_TR_NOOP("PERIODIC");
        QT_TR_NOOP("DATABASE");
        QT_TR_NOOP("EXTERNAL");
        QT_TR_NOOP("TRACE");

        QT_TR_NOOP("User logined to %1%");
        QT_TR_NOOP("System Manager - %1%");
        QT_TR_NOOP("Standby");
        QT_TR_NOOP("System Manager");
        QT_TR_NOOP("Cannot connect to system controller");
        QT_TR_NOOP("Shutdown system controller...");
        QT_TR_NOOP("System controller is shutdown");

        QT_TR_NOOP("Name");
        QT_TR_NOOP("Location");
        QT_TR_NOOP("Mode");
        QT_TR_NOOP("Status");
        QT_TR_NOOP("Request");
        QT_TR_NOOP("Start");
        QT_TR_NOOP("Restarts");
        QT_TR_NOOP("Log");
    }


    QString Translation::tr(const std::string& s)
    {
        return tr(s.c_str());
    }


    QStringList Translation::tr(const std::vector<std::string>& strs)
    {
        QStringList qs;
        BOOST_FOREACH(const std::string& s, strs)
        {
            qs << tr(s.c_str());
        }
        return qs;
    }


    QString Translation::tr(const FormattedString& s)
    {
        QString format = tr(s.format);
        const std::vector<std::string>& parms = s.params;

        for (size_t i = 0; i < s.params.size(); ++i)
        {
            std::string placeholder = "%" + boost::lexical_cast<std::string>(i + 1) + "%";
            format.replace(placeholder.c_str(), tr(s.params[i].c_str()));
        }

        return format;
    }
}
