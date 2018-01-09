#pragma once
#include <ace/Future.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

template <typename T>
struct DefaultNewCreator
{
    T* operator()()
    {
        return new T;
    }
};

template <typename F>
struct ReturnSharedFunctionAdapter
{
    typedef typename boost::result_of<F()>::type R;
    typedef typename boost::remove_const<typename boost::remove_pointer<typename boost::remove_reference<R>::type>::type>::type T;
    typedef boost::shared_ptr<T> Ptr;
    typedef Ptr ResultType;
    typedef boost::shared_future<Ptr> FutureType;
    enum { isReturnPtr = boost::is_pointer<R>::value };

    ReturnSharedFunctionAdapter(F f)
        : m_f(f)
    {
    }

    ResultType operator()()
    {
        return impl<isReturnPtr>();
    }

private:

    template <bool is_ptr> Ptr impl();

    template <>
    ResultType impl<true>()
    {
        return Ptr(m_f());
    }

    template <>
    ResultType impl<false>()
    {
        return Ptr(new T(m_f()));
    }

    F m_f;
};

template <typename F>
typename ReturnSharedFunctionAdapter<F>::FutureType async(F x)
{
    return boost::async(ReturnSharedFunctionAdapter<F>(x));
}

template <typename T>
struct MyFuture
{
    boost::shared_future< boost::shared_ptr<T> > m_future;

    MyFuture()
    {
    }

    void setCreator()
    {
        setCreator(DefaultNewCreator<T>());
    }

    template <typename F>
    void setCreator(F f)
    {
        m_future = async(f);
    }

    template <typename F>
    void setSharedCreator(F f)
    {
        m_future = boost::async(boost::function<boost::shared_ptr<T>()>(f)).share();
    }

    boost::shared_ptr<T> get()
    {
        return m_future.get();
    }

    operator T& ()
    {
        return *get();
    }

    operator T* ()
    {
        return get().get();
    }

    operator boost::shared_future< boost::shared_ptr<T> > ()
    {
        return m_future;
    }

    T* operator-> ()
    {
        return get().get();
    }

    T& operator* ()
    {
        return *get();
    }

    bool ready() const
    {
        return m_future.is_ready();
    }

    void set(const T& x)
    {
        m_future.set_value(boost::shared_ptr<T>(new T(x)));
    }

    void set(const T* x)
    {
        m_future.set_value(boost::shared_ptr<T>(m_ptr));
    }
};
