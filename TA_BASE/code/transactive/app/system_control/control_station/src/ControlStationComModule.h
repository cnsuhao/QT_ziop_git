#pragma once
#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class ControlStationComModule : public CComModule
{
public:
    ~ControlStationComModule();
    LONG Unlock();
    LONG Lock();
    LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
    bool initATL();
    DWORD dwThreadID;
};

extern ControlStationComModule _Module;
#include <atlcom.h>
