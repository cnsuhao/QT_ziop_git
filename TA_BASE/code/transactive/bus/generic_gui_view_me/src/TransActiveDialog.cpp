/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveDialog.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/11/29 14:23:30 $
  * Last modified by:  $Author: limin.zhu $
  *
  * This inherits from CDialog and implements all common functionality for
  * TransActive dialogs..
  */

#pragma warning(disable:4786)

#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/MFCControlHelper.h"
#include "bus/generic_gui_view/src/TransActiveConsts.h"
#include "bus/generic_gui_view/src/TopmostWindowMessageSender.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "bus/resources/resource.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/ApplicationException.h"
#include <QtCore/QEvent>
#include <QResizeEvent>
#include <QMoveEvent>

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using namespace TA_Base_Bus::TA_GenericGui;


namespace TA_Base_Bus
{
    /////////////////////////////////////////////////////////////////////////////

    AbstractDialog::AbstractDialog( ITransActiveWinApp * pWinApp, QWidget* pParent )
        : QMainWindow( pParent),
          AbstractTransActiveFormView( pWinApp )
    {
        this->setAttribute( Qt::WA_DeleteOnClose, true );  //delete dialog after closing.
    }


    AbstractDialog::~AbstractDialog()
	{
		//QDialog::~QDialog();
	}


	void AbstractDialog::hideView()
	{
		//this->hide();
		QMetaObject::invokeMethod(this,"hide",Qt::QueuedConnection);
	}


	bool AbstractDialog::closeView()
	{
		//return this->close();
		QMetaObject::invokeMethod(this,"close",Qt::QueuedConnection);
		return true;
	}


    void AbstractDialog::showView(bool modal)
    {
        if (modal) 
        { 
            //this->exec();
            QMetaObject::invokeMethod(this,"exec",Qt::QueuedConnection);

        }
        else
        {
            //this->show();
            QMetaObject::invokeMethod(this,"show",Qt::QueuedConnection);
        }
    }


    TA_Rect AbstractDialog::getViewPosition()
    {
        return TA_Rect::fromQRect( geometry() );
    }


    bool AbstractDialog::setVeiwPosition( const TA_Rect& rect )
    {
        if ( rect != TA_Rect::fromQRect( geometry() ) )
        {
            this->setGeometry( QRect( rect.left, rect.top, rect.width_qt(), rect.height_qt() ) );
        }

        return true; // ???
    }


    /////////////////////////////////////////////////////////////////////////////
    // TransActiveDialog message handlers

    AbstractTransActiveDialog::AbstractTransActiveDialog( ITransActiveWinApp * pWinApp, QWidget* pParent /*=NULL*/ )
        : AbstractDialog( pWinApp, pParent )
    {
        m_resizeViewHelper.reset( new ResizedViewHelper(pWinApp, this) );
    }


    AbstractTransActiveDialog::~AbstractTransActiveDialog()
    {
        DestroyWindow();
    }


    void AbstractTransActiveDialog::DestroyWindow()
    {
        LOG( SourceInfo, DebugUtil::FunctionEntry, "DestroyWindow");
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Destroy Main window");
        //if it is a main window, then need do some work before destroy window
        if ( isMainView())
        {
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "User close the window, do some work before call DestroyWindow");
            getTransActiveWinApp()->getApplicationController()->terminateFromUser();
        }
        LOG( SourceInfo, DebugUtil::FunctionExit, "DestroyWindow");      
    }


	void AbstractTransActiveDialog::resizeEvent(QResizeEvent * event)
	{
        m_resizeViewHelper->onResizeEvent( event );
        QMainWindow::resizeEvent( event );
	}


	void AbstractTransActiveDialog::closeEvent(QCloseEvent *event)
	{
	}


	void AbstractTransActiveDialog::changeEvent(QEvent *event)
	{
	    //the local change event is only supported on windows, so we have to provide our own way.
		//if (event->type() == QEvent::LanguageChange)			  
		//QDialog::changeEvent(event);
	}


    bool AbstractTransActiveDialog::event( QEvent* e )
    {
        m_resizeViewHelper->onEvent( e );
        return QMainWindow::event( e );
    }

   
    //void AbstractTransActiveDialog::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
    //{
    //}


    //LRESULT AbstractTransActiveDialog::OnSetWindowPosition(WPARAM wParam, LPARAM lParam)
    //{
    //    if (m_pModel == NULL)
    //    {
    //        LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore SetWindowPosition message");
    //        return true;
    //    }
    //    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnSetWindowPosition");
    //    return MFCControlHelper::setWindowPosition(this, wParam, lParam);
    //}


    void AbstractTransActiveDialog::initUIControl()
    {
        // Add "About..." menu item to system menu.

        //CMenu* pSysMenu = GetSystemMenu(FALSE);
        //if (pSysMenu != NULL)
        //{
        //    TAUnicodeString strAboutMenu = _T("About ");

        //    TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(RunParams::getInstance().get(RPARAM_APPNAME).c_str());
        //    strAboutMenu += tmpUniStr;

        //    strAboutMenu += _T("...");
        //    pSysMenu->AppendMenu(MF_SEPARATOR);
        //    pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu.c_str());

        //    // Add "Language..." menu item to system menu
        //    TAAppendLangMenu( 0, pSysMenu );
        //}
    }


}
