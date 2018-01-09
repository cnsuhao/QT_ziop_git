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
#include <QtWidgets/QMainWindow>
#include <boost/shared_ptr.hpp>
#include <QtWidgets/QDialog>

namespace TA_Base_Bus
{
    /**
      * Dialog based TransActive view
      *
      */
    class AbstractDialog : public QDialog, public AbstractTransActiveFormView
    {
        Q_OBJECT

    public slots:

        void notifyMessageSlot(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam);
        virtual void OnInitDialog() { }; //Reimplement for those who want to process this

    public:

        /**
          * Constructor
          *
          * @param pWinApp - The implementation of an IGUIApplicationConfig.
          * @param viewType - name of view type.
          * @param pParent - The parent window.
          */
        AbstractDialog(ITransActiveApp* pWinApp, std::string viewType,  QWidget* pParent = NULL);

        /**
          * Destructor
          *
          */
        virtual ~AbstractDialog();

        virtual void showView(bool modal);

        virtual int doModal();

        virtual void hideView();

        virtual void autoResize();

        virtual bool closeView();

        virtual TA_Rect getViewPosition();

        virtual bool setVeiwPosition(const TA_Rect& rect);

        /**
          *  PostMessage, simulate WIN32:PostMessage
          *  The message will be queued in main thread
          * @param messageID:  message ID(should be unique in one component)
          * @param wParam: additional message-specific information
          * @param lParam: additional message-specific information
          */
        virtual void postMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam);

        /**
          *  sendMessage, simulate WIN32:sendMessage
          *  The message will be processed before return of the function.
          *  @param messageID:  message ID(should be unique in one component)
          *  @param wParam: additional message-specific information
          *  @param lParam: additional message-specific information
          */
        virtual void sendMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam);

        /**
          * setObjectName
          *
          * set object name and accessible Name
          */
        void setObjectName(const QString& name);

    protected:

        void showEvent(QShowEvent* event);
        //void notifyMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam, bool isSync = true);
    };

    /**
      * Dialog based TransActive view, has implement some display property
      * It should be ancestor of main window of an application
      *
      *  __NOTE__: need to override initFromPModel initUIControl and call m_resizeViewHelper::setViewResizeProperty method during constructor
      */
    class AbstractTransActiveDialog :  public QMainWindow, public AbstractTransActiveFormView
    {
        Q_OBJECT

    public slots:

        void notifyMessageSlot(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam);

    public:

        /**
          * Constructor
          *
          * @param pWinApp - The implementation of an IGUIApplicationConfig.
          * @param viewType - Name of view type.
          * @param pParent - The parent window.
          */
        AbstractTransActiveDialog(ITransActiveApp* pWinApp, std::string viewType, QWidget* pParent = NULL);

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

        virtual void showView(bool modal);

        virtual void hideView();
        virtual void autoResize();
        virtual bool closeView();

        /**
          *  PostMessage, simulate WIN32:PostMessage
          *  The message will be queued in main thread
          * @param messageID:  message ID(should be unique in one component)
          * @param wParam: additional message-specific information
          * @param lParam: additional message-specific information
          */
        virtual void postMessage(unsigned long messageID, TRA_WPARAM  wParam = 0, TRA_LPARAM lParam = 0);

        /**
          *  sendMessage, simulate WIN32:sendMessage
          *  The message will be processed before return of the function.
          *  @param messageID:  message ID(should be unique in one component)
          *  @param wParam: additional message-specific information
          *  @param lParam: additional message-specific information
          */
        virtual void sendMessage(unsigned long messageID, TRA_WPARAM  wParam = 0, TRA_LPARAM lParam = 0);

        /**
          * setObjectName
          *
          * set object name and accessible Name
          */
        void setObjectName(const QString& name);

    protected:

        virtual void resizeEvent(QResizeEvent* event);
        virtual void closeEvent(QCloseEvent* event);
        virtual void changeEvent(QEvent* event);
        virtual bool event(QEvent* event);

    protected:

        virtual void initUIControl();

    protected:

        boost::shared_ptr<ResizedViewHelper> m_resizeViewHelper; ///< wrapped function for resizing view
    };
}

#endif // !defined(AFX_TRANSACTIVEDIALOG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
