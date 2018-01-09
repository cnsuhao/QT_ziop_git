#pragma once

class IRunningApplication
{
public:

    virtual ~IRunningApplication() {}
    virtual unsigned long getProcessId() const = 0;
    virtual unsigned long getAppType() const = 0;
    virtual std::string getEntityName() const = 0;
    virtual std::string getApplicationName() const = 0;
    virtual std::string getApplicationFullName() const = 0;
    virtual std::string getHelpFile()const = 0;
    virtual bool isVisibleToUserAsRunning() const = 0;
    virtual bool areInstancesLimited() const = 0;
    virtual void move(unsigned long posFlag, unsigned long alignFlag, const RECT& objectDim, const RECT& boundaryDim) = 0;
};
