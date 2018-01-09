/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
  */

#pragma warning(disable:4786)

#include "StdAfx.h"
#include "TransActiveLoginDlg.h"
#include "../pmodel/src/TransActiveLoginDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include <algorithm>

#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/EntityTypeException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/ObjectResolutionException.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace TA_Base_App
{
    TransActiveLoginDlg::TransActiveLoginDlg(TA_Base_Bus::ITransActiveWinApp * app)
		: TA_Base_Bus::AbstractDialog(app, TransActiveLoginDlg::IDD, NULL)
		, m_realPModel(NULL)
    {
	    //{{AFX_DATA_INIT(TransActiveLoginDlg)
		    // NOTE: the ClassWizard will add member initialization here
	    //}}AFX_DATA_INIT
    	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    }


    TransActiveLoginDlg::~TransActiveLoginDlg()
    {
    }


    void TransActiveLoginDlg::DoDataExchange(CDataExchange* pDX)
    {
	    CDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(TransActiveLoginDlg)
	    DDX_Control(pDX, IDC_PROFILE_COMBO, m_profileCombo);
	    DDX_Control(pDX, IDC_PASSWORD_EDIT, m_passwordEdit);
	    DDX_Control(pDX, IDC_NAME_COMBO, m_nameCombo);
	    DDX_Control(pDX, IDC_LOCATION_COMBO, m_locationCombo);
	    //}}AFX_DATA_MAP
    }


BEGIN_MESSAGE_MAP(TransActiveLoginDlg, CDialog)
	//{{AFX_MSG_MAP(TransActiveLoginDlg)
	ON_BN_CLICKED(IDOK, onOK)
	ON_CBN_SELCHANGE(IDC_NAME_COMBO, onSelchangeNameCombo)
	ON_CBN_SELCHANGE(IDC_PROFILE_COMBO, onSelchangeProfileCombo)
	ON_CBN_SELCHANGE(IDC_LOCATION_COMBO, onSelchangeLocationCombo)
	ON_EN_CHANGE(IDC_PASSWORD_EDIT, onChangePasswordEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



	void TransActiveLoginDlg::populateOperatorCombo()
    {
        FUNCTION_ENTRY("populateOperatorCombo");

        m_nameCombo.ResetContent();

		for (TransActiveLoginDlgModel::OperatorVector::iterator iter = m_realPModel->m_allOperators.begin(); 
			     iter != m_realPModel->m_allOperators.end(); ++iter)
        {
			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString((*iter)->getName().c_str());
            int index = m_nameCombo.AddString( tmpUniStr.c_str());
            m_nameCombo.SetItemData(index, reinterpret_cast<DWORD>(*iter));
        }

        FUNCTION_EXIT;
    }


    void TransActiveLoginDlg::populateLocationCombo()
    {
        FUNCTION_ENTRY("populateLocationCombo");
        TA_ASSERT(m_realPModel->m_selectedProfile != NULL, "Must have selected a profile");

        m_locationCombo.ResetContent();

        TransActiveLoginDlgModel::LocationVector locations = m_realPModel->getListOfLocations(m_realPModel->m_selectedProfile);

        for (TransActiveLoginDlgModel::LocationVector::iterator iter = locations.begin(); iter != locations.end(); ++iter)
        {
			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString((*iter)->getDescription().c_str());
            int index = m_locationCombo.AddString( tmpUniStr.c_str() );
            m_locationCombo.SetItemData(index, (*iter)->getKey());
        }

        FUNCTION_EXIT;
    }


    void TransActiveLoginDlg::populateProfileCombo()
    {
        FUNCTION_ENTRY("populateProfileCombo");

        TA_ASSERT(m_realPModel->m_selectedOperator != NULL, "Must have selected an operator");
        m_profileCombo.ResetContent();

        TransActiveLoginDlgModel::ProfileVector profiles = m_realPModel->getListOfProfiles(m_realPModel->m_selectedOperator);

        for (TransActiveLoginDlgModel::ProfileVector::iterator iter = profiles.begin(); iter != profiles.end(); ++iter)
        {
			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString((*iter)->getName().c_str());
            int index = m_profileCombo.AddString( tmpUniStr.c_str() );
            m_profileCombo.SetItemData(index, reinterpret_cast<DWORD>(*iter));
        }

        FUNCTION_EXIT;
    }


    void TransActiveLoginDlg::onSelchangeNameCombo() 
    {
        m_profileCombo.SetCurSel(-1);
        m_locationCombo.SetCurSel(-1);

        // Passwords are based on operator so if the operator is changed the password should be removed
        m_passwordEdit.SetWindowText(_T(""));

        int selected = m_nameCombo.GetCurSel();

        if (m_nameCombo.GetCurSel() != CB_ERR)
        {
			m_realPModel->m_selectedOperator = reinterpret_cast<TA_Base_Core::IOperator*>(m_nameCombo.GetItemData(selected));
            populateProfileCombo();
        }
        else
        {
            m_realPModel->m_selectedOperator = NULL;
        }
        m_realPModel->m_selectedProfile = NULL;
	    refresh();
    }


    void TransActiveLoginDlg::onSelchangeProfileCombo() 
    {
        m_locationCombo.SetCurSel(-1);

        int selected = m_profileCombo.GetCurSel();
        if (selected == CB_ERR)
        {
            m_realPModel->m_selectedProfile = NULL;
            refresh();
            return;
        }

        m_realPModel->m_selectedProfile = reinterpret_cast<TA_Base_Core::IProfile*>(m_profileCombo.GetItemData(selected));

	    if (m_realPModel->m_selectedProfile->requiresLocationToBeSelected())
	    {
            populateLocationCombo();
		    // Select a default location.
			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->m_locationDescription.c_str());
		    m_locationCombo.SetCurSel(m_locationCombo.FindStringExact(-1, tmpUniStr.c_str() ));
	    }

		refresh();
    }


    void TransActiveLoginDlg::onSelchangeLocationCombo() 
    {
	    refresh();
    }


    void TransActiveLoginDlg::onChangePasswordEdit() 
    {
	    refresh();
    }

    void TransActiveLoginDlg::refresh()
    {
        if (m_realPModel->m_selectedOperator == NULL)
        {
            m_profileCombo.EnableWindow(false);
            m_locationCombo.EnableWindow(false);
            m_passwordEdit.EnableWindow(false);
            GetDlgItem(IDOK)->EnableWindow(false);
            return;
        }

        m_profileCombo.EnableWindow(true);
        
        if (m_realPModel->m_selectedProfile == NULL)
        {
            m_locationCombo.EnableWindow(false);
            m_passwordEdit.EnableWindow(false);
            GetDlgItem(IDOK)->EnableWindow(false);
            return;
        }

        if (m_realPModel->m_selectedProfile->requiresLocationToBeSelected())
        {
            m_locationCombo.EnableWindow(true);			

	        int selectedLocation = m_locationCombo.GetCurSel();
	        if (selectedLocation == CB_ERR)
	        {
                m_passwordEdit.EnableWindow(false);
                GetDlgItem(IDOK)->EnableWindow(false);
                return;
	        }
        }
        else
        {
            m_locationCombo.EnableWindow(false);
        }

        m_passwordEdit.EnableWindow(true);

        TAUnicodeString password;
        m_passwordEdit.GetWindowText(password.str());
        if (password.IsEmpty())
        {
            GetDlgItem(IDOK)->EnableWindow(false);
		    return;
        }

        GetDlgItem(IDOK)->EnableWindow(true);
    }


	void TransActiveLoginDlg::performDatabaseVerification(const TA_Base_Bus::SessionDetails& details)
    {
        FUNCTION_ENTRY("performDatabaseVerification");

        // Try to validate password with database directly.
        try
        {
            m_realPModel->verifyDetailsWithDatabase(details);
        }
        catch(const TA_Base_Core::DataException&)
        {
            TAUnicodeString action, error, actionName;
            action.LoadString(IDS_LOGIN);
            error.LoadString(IDS_INVALID_USER);
            actionName = action + error;
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << actionName;
            userMsg.showMsgBox(IDS_UE_020046);

            // Convert exception to an AuthenticationSecurityException
            TA_THROW(TA_Base_Core::AuthenticationSecurityException("The user is unknown", 
                TA_Base_Core::AuthenticationSecurityException::INVALID_USER));

        }
        catch(const TA_Base_Core::DatabaseException&)
        {
            TAUnicodeString action, error, actionName;
            action.LoadString(IDS_LOGIN);
            error.LoadString(IDS_DATABASE_ERROR);
            actionName = action + error;
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << actionName;
            userMsg.showMsgBox(IDS_UE_020046);

            // Convert exception to an AuthenticationSecurityException
            TA_THROW(TA_Base_Core::AuthenticationSecurityException("Database errors occurred.", 
                TA_Base_Core::AuthenticationSecurityException::INVALID_USER));

        }
        catch(const TA_Base_Core::AuthenticationSecurityException& ex)
        {
            TAUnicodeString action, error, actionName;

            switch( ex.getReason() )
            {
                case(TA_Base_Core::AuthenticationSecurityException::INVALID_PASSWORD):
                    error.LoadString(IDS_INVALID_PASSWORD);
                    break;
                default:
                    error.LoadString(IDS_UNKNOWN_ERROR);
                    break;
            }

            action.LoadString(IDS_LOGIN);
            actionName = action + error;
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << actionName;
            userMsg.showMsgBox(IDS_UE_020046);

            throw;
        }

        FUNCTION_EXIT;
    }


	void TransActiveLoginDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<TransActiveLoginDlgModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	}

	void TransActiveLoginDlg::initUIControl()
	{
		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// Passwords can only be a maximum of 50 characters.
		m_passwordEdit.SetLimitText(50); 


		populateOperatorCombo();
	}

	void TransActiveLoginDlg::updateView()
	{
		refresh();
	}


	void TransActiveLoginDlg::onOK() 
	{
		FUNCTION_ENTRY("onOK");

		TA_ASSERT(m_realPModel->m_selectedOperator != NULL, "Must select an operator before OK is activated");
		TA_ASSERT(m_realPModel->m_selectedProfile!= NULL, "Must select an operator before OK is activated");

		using TA_Base_Core::RunParams;
		using TA_Base_Core::AuthenticationSecurityException;

		TA_Base_Bus::SessionDetails details;
		details.operatorKey = m_realPModel->m_selectedOperator->getKey();
		details.profileKey = m_realPModel->m_selectedProfile->getKey();
		details.locationKey = 0;
		if (m_realPModel->m_selectedProfile->requiresLocationToBeSelected())
		{
			details.locationKey = m_locationCombo.GetItemData( m_locationCombo.GetCurSel() );
		}
		TAUnicodeString password;
		m_passwordEdit.GetWindowText(password.str());

		details.password = password.toMultiByteStdString();

		details.consoleKey = m_realPModel->m_consoleKey;
		try
		{
			m_realPModel->authenticate(details);
			m_realPModel->checkUserPermissions();
		}
		catch(const AuthenticationSecurityException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationSecurityException","The details entered did not have permission to login");

			unsigned int errorId = m_realPModel->getErrorIdFromException(ex);

			TAUnicodeString action, error, actionName;
			action.LoadString(IDS_LOGIN);
			error.LoadString(errorId);
			actionName = action + error;
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << actionName;
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_020046);

			FUNCTION_EXIT;
			return;
		}
		catch (const TA_Base_Core::AuthenticationAgentException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException","This is an unknown exception from the Authentication Agent. It could mean the Authentication agent can't contact the database");

			TA_Base_Bus::TransActiveMessage userMsg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_020018);            

			// Try to validate password with database directly.
			try
			{
				performDatabaseVerification(details);
			}
			catch(const AuthenticationSecurityException&)
			{
				FUNCTION_EXIT;
				return;
			}

		}
		catch (const TA_Base_Core::ObjectResolutionException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException","The Authentication Agent may not be running or may be running incorrectly.");

			TA_Base_Bus::TransActiveMessage userMsg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_020018);            

			// Try to validate password with database directly.
			try
			{
				performDatabaseVerification(details);
			}
			catch(const AuthenticationSecurityException&)
			{
				FUNCTION_EXIT;
				return;
			}
		}
		catch ( const TA_Base_Core::GenericGUIException& ex)
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "GenericGUIException", "Caught but can only deal with the ACCESS_DENIED problem. This will be thrown on if this is not the type");

			switch(ex.getFailType())
			{
			case(TA_Base_Core::GenericGUIException::ACCESS_DENIED):
				{
					TA_Base_Bus::TransActiveMessage userMsg;
					std::string operatorName = m_pWinApp->getApplicationController()->getOperatorName();

					std::string actionName = "start this application";
					userMsg << operatorName;
					userMsg << actionName;
					UINT selectedButton = userMsg.showMsgBox(IDS_UE_010021);
					FUNCTION_EXIT;
					return;
				}
			default:
				throw;
			}
		}

		CDialog::OnOK();
	}

    std::string TransActiveLoginDlg::getMyPModelName()
    {
        return PMODEL_TRANSACTIVE_LOGIN_DLG;
    }
}
