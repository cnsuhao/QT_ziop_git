#include "ProcessDataInformation.h"
#include "IMessage.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IProcess.h"
#include "core/utilities/src/RunParams.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>


namespace TA_Base_App
{
    ProcessDataInformation::Info& ProcessDataInformation::getProcessInfo(const std::string& name)
    {
        std::map<std::string, Info>::iterator it = m_infoMap.find(name);

        if (it != m_infoMap.end())
        {
            return it->second;
        }

        static Info info;
        info.description = info.name = name;
        return info;
    }


    void ProcessDataInformation::onMessageBeforeUseProcessData(const std::string& hostname, const TA_Base_Core::ProcessDataSeq& data)
    {
        if (TA_Base_Core::RunParams::getInstance().isAppTestAlone())
        {
            return;
        }

        enquireProcessInformation(hostname, data);
    }


    void ProcessDataInformation::enquireProcessInformation(const std::string& hostname, const TA_Base_Core::ProcessDataSeq& data)
    {
        if (m_hostnames.find(hostname) != m_hostnames.end())
        {
            return;
        }

        m_hostnames.insert(hostname);
        size_t cnt = 0;
        std::stringstream token;

        for (size_t i = 0; i < data.length(); i++)
        {
            token << (cnt++ ? "," : "") << "'" << boost::to_upper_copy(std::string(data[i].ProcessId.in())) << "'";
        }

        try
        {
            TA_Base_Core::IEntityDataList entities = TA_Base_Core::EntityAccessFactory::getInstance().getEntityWithNameInToken(token.str());

            BOOST_FOREACH(TA_Base_Core::IEntityData * entity, entities)
            {
                TA_Base_Core::IProcess& process = *dynamic_cast<TA_Base_Core::IProcess*>(entity);
                std::string name = process.getName();
                Info& info = m_infoMap[name];
                info.name = name;
                info.description = process.getDescription();
                info.isManagedProcess = process.isManagedProcess();
                info.isServiceProcess = process.isServiceProcess();
                info.isAutoFailBack = process.shouldAutoFailback();
                delete entity;
            }
        }
        catch (...)
        {
        }
    }
}
