#include "StdAfx.h"
#include "bus/language/src/ApplicationLanguageResourceManager.h"
#include "bus/language/src/ResourceFileLoader.h"
#include "bus/language/src/TALanguage.h"
#include "bus/language/src/IDialogResource.h"
#include "bus/language/src/CharacterCodeConverter.h"
#include "bus/language/src/MenuResource.h"
#include "bus/language/src/PopupResource.h"
#include "bus/language/src/MenuItemResource.h"
#include "bus/language/src/StringResource.h"

#define REG_VALUE_NAME_DEFAULT_LANGUAGE     _T("DefaultLanguage")
#define REG_KEY_SOFTWARE                    _T("SOFTWARE")
#define REG_KEY_TRANSACTIVE                 _T("Transactive")
#define REG_KEY_SOFTWARE_TRANSACTIVE        _T("SOFTWARE\\Transactive")


namespace TA_Base_Bus
{

    ApplicationLanguageResourceManager::ApplicationLanguageResourceManager()
        : m_defaultLanguageID( LANGUAGE_ID_FIRST ),
          m_currentLanguageID( LANGUAGE_ID_NONE ),
          m_applicationID( APPLICATION_ID_ALL )
    {
        setupLanguageID2FileMap();
        setupApplicationID2NameMap();
    }


    ApplicationLanguageResourceManager& ApplicationLanguageResourceManager::instance()
    {
        static ApplicationLanguageResourceManager* singleton = new ApplicationLanguageResourceManager;
        return *singleton;
    }


    void ApplicationLanguageResourceManager::initialize( UINT applicationID )
    {
        m_applicationID = applicationID;
        m_applicationName = getApplicationName( applicationID );

        m_defaultLanguageID = loadDefaultLanguageID();
        m_currentLanguageID = LANGUAGE_ID_NONE;

        setCurrentLanguage( m_defaultLanguageID );
    }


    void ApplicationLanguageResourceManager::initialize( const CString& applicationName )
    {
        m_applicationID = getApplicationID( applicationName );
        ApplicationLanguageResourceManager::initialize( m_applicationID );
    }


    void ApplicationLanguageResourceManager::setCurrentLanguage( UINT newLanguageID )
    {
        if ( m_currentLanguageID != newLanguageID )
        {
            IResourceContainerPtr resourceContainer = m_resourceContainerMap[ newLanguageID ];

            if ( ! resourceContainer )
            {
                std::string resourceFilePath = CharacterCodeConverter::convertCStringtoUTF8( getResourceFilePath(newLanguageID) );
                resourceContainer = ResourceFileLoader::instance().loadResource( resourceFilePath, m_applicationID );
            }

            m_currentResourceContainer = m_resourceContainerMap[ newLanguageID ] = resourceContainer;
            m_currentLanguageID = newLanguageID;
        }
    }


    UINT ApplicationLanguageResourceManager::getCurrentLanguage()
    {
        return m_currentLanguageID;
    }


    void ApplicationLanguageResourceManager::saveDefaultLanguageID( UINT newDefaultLanguageID )
    {
        if ( m_defaultLanguageID != newDefaultLanguageID && false == m_applicationName.IsEmpty() )
        {
            m_defaultLanguageID = newDefaultLanguageID;

            CString keyName = REG_KEY_SOFTWARE_TRANSACTIVE;
            keyName += _T("\\");
            keyName += m_applicationName;

            CRegKey applicationKey;

            int iRet = applicationKey.Open( HKEY_LOCAL_MACHINE, keyName );

            if ( iRet != ERROR_SUCCESS )
            {
                CRegKey softwareKey;

                if ( ERROR_SUCCESS == softwareKey.Open( HKEY_LOCAL_MACHINE, REG_KEY_SOFTWARE ) )
                {
                    CRegKey transactiveKey;

                    if ( ERROR_SUCCESS == transactiveKey.Create( softwareKey, REG_KEY_TRANSACTIVE ) )
                    {
                        iRet = applicationKey.Create( transactiveKey, m_applicationName );
                    }
                }
            }

            if ( ERROR_SUCCESS == iRet )
            {
                applicationKey.SetDWORDValue( REG_VALUE_NAME_DEFAULT_LANGUAGE, m_defaultLanguageID );
            }
        }
    }


    void ApplicationLanguageResourceManager::translateDialog( UINT dialogID, CDialog* pDialog )
    {
        if ( m_currentResourceContainer && pDialog )
        {
            IResourcePtr resource = m_currentResourceContainer->getResource( DialogResourceKey(dialogID) );

            if ( resource )
            {
                resource->applyDialog( pDialog );
            }
        }
    }


    void ApplicationLanguageResourceManager::translateMenu( UINT menuID, CMenu* pMenu )
    {
        if ( m_currentResourceContainer && pMenu )
        {
            if ( true == shouldHaveLanguageMenu( menuID ) )
            {
                addLanguageMenuToResource( menuID, pMenu );
            }

            IResourcePtr resource = m_currentResourceContainer->getResource( MenuResourceKey(menuID) );

            if ( resource )
            {
                resource->applyMenu( pMenu );
            }
        }
    }


    void ApplicationLanguageResourceManager::translateToolBar( UINT toolBarID, CToolBar* pToolBar )
    {
        if ( m_currentResourceContainer && pToolBar )
        {
            IResourcePtr resource = m_currentResourceContainer->getResource( ToolBarResourceKey(toolBarID) );

            if ( resource )
            {
                resource->applyToolBar( pToolBar );
            }
        }
    }


    void ApplicationLanguageResourceManager::appendLanguageMenu( UINT menuID, CMenu* pMenu )
    {
        if ( m_currentResourceContainer && pMenu )
        {
            if ( false == isHaveLanguageMenu( pMenu ) )
            {
                m_shouldHaveLanguageMenuIDs.insert( menuID );

                LanguageMenuInfoPtr languageMenuInfo = getLanguageMenuInfo();

                if ( languageMenuInfo )
                {
                    CMenu languageMenu;

                    languageMenu.CreatePopupMenu();
                    pMenu->AppendMenu( MF_POPUP,(UINT)languageMenu.m_hMenu, languageMenuInfo->m_popup );

                    for ( size_t i = 0; i < languageMenuInfo->m_menuItems.size(); ++i )
                    {
                        languageMenu.AppendMenu( MF_STRING, languageMenuInfo->m_menuItems[i].first, languageMenuInfo->m_menuItems[i].second );
                    }

                    languageMenu.Detach();
                }
            }
        }
    }


    const CString& ApplicationLanguageResourceManager::getCString( UINT stringID )
    {
        if ( m_currentResourceContainer )
        {
            IResourcePtr resource = m_currentResourceContainer->getResource( StringResourceKey(stringID) );

            if ( resource )
            {
                return resource->getCString();
            }
        }

        std::map<UINT, CString>::iterator it = m_defaultStringMap.find( stringID );

        if ( it == m_defaultStringMap.end() )
        {
            it = m_defaultStringMap.insert( std::map<UINT, CString>::value_type( stringID, CString() ) ).first;
            it->second.LoadString( stringID );
        }

        return it->second;
    }


    const CString& ApplicationLanguageResourceManager::getDialogCaption( UINT dialogID )
    {
        if ( m_currentResourceContainer )
        {
            IResourcePtr resource = m_currentResourceContainer->getResource( DialogResourceKey(dialogID) );

            if ( resource )
            {
                IDialogResourcePtr dialogResource = boost::dynamic_pointer_cast<IDialogResource>(resource);

                if ( dialogResource )
                {
                    return dialogResource->getCaption();
                }
            }
        }

        static CString empty;
        return empty;
    }


    UINT ApplicationLanguageResourceManager::loadDefaultLanguageID()
    {
        DWORD defaultLanguage = LANGUAGE_ID_FIRST;

        if ( m_applicationName.IsEmpty() != true )
        {
            CString keyName = REG_KEY_SOFTWARE_TRANSACTIVE + CString("\\") + m_applicationName;

            CRegKey applicationKey;

            if ( ERROR_SUCCESS == applicationKey.Open( HKEY_LOCAL_MACHINE, keyName ) )
            {
                applicationKey.QueryDWORDValue( REG_VALUE_NAME_DEFAULT_LANGUAGE, defaultLanguage );
            }
        }

        return defaultLanguage;
    }


    void ApplicationLanguageResourceManager::setupLanguageID2FileMap()
    {
        m_languageID2FileMap[LANGUAGE_ID_EN_US] = "C:\\transactive\\language\\en_US.txt";
        m_languageID2FileMap[LANGUAGE_ID_ZH_CN] = "C:\\transactive\\language\\zh_CN.txt";
        // TODO: add languages here
    }


    const CString& ApplicationLanguageResourceManager::getResourceFilePath( UINT languageID )
    {
        std::map<UINT, CString>::iterator findIt = m_languageID2FileMap.find( languageID );

        if ( findIt != m_languageID2FileMap.end() )
        {
            return findIt->second;
        }

        static CString empty;
        return empty;
    }


    void ApplicationLanguageResourceManager::setupApplicationID2NameMap()
    {
        #define insert_value_to_map(l, r) m_applicationID2NameMap.left.insert(boost::bimaps::bimap<UINT, CString>::left_value_type(l, r))
        insert_value_to_map(APPLICATION_ID_AlarmManager, "AlarmManager");
        insert_value_to_map(APPLICATION_ID_AlarmManager, "AlarmManager");
        insert_value_to_map(APPLICATION_ID_AlarmGenerator, "AlarmGenerator");
        insert_value_to_map(APPLICATION_ID_IscsAlarmDisplay, "IscsAlarmDisplay");
        insert_value_to_map(APPLICATION_ID_IscsAlarmActionBridge, "IscsAlarmActionBridge");
        insert_value_to_map(APPLICATION_ID_AtsAlarmBridgeUtility, "AtsAlarmBridgeUtility");
        insert_value_to_map(APPLICATION_ID_AtsAlarmGenerator, "AtsAlarmGenerator");
        insert_value_to_map(APPLICATION_ID_AtsIscsAlarmViewer, "AtsIscsAlarmViewer");
        insert_value_to_map(APPLICATION_ID_EventExtractor, "EventExtractor");
        insert_value_to_map(APPLICATION_ID_EventViewer, "EventViewer");
        insert_value_to_map(APPLICATION_ID_CrystalReportsViewer, "CrystalReportsViewer");
        insert_value_to_map(APPLICATION_ID_NewTrendViewer, "NewTrendViewer");
        insert_value_to_map(APPLICATION_ID_TrendViewer, "TrendViewer");
        insert_value_to_map(APPLICATION_ID_HistoryViewer, "HistoryViewer");
        insert_value_to_map(APPLICATION_ID_PlanManager, "PlanManager");
        insert_value_to_map(APPLICATION_ID_SchedulingManager, "SchedulingManager");
        insert_value_to_map(APPLICATION_ID_EcsManager, "EcsManager");
        insert_value_to_map(APPLICATION_ID_IncidentManager, "IncidentManager");
        insert_value_to_map(APPLICATION_ID_JobRequestGui, "JobRequestGui");
        insert_value_to_map(APPLICATION_ID_MmsController, "MmsController");
        insert_value_to_map(APPLICATION_ID_DataNodeController, "DataNodeController");
        insert_value_to_map(APPLICATION_ID_EquipmentStatusViewer, "EquipmentStatusViewer");
        insert_value_to_map(APPLICATION_ID_InspectorPanel, "InspectorPanel");
        insert_value_to_map(APPLICATION_ID_PowerDemandEditor, "PowerDemandEditor");
        insert_value_to_map(APPLICATION_ID_DutyManager, "DutyManager");
        insert_value_to_map(APPLICATION_ID_RegionManager, "RegionManager");
        insert_value_to_map(APPLICATION_ID_SystemManager, "SystemManager");
        insert_value_to_map(APPLICATION_ID_ControlStation, "ControlStation");
        insert_value_to_map(APPLICATION_ID_SoundManager, "SoundManager");
        insert_value_to_map(APPLICATION_ID_Banner, "Banner");
        insert_value_to_map(APPLICATION_ID_IscsPage, "IscsPage");
        insert_value_to_map(APPLICATION_ID_PlanPage, "PlanPage");
        insert_value_to_map(APPLICATION_ID_AtsPage, "AtsPage");
        insert_value_to_map(APPLICATION_ID_CallPage, "CallPage");
        insert_value_to_map(APPLICATION_ID_ConfigurationEditor, "ConfigurationEditor");
        insert_value_to_map(APPLICATION_ID_Pluginxxx, "Pluginxxx");
        insert_value_to_map(APPLICATION_ID_VideoDisplayManager, "VideoDisplayManager");
        insert_value_to_map(APPLICATION_ID_PaManager, "PaManager");
        insert_value_to_map(APPLICATION_ID_RadioAuthoriseGui, "RadioAuthoriseGui");
        insert_value_to_map(APPLICATION_ID_RadioManagerGui, "RadioManagerGui");
        insert_value_to_map(APPLICATION_ID_RadioManagerGuiIcdv4, "RadioManagerGuiIcdv4");
        insert_value_to_map(APPLICATION_ID_RadioMonitorGui, "RadioMonitorGui");
        insert_value_to_map(APPLICATION_ID_RadioEecAcceptGui, "RadioEecAcceptGui");
        insert_value_to_map(APPLICATION_ID_StisManager, "StisManager");
        insert_value_to_map(APPLICATION_ID_TislogViewer, "TislogViewer");
        insert_value_to_map(APPLICATION_ID_TtisManager, "TtisManager");
        insert_value_to_map(APPLICATION_ID_TelephoneManager, "TelephoneManager");
        insert_value_to_map(APPLICATION_ID_TrainBorneDisplayManager, "TrainBorneDisplayManager");
        insert_value_to_map(APPLICATION_ID_TrainBorneManager, "TrainBorneManager");
        insert_value_to_map(APPLICATION_ID_TrainControlManager, "TrainControlManager");
        #undef insert_value_to_map
    }


    UINT ApplicationLanguageResourceManager::getApplicationID( const CString& applicationName )
    {
        boost::bimaps::bimap<UINT, CString>::right_map::iterator findIt = m_applicationID2NameMap.right.find( applicationName );

        if ( findIt != m_applicationID2NameMap.right.end() )
        {
            return findIt->second;
        }

        return APPLICATION_ID_ALL;
    }


    const CString& ApplicationLanguageResourceManager::getApplicationName( UINT applicationID )
    {
        boost::bimaps::bimap<UINT, CString>::left_map::iterator findIt = m_applicationID2NameMap.left.find( applicationID );

        if ( findIt != m_applicationID2NameMap.left.end() )
        {
            return findIt->second;
        }

        static CString empty;
        return empty;
    }


    ApplicationLanguageResourceManager::LanguageMenuInfoPtr ApplicationLanguageResourceManager::getLanguageMenuInfo()
    {
        const CString& languageMenuPoupu = getCString(LANGUAGE_ID_LANGUAGE);

        if ( TRUE == languageMenuPoupu.IsEmpty() )
        {
            return LanguageMenuInfoPtr();
        }

        LanguageMenuInfoPtr languageMenuInfo( new LanguageMenuInfo );

        languageMenuInfo->m_popup = languageMenuPoupu;

        for ( UINT i = LANGUAGE_ID_FIRST; i < LANGUAGE_ID_LAST; ++i )
        {
            const CString& languageMenuItem = getCString( i );

            if ( languageMenuItem.IsEmpty() != TRUE )
            {
                languageMenuInfo->m_menuItems.push_back( std::make_pair( i, languageMenuItem ) );
            }
        }

        return languageMenuInfo;
    }


    bool ApplicationLanguageResourceManager::isHaveLanguageMenu( CMenu* pMenu )
    {
        LanguageMenuInfoPtr languageMenuInfo = getLanguageMenuInfo();
        CMenu* lastPopup = pMenu->GetSubMenu( pMenu->GetMenuItemCount() - 1 );

        if ( NULL == lastPopup || !languageMenuInfo )
        {
            return false;
        }

        for ( size_t i = 0; i < languageMenuInfo->m_menuItems.size(); ++i )
        {
            for ( size_t j = 0; j < lastPopup->GetMenuItemCount(); ++j )
            {
                if ( lastPopup->GetMenuItemID( j ) == languageMenuInfo->m_menuItems[i].first )
                {
                    return true;
                }
            }
        }

        return false;
    }


    bool ApplicationLanguageResourceManager::shouldHaveLanguageMenu( UINT menuID )
    {
        return m_shouldHaveLanguageMenuIDs.find( menuID ) != m_shouldHaveLanguageMenuIDs.end();
    }


    void ApplicationLanguageResourceManager::addLanguageMenuToResource( UINT menuID, CMenu* pMenu )
    {
        if ( ! m_currentResourceContainer || NULL == pMenu )
        {
            return;
        }

        if ( false == m_currentResourceContainer->isExist( MenuResourceKey(menuID) ) )
        {
            MenuResourcePtr menuResource( new MenuResource(menuID) );
            m_currentResourceContainer->addResource( MenuResourceKey(menuID), menuResource );
        }

        IResourcePtr resource = m_currentResourceContainer->getResource( MenuResourceKey(menuID) );
        MenuResourcePtr menu = boost::dynamic_pointer_cast<MenuResource>( resource );
        LanguageMenuInfoPtr languageMenuInfo = getLanguageMenuInfo();

        if ( ! resource || ! menu || ! languageMenuInfo )
        {
            return;
        }

        std::vector<UINT> index;
        index.push_back( pMenu->GetMenuItemCount() - 1 );
        PopupResourcePtr popupResource( new PopupResource(languageMenuInfo->m_popup, index) );
        menu->addPopupResource( popupResource );

        for ( size_t i = 0; i < languageMenuInfo->m_menuItems.size(); ++i )
        {
            MenuItemResourcePtr menuItemResource( new MenuItemResource(languageMenuInfo->m_menuItems[i].first, languageMenuInfo->m_menuItems[i].second ) );
            menu->addMenuItemResource( languageMenuInfo->m_menuItems[i].first, menuItemResource );
        }
    }


    BOOL ApplicationLanguageResourceManager::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
    {
        // CFrameWnd::OnToolTipText

        ENSURE_ARG(pNMHDR != NULL);
        ENSURE_ARG(pResult != NULL);
        ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

        // need to handle both ANSI and UNICODE versions of the message
        TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
        TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
        //TCHAR szFullText[256];
        CString szFullText;
        CString strTipText;
        UINT_PTR nID = pNMHDR->idFrom;
        if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
            pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
        {
            // idFrom is actually the HWND of the tool
            //nID = _AfxGetDlgCtrlID((HWND)nID);
            nID = ((UINT)::GetDlgCtrlID((HWND)nID));
        }

        if (nID != 0) // will be zero on a separator
        {
            // don't handle the message if no string resource found
            szFullText = getCString( nID );

            if ( szFullText.IsEmpty() )
            {
                return FALSE;
            }

            //if (AfxLoadString((UINT)nID, szFullText) == 0)
            //    return FALSE;

            // this is the command id, not the button index
            AfxExtractSubString(strTipText, szFullText, 1, '\n');
        }
#ifndef _UNICODE
        if (pNMHDR->code == TTN_NEEDTEXTA)
            Checked::strncpy_s(pTTTA->szText, _countof(pTTTA->szText), strTipText, _TRUNCATE);
        else
            _mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
        if (pNMHDR->code == TTN_NEEDTEXTA)
            _wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
        else
            Checked::wcsncpy_s(pTTTW->szText, _countof(pTTTW->szText), strTipText, _TRUNCATE);
#endif
        *pResult = 0;

        // bring the tooltip window above other popup windows
        ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
            SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

        return TRUE;    // message was handled
    }

}
