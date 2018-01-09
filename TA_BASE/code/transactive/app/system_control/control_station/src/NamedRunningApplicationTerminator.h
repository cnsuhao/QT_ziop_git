#pragma once
#include "StriCmp.h"
#include "AsyncSignal.h"

class NamedRunningApplicationTerminator
{
public:

    NamedRunningApplicationTerminator(const std::string& csv);

    template <typename T>
    void addSignal(T& signal)
    {
        signal.connect(boost::bind(&NamedRunningApplicationTerminator::terminateApplications, this));
    }

    void addApplicationName(const std::string& name);

protected:

    void terminateApplications();
    static void terminateByPid(const std::string& name, unsigned long pid);

protected:

    std::set<std::string, StriCmp> m_names;
};

typedef boost::shared_ptr<NamedRunningApplicationTerminator> NamedRunningApplicationTerminatorPtr;
typedef std::vector<NamedRunningApplicationTerminatorPtr> NamedRunningApplicationTerminatorPtrList;
