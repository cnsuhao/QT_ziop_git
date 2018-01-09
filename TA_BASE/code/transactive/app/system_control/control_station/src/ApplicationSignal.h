#pragma once
#include "AsyncSignal.h"
#include "Promise.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"
#include "core/process_management/IDL/src/IManagedGuiCorbaDef.h"

struct ApplicationSignal
{
    static boost::container::map<unsigned long, AsyncSignal<void(unsigned long)> > processTerminating;
    static AsyncSignal<void(unsigned long, const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> launchApplication;
    static boost::container::map<unsigned long, AsyncSignal<void(const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> > launchApplicationByAppType;
    static AsyncSignal<void(TA_Base_Core::IManagedProcessCorbaDef_var, unsigned long, unsigned long, const char*)> registerManagedProcess;
    static boost::container::map<unsigned long, AsyncSignal<void(unsigned long, TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > registerManagedProcessByAppType;
    static boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > registerManagedProcessByPid;
    static boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::IManagedGUICorbaDef_var)> > registerManagedApplication;
    static boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EProcessStatus)> > statusChanged;
    static boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EManagedProcessTerminateCode)> > terminating;
    static boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EFocusType focus)> > giveApplicationFocus;
};
