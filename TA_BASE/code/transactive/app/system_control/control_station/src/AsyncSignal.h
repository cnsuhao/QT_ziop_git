#pragma once
#include <boost/utility.hpp>
#include <boost/move/move.hpp>

enum class AsyncSignalWait { NoWait, WaitForAll, WaitForAny };

template <typename Signature, AsyncSignalWait wait = AsyncSignalWait::NoWait>
struct AsyncSignal
{
    typedef AsyncSignal<Signature, wait> AsyncSignalType;
    typedef boost::signals2::signal<Signature> SignalType;
    typedef typename SignalType::result_type ResultType;
    typedef typename SignalType::slot_type SlotType;
    typedef boost::shared_future<ResultType> FutureType;
    SignalType m_signal;
    std::vector<FutureType> m_futures;

    struct AsyncSlot
    {
        SlotType m_slot;
        AsyncSignalType* m_signal;

        AsyncSlot(const SlotType& slot, AsyncSignalType* signal)
            : m_slot(slot),
              m_signal(signal)
        {
        }

        template <typename... Args>
        void operator()(Args... args)
        {
            FutureType future = boost::async(boost::bind(m_slot, args...)).share();
            m_signal->addFuture(future);
        }
    };

    boost::signals2::connection connect(const SlotType& slot)
    {
        return m_signal.connect(AsyncSlot(slot, this));
    }

    template <typename... Args>
    void operator()(Args&& ... args)
    {
        m_signal(args...);

        if (AsyncSignalWait::WaitForAll == wait)
        {
            boost::wait_for_all(m_futures.begin(), m_futures.end());
        }

        if (AsyncSignalWait::WaitForAny == wait)
        {
            boost::wait_for_any(m_futures.begin(), m_futures.end());
        }

        m_futures.clear();
    }

    operator SignalType& ()
    {
        return m_signal;
    }

    void addFuture(FutureType future)
    {
        m_futures.push_back(future);
    }
};
