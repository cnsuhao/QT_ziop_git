#pragma once

template <typename T>
struct Promise
{
    Promise()
    {
        future = promise.get_future().share();
    }

    template <typename T>
    void set_value(const T& value)
    {
        promise.set_value(value);
    }

    const T& get()
    {
        return future.get();
    }

    template <typename F>
    void set_wait_callback(F f)
    {
        promise.set_wait_callback(f);
    }

    template <typename X>
    operator const X& ()
    {
        return future.get();
    }

    operator const T& ()
    {
        return future.get();
    }

    boost::promise<T> promise;
    boost::shared_future<T> future;
};
