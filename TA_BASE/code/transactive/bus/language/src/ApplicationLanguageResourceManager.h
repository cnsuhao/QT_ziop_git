#pragma once
#include "bus/language/src/IResourceContainer.h"


namespace TA_Base_Bus
{

    class ApplicationLanguageResourceManager
    {
    public:

        static ApplicationLanguageResourceManager& instance();

        void initialize( UINT applicationID );
        void initialize( const CString& applicationName );

        void setCurrentLanguage( UINT newLanguageID );
        UINT getCurrentLanguage();
        void saveDefaultLanguageID( UINT newDefaultLanguageID );

        void translateDialog( UINT dialogID, CDialog* pDialog );
        void translateMenu( UINT menuID, CMenu* pMenu );
        void translateToolBar( UINT toolBarID, CToolBar* pToolBar );
        void appendLanguageMenu( UINT menuID, CMenu* pMenu );
        const CString& getCString( UINT stringID );
        const CString& getDialogCaption( UINT dialogID );
        BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);

    private:

        ApplicationLanguageResourceManager();

        UINT loadDefaultLanguageID();
        void setupLanguageID2FileMap();
        const CString& getResourceFilePath( UINT languageID );
        void setupApplicationID2NameMap();
        UINT getApplicationID( const CString& applicationName );
        const CString& getApplicationName( UINT applicationID );

    private:

        struct LanguageMenuInfo
        {
            CString m_popup;
            std::vector< std::pair<UINT, CString> > m_menuItems;
        };

        typedef boost::shared_ptr<LanguageMenuInfo> LanguageMenuInfoPtr;

        LanguageMenuInfoPtr getLanguageMenuInfo();
        bool isHaveLanguageMenu( CMenu* pMenu );
        void addLanguageMenuToResource( UINT menuID, CMenu* pMenu );
        bool shouldHaveLanguageMenu( UINT menuID );

        std::set<UINT> m_shouldHaveLanguageMenuIDs;

    private:

        UINT m_applicationID;
        CString m_applicationName;
        UINT m_currentLanguageID;
        UINT m_defaultLanguageID;
        IResourceContainerPtr m_currentResourceContainer;
        std::map<UINT, IResourceContainerPtr> m_resourceContainerMap;
        std::map<UINT, CString> m_languageID2FileMap;
        boost::bimaps::bimap<UINT, CString> m_applicationID2NameMap;
        std::map<UINT, CString> m_defaultStringMap;
    };

}
