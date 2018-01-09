#pragma once
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"

namespace Utilities
{
    const std::string& getOperatorNameFromSession();

    template <typename NameT, typename ValueT>
    TA_Base_Core::RunParam makeRunParam(const NameT& n, const ValueT& v)
    {
        TA_Base_Core::RunParam param;
        param.name = boost::lexical_cast<std::string>(n).c_str();
        param.value = boost::lexical_cast<std::string>(v).c_str();
        return param;
    }

    inline TA_Base_Core::RunParam makeRunParam(const char* n, const char* & v)
    {
        TA_Base_Core::RunParam param;
        param.name = n;
        param.value = v;
        return param;
    }

    inline TA_Base_Core::RunParam makeRunParam(const std::string& n, const std::string& v)
    {
        return makeRunParam(n.c_str(), v.c_str());
    }

    template<typename T>
    inline std::set<unsigned long> getEntityKeys(std::vector<T*>& entities)
    {
        std::set<unsigned long> keys;

        for (T* item : entities)
        {
            keys.insert(item->getKey());
        }

        return keys;
    }

    struct StriCmp
    {
        bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return boost::ilexicographical_compare(lhs, rhs);
        }
    };

    struct iEqualsTo
    {
        const std::string& m_str;
        iEqualsTo(const std::string& str) : m_str(str) {}
        bool operator()(const std::string& str) const
        {
            return boost::ilexicographical_compare(m_str, str);
        }
    };

    bool isStation(const std::string& stationName);
    bool isDepot(const std::string& stationName);
}
