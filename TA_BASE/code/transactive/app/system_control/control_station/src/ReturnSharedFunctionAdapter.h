#pragma once

template <typename F>
struct ReturnSharedFunctionAdapter
{
    template <typename T> struct IsShared
    {
        enum { value = 0 };
    };

    template <typename T> struct IsShared<boost::shared_ptr<T> >
    {
        enum { value = 1 };
    };

    template <typename T> struct RemoveShared
    {
        typedef T type;
    };

    template <typename T> struct RemoveShared<boost::shared_ptr<T> >
    {
        typedef T type;
    };

    typedef typename boost::result_of<F()>::type R;
    typedef typename RemoveShared<typename boost::remove_pointer<typename boost::remove_reference<R>::type>::type>::type T;
    typedef boost::shared_ptr<T> ResultType;
    typedef boost::shared_future<ResultType> FutureType;
    enum { isPointer = boost::is_pointer<R>::value };
    enum { isShared = IsShared<R>::value };

    ReturnSharedFunctionAdapter(F f)
        : m_f(f)
    {
    }

    ResultType operator()()
    {
        return impl<isPointer, isShared>();
    }

protected:

    template <bool is_ptr, bool IsShared> ResultType impl()
    {
        return ResultType(new T(m_f()));
    }

    template <> ResultType impl<true, false>() // pointer
    {
        return ResultType(m_f());
    }

    template <> ResultType impl<false, true>() // shared_ptr
    {
        return m_f();
    }

    F m_f;
};

template <typename F>
typename ReturnSharedFunctionAdapter<F>::FutureType sharedAsync(F x)
{
    return boost::async(ReturnSharedFunctionAdapter<F>(x)).share();
}
