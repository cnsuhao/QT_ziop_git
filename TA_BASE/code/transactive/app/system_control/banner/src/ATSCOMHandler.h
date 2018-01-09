
#ifndef __ATS_COM_HANDLER_H_INCLUDED__
#define __ATS_COM_HANDLER_H_INCLUDED__

//#include <afxwin.h>
//
//#define _ATL_APARTMENT_THREADED
//#include <atlbase.h>
////You may derive a class from CComModule and use it if you want to override
////something, but do not change the name of _Module
//class CBannerInterfaceModule : public CComModule
//{
//public:
//	LONG Unlock();
//	LONG Lock();
//	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
//	DWORD dwThreadID;
//};
//extern CBannerInterfaceModule _Module;
//#include <atlcom.h>

#include <string>

class ATSCOMHandler
{
public: 
	ATSCOMHandler(const std::string& commandLine)
		: m_commandLine(commandLine) {}
	~ATSCOMHandler() {};

	bool initialize();

private:
	BOOL InitATL();
	BOOL m_bATLInited;
	std::string m_commandLine;
};

#endif //__ATS_COM_HANDLER_H_INCLUDED__