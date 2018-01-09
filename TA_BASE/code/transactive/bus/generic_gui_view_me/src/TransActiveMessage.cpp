// TransActiveMessage.cpp: implementation of the TransActiveMessage class.
//
//////////////////////////////////////////////////////////////////////
/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveMessage.cpp $
  * @author:  Brett Zimmerman
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/09/07 14:52:23 $
  * Last modified by:  $Author: limin.zhu $
  *
  * Standard message box format, display and logging functions
  */
#include "bus/generic_gui_view/src/stdafx.h"

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif // defined (_MSC_VER)

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/mfc_extensions/src/LoadString.h"
#include "bus/generic_gui_view/src/TransActiveMessageConfigParser.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/MessageResourceException.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/UserInteractionAudit_MessageTypes.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include <QMessageBox>


//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

// WM_HELPPROMPTADDR is used internally by MFC to get the address of
//    m_dwPromptContext from the associated frame window. This is used
//    during message boxes to setup for F1 help while that msg box is
//    displayed. lResult is the address of m_dwPromptContext.
#ifndef WM_HELPPROMPTADDR
#define WM_HELPPROMPTADDR    0x0376
#endif

// Context sensitive help support from afxpriv.h
#ifndef HID_BASE_PROMPT
#define HID_BASE_PROMPT     0x00030000UL
#endif

using TA_Base_Core::DebugUtil;


namespace
{
    QMessageBox::StandardButtons convertButtonFromWindowsToQt( int messageBoxStyle )
    {
        switch ( messageBoxStyle & 0x0000000FL )
        {
            case 0x00000000L: return QMessageBox::Ok;                                                // MB_OK
            case 0x00000001L: return QMessageBox::Ok | QMessageBox::Cancel;                          // MB_OKCANCEL
            case 0x00000002L: return QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore;  // MB_ABORTRETRYIGNORE
            case 0x00000003L: return QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;       // MB_YESNOCANCEL
            case 0x00000004L: return QMessageBox::Yes | QMessageBox::No;                             // MB_YESNO
            case 0x00000005L: return QMessageBox::Retry | QMessageBox::Cancel;                       // MB_RETRYCANCEL
            default: return QMessageBox::Ok;
        }
    }

    QMessageBox::Icon convertIconFromWindowsToQt( int messageBoxStyle )
    {
        switch ( messageBoxStyle & 0x000000F0L )
        {
            case 0x00000010L: return QMessageBox::Critical;    // MB_ICONHAND
            case 0x00000020L: return QMessageBox::Question;    // MB_ICONQUESTION
            case 0x00000030L: return QMessageBox::Warning;     // MB_ICONEXCLAMATION
            case 0x00000040L: return QMessageBox::Information; // MB_ICONASTERISK
            default:   return QMessageBox::Information;
        }
    }

    int convertResultFromQtToWindows( int result )
    {
        switch ( result )
        {
            case QMessageBox::Ok:       return 1; // IDOK
            case QMessageBox::Cancel:   return 2; // IDCANCEL
            case QMessageBox::Abort:    return 3; // IDABORT
            case QMessageBox::Retry:    return 4; // IDRETRY
            case QMessageBox::Ignore:   return 5; // IDIGNORE
            case QMessageBox::Yes:      return 6; // IDYES
            case QMessageBox::No:       return 7; // IDNO
            default:                    return 1; // IDOK
        }
    }
}


namespace TA_Base_Bus
{

    TransActiveMessage::TransActiveMessage()
        : m_sResourceFilePath( "TAMessageLibrary.xml" )
    {
        loadMessageConfigFile( m_sResourceFilePath );
    }


    TransActiveMessage::~TransActiveMessage()
    {
    }


    int TransActiveMessage::showMsgBox( int uiMessageID, const std::string& caption )
    {
        m_config = getMessageConfig( uiMessageID );

        if ( m_config != NULL )
        {
            QMessageBox msg;

            msg.setText( QString::fromStdString( m_config->parameters.empty() ? m_config->string : constructMessage( m_config->string ) ) );
            msg.setStandardButtons( convertButtonFromWindowsToQt( m_config->messageBoxStyle ) );
            msg.setIcon( convertIconFromWindowsToQt( m_config->messageBoxStyle ) );

            if ( ! caption.empty() )
            {
                msg.setWindowTitle( QString::fromStdString(caption) );
            }

            // TODO: audit event
            return convertResultFromQtToWindows( msg.exec() );
        }

        return 1; // IDOK
    }


    int TransActiveMessage::doMsgBox(std::string& pszCaption, std::string& pszMessage, int uiMsgBoxType, int uiHelpID, bool bAuditEvent)
    {
        return 0;
    }


    //bool TransActiveMessage::logMessage(std::string& pszCaption, std::string& pszMessage, int uiResult, int bAuditEvent)
    //{
    //    return true;
    //}


    std::string TransActiveMessage::constructMessage(int uiMessageID)
    {
        return "";
    }


    std::string TransActiveMessage::constructMessage( std::string sRawMessage )
    {
        for ( size_t i = 0; i < m_config->parameters.size(); ++i )
        {
            std::string parm = "{" + boost::lexical_cast<std::string>(i) + "}";
            size_t pos = sRawMessage.find( parm );

            if ( pos != std::string::npos )
            {
                if ( i < m_args.size() )
                {
                    sRawMessage.replace( pos, parm.size(), m_args[i] );
                }
                else
                {
                    sRawMessage.replace( pos + 1, parm.size() - 2, m_config->parameters[i] );
                }
            }
        }

        return sRawMessage;
    }


    void TransActiveMessage::reformatCurlyBraces(std::string& sMessage)
    {
        //sMessage.str().Replace(_T("{{"), _T("{")); //SL
        //sMessage.str().Replace(_T("}}"), _T("}")); //SL
    }


    bool TransActiveMessage::getRawMessage(int uiMessageID, std::string& sMessage)
    {
        return true;
    }


    void TransActiveMessage::throwMessageResourceException(const char * formatStr, const std::string & rawMessage)
    {
    }


    bool TransActiveMessage::parseRawMessage(const std::string& sRawMessage, int& uiMsgBoxType, int& uiHelpID, bool&bAudit, std::string& sMessage)
    {
        return true;
    }


    void TransActiveMessage::formatCaption(std::string& sCaption, int uiMessageID)
    {
    }


    /*bool TransActiveMessage::sendAuditMessage(std::string& pszMessage, std::string& pszCaption, std::string& pszResult)
    {
        return true;
    }*/


    void TransActiveMessage::closeAllMessages()
    {
    }

} // End namespace TA_Base_Bus
