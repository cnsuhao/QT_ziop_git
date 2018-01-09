#include "ProcessDataPuller.h"
#include "Utilities.h"
#include "IProcessDataObserver.h"
#include "ForeachCall.h"
#include "bus/common_subsystem_facade/src/SystemControllerFacade.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    static const unsigned int DEFAULT_REFRESH_RATE = 1000;
    size_t ProcessDataPuller::m_interval = DEFAULT_REFRESH_RATE;


    ProcessDataPuller::ProcessDataPuller(TA_Base_Bus::ISystemControllerAccessor& systemController)
        : m_systemController(systemController),
          m_running(true),
          m_paused(false)
    {
        m_mutex = new omni_mutex;
        m_condition = new omni_condition(m_mutex);
    }


    ProcessDataPuller::~ProcessDataPuller()
    {
        terminate();
        m_condition->signal();
        terminateAndWait();
    }


    void ProcessDataPuller::registerProcessDataObserver(IProcessDataObserver* observer)
    {
        m_observers.insert(observer);
        m_condition->signal();
    }


    void ProcessDataPuller::unregisterProcessDataObserver(IProcessDataObserver* observer)
    {
        m_observers.erase(observer);
    }


    void ProcessDataPuller::pause()
    {
        m_paused = true;
    }


    void ProcessDataPuller::continuing()
    {
        m_paused = false;
        m_condition->signal();
    }


    void ProcessDataPuller::refresh()
    {
        m_condition->signal();
    }


    void ProcessDataPuller::sleep()
    {
        unsigned long absoluteSecs = 0;
        unsigned long absoluteNanoSecs = 0;
        omni_thread::get_time(&absoluteSecs, &absoluteNanoSecs, m_interval / 1000, (m_interval % 1000) * 1000000);

        if (m_running && !m_paused)
        {
            m_mutex->lock();
            m_condition->timedwait(absoluteSecs, absoluteNanoSecs);
            m_mutex->unlock();
        }
    }


    void ProcessDataPuller::wait()
    {
        if (m_running && m_paused || m_observers.empty())
        {
            m_mutex->lock();

            while (m_running && m_paused || m_observers.empty())
            {
                m_condition->wait();
            }

            m_mutex->unlock();
        }
    }


    void ProcessDataPuller::run()
    {
        while (m_running)
        {
            wait();

            if (m_running && !m_paused)
            {
                try
                {
                    TA_Base_Core::ProcessDataSeq_var data = m_systemController.getProcessData();
                    notifyData(data);
                }
                catch (...)
                {
                    notifyError();
                }
            }

            sleep();
        }
    }


    void ProcessDataPuller::terminate()
    {
        m_running = false;
        m_paused = false;
    }


    void ProcessDataPuller::notifyData(const TA_Base_Core::ProcessDataSeq& data)
    {
        foreachCall(m_observers, &IProcessDataObserver::onReceivedProcessData, data);
    }


    void ProcessDataPuller::notifyError()
    {
        foreachCall(m_observers, &IProcessDataObserver::onSystemControllerError);
    }
}
