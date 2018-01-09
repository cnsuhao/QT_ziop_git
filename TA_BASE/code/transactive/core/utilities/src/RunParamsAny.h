#ifndef RUNPARAMS_ANY_H_INCLUDED
#define RUNPARAMS_ANY_H_INCLUDED

#include <ace/Singleton.h>
#include <boost/thread/future.hpp>
#include <map>
#include <string>

namespace TA_Base_Core
{
    class RunParamsAny
    {
        friend class ACE_Singleton<RunParamsAny, ACE_Null_Mutex>;

    public:

        static RunParamsAny& getInstance()
        {
            return *ACE_Unmanaged_Singleton<RunParamsAny, ACE_Null_Mutex>::instance();
        }

        void clear()
        {
            m_params.clear();
            m_promises.clear();
        }

        template <typename T>
        void set(const std::string& name, const T& value)
        {
            boost::unique_lock<boost::mutex> lock(m_mutex);
            m_params[name] = value;

            if (m_promises.find(name) != m_promises.end())
            {
                m_promises[name].first.set_value();
            }
        }

        bool isSet(const std::string& name)
        {
            boost::unique_lock<boost::mutex> lock(m_mutex);
            return m_params.find(name) != m_params.end();
        }

        template <typename T>
        const T& get(const std::string& name)
        {
            boost::unique_lock<boost::mutex> lock(m_mutex);

            if (m_params.find(name) == m_params.end())
            {
                static T defaultValue = T();
                return defaultValue;
            }

            return *boost::any_cast<T>(&m_params[name]);
        }

        template <typename T>
        const T& promiseGet(const std::string& name, int timeoutMs = -1)
        {
            if (!isSet(name))
            {
                {
                    boost::unique_lock<boost::mutex> lock(m_mutex);

                    if (m_promises.find(name) == m_promises.end())
                    {
                        m_promises[name].second = m_promises[name].first.get_future().share();
                    }
                }

                if (!m_promises[name].second.is_ready())
                {
                    if (timeoutMs != -1)
                    {
                        m_promises[name].second.wait_for(boost::chrono::milliseconds(timeoutMs));
                    }
                    else
                    {
                        m_promises[name].second.wait();
                    }
                }
            }

            return get<T>(name);
        }

    protected:

        boost::mutex m_mutex;
        std::map<std::string, boost::any> m_params;
        std::map<std::string, std::pair<boost::promise<void>, boost::shared_future<void>>> m_promises;
    };
}

#endif
