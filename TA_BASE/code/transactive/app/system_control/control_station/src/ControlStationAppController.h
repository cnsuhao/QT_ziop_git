/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ControlStationAppController.h $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the System Manager implementation of the
  * GenericGUI IApplicationController interfaces.
  *
  */
#pragma once
#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include "core/process_management/IDL/src/IProcessManagerCorbaDef.h"

struct ControlStationAppController : TA_Base_Bus::AbstractGUIAppController
{
public:

    ControlStationAppController(TA_Base_Bus::ITransActiveApp* pApp, const char* appName);
    ~ControlStationAppController();
    virtual void initAppController(const std::string& commandLine, bool = false) override;
    virtual void runController() override {}
    virtual unsigned long getApplicationType() override { return 0; }

protected:

    virtual TA_Base_Bus::ITransActivePModel* createPModel(const std::string& pModelName) { return NULL; }

protected:

    void setupSignalConnections();
    void waitInitializationComplete();
    void initializeRunParams(const std::string& commandLine);
    void initializeDebugUtility();
    void archiveLogFiles(const std::string& debugFileName);
    void locateSystemController();
    void retrieveRunParamsFromSystemController();
    void initializeCorba();
    void handleSystemControllerStateChangedSignal(bool isGroupOnline, const std::string& groupName);
    virtual void cleanup() override;
    static TA_Base_Core::IProcessManagerCorbaDef_var locateSpecificSystemController(const std::string& hostname, unsigned long portNumber);

protected:

    TA_Base_Core::IProcessManagerCorbaDef_var m_systemController;
};
