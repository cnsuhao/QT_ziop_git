#include "IMessage.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_App
{
    struct ProcessDataInformation : IMessage
    {
        struct Info
        {
            Info() : isManagedProcess(true), isServiceProcess(false), isAutoFailBack(false) {}
            std::string name;
            std::string description;
            bool isManagedProcess;
            bool isServiceProcess;
            bool isAutoFailBack;
        };

    public:

        Info& getProcessInfo(const std::string& name);

    private:

        void enquireProcessInformation(const std::string& hostname, const TA_Base_Core::ProcessDataSeq& data);
        virtual void onMessageBeforeUseProcessData(const std::string& hostname, const TA_Base_Core::ProcessDataSeq& data);

    private:

        std::map<std::string, Info> m_infoMap;
        std::set<std::string> m_hostnames;
    };
}
