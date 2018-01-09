/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/trends/src/ItemInfoDetails.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
 
#ifndef ItemInfoDetails
#define ItemInfoDetails

      
namespace TA_Base_Core
{
	typedef struct
    {
        int index;				    // Index of the item
        //COLORREF colour;		    // Colour of the item
        std::string colour;         // Hex string representing colour
        std::string displayName;    // Display name of the item
    } ItemInfo;
}


#endif //ItemInfoDetails
