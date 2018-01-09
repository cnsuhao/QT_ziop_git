/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ControlStation2.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class holds the COM server code for the Control Station. It will process all calls
  * from COM clients and has the ability to fire events to them.
  *
  */

#include "StdAfx.h"
#include "ControlStation2.h"
#include "ControlStation_i.h"
#include "ComSignal.h"
#include "ControlStationRootDialog.h"
#include "core/utilities/src/DebugUtil.h"
#include <comutil.h>
#pragma warning(disable: 4927)

using namespace TA_Base_Core;
typedef IProcessManager::ApplicationProperties ApplicationProperties;
boost::regex CControlStation2::APP_PATTERN("--entity-name[[:space:]]*=[[:space:]]*.+\\\\([a-z,A-Z]+)\\.exe");

CControlStation2::CControlStation2()
{
    FUNCTION_ENTRY("CControlStation2");
    // Set ourselves in the root dialog so it can perform callbacks.
    ControlStationRootDialog::getInstance().setControlStationComObject(this);
    FUNCTION_EXIT;
}

CControlStation2::~CControlStation2()
{
    ControlStationRootDialog::getInstance().setControlStationComObject(NULL);
}

STDMETHODIMP CControlStation2::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] =
    {
        &IID_IControlStation2
    };

    for (const IID* id : arr)
    {
        if (InlineIsEqualGUID(*id, riid))
        {
            return S_OK;
        }
    }

    return S_FALSE;
}

STDMETHODIMP CControlStation2::logout()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("logout");

    ComSignal::logout();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::giveApplicationFocus(long processId)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("giveApplicationFocus");

    ComSignal::giveApplicationFocus[processId]();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getOperator(BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getOperator");

    DECLEAR_SHARED(Promise<std::string>, operatorName);
    ComSignal::getOperator(operatorName);
    copyToBSTR(pRet, operatorName);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getSite(BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getSite");

    DECLEAR_SHARED(Promise<std::string>, name);
    ComSignal::getSite(name);
    copyToBSTR(pRet, name);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getProfile(BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getProfile");

    DECLEAR_SHARED(Promise<std::string>, profileName);
    ComSignal::getProfile(profileName);
    copyToBSTR(pRet, profileName);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchApplicationEx(long appType, BSTR* extraCommandLine, long posType, long alignType, RECT* objectDim, RECT* boundaryDim)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchApplicationEx");

    ComSignal::launchApplicationEx(appType, toString(extraCommandLine), posType, alignType, *objectDim, *boundaryDim);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getAllApplications(VARIANT* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getAllApplications");

    VariantInit(pRet);

    DECLEAR_SHARED(Promise<std::vector<ApplicationProperties> >, allAppPromise);
    ComSignal::getAllApplications(allAppPromise);
    std::vector<ApplicationProperties> allApps = allAppPromise->get();
    createArray(allApps, *pRet);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getRunningApplications(VARIANT* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getRunningApplications");

    VariantInit(pRet);

    boost::shared_ptr<Promise<std::map<unsigned long, std::string>>> allAppsPromise(new Promise<std::map<unsigned long, std::string>>);
    ComSignal::getRunningApplications(allAppsPromise);
    std::map<unsigned long, std::string> allApps = allAppsPromise->get();

    createArray(allApps, *pRet);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getLocation(BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getLocation");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getLocation(str);
    copyToBSTR(pRet, str);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::setSelectedAlarms(BSTR FAR* asset, long leftPosition)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("setSelectedAlarms");

    ComSignal::setSelectedAlarms(toString(asset), leftPosition);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getProfileDisplay(long leftPosition, BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getProfileDisplay");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getProfileDisplay(leftPosition, str);
    copyToBSTR(pRet, str);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getProfileDisplayAtLocation(long leftPosition, BSTR locationName, BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getProfileDisplayAtLocation");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getProfileDisplayAtLocation(leftPosition, toString(locationName), str);
    copyToBSTR(pRet, str);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::changeProfile()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("changeProfile");

    ComSignal::changeProfile();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::changeDisplayMode()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("changeDisplayMode");

    ComSignal::changeDisplayMode();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::changePassword()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("changePassword");

    ComSignal::changePassword();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::isServerContactable(BOOL* isContactable)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("isServerContactable");

    DECLEAR_SHARED(Promise<bool>, promise);
    ComSignal::isServerContactable(promise);
    *isContactable = promise->get();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::displayTaskManager()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("displayTaskManager");

    ComSignal::displayTaskManager();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::changeOperatorOverride()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("changeOperatorOverride");

    ComSignal::changeOperatorOverride();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::isOperatorOverridden(BOOL* isOverridden)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("isOperatorOverridden");

    DECLEAR_SHARED(Promise<bool>, promise);
    ComSignal::isOperatorOverridden(promise);
    *isOverridden = promise->get();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchDisplayEx(BSTR display, BSTR asset, long leftPosition)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchDisplayEx");

    LOG_INFO("Trying to launch display %s with asset %s on screen %d", toString(display).c_str(), toString(asset).c_str(), leftPosition);

    ComSignal::launchDisplayEx(toString(display), toString(asset), leftPosition);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::setDisplayDimension(RECT* dim)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("setDisplayDimension");

    ComSignal::setDisplayDimension(*dim);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getRect(EScreen targetScreen, EArea targetArea, long val, RECT* dim)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FUNCTION_ENTRY("getRect");

    DECLEAR_SHARED(Promise<RECT>, dimPromise);
    ComSignal::getRect(targetScreen, targetArea, val, dimPromise);
    *dim = dimPromise->get();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getCurrentDisplayName(BSTR* display)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getCurrentDisplayName");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getCurrentDisplayName(str);
    copyToBSTR(display, str);

    FUNCTION_EXIT;
    return S_OK;
}

// these are old methods that should probably be deprecated some time in future when none
// of the GraphWorx displays uses these methods.

STDMETHODIMP CControlStation2::launchApplication(long appType, long leftPosition, long topPosition, long width, long height, BSTR FAR* addtionalCommands)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchApplication");

    ComSignal::launchApplication(appType, leftPosition, topPosition, width, height, toString(addtionalCommands));

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchDisplay(BSTR display, long leftPosition)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchDisplay");

    LOG_INFO("Trying to launch display %s on screen %li", display, leftPosition);

    ComSignal::launchDisplay(toString(display), leftPosition);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::getSessionId(BSTR* sessionId)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getSessionId");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getSessionId(str);
    copyToBSTR(sessionId, str);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::loadComplete(long leftPosition)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("loadComplete");

    ComSignal::loadComplete(leftPosition);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchHelp()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchHelp");

    ComSignal::launchHelp();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchPlan(long planId)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchPlan");

    ComSignal::launchPlan(planId);

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::launchPlanByName(BSTR planPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("launchPlanByName");

    ComSignal::launchPlanByName(toString(planPath));

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::setRunParam(BSTR paramName, BSTR paramValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("setRunParam");

    ComSignal::setRunParam(toString(paramName), toString(paramValue));

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::isActionPermittedOnEntity(BSTR entityName, long actionKey, /*[out, retval]*/ ResponseEnum* response)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("isActionPermittedOnEntity");

    DECLEAR_SHARED(Promise<ResponseEnum>, promise);
    ComSignal::isActionPermittedOnEntity(toString(entityName), actionKey, promise);
    *response = promise->get();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CControlStation2::entityCommand(EntityMessageType entityType, BSTR location, BSTR sourceSchematicID, BSTR message)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("entityCommand");

    ComSignal::entityCommand(entityType, toString(location), toString(sourceSchematicID), toString(message));

    FUNCTION_EXIT;
    return S_OK;
}

//TD12495 - added login verification
STDMETHODIMP CControlStation2::isLoginAllowed(BSTR bstrOperator, BSTR bstrPassword, BOOL* bLogin)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("isLoginAllowed");

    DECLEAR_SHARED(Promise<bool>, promise);
    ComSignal::isLoginAllowed(toString(bstrOperator), toString(bstrPassword), promise);
    *bLogin = promise->get();

    return S_OK;
}

STDMETHODIMP CControlStation2::getDisplayMode(BSTR* pRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FUNCTION_ENTRY("getDisplayMode");

    DECLEAR_SHARED(Promise<std::string>, str);
    ComSignal::getDisplayMode(str);
    copyToBSTR(pRet, str);

    FUNCTION_EXIT;
    return S_OK;
}

void CControlStation2::createArray(std::map<unsigned long, std::string>& mapData, VARIANT FAR& vaResult)
{
    const long ID_COLUMN = 0;
    const long NAME_COLUMN = 1;
    const long NUM_COLUMNS = 2;
    const long NUM_DIMENSIONS = 2;

    // Create an array large enough for all elements in the map. It is a 2D array.
    SAFEARRAYBOUND sabound[NUM_DIMENSIONS];
    sabound[0].cElements = NUM_COLUMNS;        //columns
    sabound[0].lLbound = 0;
    sabound[1].cElements = mapData.size();     //rows
    sabound[1].lLbound = 0;

    // Two dimensional array of strings
    SAFEARRAY FAR* psa = SafeArrayCreate(VT_BSTR, NUM_DIMENSIONS, sabound);

    if (psa == NULL)
    {
        AfxThrowOleDispatchException(1004, "Out of Memory");
    }

    int insertPosition = 0;

    for (std::map<unsigned long, std::string>::iterator iter = mapData.begin();
            iter != mapData.end();
            ++iter
        )
    {
        long indexes[NUM_DIMENSIONS];
        indexes[1] = insertPosition;  //row

        // First add the application type id as the first entry
        indexes[0] = ID_COLUMN;       //column
        CString id;
        id.Format("%lu", iter->first);
        HRESULT hr = SafeArrayPutElement(psa, indexes, id.AllocSysString());

        if (FAILED(hr))
        {
            if (psa)
            {
                SafeArrayDestroy(psa);
            }

            logError(hr);
            AfxThrowOleDispatchException(1003, "Unexpected Failure");
            return;
        }

        // Now insert the name of the application
        indexes[0] = NAME_COLUMN;     //column
        CString application = iter->second.c_str();
        hr = SafeArrayPutElement(psa, indexes, application.AllocSysString());

        if (FAILED(hr))
        {
            if (psa)
            {
                SafeArrayDestroy(psa);
            }

            logError(hr);
            AfxThrowOleDispatchException(1003, "Unexpected Failure");
            return;
        }

        // Increase the insert position so we are ready to go around again and insert the next
        // application.
        ++insertPosition;
    }

    V_VT(&vaResult) = VT_ARRAY | VT_BSTR;
    V_ARRAY(&vaResult) = psa;
}

void CControlStation2::createArray(std::vector<ApplicationProperties>& applist, VARIANT FAR& vaResult)
{
    const long ID_COLUMN = 0;
    const long NAME_COLUMN = 1;
    const long ISACCESSIBLE_COLUMN = 2;
    const long NUM_COLUMNS = 3;
    const long NUM_DIMENSIONS = 3;

    // Create an array large enough for all elements in the vector. It is a 3D array.
    SAFEARRAYBOUND sabound[NUM_DIMENSIONS];
    sabound[0].cElements = NUM_DIMENSIONS;        //columns
    sabound[0].lLbound = 0;
    sabound[1].cElements = applist.size();     //rows
    sabound[1].lLbound = 0;
    sabound[2].cElements = 1;     //rows
    sabound[2].lLbound = 0;

    // Two dimensional array of strings
    SAFEARRAY FAR* psa = SafeArrayCreate(VT_BSTR, NUM_DIMENSIONS, sabound);

    if (psa == NULL)
    {
        AfxThrowOleDispatchException(1004, "Out of Memory");
    }

    int insertPosition = 0;

    for (unsigned int i = 0; i < applist.size(); i++)
    {
        long indexes[NUM_DIMENSIONS];
        indexes[2] = 0;
        indexes[1] = i;               //row

        // First add the application type id as the first entry
        indexes[0] = ID_COLUMN;       //column
        CString id;
        id.Format("%lu", applist[i].key);
        HRESULT hr = SafeArrayPutElement(psa, indexes, id.AllocSysString());

        if (FAILED(hr))
        {
            if (psa)
            {
                SafeArrayDestroy(psa);
            }

            logError(hr);
            AfxThrowOleDispatchException(1003, "Unexpected Failure");
            return;
        }

        // Now insert the name of the application
        indexes[0] = NAME_COLUMN;     //column
        CString application = applist[i].name.c_str();
        hr = SafeArrayPutElement(psa, indexes, application.AllocSysString());

        if (FAILED(hr))
        {
            if (psa)
            {
                SafeArrayDestroy(psa);
            }

            logError(hr);
            AfxThrowOleDispatchException(1003, "Unexpected Failure");
            return;
        }

        // Now insert the enable condition of the application
        indexes[0] = ISACCESSIBLE_COLUMN;     //column
        CString isAccessible;
        // display the boolean as unsigned int string
        isAccessible.Format("%lu", applist[i].isAccessible);
        hr = SafeArrayPutElement(psa, indexes, isAccessible.AllocSysString());

        if (FAILED(hr))
        {
            if (psa)
            {
                SafeArrayDestroy(psa);
            }

            logError(hr);
            AfxThrowOleDispatchException(1003, "Unexpected Failure");
            return;
        }

        // Increase the insert position so we are ready to go around again and insert the next
        // application.
        ++insertPosition;
    }

    V_VT(&vaResult) = VT_ARRAY | VT_BSTR;
    V_ARRAY(&vaResult) = psa;
}

void CControlStation2::logError(const HRESULT& hr)
{
    LOG_ERROR("Array of applications could not be returned because: ");

    switch (hr)
    {
    case (DISP_E_BADINDEX) :
        LOGMORE(SourceInfo, "The specified index was invalid.");
        break;

    case (E_INVALIDARG) :
        LOGMORE(SourceInfo, "One of the arguments is invalid.");
        break;

    case (E_OUTOFMEMORY) :
        LOGMORE(SourceInfo, "Memory could not be allocated for the element.");
        break;
    }
}

std::string CControlStation2::getAppName(const std::string& ex_str)
{
    boost::match_results<std::string::const_iterator> what;

    if (0 == boost::regex_search(ex_str, what, APP_PATTERN, boost::match_default))
    {
        LOG_ERROR("the application name format <%s> has parse error", ex_str.c_str());
        return "UnknownAppName";
    }

    if (what[1].matched)
    {
        return std::string(what[1].first, what[1].second);
    }

    LOG_ERROR("could not find correct application name from string <%s>", ex_str.c_str());
    return "UnknownAppName";
}

void CControlStation2::copyToBSTR(BSTR*& bstr, const std::string& str)
{
    *bstr = _bstr_t(str.c_str()).Detach();
}

void CControlStation2::copyToBSTR(BSTR*& bstr, boost::shared_ptr<Promise<std::string>> str)
{
    *bstr = _bstr_t(str->get().c_str()).Detach();
}

std::string CControlStation2::toString(const BSTR& bstr)
{
    return _bstr_t(bstr);
}

std::string CControlStation2::toString(const BSTR* bstr)
{
    return _bstr_t(*bstr);
}
