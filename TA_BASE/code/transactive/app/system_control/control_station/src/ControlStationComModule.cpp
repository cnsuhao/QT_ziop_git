#include "StdAfx.h"
#include <afxwin.h>
#include "ControlStationComModule.h"
#include "ControlStation2.h"
#include "SchematicDisplay.h"
#include "ControlStationConstants.h"
#include "ComUtilities.h"
#include "core/utilities/src/TAAssert.h"
#include "app/system_control/control_station/src/ControlStation_i.c"
#include "app/system_control/control_station/src/RippleControlStation_i.c"

ControlStationComModule _Module;
BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_ControlStation2, CControlStation2)
OBJECT_ENTRY(CLSID_SchematicDisplay, CSchematicDisplay)
END_OBJECT_MAP()

ControlStationComModule::~ControlStationComModule()
{
    _Module.RevokeClassObjects();
    _Module.Term();
    CoUninitialize();
}

LONG ControlStationComModule::Unlock()
{
    AfxOleUnlockApp();
    return 0;
}

LONG ControlStationComModule::Lock()
{
    AfxOleLockApp();
    return 1;
}

bool ControlStationComModule::initATL()
{
    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _Module.Init(ObjectMap, NULL);
    _Module.dwThreadID = GetCurrentThreadId();

    std::string cmd = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
    bool bRun = true;

    // uninstall it
    if (cmd.find(TA_Base_App::TA_ControlStation::UNREGISTER_CMD) != std::string::npos)
    {
        _Module.UpdateRegistryFromResource(IDR_CONTROLSTATION2, FALSE);
        _Module.UpdateRegistryFromResource(IDR_SCHEMATICDISPLAY, FALSE);
        _Module.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
        bRun = false;
    }
    else if (cmd.find(TA_Base_App::TA_ControlStation::REGISTER_CMD) != std::string::npos)
    {
        _Module.UpdateRegistryFromResource(IDR_SCHEMATICDISPLAY, TRUE);
        _Module.UpdateRegistryFromResource(IDR_CONTROLSTATION2, TRUE);
        HRESULT result = _Module.RegisterServer(TRUE);
        std::string msg = ComUtilities::msg(result);
        TA_Assert(S_OK == result);
        bRun = false;
    }

    if (!bRun)
    {
        _Module.Term();
        CoUninitialize();
        return false;
    }

    hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_SURROGATE);

    if (FAILED(hRes))
    {
        CoUninitialize();
        return false;
    }

    return true;
}
