/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/DataNodeSummaryListItem.cpp $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * A specialised DataNode Summary item to be displayed in (or filtered from) the EquipmentStatusListCtrl.
  * This class inherits from the DataNodeListItem and extends that class by extracting summary info
  * for the datanode, and then discards the required datanode proxy once it has loaded so that it 
  * unsubscribes from update messages.
  *
  */

#include "stdafx.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/DataNodeSummaryListItem.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/exceptions/src/ScadaProxyException.h"

namespace TA_Base_App
{

    //////////////////////////////////////////////////////////////////////
    // Construction/Destruction
    //////////////////////////////////////////////////////////////////////


    DataNodeSummaryListItem::DataNodeSummaryListItem(unsigned long entitykey, TA_Base_Bus::ScadaProxyFactory& proxyFactory, 
													EquipmentItemPopulateLogic* populateLogic)
        : DataNodeListItem(entitykey, proxyFactory, populateLogic)
    {
    }

    DataNodeSummaryListItem::DataNodeSummaryListItem(boost::shared_ptr<TA_Base_Core::DataNodeEntityData> entityData, 
													TA_Base_Bus::ScadaProxyFactory& proxyFactory,
													EquipmentItemPopulateLogic* populateLogic)
        : DataNodeListItem(entityData, proxyFactory, populateLogic)
    {
    }

    DataNodeSummaryListItem::~DataNodeSummaryListItem()
    {
    }


    std::string DataNodeSummaryListItem::buildNewColumnText( TA_Base_App::EslColumns column )
    {
        //  Get the new text for the requested column. By calling this function 
        //  for each column we need only one try-catch(ScadaProxyException) block.
        std::string newText("");
        try
        {
            //  Requested column probably uses a field shared between DataNodes and DataPoints
            newText = DataNodeListItem::buildNewColumnText( column);
        }
        catch (TA_Base_Core::ScadaProxyException& )
        {
            //  The proxy data for this column is not ready yet
        }
        return newText;
    }


    void DataNodeSummaryListItem::processEntityUpdateEvent(unsigned long entityKey, TA_Base_Bus::ScadaEntityUpdateType updateType)
    {
        DataNodeListItem::processEntityUpdateEvent( entityKey, updateType);
    }

} // TA_Base_App
