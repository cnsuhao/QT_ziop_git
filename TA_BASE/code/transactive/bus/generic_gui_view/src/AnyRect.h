#include "bus/generic_gui_pmod/src/TA_Rect.h"
#include <boost/operators.hpp>
#include <QRect>
#define MFC_MEAN_AND_LEAN
#include <windows.h>

namespace TA_Base_Bus
{
    struct AnyRect
        : TA_Rect,
          boost::equality_comparable<AnyRect>,
          boost::less_than_comparable<AnyRect>
    {
        struct Detail
        {
            struct Impl
            {
                struct WinRectTag {};
                struct QtRectTag {};
                template <typename T> struct RectTraits;
                template <> struct RectTraits<TA_Rect>  { typedef WinRectTag RectCategory; };
                template <> struct RectTraits<RECT>     { typedef WinRectTag RectCategory; };
                template <> struct RectTraits<QRect>    { typedef QtRectTag RectCategory; };

                template <typename T> static AnyRect fromRectImpl(const T& r, WinRectTag)
                {
                    return AnyRect(r.left, r.top, r.right, r.bottom);
                }

                template <typename T> static AnyRect fromRectImpl(const T& r, QtRectTag)
                {
                    return AnyRect(r.left(), r.top(), r.right(), r.bottom());
                }

                template <typename T> static T toRectImpl(const AnyRect& r, WinRectTag)
                {
                    return T{ r.left, r.top, r.right, r.bottom };
                }

                template <typename T> static T toRectImpl(const AnyRect& r, QtRectTag)
                {
                    return T(r.left, r.top, r.width_qt(), r.height_qt());
                }

                template <typename T> static void assign(AnyRect& r, const T& rhs, WinRectTag)
                {
                    r.left = rhs.left;
                    r.top = rhs.top;
                    r.right = rhs.right;
                    r.bottom = rhs.bottom;
                }

                template <typename T> static void assign(AnyRect& r, const T& rhs, QtRectTag)
                {
                    r.left = rhs.left();
                    r.top = rhs.top();
                    r.right = rhs.right();
                    r.bottom = rhs.bottom();
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

            template <typename T> static void assign(AnyRect& r, const T& rhs)
            {
                Impl::assign(r, rhs, typename Impl::RectTraits<T>::RectCategory());
            }
        };

        AnyRect(long l = -1, long t = -1, long r = -1, long b = -1)
            : TA_Rect(l, t, r, b)
        {}

        template <typename T>
        AnyRect(const T& rhs)
        {
            Detail::assign(*this, rhs);
        }

        template <typename T>
        operator T() const
        {
            return Detail::toRect<T>(*this);
        }

        template <typename T>
        AnyRect& operator = (const T& rhs)
        {
            Detail::assign(*this, rhs);
            return *this;
        }

        friend bool operator == (const AnyRect& lhs, const AnyRect& rhs)
        {
            return (lhs.empty() && rhs.empty()) ||
                   (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom);
        }

        friend bool operator < (const AnyRect& lhs, const AnyRect& rhs)
        {
            return (lhs.left < rhs.left) ||
                   (lhs.left == rhs.left && lhs.top < rhs.top) ||
                   (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right < rhs.right) ||
                   (lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom < rhs.bottom);
        }

        template <typename T>
        bool operator == (const T& rhs) const
        {
            return *this == static_cast<AnyRect>(rhs);
        }

        template <typename T>
        bool operator < (const T& rhs) const
        {
            return *this < static_cast<AnyRect>(rhs);
        }

        static const AnyRect DEFAULT;
    };
}
