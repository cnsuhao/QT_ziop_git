#ifndef RUNPARAMS_H_INCLUDED
#define RUNPARAMS_H_INCLUDED

/**
  * RunParams.h
  *
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/src/RunParams.h $
  * Author:  B. Fountain
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/11/17 13:31:34 $
  * Last modified by:  $Author: huirong.luo $
  *
  * A singleton class that stores all the runtime parameters. Runtime parameters
  * can come from several different sources - command line, process manager,
  * entity configuration tables.
  *
  * Because the class is a singleton, runtime parameters can be accessed from
  * anywhere in the code instead of passing them around as parameters
  *
  */
#include <string>
#include <vector>
#include <map>

// Standard runtime parameter names

#define RPARAM_GROUPOFFLINECHECKTIMEOUT     "GroupOfflineCheckTimeout"
#define RPARAM_NAMESERV                     "NameServIor"
#define RPARAM_DBCONNECTION                 "DbConnection"
#define RPARAM_DBCONNECTIONFILE             "DbConnectionFile"
#define RPARAM_NOTIFYHOSTS                  "NotifyHosts"
#define RPARAM_NOTIFYPORT                   "NotifyPort"
#define RPARAM_NOTIFYSERVICEPORT            "NotifyServicePort"
#define RPARAM_RESOLVE_TIMEOUT_MS           "ResolveTimeoutMs"
#define RPARAM_FAILOVER_RESOLVE_TIMEOUT_MS  "FailoverResolveTimeoutMs"
#define RPARAM_SUCCESSBATCHINGINTERVAL      "MsgSuccessBatchingInterval"
#define RPARAM_FAILEDBATCHINGINTERVAL       "MsgFailedBatchingInterval"
#define RPARAM_BATCHSIZE                    "MsgBatchSize"

#define RPARAM_DEBUGLEVEL                   "DebugLevel"
#define RPARAM_DEBUGFILE                    "DebugFile"
#define RPARAM_DEBUGFILEMAXSIZE             "DebugFileMaxSize"
#define RPARAM_DEBUGMAXFILES                "DebugMaxFiles"
#define RPARAM_DEBUGCACHEMAXSIZE            "DebugCacheMaxSize"
#define RPARAM_DEBUGPIDFILENAMES            "DebugPidFilenames"
#define RPARAM_DEBUGLEVELOFF                "DebugLevelOff"
#define RPARAM_DEBUGLEVELFILE               "DebugLevelFile"
#define RPARAM_DEBUGLOGSTACKTRACE           "DebugLogStackTrace"

#define RPARAM_MGDPROCID                    "ManagedProcessId"
#define RPARAM_SESSIONID                    "SessionId"
#define RPARAM_PROGNAME                     "ProgName"
#define RPARAM_ENTITYNAME                   "EntityName"
#define RPARAM_APPTYPE                      "AppType"
#define RPARAM_MGRPORT                      "MgrPort"
#define RPARAM_GETFOCUS                     "GetFocus"
#define RPARAM_USERPREFERENCES              "UserPreferences"
#define RPARAM_VERSION                      "Version"
#define RPARAM_CONTROLENTITY                "ControlEntity"

#define RPARAM_CORBATIMEOUT                 "CorbaTimeout"

#define RPARAM_OPERATIONMODE                "OperationMode"
#define RPARAM_CONTROL                      "Control"
#define RPARAM_MONITOR                      "Monitor"
#define RPARAM_NOTAPPLICABLE                "NotApplicable"
#define RPARAM_STANDBY                      "Standby"
#define RPARAM_GROUPSOFFLINE                "GroupsOffline"
#define RPARAM_PROCESSSTATUS                "ProcessStatus"
#define RPARAM_UNSTARTED                    "Unstarted"
#define RPARAM_STARTUP                      "Startup"
#define RPARAM_GOINGTOCONTROL               "GoingToControl"
#define RPARAM_GOINGTOMONITOR               "GoingToMonitor"
#define RPARAM_RUNNINGCONTROL               "RunningControl"
#define RPARAM_RUNNINGMONITOR               "RunningMonitor"
#define RPARAM_TERMINATING                  "Terminating"
#define RPARAM_NOTRUNNING                   "NotRunning"
#define RPARAM_STOPPED                      "Stopped"
#define RPARAM_RUNNINGSTANDBY               "RunningStandby"
#define RPARAM_GOINGTOSTANDBY               "GoingToStandby"

#define RPARAM_HELPFILE                     "HelpFile"

#define RPARAM_EXECPLANID                   "ExecPlanId"
#define RPARAM_VIEWPLANID                   "ViewPlanId"
#define RPARAM_ASSOC_ALARMID                "AssocAlarmId"

#define RPARAM_APPNAME                      "AppName"

#define RPARAM_LOCATIONKEY                  "LocationKey"
#define RPARAM_STATION_DEPLOY_KEY           "StationDeployKey"

#define RPARAM_NAMINGTTL                    "NamingTtl"
#define RPARAM_SCREENNAME                   "ScreenName"

#define RPARAM_SYSTEMCONTROLLERNAME         "SystemControllerName"

// database related runparam stuff
#define RPARAM_DBPREFIX                     "DataBase_"
#define RPARAM_DBONLINE                     "DataBaseGood"
#define RPARAM_DBOFFLINE                    "DataBaseBad"
#define RPARAM_DBUNKNOWN                    "DataBaseUnknown"

// Generic Service
#define RPARAM_SERVICEDEBUG                 "ServiceDebug"

// SCADA
#define RPARAM_SCADA_OBSWORKERS             "ScadaObserverWorkers"

#define RPRAM_KRTCDATEFORMAT                "KrtcDateFormat" // TD12474
#define RPARAM_KRTCALARMSORTING             "KrtcAlarmSorting"
#define RPARAM_MONITOREDDATABASE            "MonitoredDatabase" // Database for monitored agents
#define RPARAM_CENTRALDATABASE              "CentralDatabase" // Central Database

// Generic GUI
#define RPARAM_RIGHTCHECKING                "IgnoreCheckRight"

// The buffer size for the import command of archive manager
#define RPARAM_IMPBUFFERSIZE                "ImpBufferSize"

// The hostname used by naming service
#define RPARAM_NSHOSTNAME                   "NamingHostname" //yanrong++: CL-21230
#define RPARAM_ENABLESUBSCRIPTIONCHANGE     "EnableSubscriptionChange"
#define RPARAM_CORBA_NAMING_PORT            "CorbaNamingPort"
#define RPARAM_DLL_HOST_PORT                "DllHostPort"
#define RPARAM_TRIGGER_ENTITY               "TriggerEntity"

// add by hongzhi for TDS
#define RPARAM_TDSCE                        "TdsCe"
#define RPARAM_CHARSET                      "Charset"

#define RPARAM_DOUBLEAGENTSCONTROLRESTORED  "DoubleAgentsControlRestored"

// add for Configuration Editor
#define RPARAM_CE_PLUGINS                   "Plugins"

#define RPARAM_LANG                         "Lang"

// Qt Style sheet support
#define RPARAM_STYLE                        "Style"

namespace TA_Base_Core
{
    class RunParams;
    class RunParamUser;
    class RunParamsImpl;

    class RunParams
    {
        RunParamsImpl* m_impl;

    public:

        struct ParamNameValue
        {
            std::string name;
            std::string value;
            ParamNameValue() {}
            ParamNameValue(const std::string& n) : name(n) {}
            ParamNameValue(const std::string& n, unsigned long);
            ParamNameValue(const std::string& n, const std::string& v) : name(n), value(v) {}
        };

        typedef std::vector<ParamNameValue> ParamVector;

        /**
          * getInstance
          *
          * Returns a single static instance of RunParams
          *
          * @return The one and only RunParams instance
          */
        static RunParams& getInstance();

        void cleanUp();

        /**
          * ~RunParams
          *
          * Destructor
          */
        virtual ~RunParams() {}

        /**
          * set
          *
          * Add a parameter
          *
          * @param name Name of parameter
          * @param value Value of parameter
          *
          * Pre: name and value are not NULL
          */
        void set(const char* name, const char* value);
        void set(const std::string& name, const std::string& value = "") { set(name.c_str(), value.c_str()); }
        void set(const std::string& name, unsigned long value);
        void forceSet(const std::string& name, const std::string& value);

        /**
          * get
          *
          * Retrieve a parameter value
          *
          * @return Value of parameter
          *
          * @param name Name of parameter
          *
          * Pre: name is not NULL
          */
        std::string get(const char* name);
        std::string get(const std::string& name) { return get(name.c_str()); }
        std::string promiseGet(const std::string& name, int timeoutMs = -1);

        /**
          * isSet
          *
          * Determine whether a parameter with the given name has been set
          *
          * @return True (parameter set), False (parameter not set)
          *
          * @param name Name of parameter
          *
          * Pre: name is not NULL
          */
        bool isSet(const char* name);
        bool isSet(const std::string& name) { return isSet(name.c_str()); }
        bool promiseIsSet(const std::string& name, int timeoutMs = -1);

        /**
          * getAll
          *
          * Retrieve all the parameters
          *
          * @param params Vector of Name/Value parameter pairs
          *
          * Pre: name is not NULL
          */
        void getAll(ParamVector& params);
        ParamVector getAll();

        /**
          * COMMAND LINE MAPPING
          *
          * Command line parameters are expected to conform to the gnu format -
          * parameter names are prefixed with "--" and words within a parameter name
          * are separated with a single hyphen. Names and values are separated with
          * an "=". Values are optional - if there is no "=" then the parameter is a
          * flag.
          *
          * eg "foo --entity-name=BAR --verbose"
          *
          * If the first argument does not start with "--" then it is assumed to be
          * the program name.
          *
          * When we parse the command line we convert the parameter names to
          * CamelHump notation - the leading "--" is discarded, the first letter is
          * upshifted, subsequent hyphens are discarded and the succeeding character
          * upshifted. The conversion only applies to the name, the value is left
          * unmodified. The program name is handled as the value of a parameter with
          * name RPARAM_PROGNAME. In the above example we would set parameter
          * RPARAM_PROGNAME to "foo", parameter "EntityName" to "BAR" and
          * parameter "Verbose" to "".

          * The following methods parse the given command line (several command line
          * representations) and set corresponding parameters. Return false if the
          * command line does not conform to the expected format
          *
          */

        /**
          * parseCmdLine
          *
          * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
          *
          * @return Success of parse (true: success)
          *
          * @param argc Number of parameters
          * @param argv Vector of parameters
          */
        bool parseCmdLine(int argc, char* argv[]);

        /**
          * parseCmdLine
          *
          * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
          *
          * @return Success of parse (true: success)
          *
          * @param cmdline Number of parameters
          * @param argv Vector of parameters
          *
          * Pre: cmdline is not NULL
          */
        bool parseCmdLine(const char* cmdline);

        /**
          * parseCmdLine
          *
          * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
          *
          * @return Success of parse (true: success)
          *
          * @param args Vector of strings consisting of
          *             parameters and parameter values
          *
          * Pre: cmdline is not NULL
          */
        bool parseCmdLine(const std::vector<std::string>& args);

        /**
          * parseCmdLineAndReturnIt
          *
          * Parses the command line parameters. It will return these command line parameter
          * in the map passed instead of setting up the RunParams. This is a helper method that
          * can be used to convert command lines into a map.
          *
          * @return Success of parse (true: success)
          *
          * @param args string - A command line
          * @param map<string,string> - Out parameter. This will be populated with the
          *                             parameters read from cmdLine. This map holds values
          *                             and is keyed by parameter name.
          *
          * Pre: cmdline is not NULL
          */
        bool parseCmdLineAndReturnIt(const std::string& cmdline,
                                     std::map<std::string, std::string>& parameters);
        /**
          * checkUsage
          *
          * This method checks to see whether the parameters specified in the given
          * usage string have been set. It ignores the leading program name and uses
          * the values only to distinguish between parameters that take values and
          * flag parameters. Returns true if all specified parameters have been set
          * correctly. For instance, a usage string "foo --entity-name=BAR --verbose"
          * would return true if parameter "EntityName" has been set and parameter
          * "Verbose" has been set to a blank string. The error message will be set
          * to blank if the method succeeds
          *
          * You can also make parameters optional by surrounding them with square
          * brackets. eg "foo --entity-name=BAR [--verbose]". In this case failure
          * to specify a parameter is not an error, but providing a non-blank value
          * to the flag parameter would be
          *
          * @return True (correct usage string), False (incorrect usage string)
          *
          * @param usage The usage string
          * @param error The reason for the error
          *
          * Pre: The usage cannot be NULL
          */
        bool checkUsage(const char* usage, std::string& error);
        bool checkUsage(const std::string& usage, std::string& error) { return checkUsage(usage.c_str(), error); }

        /**
          * registerRunParamUser
          *
          * Register this given callback object against the parameter with the
          * given name.
          *
          * @param user Pointer to RunParamUser object
          * @param name Name of the parameter
          *
          * Pre: The RunParamUser cannot be NULL
          *      The name cannot be NULL
          */
        void registerRunParamUser(RunParamUser* user, const char* name);
        void registerRunParamUser(RunParamUser* user, const std::string& name) { registerRunParamUser(user, name.c_str()); }

        /**
          * deregisterRunParamUser
          *
          * Cleanup any callback registrations associated with this object
          * Note: this used to be called Deregister().
          *
          * @param user Pointer to RunParamUser object
          *
          * Pre: The RunParamUser cannot be NULL
          */
        void deregisterRunParamUser(RunParamUser* user);

        /**
          * forwardUpdatesTo
          *
          * Forwards RunParam updates to different instances of RunParams. This may
          * sound strange as RunParams is a singleton, however in an environment
          * that uses DLLs each DLL will have its own instance of the RunParams class.
          * In order to allow these different instances to correctly receive updates
          * the 'main' RunParams instance needs the ability to forward them on.
          *
          * @param child The child to forward set requests to.
          */
        void forwardUpdatesTo(RunParams* child);

    protected:

        RunParams();
    };

    // Returns the unsigned integer value of the RunParam with the given name,
    // or defaultVal if none exists.
    // TODO LPP: If we had a set of templatized string conversion functions,
    // we could write this as a template function usable for all types.
    unsigned int getRunParamValue(const std::string& name, unsigned int defaultVal);
    std::string getRunParamValue(const std::string& name, const std::string& defaultVal);

    //Try to load local configuration file with name <EntityName>.cfg, and give
    //them first priority.
    void parseLocalConfigurationFile();
}

#endif
