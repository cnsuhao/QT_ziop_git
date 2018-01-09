/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusListItem.cpp $
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

#include "stdafx.h"

#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemPopulateLogic.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusListItem.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/ScadaProxyException.h"
#include "core/exceptions/src/DataException.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/src/ISession.h"
#include "core/data_access_interface/src/SessionAccessFactory.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"


namespace TA_Base_App
{
	const std::string NotLoaded ( "StringNotLoaded" );
	std::map<unsigned long, std::string> EquipmentStatusListItem::m_consoles;
	std::map<unsigned long, std::string> EquipmentStatusListItem::m_profiles;
	std::map<unsigned long, std::string> EquipmentStatusListItem::m_operators;
	bool EquipmentStatusListItem::m_initProfileConsole = false;
	TA_Base_Core::ReEntrantThreadLockable  EquipmentStatusListItem::m_initLock;
	std::map<std::string, std::string>	EquipmentStatusListItem::m_sessionInfoCache;
	TA_Base_Core::ReEntrantThreadLockable  EquipmentStatusListItem::m_sessionInfoCacheLock;
    //////////////////////////////////////////////////////////////////////
    // Construction/Destruction
    //////////////////////////////////////////////////////////////////////

    EquipmentStatusListItem::EquipmentStatusListItem(unsigned long entitykey, EquipmentItemPopulateLogic* populateLogic)
        : m_entityKey( entitykey)
        , m_columnUpdateFlags( 0)
		, m_updateAllCol(true)
        , m_updateType( ADD_MODIFY_ITEM)
        , m_reportItemKey("")
		, m_populateLogic(populateLogic)
    {
		std::string testT;
        for (int nCol = ESL_COLUMN_MIN; nCol < ESL_COLUMN_MAX; nCol++)
        {
            m_icon[ nCol] = -1;
			m_text[ nCol] = "";
        }
		initCache();
    }

    EquipmentStatusListItem::~EquipmentStatusListItem()
    {
		
    }

    unsigned long EquipmentStatusListItem::getEntityKey() const
    {
        return m_entityKey;
    }

    std::string EquipmentStatusListItem::getReportItemKey()     {
        return m_reportItemKey;
    }

    void EquipmentStatusListItem::setReportItemKey(std::string& key)
    {
        m_reportItemKey = key;
    }

    void EquipmentStatusListItem::processEntityUpdateEvent(unsigned long entityKey, TA_Base_Bus::ScadaEntityUpdateType updateType)
    {
		TA_Base_Core::ThreadGuard guard( m_lock );

		if ( DELETE_ITEM == m_updateType )
		{
			m_updateAllCol = true;
			m_populateLogic->pocessItemUpdate(getEntityKey(), 0, true);
		}
		else
		{
			for ( int col=ESL_COLUMN_MIN; col < ESL_COLUMN_MAX; col++)
			{
				TA_Base_App::EslColumns column = static_cast<TA_Base_App::EslColumns>(col);
				updateTextIfChanged( column, buildNewColumnText( column));
				updateIconIfChanged( column, buildNewColumnIcon( column));
			}

			if ( 0 < m_columnUpdateFlags )
			{
				const FilterFlagsAndStrings& filter = m_populateLogic->getFilters();
				bool			isFilter  = checkFilters(filter);
				unsigned long	entityKey = getEntityKey();
				
				if (isFilter)
				{
					m_updateAllCol = true;
				}
				else if (m_updateAllCol)
				{
					for ( int jCol=ESL_COLUMN_MIN; jCol < ESL_COLUMN_MAX; jCol++)
					{
						m_columnUpdateFlags |= (1 << jCol);
					}
					m_updateAllCol = false;
				}

				m_populateLogic->pocessItemUpdate(entityKey, m_columnUpdateFlags, isFilter);
			}
			m_columnUpdateFlags = 0;
		}
    }

    int EquipmentStatusListItem::compare( const EquipmentStatusListItem &rhs, TA_Base_App::EslColumns column) const
    {
        int compareResult = 0;

        //  Timestamp is in dd/mm/yyyy format: not sortable by text comparison
        /*if ( TIMESTAMP_COLUMN == column)
        {
            if (m_timeStamp < rhs.m_timeStamp)
            {
                compareResult = -1;
            }
            else if (m_timeStamp > rhs.m_timeStamp)
            {
                compareResult = 1;
            }
        }
        else*/
        {
            //  All other columns are sorted by their text values
            if ( getColumnText( column) < rhs.getColumnText( column) )
            {
                compareResult = -1;
            }
            else if ( getColumnText( column) > rhs.getColumnText( column) )
            {
                compareResult = 1;
            }
        }
        return compareResult;
    }


	std::string const& EquipmentStatusListItem::getColumnText( TA_Base_App::EslColumns column) const
    {
        TA_ASSERT( (( column >= ESL_COLUMN_MIN) && (column < ESL_COLUMN_MAX)), "Column out of bounds" );
        return m_text[column];
    }


    int const& EquipmentStatusListItem::getColumnIcon( TA_Base_App::EslColumns column) const
    {
        TA_ASSERT( (( column >= ESL_COLUMN_MIN) && (column < ESL_COLUMN_MAX)), "Column out of bounds" );
        return m_icon[column];
    }

    bool EquipmentStatusListItem::updateIconIfChanged(EslColumns column, int newIcon)
    {
        TA_ASSERT( (( column >= ESL_COLUMN_MIN) && (column < ESL_COLUMN_MAX)), "Column out of bounds" );
        if ( newIcon != m_icon[column])
        {
            m_icon[column] = newIcon;
            m_columnUpdateFlags|= (1 << column);
            return true;
        }
        return false;
    }


	bool EquipmentStatusListItem::updateTextIfChanged(EslColumns column, std::string& newText)
    {
        return false;
    }


    std::string EquipmentStatusListItem::buildNewColumnText( TA_Base_App::EslColumns column )
    {
        //  Get the new text for the requested column. By calling this function 
        //  for each column we only need one try-catch(ScadaProxyException) block.

        std::string newText= "";
        return newText;
    }


    int EquipmentStatusListItem::buildNewColumnIcon( TA_Base_App::EslColumns column )
    {
        //  Icons will be set by the derived classes. If this base method is called
        //  then there is no icon for this column.
        return -1;
    }


    bool EquipmentStatusListItem::checkFilters( FilterFlagsAndStrings const& filters)
    {
		bool isFilter  = false;
        bool statusMatch = false;
		bool statusFilterInUse = filters[STATUS_FILTER_OFFSET].filterInUse;

        for ( int nf = ESL_FILTER_OFFSET_MIN; nf < ESL_FILTER_OFFSET_MAX; nf++)
        {
            if ( !filters[nf].filterInUse || USE_DERIVED_CLASS == EslFilterMap[nf]) continue;

			std::string columnText = getColumnText( static_cast<EslColumns>(EslFilterMap[nf])) ;
			std::transform(columnText.begin(), columnText.end(), columnText.begin(), std::toupper);
			std::string filterText = filters[nf].filterString;

            if ( ( nf > STATUS_FILTER_OFFSET ) && (nf <= STATUS_OVERRIDDEN_FILTER_OFFSET) )
            {
				if ( statusFilterInUse && !statusMatch && 
					(columnText.find(filterText) != std::string::npos)) // if found
                {
                    statusMatch = true;
                }
            }
            else
            {  
				if (columnText.find(filterText) == std::string::npos) // if not found
                {            
                    isFilter = true;
                    break;
                }
            }
        }

		if (!isFilter && statusFilterInUse)
		{
			isFilter = !statusMatch;
		}

		return isFilter;
    }
    
    unsigned long EquipmentStatusListItem::getItemDataNodeEntityKey()
    {
        LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
                    "Base EquipmentStatusListItem::getItemDataNodeEntityKey() is called!"); 
        return 0;
    }

    std::string EquipmentStatusListItem::getItemDataNodeEntityName()
    {
        LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
                    "Base EquipmentStatusListItem::getItemDataNodeEntityName() is called!"); 
        return "";
    }
	
	std::string EquipmentStatusListItem::getInfoFromSession(const std::string& session)
	{
		if (session == NotLoaded)
		{
			return std::string("");
		}

		std::string infoText("");
		{
			TA_THREADGUARD(m_sessionInfoCacheLock);
			std::map<std::string, std::string>::iterator iter = m_sessionInfoCache.find(session);
			if (iter != m_sessionInfoCache.end())
			{
				infoText = iter->second;
				return infoText;
			}
		}

		TA_Base_Core::ISession* thesession = NULL;
		try
		{
			thesession = TA_Base_Core::SessionAccessFactory::getInstance().getSession(session);

			if (thesession != NULL)
			{
				unsigned long operatorKey = thesession->getOperatorKey();
				unsigned long profileKey = thesession->getProfileKey();
				unsigned long consoleKey = thesession->getConsoleKey();

				infoText = m_operators[operatorKey];
				infoText += " / ";
				infoText += m_profiles[profileKey];
				infoText += " / ";
				infoText += m_consoles[consoleKey];

				TA_THREADGUARD(m_sessionInfoCacheLock);
				m_sessionInfoCache[session] = infoText;
			}
		}
		catch(TA_Base_Core::DataException& ) 
		{
			//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
			//	"Failed to get session or profile data from database for DataException");                                
		}
		catch(...)
		{
			//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
			//	"Failed to get session or profile data from database for unknown reason");
		}

		delete thesession;
		
		return infoText;
	}

	void EquipmentStatusListItem::initCache()
	{
		if (!m_initProfileConsole)
		{
			TA_THREADGUARD(m_initLock);
			if (!m_initProfileConsole)
			{
				unsigned long timeBegin = GetTickCount();
				try
				{
					std::vector<TA_Base_Core::IProfile*> profiles = TA_Base_Core::ProfileAccessFactory::getInstance().getAllProfiles();
					std::vector<TA_Base_Core::IProfile*>::iterator itProfile = profiles.begin();
					for (; itProfile!= profiles.end(); ++itProfile)
					{
						if (*itProfile != NULL)
						{
							std::string profileName = (*itProfile)->getName();
							m_profiles[(*itProfile)->getKey()] = profileName;
						}
						delete *itProfile;
					}
					
					std::map<unsigned long, std::string> locDispNames;
					std::vector<TA_Base_Core::ILocation*> aLocations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations(false);
					std::vector<TA_Base_Core::ILocation*>::iterator itLoc = aLocations.begin();
					for (; itLoc != aLocations.end(); ++itLoc)
					{
						std::string locDisplayName = (*itLoc)->getDisplayName();
						unsigned long locKey = (*itLoc)->getKey();
						locDispNames[locKey] = locDisplayName;
						delete *itLoc;
					}

					std::vector<TA_Base_Core::IConsole*> consoles = TA_Base_Core::ConsoleAccessFactory::getInstance().getAllConsoles();
					std::vector<TA_Base_Core::IConsole*>::iterator itConsole = consoles.begin();
					for (; itConsole!=consoles.end(); ++itConsole)
					{
						if (*itConsole != NULL) 
						{
							unsigned long consoleKey = (*itConsole)->getKey();
							unsigned long consoleLocKey = (*itConsole)->getLocation();
							m_consoles[consoleKey] = locDispNames[consoleLocKey];
						}
						delete *itConsole;
					}
				
					m_initProfileConsole = true;

					std::vector<TA_Base_Core::IOperator*> operators = TA_Base_Core::OperatorAccessFactory::getInstance().getAllOperators();
					std::vector<TA_Base_Core::IOperator*>::iterator itOperator = operators.begin();
					for (; itOperator != operators.end(); ++itOperator)
					{
						if (*itOperator != NULL)
						{
							std::string operatorName = (*itOperator)->getName();
							m_operators[(*itOperator)->getKey()] = operatorName;
						}
						delete *itOperator;
					}

					unsigned long timeEnd = GetTickCount();
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo,
						"init console count:%d, profile count:%d, operator count:%d, cost time:%d", 
						m_consoles.size(), m_profiles.size(), m_operators.size(), (timeEnd-timeBegin));
				}
				catch(...)
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
						"init cache exception");
				}
			}
		}
	}

} // TA_Base_App
