#if !defined(AFX_EQUIPMENTSTATUSLISTITEM_H__15E3D0CC_4D86_426A_B4EA_C0D5FFE9F063__INCLUDED_)
#define AFX_EQUIPMENTSTATUSLISTITEM_H__15E3D0CC_4D86_426A_B4EA_C0D5FFE9F063__INCLUDED_

/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusListItem.h $
 * @author:  Alan Brims
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * An item to be displayed in (or filtered from) the EquipmentStatusListCtrl.
 * This class is extended to implement a local cache of datanode and datapoint proxies.
 *
 */

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/common_library/src/CommonDefs.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include <map>

namespace TA_Base_Core
{
    // forward declaration
	class IEntityData;
}

namespace TA_Base_App
{  
	class EquipmentItemPopulateLogic;

    class EquipmentStatusListItem : public TA_Base_Bus::IEntityUpdateEventProcessor
    {
    public:

        /** 
          * EquipmentStatusListItem
          *
          * Construct the ESLItem for this entity using its corbaname.
          * This will allocate the appropriate proxy to manage config updates.
          *
          * @param entitykey     The key of entity to examine.
		  * @param populateLogic call back interface when item update
          */
		EquipmentStatusListItem(unsigned long entitykey, EquipmentItemPopulateLogic* populateLogic);

        /** 
          * ~EquipmentStatusListItem
          *
          * Destructor.
          * This does not have to do much since the proxies are kept through smart (auto) pointers
          * and will be freed when the ESLItem is deallocated.
          */
        virtual ~EquipmentStatusListItem();

        /** 
          * getItemType
          *
          * The SCADA level of the item.
          * This pure function makes EquipmentStatusListItem abstract.
          *
          * @return     The type of the derived ESL Item.
          *             Currently only DATANODE, DATAPOINT and DATAPOINT_SUMMARY are available.
          */
        virtual Node_Item getItemType() const = 0;

         /** 
          * compare
          *
          * Standard comparison function to be used by the sort routines.
          *
          * @param &rhs     The other ESLItem to compare against.
          * @param column   Compare the two ESLItems using values in this column.
          *
          * @return         -1 if this < rhs
          *                  0 if equal
          *                 +1 if this > rhs
          *
          */
        virtual int compare( const EquipmentStatusListItem &rhs, TA_Base_App::EslColumns column) const;

        /** 
          * processEntityUpdateEvent
          *
          * Required by IEntityUpdateEventProcessor.
          *
          * @param entityKey    Who the update is for; probably will be this->m_corbaName.getEntityKey()
          *                     or its parent entity id.
          * @param updateType   What type of update (see bus/scada/proxies2/ProxyDefs.h)
          *
          */
        virtual void processEntityUpdateEvent(unsigned long entityKey, TA_Base_Bus::ScadaEntityUpdateType updateType);

        /** 
          * getColumnText
          *
          * Read-only accessor for m_text[]
          *
          * @param column   Which column to return the text for.
          *
          * @return         The text in the requested column for this ESLItem
          */
		std::string const& getColumnText( TA_Base_App::EslColumns column) const;

        /** 
          * getColumnIcon
          *
          * Read-only accessor for m_icon[]
          *
          * @param column   Which column to return the icon for.
          *
          * @return         The imagelist index of the icon in the requested column for this ESLItem.
          */
        int const& getColumnIcon( TA_Base_App::EslColumns column) const;

        /** 
          * getEntityData
          *
          * Read-only accessor for this entity's EntityData
          *
          * @return     This ESLItem's corba name
          */
        unsigned long getEntityKey() const;

        /** 
          * setReportItemKey
          *
          * Write-accessor for this report item's key
          * Currently this key is used only by Configuration Dialog
          *
          * @param key    The key of the report item (not the entity it reports on).
          */
        void setReportItemKey( std::string& key);

        /** 
          * getReportItemKey
          *
          * Read-only accessor for this report item's key
          * (note: this is not the report key but the key of the item within the report)
          *
          * @return     Key of this item in the equipment report entities table
          */
        std::string getReportItemKey();
        /** 
          * isToBeDeletedUpdateInList
          *
          * Is this item to be removed entirely (from list control and proxy list) because it
          * is not for the current report? (eg. it is the wrong scada level, neither DATANODE nor DATAPOINT)
          *
          * @return bool    True:  the item is to be removed
          *                 False: the item is to be displayed (or hidden depending on filters)
          */
        bool isToBeDeleted() const;

        /** 
          * checkFilters
          *
          * Evaluate this ESL Item against the parameter filter set and set the "is Filtered" 
          * flag accordingly. If the item "is Filtered" then it will not be displayed.
          *
          * @param filters  vector of FilterElements    On-off flags and the strings they enable.
          *
          */
        virtual bool checkFilters( FilterFlagsAndStrings const& filters);

		TA_Base_Core::ReEntrantThreadLockable   m_lock;

        /** 
          * getItemDataNodeEntityKey
          *
          * Read-only accessor for this entity's EntityData
          *
          * @return     This ESLItem's corba name
          */
        virtual unsigned long getItemDataNodeEntityKey();

        /** 
          * getItemDataNodeEntityName
          *
          * Read-only accessor for this entity's EntityData
          *
          * @return     This ESLItem's corba name
          */
        virtual std::string getItemDataNodeEntityName();

    protected:
        /** 
          * buildNewColumnText
          *
          * Get the new text for the requested column. Derived classes should override
          * this member to provide entity-type specialisations for the relevant columns.
          * For example, the datapoint description column will be filled by the DataPoint
          * derivation but left blank by the DataNode derivation.
          *
          * This is where the proxy is queried and ScadaProxyExceptions are caught, so
          * this function will always return a value even if it is blank.
          * The value returned from this function is compared against the stored value
          * to determine if the column needs updating and therefore whether to post
          * an update to the user inferface thread.
          *
          * @param  column      Which column to get the text for.
          *
          * @return std::string   The new text
          */
		virtual std::string buildNewColumnText( TA_Base_App::EslColumns column );

        /** 
          * buildNewColumnIcon
          *
          * Get the new icon for the requested column. Derived classes should override
          * this member to provide entity-type specialisations for the relevant columns.
          * At present the only icon is for datapoint quality display in the value column. 
          *
          * @param  column      Which column to get the icon for.
          *
          * @return std::string     The index number of the new icon (see CEquipmentStatusListCtrl::m_imageList)
          */
        virtual int     buildNewColumnIcon( TA_Base_App::EslColumns column );

        /** 
          * updateTextIfChanged
          *
          * Compares the string parameter to the current value for this column and updates
          * the column if different. Also sets the relevant bit in m_columnUpdateFlags to
          * indicate that column has changed. If m_columnUpdateFlags is not blank after 
          * calling updateTextIfChanged() on all columns then post an update to the user
          * interface to refresh the list control display.
          *
          * @param column   The column to check the new string against.
          * @param newText  The new value for this column.
          *
          * @return         True if the column text was updated.
          *
          */
		virtual bool updateTextIfChanged( EslColumns column, std::string& newText);

        /** 
          * updateIconIfChanged
          *
          * Compares the icon-index parameter to the current value for this column and updates
          * the column if different. Also sets the relevant bit in m_columnUpdateFlags to
          * indicate that column has changed. If m_columnUpdateFlags is not blank after 
          * calling updateIconIfChanged() on all columns then post an update to the user
          * interface to refresh the list control display.
          *
          * @param column   The column to check the new string against.
          * @param newIcon  The new icon for this column.
          *
          * @return         True if the icon was updated.
          *
          */
        bool updateIconIfChanged( EslColumns column, int newIcon);

		std::string getInfoFromSession(const std::string& session);
		/** 
		 * This item's entitykey
		 */
		unsigned long           m_entityKey;
		/** 
		 * Bitflag to indicate which columns have had their text or icon changed.
		 */
        unsigned long           m_columnUpdateFlags;

		/** 
		 * Flag to indicate how to handle this item. Mainly used to delete the item
		 * from its container when update type is DELETE_ITEM.
		 */
        ItemUpdateType  m_updateType;

        /** 
		 * DataNode or DataPoint (these maintain live proxies), 
		 * or DataNode Summary (this one kills its proxy when loading is complete)
		 */
        Node_Item       m_itemType;             //  DATANODE, DATAPOINT or DATANODE_SUMMARY

        /** 
		 * The time value in the TIMESTAMP column for sort-comparison 
		 * (the displayed timestamp in m_text[TIMESTAMP] is in dd/mm/yy format)
		 */
        unsigned long   m_timeStamp;

		bool			m_updateAllCol;     
		int			m_icon[ESL_COLUMN_MAX];
		std::string m_text[ESL_COLUMN_MAX];
		
		static std::map<unsigned long, std::string> m_consoles;
		static std::map<unsigned long, std::string> m_profiles;
		static std::map<unsigned long, std::string> m_operators;
    private:
        //
        // Copy constructor and assignment operator are not used
        //
    //    EquipmentStatusListItem( const EquipmentStatusListItem& theSource) {};
		EquipmentStatusListItem& operator=(const EquipmentStatusListItem &) {};

		
		void initCache();
        /** 
		 * Key of this item's row in the SC_EQUIP_REPORT_ENTITIES table
		 * Currently used only in ConfigurationDialog.
		 */
        std::string											m_reportItemKey;
		static bool											m_initProfileConsole;
		EquipmentItemPopulateLogic*							m_populateLogic;							
		static TA_Base_Core::ReEntrantThreadLockable		m_initLock;
		static std::map<std::string, std::string>			m_sessionInfoCache;
		static TA_Base_Core::ReEntrantThreadLockable		m_sessionInfoCacheLock;
    };

	typedef boost::shared_ptr<EquipmentStatusListItem>   EquipmentStatusListItemSharedPtr;
	typedef std::map < unsigned long, EquipmentStatusListItemSharedPtr> EquipmentStatusListItemMap;
} // TA_Base_App


#endif // !defined(AFX_EQUIPMENTSTATUSLISTITEM_H__15E3D0CC_4D86_426A_B4EA_C0D5FFE9F063__INCLUDED_)
