
#include "app/system_control/banner/src/stdafx.h"
#include "app/system_control/banner/src/ATSCOMHandler.h"
#include "app/system_control/banner/src/BannerInterface.h"
#include "app/system_control/banner/src/IscsBanner.h"
#include "app/system_control/banner/src/BannerInterface_i.c"
#include "bus\banner_framework\src\constants.h"

#include <atlconv.h>
#include <initguid.h>

bool ATSCOMHandler::initialize()
{
	if (!InitATL())
	{
		//if ( !AfxOleInit() )	// ly++, vs2005 init OLE object
		return false;
	}

	return true;
}


CBannerInterfaceModule _Module;


BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_IscsBanner, CIscsBanner)
END_OBJECT_MAP()


LONG CBannerInterfaceModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}


LONG CBannerInterfaceModule::Lock()
{
	AfxOleLockApp();
	return 1;
}


BOOL ATSCOMHandler::InitATL()
{
	USES_CONVERSION;
	m_bATLInited = TRUE;

#if _WIN32_WINNT >= 0x0400
	// changed from multithread inorder to avoid application stop responding based on article
	// http://support.microsoft.com/default.aspx?scid=kb;en-us;828643

	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}

	_Module.Init(ObjectMap, GetModuleHandle(NULL)); // AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	//std::string lpCmdLine = W2A(GetCommandLine()); //this line necessary for _ATL_MIN_CRT

	BOOL bRun = TRUE;
	if (m_commandLine.find(BANNER_CONST::UNREGISTER_CMD) != std::string::npos)
	{
		_Module.UpdateRegistryFromResource(IDR_BANNERLIB, FALSE);
		_Module.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
		bRun = FALSE;
	}
	if (m_commandLine.find(BANNER_CONST::REGISTER_CMD) != std::string::npos)
	{
		_Module.UpdateRegistryFromResource(IDR_BANNERLIB, TRUE);
		_Module.RegisterServer(TRUE);
		bRun = FALSE;
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		CoUninitialize();
		return FALSE;
	}

	hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		CoUninitialize();
		return FALSE;
	}

	return TRUE;
}
