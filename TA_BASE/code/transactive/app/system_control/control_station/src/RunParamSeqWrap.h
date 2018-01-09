#pragma once
#include "core/utilities/src/RunParams.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"

class RunParamSeqWrap : public TA_Base_Core::RunParamSeq
{
public:

    template <typename T>
    RunParamSeqWrap& push_name_value(const T& nv)
    {
        return assign(push_back(), nv.name, nv.value);
    }

    template <typename T>
    RunParamSeqWrap& push_name_value_list(const T& nvl)
    {
        size_t index = pd_len;
        this->length(pd_len + nvl.size());

        for (const auto& nv : nvl)
        {
            assign(pd_buf[index++], nv.name, nv.value);
        }

        return *this;
    }

    template <typename T>
    RunParamSeqWrap& push_pair(const T& pair)
    {
        return assign(push_back(), pair.first, pair.second);
    }

    template <typename T>
    RunParamSeqWrap& push_pair_list(const T& pairs)
    {
        size_t index = pd_len;
        this->length(pd_len + pairs.size());

        for (const auto& pair : pairs)
        {
            //assign<typename T::value_type::first_type, typename T::value_type::second_type>(pd_buf[index++], pair.first, pair.second);
            assign(pd_buf[index++], pair.first, pair.second);
        }

        return *this;
    }

    template <typename NameT, typename ValueT>
    RunParamSeqWrap& push_back(const NameT& n, const ValueT& v)
    {
        return assign(push_back(), n, v);
    }

    template <typename NameT, typename ValueT>
    RunParamSeqWrap& assign(TA_Base_Core::RunParam& param, const NameT& n, const ValueT& v)
    {
        return assign(param, boost::lexical_cast<std::string>(n), boost::lexical_cast<std::string>(v));
    }

    RunParamSeqWrap& assign(TA_Base_Core::RunParam& param, const char* n, const char* v)
    {
        param.name = n;
        param.value = v;
        return *this;
    }

    RunParamSeqWrap& assign(TA_Base_Core::RunParam& param, const std::string& n, const std::string& v)
    {
        return assign(param, n.c_str(), v.c_str());
    }

    template <typename NameT, typename ValueT>
    RunParamSeqWrap& operator()(const NameT& n, const ValueT& v)
    {
        return push_back(n, v);
    }

    TA_Base_Core::RunParam& back()
    {
        return pd_buf[pd_len - 1];
    }

    TA_Base_Core::RunParam& front()
    {
        return pd_buf[0];
    }

    TA_Base_Core::RunParam& push_back()
    {
        this->length(pd_len + 1);
        return pd_buf[pd_len - 1];
    }

    TA_Base_Core::RunParamSeq* _return()
    {
        struct Hack
        {
            _CORBA_ULong    pd_max;
            _CORBA_ULong    pd_len;
            _CORBA_Boolean  pd_rel;
            _CORBA_Boolean  pd_bounded;
            TA_Base_Core::RunParam* pd_buf;
        };

        Hack* hack = new Hack { pd_max, pd_len, pd_rel, pd_bounded, pd_buf };
        pd_rel = 1;
        pd_max = pd_len = pd_bounded = pd_buf = 0;
        return reinterpret_cast<TA_Base_Core::RunParamSeq*>(hack);
    }

    std::string str(size_t i) const
    {
        return boost::str(boost::format("%s=%s") % pd_buf[i - 1].name.in() % pd_buf[i - 1].value.in());
    }

    std::string last_str() const
    {
        return boost::str(boost::format("%s=%s") % pd_buf[pd_len - 1].name.in() % pd_buf[pd_len - 1].value.in());
    }

    friend std::ostream& operator << (std::ostream& os, const RunParamSeqWrap& rhs)
    {
        for (size_t i = 0; i < rhs.length(); ++i)
        {
            os << (0 == i ? "" : ", ") << rhs[i].name.in() << "=" << rhs[i].value.in();
        }

        return os;
    }
};
