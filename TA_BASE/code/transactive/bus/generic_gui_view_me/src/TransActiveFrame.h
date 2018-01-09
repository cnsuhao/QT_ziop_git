/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveFrame.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * This inherits from CFrameWnd and implements all common functionality for
  * TransActive document frames.
  */

#if !defined(AFX_TRANSACTIVE_FRAME_H__8FF8DE95_4565_4123_860A_1F8475DC763D__INCLUDED_)
#define AFX_TRANSACTIVE_FRAME_H__8FF8DE95_4565_4123_860A_1F8475DC763D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "bus/generic_gui_view/src/ResizeViewHelper.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_Bus
{
    class IGUIApplicationConfig;

    /** SDI Frame based TransActive View
     *	
     */
    class AbstractFrame : public CFrameWnd
                           , public AbstractTransActiveFormView
    {
        DECLARE_DYNCREATE(AbstractFrame)
        
    protected: // create from serialization only

       /**
        * Constructor
        */
        AbstractFrame();


    public:

        /**
        * Destructor
        */
       virtual ~AbstractFrame();

    protected:

        /** OnCreate
         * 
         *
         *  @param lpCreateStruct
         *  @return int
         */
        virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        /**
         * OnClose
         *
         * This method must be implemented to ensure that when the window
         * is closed the window tells the GUIAccess object to clean up
         */
        virtual afx_msg void OnClose();

        /** The MFC framework calls this member function to inform the CWnd object that it is being destroyed. 
         *	
         */
        virtual afx_msg void OnDestroy();

        virtual afx_msg void GetMessageString(UINT nID, CString& strMessage) const;
        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);
        afx_msg void OnLangCommand(UINT nID);
        afx_msg BOOL OnToolTipText(UINT nID , NMHDR* pNMHDR , LRESULT * pResult);

        DECLARE_MESSAGE_MAP()

        /*
        ITransActiveView virtual method
        */
    public:
        virtual HWND getViewHandle()
        {
            HWND wnd = GetSafeHwnd();
            TA_ASSERT(wnd != NULL && ::IsWindow(wnd), "View should be initialized sucessfully");
            return wnd;
        };
	protected:
        virtual std::string getMyPModelName()
        {
            return "";
        }

        virtual void setupPModel()  {};


    };

    /** SDI Frame based TransActive view, has implement some display property
     *	
     */
    class AbstractTransActiveFrame : public AbstractFrame
    {
        DECLARE_DYNCREATE(AbstractTransActiveFrame)
    protected:
		/**
		* Constructor
		*/
		AbstractTransActiveFrame();

        /**
         * OnSysCommand
         *
         * This method must be implemented to handle requests to display
         * the About box.
         */
        virtual afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

        /**
         * OnGetMinMaxInfo
         *
         * This method controls the resizing and maximizing of the frame.
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
		std::auto_ptr<ResizedViewHelper> m_resizeViewHelper; ///< wrapped function for resizing view
	public:
		virtual BOOL DestroyWindow();
	};



}


#endif