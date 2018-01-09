/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui/src/PModelDataType.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  */

#ifndef PMODEL_DATA_TYPE_H__INCLUDE_
#define PMODEL_DATA_TYPE_H__INCLUDE_


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

        TA_Rect( long l = -1, long t = -1, long r = -1, long b = -1 ) : left(l), top(t), right(r), bottom(b) {}

        long width() const { return right - left; }
        long height() const { return bottom - top; }
        long width_qt() const { return width() + 1; }
        long height_qt() const { return height() + 1; }

        bool operator == ( const TA_Rect& rhs ) const { return equal(rhs); }
        bool operator != ( const TA_Rect& rhs ) const { return ! equal(rhs); }
        bool equal( const TA_Rect& rhs ) const { return ( empty() && rhs.empty() ) || ( rhs.left == left && rhs.top == top && rhs.right == right && rhs.bottom == bottom ); }
        bool empty() const { return ( 0 == left && 0 == top && 0 == right && 0 == bottom ) || ( -1 == left && -1 == top && -1 == right && -1 == bottom ); }

        template<typename T> T toRect() const { T o; o.left = left; o.top = top; o.right = right; o.bottom = bottom; return o; }
        template<typename T> T toQRect() const { return T(left, top, width_qt(), height_qt()); }

        template<typename T> static TA_Rect fromRect( const T& x ) { return TA_Rect( x.left, x.top, x.right, x.bottom ); }
        template<typename T> static TA_Rect fromQRect( const T& q ) { return TA_Rect( q.left(), q.top(), q.right(), q.bottom() ); }
        template<typename T> static T toRect( const TA_Rect& r ) { T o; o.left = r.left; o.top = r.top; o.right = r.right; o.bottom = r.bottom; return o; }
        template<typename T> static T toQRect( const TA_Rect& r ) { return T( r.left, r.top, r.width_qt(), r.height_qt() ); }
    };

    /**
      * Window position info. Including align flag, position flag, boundary dimension, etc
      *
      */
    struct WindowPositionInfo
    {
        unsigned long posFlag;      ///< Contains the positioning flags
        unsigned long alignFlag;    ///< Contains the alignment flags
        TA_Rect boundaryDim;        ///< The dimension of the boundary to bind the application within
        TA_Rect objectDim;          ///< The dimension of the object
        TA_Rect appDim;             ///< The calculated dimension of the object
    };

    /**
      * Control the view resizing property. For views that need control its position and size, mainly for main view.
      *
      */
    struct ViewResizeProperty
    {
        bool canMaximise; ///< can maximize or not
        long maxWidth;    ///< max width
        long maxHeight;   ///< max height
        long minWidth;    ///< min width
        long minHeight;   ///< min height

        ViewResizeProperty( bool cmm = true, long maxw = -1, long maxh = -1, long minw = -1, long minh = -1 )
            : canMaximise( cmm ),
              maxWidth( maxw ),
              maxHeight( maxh ),
              minWidth( minw ),
              minHeight( minh )
        {
        }
    };

    inline void  TA_SetRectEmpty( TA_Rect& rect )
    {
        rect.left = rect.top = rect.right = rect.bottom = 0;
    };

    inline bool TA_EqualRECT( const TA_Rect& lhs, const TA_Rect& rhs )
    {
        return lhs == rhs;
    };

    inline bool TA_IsRectEmpty(const TA_Rect& r)
    {
        return r.empty();
    };

    /**
      * GUI Application classification based on main view type. So far support Dialog,Frame and PropertSheet based,
      * SDI and MDL both belongs to Frame based application
      *
      */
    enum MainViewType { MVT_NONE, MVT_DIALOG, MVT_PROPERTY_SHEET, MVT_SDI_FRAME, MVT_MDI_FRAME, MVT_NET_FORM };

    /**
      * base class for event Notification argument
      * So far, we only support simple synchronous event, and sender is responsible to free the argument
      */
    class EventArgs
    {
    public:

        /**
          * Constructor function
          *
          */
        EventArgs(int typeCode = 0)
            : m_typeCode(typeCode)
        {
        }

        /**
          * Destructor function
          *
          */
        virtual ~EventArgs()
        {
        }

        /// get type code
        int getTypeCode()
        {
            return m_typeCode;
        }
        //const static EventArgs * Empty;

    protected:

        int m_typeCode;  ///< event type code
    };

    //UI reply IDs
    enum UIReplyType
    {
        UIR_IDNONE = 0,
        UIR_IDOK  =  1,
        UIR_IDCANCEL = 2,
        UIR_IDABORT = 3,
        UIR_IDRETRY = 4,
        UIR_IDIGNORE = 5,
        UIR_IDYES = 6,
        UIR_IDNO = 7,
        UIR_IDCLOSE = 8,
        UIR_IDHELP = 9,
        UIR_IDTRYAGAIN = 10,
        UIR_IDCONTINUE = 11,
        UIR_IDTIMEOUT = 12
    };
}


#endif //TABUS_TAGENERICGUI_RESIZINGPROPERTIES
