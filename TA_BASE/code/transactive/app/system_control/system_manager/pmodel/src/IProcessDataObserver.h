#pragma once

namespace TA_Base_Core
{
    class ProcessDataSeq;
}

namespace TA_Base_App
{
    struct IProcessDataObserver
    {
        ~IProcessDataObserver() {}
        virtual void onReceivedProcessData(const TA_Base_Core::ProcessDataSeq&) = 0;
        virtual void onSystemControllerError() = 0;
    };
}
