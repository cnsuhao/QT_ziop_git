/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/data_access_interface/interlocking_entity/src/IInterlockEntity.h $
  * @author:  Robert Stagg
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  */

#ifndef I_INTERLOCK_ENTITY_H
#define I_INTERLOCK_ENTITY_H

#include <string>
#include <vector>

#include "core/data_access_interface/src/IItem.h"

namespace TA_Base_Core
{
    /**
     * the public interface to this package.
     */
    class IInterlockEntity //: public virtual IItem
    {

    public:


        /**
          * getName
          *
          * Returns the name of the DLL that contains the interlocking support code for this entity type.
          *
          * @return The name of a dll
          *
          * @exception DatabaseException A DatabaseException will be thrown if there is a
          *            problem establishing a connection with the database, or if an
          *            error is encountered while retrieving data.
          * @exception DataException A DataException will be thrown if the data cannot be 
          *            converted to the required format, or if the wrong ammount of data is retrieved.
          */
        virtual std::string getDllName() = 0;


        virtual ~IInterlockEntity() {};
    };
} //close namespace TA_Base_Core

#endif // !defined(IInterlockEntity_H)
