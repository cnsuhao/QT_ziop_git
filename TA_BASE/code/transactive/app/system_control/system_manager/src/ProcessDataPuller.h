#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include "core/threads/src/Thread.h"

class omni_mutex;
class omni_condition;

namespace TA_Base_Core
{
    class ProcessDataSeq;
}

namespace TA_Base_Bus
{
    class ISystemControllerAccessor;
}

namespace TA_Base_App
{
    struct IProcessDataObserver;

    struct ProcessDataPuller : TA_Base_Core::Thread
    {
    public:

        ProcessDataPuller(TA_Base_Bus::ISystemControllerAccessor& systemController);
        ~ProcessDataPuller();
        void registerProcessDataObserver(IProcessDataObserver* observer);
        void unregisterProcessDataObserver(IProcessDataObserver* observer);
        void pause();
        void continuing();
        void refresh();
        static void setInterval(size_t interval) { m_interval = interval * 1000; }
        static size_t getInterval() { return m_interval / 1000; }

    private:

        virtual void run();
        virtual void terminate();
        void wait();
        void sleep();
        void notifyData(const TA_Base_Core::ProcessDataSeq& data);
        void notifyError();

    private:

        bool m_running;
        bool m_paused;
        static size_t m_interval;
        omni_mutex* m_mutex;
        omni_condition* m_condition;
        std::set<IProcessDataObserver*> m_observers;
        TA_Base_Bus::ISystemControllerAccessor& m_systemController;
    };
}
