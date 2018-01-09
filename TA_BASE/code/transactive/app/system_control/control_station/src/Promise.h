#pragma once

template <typename T>
struct Promise
{
    boost::promise<T> promise;
    boost::shared_future<T> future;

    Promise()
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

    //template <typename X>
    //operator X& ()
    //{
    //    return future.get();
    //}

    template <typename X>
    operator const X& () const
    {
        return future.get();
    }
};

typedef Promise<void> VoidPromise;
typedef boost::shared_ptr<VoidPromise> VoidPromisePtr;
typedef VoidPromisePtr PromisePtr;

typedef Promise<bool> BoolPromise;
typedef boost::shared_ptr<BoolPromise> BoolPromisePtr;

typedef Promise<std::string> StringPromise;
typedef boost::shared_ptr<StringPromise> StringPromisePtr;

typedef Promise<long> LongPromise;
typedef boost::shared_ptr<LongPromise> LongPromisePtr;

typedef Promise<unsigned long> ULongPromise;
typedef boost::shared_ptr<ULongPromise> ULongPromisePtr;

#define DECLEAR_SHARED(type, name) boost::shared_ptr<type> name(new type())
