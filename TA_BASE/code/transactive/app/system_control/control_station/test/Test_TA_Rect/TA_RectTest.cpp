#include "StdAfx.h"
#include <boost/test/unit_test.hpp>
#include <boost/operators.hpp>
#include <QRect>

struct tagRECT;
typedef struct tagRECT RECT;
class QRect;

#if 0
struct TA_Rect
    : boost::equality_comparable<TA_Rect>,
      boost::less_than_comparable<TA_Rect>
{
    struct Detail
    {
        template <typename T> struct IsWinRect
        {
            typedef enum { value = 0 };
        };

        template <> struct IsWinRect<RECT>
        {
            typedef enum { value = 1 };
        };

        template <typename T> struct IsQtRect
        {
            typedef enum { value = 0 };
        };

        template <> struct IsQtRect<QRect>
        {
            typedef enum { value = 1 };
        };

        template <typename T, bool IsWinRect, bool IsQtRect> struct AnyRect;

        template <typename T> struct AnyRect<T, true, false> //RECT
        {
            static TA_Rect from(const T& r)
            {
                return TA_Rect(r.left, r.top, r.right, r.bottom);
            }

            static T to(const TA_Rect& r)
            {
                return T{ r.left, r.top, r.right, r.bottom };
            }
        };

        template <typename T> struct AnyRect<T, false, true> //QRect
        {
            static TA_Rect from(const T& r)
            {
                return TA_Rect(r.left(), r.top(), r.right(), r.bottom());
            }

            static T to(const TA_Rect& r)
            {
                return T(r.left, r.top, r.width_qt(), r.height_qt());
            }
        };

        template <typename T>
        static TA_Rect fromRect(const T& r)
        {
            return AnyRect<T, IsWinRect<T>::value, IsQtRect<T>::value>::from(r);
        }

        template <typename T>
        static T toRect(const TA_Rect& r)
        {
            return AnyRect<T, IsWinRect<T>::value, IsQtRect<T>::value>::to(r);
        }
    };

    long left;
    long top;
    long right;
    long bottom;

    TA_Rect(long l = -1, long t = -1, long r = -1, long b = -1) : left(l), top(t), right(r), bottom(b) {}

    long width() const { return right - left; }
    long height() const { return bottom - top; }
    long width_qt() const { return width() + 1; }
    long height_qt() const { return height() + 1; }

    bool empty() const { return (0 == left && 0 == top && 0 == right && 0 == bottom) || (-1 == left && -1 == top && -1 == right && -1 == bottom); }
    bool equal(const TA_Rect& rhs) const { return *this == rhs; }

    //template<typename T> T toRect() const { T o; o.left = left; o.top = top; o.right = right; o.bottom = bottom; return o; }
    //template<typename T> T toQRect() const { return T(left, top, width_qt(), height_qt()); }

    //template<typename T> static TA_Rect fromRect(const T& x) { return TA_Rect(x.left, x.top, x.right, x.bottom); }
    //template<typename T> static TA_Rect fromQRect(const T& q) { return TA_Rect(q.left(), q.top(), q.right(), q.bottom()); }
    //template<typename T> static T toRect(const TA_Rect& r) { T o; o.left = r.left; o.top = r.top; o.right = r.right; o.bottom = r.bottom; return o; }
    //template<typename T> static T toQRect(const TA_Rect& r) { return T(r.left, r.top, r.width_qt(), r.height_qt()); }

    template <typename T>
    operator T()
    {
        return Detail::toRect<T>(*this);
    }

    template <typename T>
    TA_Rect(const T& r)
    {
        *this = Detail::fromRect(r);
    }

    template <typename T>
    TA_Rect& operator =(const T& r)
    {
        *this = Detail::fromRect(r);
        return *this;
    }

    friend bool operator == (const TA_Rect& lhs, const TA_Rect& rhs)
    {
        return (lhs.empty() && rhs.empty()) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom);
    }

    friend bool operator < (const TA_Rect& lhs, const TA_Rect& rhs)
    {
        return (lhs.left < rhs.left) ||
               (lhs.left == rhs.left && lhs.top < rhs.top) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right < rhs.right) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom < rhs.bottom);
    }

    template <typename T>
    bool operator ==(const T& rhs) const
    {
        return *this == static_cast<TA_Rect>(rhs);
    }

    template <typename T>
    bool operator <(const T& rhs) const
    {
        return *this < static_cast<TA_Rect>(rhs);
    }
};
#endif

struct TA_Rect
    : boost::equality_comparable<TA_Rect>,
      boost::less_than_comparable<TA_Rect>
{
    struct Detail
    {
        struct Impl
        {
            struct WinRectTag {};
            struct QtRectTag {};
            template <typename T> struct RectTraits;
            template <> struct RectTraits<RECT>     { typedef WinRectTag RectCategory; };
            template <> struct RectTraits<QRect>    { typedef QtRectTag RectCategory; };

            template <typename T> static TA_Rect fromRectImpl(const T& r, WinRectTag)
            {
                return TA_Rect(r.left, r.top, r.right, r.bottom);
            }

            template <typename T> static TA_Rect fromRectImpl(const T& r, QtRectTag)
            {
                return TA_Rect(r.left(), r.top(), r.right(), r.bottom());
            }

            template <typename T> static T toRectImpl(const TA_Rect& r, WinRectTag)
            {
                return T{ r.left, r.top, r.right, r.bottom };
            }

            template <typename T> static T toRectImpl(const TA_Rect& r, QtRectTag)
            {
                return T(r.left, r.top, r.width_qt(), r.height_qt());
            }
        };

        template <typename T> static TA_Rect fromRect(const T& r)
        {
            return Impl::fromRectImpl(r, typename Impl::RectTraits<T>::RectCategory());
        }

        template <typename T> static T toRect(const TA_Rect& r)
        {
            return Impl::toRectImpl<T>(r, typename Impl::RectTraits<T>::RectCategory());
        }
    };

    long left;
    long top;
    long right;
    long bottom;

    TA_Rect(long l = -1, long t = -1, long r = -1, long b = -1) : left(l), top(t), right(r), bottom(b) {}

    long width() const { return right - left; }
    long height() const { return bottom - top; }
    long width_qt() const { return width() + 1; }
    long height_qt() const { return height() + 1; }

    bool empty() const { return (0 == left && 0 == top && 0 == right && 0 == bottom) || (-1 == left && -1 == top && -1 == right && -1 == bottom); }
    bool equal(const TA_Rect& rhs) const { return *this == rhs; }

    template <typename T>
    operator T()
    {
        return Detail::toRect<T>(*this);
    }

    template <typename T>
    TA_Rect(const T& r)
    {
        *this = Detail::fromRect(r);
    }

    template <typename T>
    TA_Rect& operator =(const T& r)
    {
        *this = Detail::fromRect(r);
        return *this;
    }

    friend bool operator == (const TA_Rect& lhs, const TA_Rect& rhs)
    {
        return (lhs.empty() && rhs.empty()) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom);
    }

    friend bool operator < (const TA_Rect& lhs, const TA_Rect& rhs)
    {
        return (lhs.left < rhs.left) ||
               (lhs.left == rhs.left && lhs.top < rhs.top) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right < rhs.right) ||
               (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom < rhs.bottom);
    }

    template <typename T>
    bool operator ==(const T& rhs) const
    {
        return *this == static_cast<TA_Rect>(rhs);
    }

    template <typename T>
    bool operator <(const T& rhs) const
    {
        return *this < static_cast<TA_Rect>(rhs);
    }
};

BOOST_AUTO_TEST_SUITE(TA_Rect_Test)

BOOST_AUTO_TEST_CASE(test_1)
{
    struct Local
    {
        static void foo(const RECT& r)
        {
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }

        static void bar(const QRect& r)
        {
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }
    };

    TA_Rect r;
    RECT r1 = r;
    QRect r2 = r;
    r = r;
    r = r1;
    r = r2;
    Local::foo(r);
    Local::bar(r);
}

BOOST_AUTO_TEST_CASE(test_compare)
{
    TA_Rect r1, r2;
    QRect r3;
    RECT r4;
    r1 == r2; //r1 != r2;
    r1 == r3; //r3 == r1; // r1 != r3;
    r1 == r4; // r1 != r4;
    r4 == r1;
}

BOOST_AUTO_TEST_CASE(test_less_than)
{
    TA_Rect r1, r2;
    r1 < r2;
    r1 <= r2;
    r1 > r2;
    r1 >= r2;
}

BOOST_AUTO_TEST_SUITE_END()
