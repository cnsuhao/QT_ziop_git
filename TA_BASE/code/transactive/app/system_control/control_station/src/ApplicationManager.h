#pragma once
#include "IProcessManager.h"
#include "Singleton.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"
#include "core/configuration_updates/src/IOnlineUpdatable.h"

namespace TA_Base_Core
{
    class IGui;
    class ConfigUpdateDetails;
}

class NamedRunningApplicationTerminator;
typedef std::map<unsigned long, Application*> ApplicationMap;
typedef boost::shared_ptr<NamedRunningApplicationTerminator> NamedRunningApplicationTerminatorPtr;
typedef std::vector<NamedRunningApplicationTerminatorPtr> NamedRunningApplicationTerminatorPtrList;
typedef boost::function<Application*(TA_Base_Core::IGui*)> ApplicationCreator;
typedef std::map<unsigned long, ApplicationCreator> ApplicationCreatorMap;

class ApplicationManager : public TA_Base_Core::IOnlineUpdatable, public Singleton<ApplicationManager>
{
public:

    std::vector<IProcessManager::ApplicationProperties> getApplications();

protected:

    ApplicationManager();

    virtual void launchApplication(unsigned long appType,
                                   const std::string& additionalCommands,
                                   unsigned long posFlag,
                                   unsigned long alignFlag,
                                   const RECT& objectDim,
                                   const RECT& boundaryDim,
                                   bool isVisible);

    virtual void registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                        TA_Base_Core::ProcessId processId,
                                        CORBA::ULong applicationType,
                                        const std::string& entity);

    virtual void processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent) override;
    virtual void asyncInitialize() override;
    void setupSignalConnections();
    void setupApplicationCreators();
    void loadApplications();
    void loadTerminators();
    void checkAppType(unsigned long appType);
    bool isValidAppType(unsigned long appType);
    ApplicationCreator getApplicationCreator(unsigned long appType);

protected:

    ApplicationMap m_applications;
    ApplicationCreatorMap m_creators;
    NamedRunningApplicationTerminatorPtrList m_terminators;
};
