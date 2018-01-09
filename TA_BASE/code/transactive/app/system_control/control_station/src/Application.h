#pragma once
#include "Promise.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"

namespace TA_Base_Core
{
    class IGui;
}

class Application
{
public:

    Application(TA_Base_Core::IGui* gui);
    virtual ~Application();
    bool isAccessible();
    TA_Base_Core::IGui& getGui() { return *m_gui; }
    static Application* create(TA_Base_Core::IGui* gui);

protected:

    virtual void launchApplicationByType(const std::string& additionalCommands,
                                         unsigned long posFlag,
                                         unsigned long alignFlag,
                                         const RECT& objectDim,
                                         const RECT& boundaryDim,
                                         bool isVisible);

    virtual void registerManagedProcessByType(TA_Base_Core::ProcessId processId,
                                              TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                              const std::string& entity);

    void setupSignalConnections();
    void disconnectSignals();
    void getResourceKeyCallback(boost::promise<unsigned int>&);
    void getDisplayNameCallback(boost::promise<std::string>&);

protected:

    static const int APPLICATION_NAME_LENGTH;
    TA_Base_Core::IGui* m_gui;
    Promise<unsigned int> m_resourceKey;
    unsigned long m_type;
    boost::container::vector<boost::signals2::connection> m_connections;
};
