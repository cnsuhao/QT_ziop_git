/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveDialog.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * This inherits from CDialog and implements all common functionality for
  * TransActive dialogs..
  */

#if !defined(AFX_TRANSACTIVEDIALOG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_TRANSACTIVEDIALOG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/generic_gui_view/src/StdAfx.h"
#include "bus/generic_gui_view/src/ResizeViewHelper.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "core/utilities/src/DebugUtil.h"
#include <QtGui/QMainWindow>
#include <boost/shared_ptr.hpp>


namespace TA_Base_Bus
{
    /**
      * Dialog based TransActive view
      *
      */
    class AbstractDialog : public QMainWindow,
                           public AbstractTransActiveFormView
    {
    public:

        /**
          * Constructor
          *
          * @param pWinApp - The implementation of an IGUIApplicationConfig.
          * @param dialogId - The resource id of the dialog this class is for.
          * @param pParent - The parent window.
          */
        AbstractDialog(ITransActiveWinApp * pWinApp,  QWidget* pParent = NULL);

        /**
          * Destructor
          *
          */
        virtual ~AbstractDialog();

        virtual void showView(bool modal);

        virtual void hideView();

        virtual bool closeView();

        virtual TA_Rect getViewPosition();

        virtual bool setVeiwPosition( const TA_Rect& rect );
    };


    /**
      * Dialog based TransActive view, has implement some display property
      * It should be ancestor of main window of an application
      *
      *  __NOTE__: need to override initFromPModel initUIControl and call m_resizeViewHelper::setViewResizeProperty method during constructor
      */
    class AbstractTransActiveDialog : public AbstractDialog
    {
    public:

        /**
          * Constructor
          *
          * @param pWinApp - The implementation of an IGUIApplicationConfig.
          * @param dialogId - The resource id of the dialog this class is for.
          * @param pParent - The parent window.
          */
        AbstractTransActiveDialog( ITransActiveWinApp * pWinApp, QWidget* pParent = NULL );

        /**
          * Destructor
          *
          */
        virtual ~AbstractTransActiveDialog() ;

        /**
          * DestroyWindow
          *
          * This method must be implemented to ensure that when the window
          * is destroyed the dialog tells the GUIAccess object to clean up
          */
        virtual void DestroyWindow();

    protected:

        virtual void resizeEvent(QResizeEvent * event);
        virtual void closeEvent(QCloseEvent *event);
        virtual void changeEvent(QEvent * event);
        virtual bool event( QEvent* event );

    protected:

        virtual void initUIControl();

    protected:

        boost::shared_ptr<ResizedViewHelper> m_resizeViewHelper; ///< wrapped function for resizing view
    };
}


#endif // !defined(AFX_TRANSACTIVEDIALOG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
