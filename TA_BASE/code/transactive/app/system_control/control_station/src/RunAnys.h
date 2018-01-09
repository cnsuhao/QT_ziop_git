#pragma once

struct RunAnys
{
public:

    static RunAnys& getInstance();

    template <typename T>
    T& get(const std::string& key)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        return *boost::any_cast<T>(&m_anys[key]);
    }

    template <typename T>
    void set(const std::string& key, const T& any)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_anys[key] = any;
    }

private:

    RunAnys() {}
    friend class ACE_Singleton<RunAnys, ACE_Null_Mutex>;

public:

    boost::mutex m_mutex;
    std::map<std::string, boost::any> m_anys;
};
