#pragma once
#include <boost/function.hpp>

struct Predicate0
{
    typedef boost::function<bool()> F;
    F f;
    Predicate0(F f_ = F()) : f(f_) {}
    bool operator()() const {
        return f();
    }
};

template<typename T1>
struct Predicate1
{
    typedef boost::function<bool(T1)> F;
    F f;
    Predicate1(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1) const {
        return f(p1);
    }
};

template<typename T1, typename T2>
struct Predicate2
{
    typedef boost::function<bool(T1, T2)> F;
    F f;
    Predicate2(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2) const {
        return f(p1, p2);
    }
};

template<typename T1, typename T2, typename T3>
struct Predicate3
{
    typedef boost::function<bool(T1, T2, T3)> F;
    F f;
    Predicate3(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3) const {
        return f(p1, p2, p3);
    }
};

template<typename T1, typename T2, typename T3, typename T4>
struct Predicate4
{
    typedef boost::function<bool(T1, T2, T3, T4)> F;
    F f;
    Predicate4(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4) const {
        return f(p1, p2, p3, p4);
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct Predicate5
{
    typedef boost::function<bool(T1, T2, T3, T4, T5)> F;
    F f;
    Predicate5(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) const {
        return f(p1, p2, p3, p4, p5);
    }
};

/////

struct Not0
{
    typedef boost::function<bool()> F;
    F f;
    Not0(F f_ = F()) : f(f_) {}
    bool operator()() const {
        return !f();
    }
};

template<typename T1>
struct Not1
{
    typedef boost::function<bool(T1)> F;
    F f;
    Not1(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1) const {
        return !f(p1);
    }
};

template<typename T1, typename T2>
struct Not2
{
    typedef boost::function<bool(T1, T2)> F;
    F f;
    Not2(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2) const {
        return !f(p1, p2);
    }
};

template<typename T1, typename T2, typename T3>
struct Not3
{
    typedef boost::function<bool(T1, T2, T3)> F;
    F f;
    Not3(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3) const {
        return !f(p1, p2, p3);
    }
};

template<typename T1, typename T2, typename T3, typename T4>
struct Not4
{
    typedef boost::function<bool(T1, T2, T3, T4)> F;
    F f;
    Not4(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4) const {
        return !f(p1, p2, p3, p4);
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct Not5
{
    typedef boost::function<bool(T1, T2, T3, T4, T5)> F;
    F f;
    Not5(F f_ = F()) : f(f_) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) const {
        return !f(p1, p2, p3, p4, p5);
    }
};

/////

struct And0
{
    typedef boost::function<bool()> F;
    F lhs;
    F rhs;
    And0(F l, F r) : lhs(l), rhs(r) {}
    bool operator()() const {
        return lhs() && rhs();
    }
};

template<typename T1>
struct And1
{
    typedef boost::function<bool(T1)> F;
    F lhs;
    F rhs;
    And1(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1) const {
        return lhs(p1) && rhs(p1);
    }
};

template<typename T1, typename T2>
struct And2
{
    typedef boost::function<bool(T1, T2)> F;
    F lhs;
    F rhs;
    And2(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2) const {
        return lhs(p1, p2) && rhs(p1, p2);
    }
};

template<typename T1, typename T2, typename T3>
struct And3
{
    typedef boost::function<bool(T1, T2, T3)> F;
    F lhs;
    F rhs;
    And3(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3) const {
        return lhs(p1, p2, p3) && rhs(p1, p2, p3);
    }
};

template<typename T1, typename T2, typename T3, typename T4>
struct And4
{
    typedef boost::function<bool(T1, T2, T3, T4)> F;
    F lhs;
    F rhs;
    And4(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4) const {
        return lhs(p1, p2, p3, p4) && rhs(p1, p2, p3, p4);
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct And5
{
    typedef boost::function<bool(T1, T2, T3, T4, T5)> F;
    F lhs;
    F rhs;
    And5(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) const {
        return lhs(p1, p2, p3, p4, p5) && rhs(p1, p2, p3, p4, p5);
    }
};

/////

struct Or0
{
    typedef boost::function<bool()> F;
    F lhs;
    F rhs;
    Or0(F l, F r) : lhs(l), rhs(r) {}
    bool operator()() const {
        return lhs() || rhs();
    }
};

template<typename T1>
struct Or1
{
    typedef boost::function<bool(T1)> F;
    F lhs;
    F rhs;
    Or1(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1) const {
        return lhs(p1) || rhs(p1);
    }
};

template<typename T1, typename T2>
struct Or2
{
    typedef boost::function<bool(T1, T2)> F;
    F lhs;
    F rhs;
    Or2(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2) const {
        return lhs(p1, p2) || rhs(p1, p2);
    }
};

template<typename T1, typename T2, typename T3>
struct Or3
{
    typedef boost::function<bool(T1, T2, T3)> F;
    F lhs;
    F rhs;
    Or3(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3) const {
        return lhs(p1, p2, p3) || rhs(p1, p2, p3);
    }
};

template<typename T1, typename T2, typename T3, typename T4>
struct Or4
{
    typedef boost::function<bool(T1, T2, T3, T4)> F;
    F lhs;
    F rhs;
    Or4(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4) const {
        return lhs(p1, p2, p3, p4) || rhs(p1, p2, p3, p4);
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct Or5
{
    typedef boost::function<bool(T1, T2, T3, T4, T5)> F;
    F lhs;
    F rhs;
    Or5(F l, F r) : lhs(l), rhs(r) {}
    bool operator()(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) const {
        return lhs(p1, p2, p3, p4, p5) || rhs(p1, p2, p3, p4, p5);
    }
};

inline Predicate0 operator !(const Predicate0& pred)
{
    return Predicate0(Not0(pred));
}
template<typename T1>
Predicate1<T1> operator !(const Predicate1<T1>& pred)
{
    return Predicate1<T1>(Not1<T1>(pred));
}
template<typename T1, typename T2>
Predicate2<T1, T2> operator !(const Predicate2<T1, T2>& pred)
{
    return Predicate2<T1, T2>(Not2<T1, T2>(pred));
}
template<typename T1, typename T2, typename T3>
Predicate3<T1, T2, T3> operator !(const Predicate3<T1, T2, T3>& pred)
{
    return Predicate3<T1, T2, T3>(Not3<T1, T2, T3>(pred));
}
template<typename T1, typename T2, typename T3, typename T4>
Predicate4<T1, T2, T3, T4> operator !(const Predicate4<T1, T2, T3, T4>& pred)
{
    return Predicate4<T1, T2, T3, T4>(Not4<T1, T2, T3, T4>(pred));
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
Predicate5<T1, T2, T3, T4, T5> operator !(const Predicate5<T1, T2, T3, T4, T5>& pred)
{
    return Predicate5<T1, T2, T3, T4, T5>(Not5<T1, T2, T3, T4, T5>(pred));
}

inline Predicate0 operator && (const Predicate0& lhs, const Predicate0& rhs)
{
    return Predicate0(And0(lhs, rhs));
}
template<typename T1>
Predicate1<T1> operator &&(const Predicate1<T1>& lhs, const Predicate1<T1>& rhs)
{
    return Predicate1<T1>(And1<T1>(lhs, rhs));
}
template<typename T1, typename T2>
Predicate2<T1, T2> operator &&(const Predicate2<T1, T2>& lhs, const Predicate2<T1, T2>& rhs)
{
    return Predicate2<T1, T2>(And2<T1, T2>(lhs, rhs));
}
template<typename T1, typename T2, typename T3>
Predicate3<T1, T2, T3> operator &&(const Predicate3<T1, T2, T3>& lhs, const Predicate3<T1, T2, T3>& rhs)
{
    return Predicate3<T1, T2, T3>(And3<T1, T2, T3>(lhs, rhs));
}
template<typename T1, typename T2, typename T3, typename T4>
Predicate4<T1, T2, T3, T4> operator &&(const Predicate4<T1, T2, T3, T4>& lhs, const Predicate4<T1, T2, T3, T4>& rhs)
{
    return Predicate4<T1, T2, T3, T4>(And4<T1, T2, T3, T4>(lhs, rhs));
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
Predicate5<T1, T2, T3, T4, T5> operator &&(const Predicate5<T1, T2, T3, T4, T5>& lhs, const Predicate5<T1, T2, T3, T4, T5>& rhs)
{
    return Predicate5<T1, T2, T3, T4, T5>(And5<T1, T2, T3, T4, T5>(lhs, rhs));
}

inline Predicate0 operator || (const Predicate0& lhs, const Predicate0& rhs)
{
    return Predicate0(Or0(lhs, rhs));
}
template<typename T1>
Predicate1<T1> operator ||(const Predicate1<T1>& lhs, const Predicate1<T1>& rhs)
{
    return Predicate1<T1>(Or1<T1>(lhs, rhs));
}
template<typename T1, typename T2>
Predicate2<T1, T2> operator ||(const Predicate2<T1, T2>& lhs, const Predicate2<T1, T2>& rhs)
{
    return Predicate2<T1, T2>(Or2<T1, T2>(lhs, rhs));
}
template<typename T1, typename T2, typename T3>
Predicate3<T1, T2, T3> operator ||(const Predicate3<T1, T2, T3>& lhs, const Predicate3<T1, T2, T3>& rhs)
{
    return Predicate3<T1, T2, T3>(Or3<T1, T2, T3>(lhs, rhs));
}
template<typename T1, typename T2, typename T3, typename T4>
Predicate4<T1, T2, T3, T4> operator ||(const Predicate4<T1, T2, T3, T4>& lhs, const Predicate4<T1, T2, T3, T4>& rhs)
{
    return Predicate4<T1, T2, T3, T4>(Or4<T1, T2, T3, T4>(lhs, rhs));
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
Predicate5<T1, T2, T3, T4, T5> operator ||(const Predicate5<T1, T2, T3, T4, T5>& lhs, const Predicate5<T1, T2, T3, T4, T5>& rhs)
{
    return Predicate5<T1, T2, T3, T4, T5>(Or5<T1, T2, T3, T4, T5>(lhs, rhs));
}
