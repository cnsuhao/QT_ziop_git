#pragma once
#include "Promise.h"

namespace TA_Base_Core
{
    class IOperator; class IProfile; class ILocation;
}

struct LoginInfo
{
    enum class Request
    {
        Login,
        ExitControlStation
    };

    LoginInfo(TA_Base_Core::IOperator* o = nullptr,
              TA_Base_Core::IProfile* p = nullptr,
              TA_Base_Core::ILocation* l = nullptr,
              const std::string& pw = "",
              bool dis = false,
              Request req = Request::ExitControlStation)
        : m_operator(o),
          m_profile(p),
          m_location(l),
          m_password(pw),
          m_displayOnly(dis),
          m_request(req)
    {
    }

    TA_Base_Core::IOperator* m_operator;
    TA_Base_Core::IProfile* m_profile;
    TA_Base_Core::ILocation* m_location;
    std::string m_password;
    bool m_displayOnly;
    Request m_request;
};

typedef Promise<LoginInfo> LoginInfoPromise;
typedef boost::shared_ptr<LoginInfoPromise> LoginInfoPromisePtr;
