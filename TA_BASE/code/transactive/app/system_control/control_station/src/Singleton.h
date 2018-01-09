#pragma once

template <typename T>
struct Singleton
{
protected:

    struct SintletonImpl : T {};

    Singleton()
    {
        asyncInitializer() = boost::bind(&Singleton::asyncInitialize, this);
    }

    virtual ~Singleton() {}

    virtual void asyncInitialize()
    {
        promiseForInit().set_value();
    }

public:

    static T& getInstanceNoInit()
    {
        T& instance = *ACE_Unmanaged_Singleton<SintletonImpl, ACE_Recursive_Thread_Mutex>::instance();
        boost::call_once(&Singleton::asyncInitializeInternal, onceFlag());
        return instance;
    }

    static T& getInstance()
    {
        T& instance = *ACE_Unmanaged_Singleton<SintletonImpl, ACE_Recursive_Thread_Mutex>::instance();

        if (!futureForInit().is_ready())
        {
            boost::call_once(&Singleton::asyncInitializeInternal, onceFlag());
            futureForInit().wait();
        }

        return instance;
    }

    template <typename MemFun, typename... Args>
    static void setInitializer(MemFun f, Args... args)
    {
        T* pThis = ACE_Unmanaged_Singleton<SintletonImpl, ACE_Recursive_Thread_Mutex>::instance();
        asyncInitializer() = boost::bind(f, pThis, args...);
    }

protected:

    void waitAsyncInitialize()
    {
        if (!futureForInit().is_ready())
        {
            futureForInit().wait();
        }
    }

    static void asyncInitializeInternal()
    {
        auto f = boost::async(asyncInitializer()).share();
        boost::async(boost::bind(&Singleton::waitForAsyncInitialize, f));
    }

    static void waitForAsyncInitialize(boost::shared_future<void> f)
    {
        f.wait();
        promiseForInit().set_value();
    }

    static boost::promise<void>& promiseForInit()
    {
        static boost::promise<void> promise;
        return promise;
    }

    static boost::shared_future<void> futureForInit()
    {
        static boost::shared_future<void> future = promiseForInit().get_future().share();
        return future;
    }

    static boost::once_flag& onceFlag()
    {
        static boost::once_flag once = BOOST_ONCE_INIT;
        return once;
    }

    static boost::function<void()>& asyncInitializer()
    {
        static boost::function<void()> initializer;
        return initializer;
    }
};
