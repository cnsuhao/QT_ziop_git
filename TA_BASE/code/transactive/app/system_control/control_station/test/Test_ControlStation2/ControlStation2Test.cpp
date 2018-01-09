#include "StdAfx.h"
#include "ControlStation2.h"
#include <boost/test/unit_test.hpp>
#include <comutil.h>

BOOST_AUTO_TEST_SUITE(ControlStation2_Test)

static CComPtr<IControlStation2> getControlStation()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CComPtr<IControlStation2> controlStation;
    HRESULT hr = CoCreateInstance(CLSID_ControlStation2, NULL, CLSCTX_LOCAL_SERVER, IID_IControlStation2, (void**)&controlStation);
    BOOST_TEST(SUCCEEDED(hr));
    return controlStation;
}

BOOST_AUTO_TEST_CASE(getOperator)
{
    CComBSTR operatorName;
    getControlStation()->getOperator(&operatorName);
    std::cout << CControlStation2::toString(operatorName) << std::endl;
}

BOOST_AUTO_TEST_CASE(getLocation)
{
    _bstr_t location;
    getControlStation()->getLocation(location.GetAddress());
    std::cout << (const char*)location << std::endl;
}

BOOST_AUTO_TEST_CASE(logout)
{
    getControlStation()->logout();
}

BOOST_AUTO_TEST_CASE(toString)
{
    CComBSTR bstr = "hello, world";
    std::string str = CControlStation2::toString(bstr);
    std::cout << str << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
