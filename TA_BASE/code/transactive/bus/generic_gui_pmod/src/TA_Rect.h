#pragma once

namespace TA_Base_Bus
{
    /**
      * Wrapper platform simple data types, which related with UI Widgets.
      * So far, only valid for window platform
      */
    struct TA_Rect
    {
        long left;
        long top;
        long right;
        long bottom;

        TA_Rect(long l = -1, long t = -1, long r = -1, long b = -1) : left(l), top(t), right(r), bottom(b) {}

        long width() const { return right - left; }
        long height() const { return bottom - top; }
        long width_qt() const { return width() + 1; }
        long height_qt() const { return height() + 1; }

        bool operator == (const TA_Rect& rhs) const { return equal(rhs); }
        bool operator != (const TA_Rect& rhs) const { return !equal(rhs); }
        bool equal(const TA_Rect& rhs) const { return (empty() && rhs.empty()) || (rhs.left == left && rhs.top == top && rhs.right == right && rhs.bottom == bottom); }
        bool empty() const { return (0 == left && 0 == top && 0 == right && 0 == bottom) || (-1 == left && -1 == top && -1 == right && -1 == bottom); }

        template<typename T> T toRect() const { T o; o.left = left; o.top = top; o.right = right; o.bottom = bottom; return o; }
        template<typename T> T toQRect() const { return T(left, top, width_qt(), height_qt()); }

        template<typename T> static TA_Rect fromRect(const T& x) { return TA_Rect(x.left, x.top, x.right, x.bottom); }
        template<typename T> static TA_Rect fromQRect(const T& q) { return TA_Rect(q.left(), q.top(), q.right(), q.bottom()); }
        template<typename T> static T toRect(const TA_Rect& r) { T o; o.left = r.left; o.top = r.top; o.right = r.right; o.bottom = r.bottom; return o; }
        template<typename T> static T toQRect(const TA_Rect& r) { return T(r.left, r.top, r.width_qt(), r.height_qt()); }
    };
}
