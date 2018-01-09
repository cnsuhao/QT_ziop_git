#pragma once
#include "MyFuture.h"
#include <ace/Singleton.h>
#include <boost/any.hpp>

class MyFutureMap
{
public:

    MyFutureMap() {}
    static MyFutureMap& getInstance();

    template <typename T, ReturnTypeOfCreator r = PointerT>
    void add(const std::string& id, MyFuture<T, r>* future)
    {
        m_map[id] = boost::shared_ptr<MyFuture<T, r> >(future);
    }

    template <typename T, ReturnTypeOfCreator r = PointerT>
    void add(const std::string& id, boost::shared_ptr<MyFuture<T, r> > future)
    {
        m_map[id] = future;
    }

    template <typename T, ReturnTypeOfCreator r = PointerT>
    MyFuture<T, r>& get(const std::string& id)
    {
        return *(boost::any_cast<boost::shared_ptr<MyFuture<T, r> > >(m_map[id]));
    }

private:

    std::map<std::string, boost::any> m_map;

private:

    friend class ACE_Singleton<MyFutureMap, ACE_Null_Mutex>;
};
