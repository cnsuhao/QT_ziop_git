#pragma once

struct PromiseAnyMap
{
    typedef std::pair<boost::any, boost::any> AnyPair;
    typedef boost::container::map<std::string, AnyPair> AnyMap;

    static PromiseAnyMap& getInstance();

    template <typename T>
    void wait(const std::string& key)
    {
        get_future<T>(key).wait();
    }

    template <typename T>
    bool is_ready(const std::string& key)
    {
        return get_future<T>(key).is_ready();
    }

    template <typename T>
    void set_value(const std::string& key, const T& v)
    {
        if (!is_ready<T>(key))
        {
            get_promise<T>(key).set_value(v);
        }
    }

    template <typename T>
    void set_value(const std::string& key)
    {
        if (!is_ready<T>(key))
        {
            get_promise<T>(key).set_value();
        }
    }

    template <typename T, typename F>
    void set_wait_callback(const std::string& key, F f)
    {
        get_promise<T>(key).set_wait_callback(f);
    }

    template <typename T>
    typename boost::shared_future<T>::move_dest_type get(const std::string& key)
    {
        return get_future<T>(key).get();
    }

    template <typename T>
    void add(const std::string& key)
    {
        clear<T>(key);
    }

    template <typename T>
    void clear(const std::string& key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_promises[key] = boost::move(make_any_pair<T>());
    }

    template <typename T>
    boost::promise<T>& get_promise(const std::string& key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        check_existance<T>(key);
        return *boost::any_cast<boost::shared_ptr<boost::promise<T> >>(m_promises[key].first);
    }

    template <typename T>
    boost::shared_future<T> get_future(const std::string& key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        check_existance<T>(key);
        return boost::any_cast<boost::shared_future<T>>(m_promises[key].second);
    }

private:

    template <typename T>
    AnyPair make_any_pair()
    {
        boost::shared_ptr<boost::promise<T> > promise(new boost::promise<T>);
        return std::pair<boost::any, boost::any>(promise, promise->get_future().share());
    }

    template <typename T>
    void check_existance(const std::string& key)
    {
        if (m_promises.find(key) == m_promises.end())
        {
            m_promises[key] = boost::move(make_any_pair<T>());
        }
    }

public:

    PromiseAnyMap() {}
    boost::mutex m_mutex;
    AnyMap m_promises;
};
