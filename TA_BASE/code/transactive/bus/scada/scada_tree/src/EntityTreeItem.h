/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/scada_tree/src/EntityTreeItem.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
// EntityTreeItem.h : header file
//

#if !defined(ENTITYTREEITEM_H_INCLUDED)
#define ENTITYTREEITEM_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus\mfc_extensions\src\mfc_extensions.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

namespace TA_Base_Core
{
	class IEntityData;
}

namespace TA_Base_Bus
{
    class IEntityFactory;

    /////////////////////////////////////////////////////////////////////////////
    // CEntityTreeItem

    class CEntityTreeItem : public CObjectTreeItem
    {
	    public:

		    CEntityTreeItem(TA_Base_Core::IEntityData* entityData, IEntityFactory* entityRetrievalFactory,
				bool bExpandAllEntityTreeItems = true);
		    ~CEntityTreeItem();

		    TA_Base_Core::IEntityData* getEntityData() { return m_entityData;};

		    virtual bool createChildItems();
		    virtual const TCHAR * getItemLabelText();
		    virtual bool hasChildren();
		    virtual const std::string getKey();
		    virtual HICON getItemImage(bool itemExpanded);

            /**
             * getDisplayName
             *
             * This is used to retrieve the name to display in the tree. SCADA entity items can have long
             * names separated into parts using '.'s. We just want part of this name. This has been made public
             * so that other classes can use this if required. If the method used to parse the name changes then'
             * code only needs to be changed in a single place.
             *
             * @param TAUnicodeString - The full name to convert to a display name
             *
             * @return TAUnicodeString - The converted name to display in the tree
             */
            virtual TAUnicodeString getDisplayName(const TAUnicodeString& fullName);

	    private:

		    TAUnicodeString            m_entityLabel;
		    TA_Base_Core::IEntityData*  m_entityData;

            // For read-only versions of this tree we store if an item has children when this object is constructed.
            bool m_hasChildren;

			bool m_bExpandAllEntityTreeItems;
            // This is used to retrieve the entity information. If this is NULL then we use the
            // EntityAccessFactory instead and must delete our own pointers. If this is not NULL then we are
            // not in charge of pointers and we must not delete them.
            IEntityFactory*        m_entityRetrievalFactory;
    };

} // namespace TA_Base_Bus

#endif // !defined(ENTITYTREEITEM_H_INCLUDED)
