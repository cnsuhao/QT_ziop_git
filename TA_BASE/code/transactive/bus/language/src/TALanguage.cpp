#include "StdAfx.h"
#include "bus/language/src/TALanguage.h"
#include "bus/language/src/ApplicationLanguageResourceManager.h"


void TALoadString( UINT nID, TAUnicodeString& strBuffer )
{
    strBuffer = TA_Base_Bus::ApplicationLanguageResourceManager::instance().getCString( nID );
}


void TALoadString( UINT nID, LPTSTR lpszBuf )
{
    const CString& s = TA_Base_Bus::ApplicationLanguageResourceManager::instance().getCString( nID );
    #pragma warning (push)
    #pragma warning(disable : 4996)
        s.CopyChars( lpszBuf, s.GetString(), s.GetLength() + 1 );
    #pragma warning (pop)
}


TAUnicodeString TALoadString( UINT nID )
{
    return TAUnicodeString( TA_Base_Bus::ApplicationLanguageResourceManager::instance().getCString( nID ) );
}


TAUnicodeString TALoadDialogCaption( UINT nID )
{
    return TAUnicodeString( TA_Base_Bus::ApplicationLanguageResourceManager::instance().getDialogCaption( nID ) );
}


void TATranslateDialog( UINT nID, CDialog* pDialog )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().translateDialog( nID, pDialog );
}


void TATranslateMenu( UINT nID, CMenu* pMenu )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().translateMenu( nID, pMenu );
}


void TAAppendLangMenu( UINT nID, CMenu* pMenu )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().appendLanguageMenu( nID, pMenu );
}


void TATranslateToolBar( UINT nID, CToolBar* pToolBar )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().translateToolBar( nID, pToolBar );
}


void TASetCurrentLang( UINT nID )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().setCurrentLanguage( nID );
}


UINT TAGetCurrentLang()
{
    return TA_Base_Bus::ApplicationLanguageResourceManager::instance().getCurrentLanguage();
}


void TASetDefaultLang( UINT nID )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().saveDefaultLanguageID( nID );
}


void TALoadLang( UINT nApplicationID )
{
    TA_Base_Bus::ApplicationLanguageResourceManager::instance().initialize( nApplicationID );
}


BOOL TAOnToolTipText( UINT nID , NMHDR* pNMHDR , LRESULT * pResult )
{
    return TA_Base_Bus::ApplicationLanguageResourceManager::instance().OnToolTipText( nID, pNMHDR, pResult );
}


TAUnicodeString TAAfxFormatString( UINT nID, LPCTSTR s1, LPCTSTR s2, LPCTSTR s3, LPCTSTR s4, LPCTSTR s5, LPCTSTR s6, LPCTSTR s7, LPCTSTR s8, LPCTSTR s9 )
{
    CString s = TA_Base_Bus::ApplicationLanguageResourceManager::instance().getCString( nID );

    if ( s1 != NULL )
    {
        s.Replace( _T("%1"), s1 );
    }
    if ( s2 != NULL )
    {
        s.Replace( _T("%2"), s2 );
    }
    if ( s3 != NULL )
    {
        s.Replace( _T("%3"), s3 );
    }
    if ( s4 != NULL )
    {
        s.Replace( _T("%4"), s4 );
    }
    if ( s5 != NULL )
    {
        s.Replace( _T("%5"), s5 );
    }
    if ( s6 != NULL )
    {
        s.Replace( _T("%6"), s6 );
    }
    if ( s7 != NULL )
    {
        s.Replace( _T("%7"), s7 );
    }
    if ( s8 != NULL )
    {
        s.Replace( _T("%8"), s8 );
    }
    if ( s9 != NULL )
    {
        s.Replace( _T("%9"), s9 );
    }

    return TAUnicodeString(s);
}
