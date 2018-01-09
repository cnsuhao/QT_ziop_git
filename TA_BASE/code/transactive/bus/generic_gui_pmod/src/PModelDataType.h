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

#include "TA_Rect.h"

namespace TA_Base_Bus
{
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

        ViewResizeProperty(bool cmm = true, long maxw = -1, long maxh = -1, long minw = -1, long minh = -1)
            : canMaximise(cmm),
              maxWidth(maxw),
              maxHeight(maxh),
              minWidth(minw),
              minHeight(minh)
        {
        }
    };

    inline void  TA_SetRectEmpty(TA_Rect& rect)
    {
        rect.left = rect.top = rect.right = rect.bottom = 0;
    };

    inline bool TA_EqualRECT(const TA_Rect& lhs, const TA_Rect& rhs)
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
