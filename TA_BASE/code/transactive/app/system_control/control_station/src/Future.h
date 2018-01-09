#pragma once

template <typename T>
struct Future
{
    boost::promise<T> promise;
    boost::shared_future<T> future;

    Future()
    {
        future = promise.get_future().share();
    }

    template <typename X>
    void set_value(const X& value)
    {
        if (!is_ready())
        {
            promise.set_value(value);
        }
    }

    void set_value()
    {
        if (!is_ready())
        {
            promise.set_value();
        }
    }

    void set_exception(boost::exception_ptr exp)
    {
        if (!is_ready())
        {
            promise.set_exception(exp);
        }
    }

    typename boost::shared_future<T>::move_dest_type get()
    {
        return future.get();
    }

    void wait()
    {
        future.wait();
    }

    template <class Rep, class Period>
    boost::future_status wait_for(const boost::chrono::duration<Rep, Period>& rel_time) const
    {
        return future.wait_for(rel_time);
    }

    boost::shared_future<T> get_future()
    {
        return future;
    }

    bool is_ready()
    {
        return future.is_ready();
    }

    bool has_exception()
    {
        return future.has_exception();
    }

    boost::exception_ptr get_exception_ptr()
    {
        return future.get_exception_ptr();
    }

    template <typename F>
    void set_wait_callback(F f)
    {
        promise.set_wait_callback(f);
    }

    void clear()
    {
        if (future.is_ready())
        {
            promise = boost::promise<T>();
            future = promise.get_future().share();
        }
    }
};
