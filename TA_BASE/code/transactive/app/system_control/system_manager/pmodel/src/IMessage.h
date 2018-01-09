#pragma once
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <set>
#include <string>

namespace TA_Base_App
{
    struct IMessage
    {
        virtual void onMessageLogin(const std::string& hostname) {}
        virtual void onMessageShutdownBegin() {}
        virtual void onMessageBeforeUseProcessData(const std::string& hostname, const TA_Base_Core::ProcessDataSeq& data) {}
        virtual void onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data) {}
        virtual void onMessageProcessSelectionChanged(const std::set<size_t>& selection) {}

        IMessage() { subscribers().insert(this); }
        virtual ~IMessage() { subscribers().erase(this); }
        static std::set<IMessage*>& subscribers() { static std::set<IMessage*> all; return all; }
    };
}
