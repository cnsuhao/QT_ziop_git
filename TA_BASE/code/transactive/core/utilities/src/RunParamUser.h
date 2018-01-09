#ifndef RUNPARAMUSER_H_INCLUDED
#define RUNPARAMUSER_H_INCLUDED
#include <string>

namespace TA_Base_Core
{
    class RunParamUser
    {
    public:

        virtual ~RunParamUser();
        virtual void onRunParamChange(const std::string& name, const std::string& value) = 0;
        virtual void onNewParam(const std::string& name, const std::string& value) {}
    };
}

#endif
