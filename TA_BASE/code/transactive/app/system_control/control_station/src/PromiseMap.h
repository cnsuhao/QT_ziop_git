#pragma once

#define PROMISE_DatabaseAvaliable                   "DatabaseAvaliable"
#define PROMISE_CorbaAvaliable                      "CorbaAvaliable"
#define PROMISE_RunParamsAvaliable                  "RunParamsAvaliable"
#define PROMISE_DebugFileAvaliable                  "DebugFileAvaliable"
#define PROMISE_SystemControllerLocated             "SystemControllerLocated"
#define PROMISE_SystemControllerRunParams           "SystemControllerRunParams"
#define PROMISE_ScreenPositioningAvaliable          "ScreenPositioningAvaliable"
#define PROMISE_GraphWorxReady                      "GraphWorxReady"
#define PROMISE_ControlStationCorbaDefActivated     "ControlStationCorbaDefActivated"

template <typename T = void>
struct PromiseMap
{
    typedef std::pair<boost::shared_ptr<boost::promise<T> >, boost::shared_future<T> > Pair;
    typedef boost::container::map<std::string, Pair> Map;
    typedef const std::string& Key;

    static PromiseMap& getInstance()
    {
        return *ACE_Unmanaged_Singleton<PromiseMap, ACE_Null_Mutex>::instance();
    }

    PromiseMap& wait(Key key)
    {
        get_future(key).wait();
        return *this;
    }

    template <class Rep, class Period>
    boost::future_status wait_for(Key key, const boost::chrono::duration<Rep, Period>& rel_time) const
    {
        return get_future(key).wait_for(rel_time);
    }

    template <class Clock, class Duration>
    boost::future_status wait_until(const boost::chrono::time_point<Clock, Duration>& abs_time) const
    {
        return get_future(key).wait_until(abs_time);
    }

    PromiseMap& wait_for_all(Key k1, Key k2 = "", Key k3 = "", Key k4 = "", Key k5 = "", Key k6 = "", Key k7 = "", Key k8 = "", Key k9 = "")
    {
        std::vector<boost::shared_future<T> > vf;
        std::vector<std::string> keys = boost::assign::list_of(k1)(k2)(k3)(k4)(k5)(k6)(k7)(k8)(k9);

        for (std::string& k : keys)
        {
            if (k.size())
            {
                vf.push_back(get_future(k));
            }
        }

        boost::wait_for_all(vf.begin(), vf.end());
        return *this;
    }

    bool is_ready(Key key)
    {
        return get_future(key).is_ready();
    }

    template <typename X>
    PromiseMap& set_value(Key key, const X& v)
    {
        if (!is_ready(key))
        {
            get_promise(key).set_value(v);
        }

        return *this;
    }

    PromiseMap& set_value(Key key)
    {
        if (!is_ready(key))
        {
            get_promise(key).set_value();
        }

        return *this;
    }

    PromiseMap& set_exception(Key key, std::exception_ptr exp)
    {
        if (!is_ready(key))
        {
            get_promise(key).set_exception(exp);
        }

        return *this;
    }

    template <typename F>
    void set_wait_callback(Key key, F f)
    {
        get_promise(key).set_wait_callback(f);
    }

    typename boost::shared_future<T>::move_dest_type get(Key key)
    {
        return get_future(key).get();
    }

    PromiseMap& add(Key key)
    {
        clear(key);
        return *this;
    }

    PromiseMap& clear(Key key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_promises[key] = boost::move(make_pair());
        return *this;
    }

    boost::promise<T>& get_promise(Key key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        check_existance(key);
        return *m_promises[key].first;
    }

    boost::shared_future<T> get_future(Key key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        check_existance(key);
        return m_promises[key].second;
    }

private:

    Pair make_pair()
    {
        boost::shared_ptr<boost::promise<T> > promise(new boost::promise<T>);
        return std::make_pair(promise, promise->get_future().share());
    }

    void check_existance(Key key)
    {
        if (m_promises.find(key) == m_promises.end())
        {
            m_promises[key] = boost::move(make_pair());
        }
    }

public:

    PromiseMap() {}
    boost::mutex m_mutex;
    Map m_promises;
};
