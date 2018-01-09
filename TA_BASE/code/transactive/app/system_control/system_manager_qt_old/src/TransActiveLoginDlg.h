/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
  */


#if !defined(AFX_TRANSACTIVELOGINDLG_H__E4B1DCC7_7AC0_48B4_91D3_8D3C199ED05F__INCLUDED_)
#define AFX_TRANSACTIVELOGINDLG_H__E4B1DCC7_7AC0_48B4_91D3_8D3C199ED05F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>

#include "resource.h"

#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"

namespace TA_Base_Core
{
    class IItem;
    class AuthenticationLibrary;
    class IOperator;
    class IProfile;
    class ILocation;

}

namespace TA_Base_App
{
	class TransActiveLoginDlgModel;

    class TransActiveLoginDlg : public TA_Base_Bus::AbstractDialog
    {
    // Construction
    public:
        /**
         * Constructor
         *
         * @param ISessionChecker* - The object to use to create the session with and check permissions
         */
        TransActiveLoginDlg(TA_Base_Bus::ITransActiveWinApp * app);   // standard constructor

        /** 
          * ~TransActiveLoginDlg
          *
          * Deletes the cached operator, profile and location objects
          *
          */
        ~TransActiveLoginDlg();

    // Dialog Data
	    //{{AFX_DATA(TransActiveLoginDlg)
	    enum { IDD = IDD_LOGIN_DIALOG };
	    CComboBox	m_profileCombo;
	    CEdit	m_passwordEdit;
	    CComboBox	m_nameCombo;
	    CComboBox	m_locationCombo;
	    //}}AFX_DATA


    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(TransActiveLoginDlg)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
    	HICON m_hIcon;

	    // Generated message map functions
	    //{{AFX_MSG(TransActiveLoginDlg)
	    afx_msg void onOK();

		void getOperatiorName();
		afx_msg void onSelchangeNameCombo();
	    afx_msg void onSelchangeProfileCombo();
	    afx_msg void onSelchangeLocationCombo();

		afx_msg void onChangePasswordEdit();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
	
	public:
        virtual std::string getMyPModelName();
        void setupPModel();
	
	protected:
		virtual void initUIControl();
		virtual void updateView();
    private:

        /**
         * refresh
         *
         * This checks the status of the controls and enables or disables them
         */
        void refresh();


        /**
         * populateOperatorCombo
         *
         * This will retrieve all operators from the database and populate the combo box
         * with their descriptions.
         */
        void populateOperatorCombo();


        /**
         * populateLocationCombo
         *
         * This will retrieve all locations from the database and populate the combo box
         * with their descriptions.
         */
        void populateLocationCombo();


        /**
         * populateProfileCombo
         *
         * This will retrieve all profiles for the currently selected operator from the database
         * and populate the combo box with their names.
         */
        void populateProfileCombo();

		/**
		* performDatabaseVerification
		*
		* Verifies the operator's details with the database by calling
		* verifyDetailsWithDatabase, and displays an error message to the user
		* if necessary.
		*
		* @exception AuthenticationSecurityException  Thrown if there is a problem
		*                                             authenticating the user.
		*/
		void performDatabaseVerification(const TA_Base_Bus::SessionDetails& details);
	private:
		TransActiveLoginDlgModel * m_realPModel;

    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}
#endif // !defined(AFX_TRANSACTIVELOGINDLG_H__E4B1DCC7_7AC0_48B4_91D3_8D3C199ED05F__INCLUDED_)
