#include "StdAfx.h"
#include "ApplicationSignal.h"

boost::container::map<unsigned long, AsyncSignal<void(unsigned long)> > ApplicationSignal::processTerminating;
AsyncSignal<void(unsigned long, const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> ApplicationSignal::launchApplication;
boost::container::map<unsigned long, AsyncSignal<void(const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> > ApplicationSignal::launchApplicationByAppType;
AsyncSignal<void(TA_Base_Core::IManagedProcessCorbaDef_var, unsigned long, unsigned long, const char*)> ApplicationSignal::registerManagedProcess;
boost::container::map<unsigned long, AsyncSignal<void(unsigned long, TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > ApplicationSignal::registerManagedProcessByAppType;
boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > ApplicationSignal::registerManagedProcessByPid;
boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::IManagedGUICorbaDef_var)> > ApplicationSignal::registerManagedApplication;
boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EProcessStatus)> > ApplicationSignal::statusChanged;
boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EManagedProcessTerminateCode)> > ApplicationSignal::terminating;
boost::container::map<unsigned long, AsyncSignal<void(TA_Base_Core::EFocusType focus)> > ApplicationSignal::giveApplicationFocus;
