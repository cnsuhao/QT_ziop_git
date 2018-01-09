#include "StdAfx.h"
#include "ComUtilities.h"

std::string ComUtilities::msg(HRESULT hr)
{
    if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
    {
        hr = HRESULT_CODE(hr);
    }

    char* szErrMsg;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (char*)&szErrMsg, 0, NULL) != 0)
    {
        std::string str = szErrMsg;
        LocalFree(szErrMsg);
        return str;
    }
    else
    {
        return str(boost::format("[Could not find a description for error # %#x.]") % hr);
    }
}
