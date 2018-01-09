#pragma once
#include <boost/foreach.hpp>

template <typename C, typename F>
void foreachCall(const C& container, F function)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)();
    }
}

template <typename C, typename F, typename P1>
void foreachCall(const C& container, F function, const P1& p1)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)(p1);
    }
}

template <typename C, typename F, typename P1, typename P2>
void foreachCall(const C& container, F function, const P1& p1, const P2& p2)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)(p1, p2);
    }
}

template <typename C, typename F, typename P1, typename P2, typename P3>
void foreachCall(const C& container, F function, const P1& p1, const P2& p2, const P3& p3)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)(p1, p2, p3);
    }
}

template <typename C, typename F, typename P1, typename P2, typename P3, typename P4>
void foreachCall(const C& container, F function, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)(p1, p2, p3, p4);
    }
}

template <typename C, typename F, typename P1, typename P2, typename P3, typename P4, typename P5>
void foreachCall(const C& container, F function, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
    BOOST_FOREACH(C::value_type ptr, container)
    {
        (ptr->*function)(p1, p2, p3, p4, p5);
    }
}
