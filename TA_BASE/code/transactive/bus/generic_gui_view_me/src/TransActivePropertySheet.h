/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActivePropertySheet.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * This inherits from CPropertySheet and implements all common functionality for
  * TransActive dialogs..
  */

#if !defined(AFX_TRANSACTIVEPROPERTYSHEET_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_TRANSACTIVEPROPERTYSHEET_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/generic_gui_view/src/StdAfx.h"

#include "bus/generic_gui_view/src/ResizeViewHelper.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

#include "core/utilities/src/DebugUtil.h"


namespace TA_Base_Bus
{

    /** PropertyPage based TransActive view
     *	
     */
    class AbstractPropertyPage : public CPropertyPage,
                                 public AbstractTransActiveFormView
    {
        // Construction
    public:

        /**
        * Constructor
        *
        */
        AbstractPropertyPage(ITransActiveWinApp * pWinApp, UINT nIDTemplate, UINT nIDCaption = 0 );


    protected:

        /**
        * OnInitDialog
        *
        * __NOTE__: no need to override in derived class
        */
        virtual BOOL OnInitDialog();

        /** The MFC framework calls this member function to inform the CWnd object that it is being destroyed. 
         *	
         */
        virtual afx_msg void OnDestroy();

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

    private:

        void setWindowText(UINT nIDTemplate, UINT nIDCaption);

        DECLARE_MESSAGE_MAP()

    public:
        /** get the window handler for this view
         * 
         *
         *  @return HWND
         */
        virtual HWND getViewHandle()
        {
            HWND wnd = GetSafeHwnd();
            TA_ASSERT(wnd != NULL, "View should be initialized successfully");
            return wnd;
        }

		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

        UINT getID() { return (LONG)(LONG_PTR)this->m_lpszTemplateName; }
	};

    /** PropertySheet based TransActive view
     *	
     */
    class AbstractPropertySheet : public CPropertySheet
        , public AbstractTransActiveFormView
    {
        // Construction
    public:
        /**
        * Constructor
        *
        * @param pWinApp - The implementation of an IGUIApplicationConfig.
        * @param captionId - The captionId id of the propertysheet.
        * @param pParent - The parent window.
        */
		AbstractPropertySheet(ITransActiveWinApp * pWinApp, const UINT captionId, CWnd* pParent = NULL);

        /**
        * Destructor
        *
        */
        virtual ~AbstractPropertySheet();
	
	public:
        /** get the window handler for this view
         * 
         *
         *  @return HWND
         */
        virtual HWND getViewHandle()
        {
            HWND wnd = GetSafeHwnd();
            TA_ASSERT(wnd != NULL, "View should be initialized successfully");
            return wnd;

        }

    protected:

        /**
        * OnInitDialog
        *
        * __NOTE__: no need to override
        */
        virtual BOOL OnInitDialog();

        /** The MFC framework calls this member function to inform the CWnd object that it is being destroyed. 
         *	
         */
        virtual afx_msg void OnDestroy();
        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()

	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	};

    /** PropertySheet based TransActive view, has implement some display property
     *	
     */
	class AbstractTransActivePropertySheet 
		: public AbstractPropertySheet
    {
    // Construction
    public:
        /**
         * Constructor
         *
         * @param pWinApp - The implementation of an IGUIApplicationConfig.
         * @param captionId - The caption id of the propertsheet.
         * @param pParent - The parent window.
         */
        AbstractTransActivePropertySheet(ITransActiveWinApp * pWinApp, const UINT captionId, CWnd* pParent = NULL);

		/**
         * Destructor
         *
         */
		virtual ~AbstractTransActivePropertySheet() {};

        /**
         * DestroyWindow
         *
         * This method must be implemented to ensure that when the window
         * is destroyed the dialog tells the GUIAccess object to clean up
         */
        virtual BOOL DestroyWindow();
	
    protected:

        /**
         * OnInitDialog
         *
         * This method must be implemented to ensure that when the dialog is
         * initialized all the correct GenericGUI calls are made.
         * PRECONDITION: setResizingProperties has been called
         */
        virtual BOOL OnInitDialog();

        /**
         * OnSysCommand
         *
         * This method must be implemented to handle requests to display
         * the About box.
         */
        virtual afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

        /**
         * OnOK
         *
         * This method has been implemented to hide accidental calls when
         * the Enter key is pressed. It can be overriden with new behaviour if required.
         */
        virtual afx_msg void OnOK();

        /**
         * OnCancel
         *
         * This method has been implemented to hide accidental calls when
         * the ESC key is pressed. It can be overriden with new behaviour if required.
         */
        virtual afx_msg void OnCancel();

        /**
         * OnClose
         *
         * This method ensures that when the user chooses the X in the top right corner,
         * the CPropertySheet version of OnCancel is called.
         */
        virtual afx_msg void OnClose();

        /**
         * OnGetMinMaxInfo
         *
         * This method controls the resizing and maximising of the frame.
         * It ensures the frame is not resized to an inappropriate size.
         * It uses the m_resizingProperties member variable.
         */
        virtual afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

        /**
         * OnWindowPosChanging
         *
         * This method controls the resizing of the frame.
         * It ensures the frame is not resized to an inappropriate size.
         * It uses the m_resizingProperties member variable.
         */
        virtual afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

        /**
         * OnSetWindowPosition
         *
         * This method is called whenever GenericGui gets a new window position
         * from control station
         * 
         */
        virtual afx_msg LRESULT OnSetWindowPosition(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()
    
    protected:
        void initUIControl();

	protected:
		std::auto_ptr<ResizedViewHelper> m_resizeViewHelper; ///< wrapped function for resizing view	
    };
}


#endif // !defined(AFX_TRANSACTIVEDIALOG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
