/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemControllerDlg.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * This is the dialog used to allow the user to enter the System Controller host name and
  * port number.
  */

#include "StdAfx.h"
#include "SystemControllerDlg.h"
#include "../pmodel/src/SystemManagerConsts.h"
#include "../pmodel/src/SystemControllerDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"


#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"


#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"

#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TA_MinMax.h"

using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{

    SystemControllerDlg::SystemControllerDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent /*=NULL*/)
	    : TA_Base_Bus::AbstractDialog(app, SystemControllerDlg::IDD, pParent)
		, m_realPModel(NULL)
    {
    	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    }


    void SystemControllerDlg::DoDataExchange(CDataExchange* pDX)
    {
	    CDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(SystemControllerDlg)
		DDX_Control(pDX, IDC_HOST_NAME_EDIT, m_hostnameEdit);
	    DDX_Control(pDX, IDC_PORT_NUMBER, m_portNumber);
	    DDX_Control(pDX, IDC_HOST_NAME, m_hostnameCombo);
	//}}AFX_DATA_MAP
    }


BEGIN_MESSAGE_MAP(SystemControllerDlg, TA_Base_Bus::AbstractDialog)
	//{{AFX_MSG_MAP(SystemControllerDlg)
	ON_BN_CLICKED(IDOK, onOK)
	ON_EN_CHANGE(IDC_PORT_NUMBER, onChangePortNumber)
	ON_CBN_SELCHANGE(IDC_HOST_NAME, OnSelectHostName)
	ON_EN_CHANGE(IDC_HOST_NAME_EDIT, OnChangeHostNameEdit)
	ON_CBN_DROPDOWN(IDC_HOST_NAME, OnDropdownHostName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


    void SystemControllerDlg::onOK() 
    {
        FUNCTION_ENTRY("onOK");

        TAUnicodeString hostUniStr(_T(""));
        if (m_hostnameEdit.IsWindowVisible())
        {
            m_hostnameEdit.GetWindowText(hostUniStr.str());
        }
        else if (m_hostnameCombo.IsWindowVisible())
        {
            int index = m_hostnameCombo.GetCurSel();
            if( index != LB_ERR )
            {
                m_hostnameCombo.GetLBText( index, hostUniStr.str());
            }
        }

        TAUnicodeString portNumberUniStr;
        m_portNumber.GetWindowText( portNumberUniStr.str() );

        // Trim whitespace
        hostUniStr.str().TrimLeft();
        hostUniStr.str().TrimRight();
        portNumberUniStr.str().TrimLeft();
        portNumberUniStr.str().TrimRight();

		std::string stdPortNumber = portNumberUniStr.toMultiByteStdString();
		std::string stdHost = hostUniStr.toMultiByteStdString();

        std::string portToText(stdPortNumber);
        if (-1 != portToText.find_first_not_of("0123456789") )
        {
            TAUnicodeString itemName;
            itemName.LoadString(IDS_PORT_NUMBER);
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << itemName;
            userMsg.showMsgBox(IDS_UE_020053);
            return;
        }

        TA_Base_Core::RunParams::getInstance().set(RPARAM_HOSTNAME, stdHost.c_str()); 
	    TA_Base_Core::RunParams::getInstance().set(RPARAM_PORTNUMBER, stdPortNumber.c_str()); 

        FUNCTION_EXIT;
        CDialog::OnOK();
    }


    void SystemControllerDlg::onChangePortNumber() 
    {
        refresh();
    }


	void SystemControllerDlg::OnChangeHostNameEdit() 
	{
        refresh();
	}


	void SystemControllerDlg::OnSelectHostName() 
	{
		refresh();
	}


	void SystemControllerDlg::OnDropdownHostName() 
	{
		// Get the number of items in the combo box
		int itemCount = m_hostnameCombo.GetCount();

		// Get the larger value between the number of items or the minimum dropdown
		itemCount = ta_max(itemCount, 2);
		
		// Get the height of the first item (all items will be the same size)
		int height = m_hostnameCombo.GetItemHeight(0);

		// Get the size of the combobox
		RECT rect;
		m_hostnameCombo.GetWindowRect(&rect);
		
		// Get width and calculate required height of combobox
		SIZE size;
		m_realPModel->computeComboboxSize(itemCount, size, rect, height);
		
		// Set the size of the dropdown
		m_hostnameCombo.SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER);	
	}


    void SystemControllerDlg::refresh()
    {
        // Check whether all the fields are completed so we can determine whether
        // the ok button should be enabled or disabled.

        // Only one of the edit or combo is used so check the one that is visible.
        if (m_hostnameEdit.IsWindowVisible() || m_hostnameEdit.IsWindowEnabled() )
        {
            TAUnicodeString hostString;
            m_hostnameEdit.GetWindowText(hostString.str());

            if (hostString.IsEmpty())
            {
                GetDlgItem(IDOK)->EnableWindow(FALSE);
                return;
            }
        }
        else if (m_hostnameCombo.IsWindowVisible() || m_hostnameCombo.IsWindowEnabled() )
        {
            if (m_hostnameCombo.GetCurSel() == LB_ERR)
            {
                GetDlgItem(IDOK)->EnableWindow(FALSE);
                return;
            }
        }
        else
        {
            // Unable to determine hostname
            GetDlgItem(IDOK)->EnableWindow(FALSE);
            return;
        }

        TAUnicodeString portNumberString;
        m_portNumber.GetWindowText( portNumberString.str() );

        if (portNumberString.IsEmpty())
        {
            GetDlgItem(IDOK)->EnableWindow(FALSE);
            return;
        }


        // If we get to here then the Ok button should be enabled as all fields have
        // been completed.
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }


	void SystemControllerDlg::populateHostnamesCombo()
	{
		FUNCTION_ENTRY("populateHostnamesCombo");

		std::vector<std::string> vectorOfHostnames;
		m_realPModel->getSystemControlHostNames(vectorOfHostnames);

		// Loop through and get the names of system controllers
		// Populate the hostname combobox
		for (std::vector<std::string>::iterator iter = vectorOfHostnames.begin();
			iter != vectorOfHostnames.end(); ++iter)
		{
			TAUnicodeString uniStr = TAUnicodeString::CreateFromMultitByteString((*iter).c_str());
			m_hostnameCombo.AddString(uniStr.c_str());
		}

		// Select the hostname that was entered on the command line if it is available
		std::string hostname = TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME).c_str();
		
		TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(hostname.c_str());

		m_hostnameCombo.SelectString(-1, tmpUniStr.c_str());

		FUNCTION_EXIT;
	}

	void SystemControllerDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<SystemControllerDlgModel*> (m_pModel);
	}

    void SystemControllerDlg::initUIControl()
    {
        // Set the icon for this dialog.  The framework does this automatically
        //  when the application's main window is not a dialog
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon

        m_hostnameEdit.SetLimitText(60); // Hostnames have a maximum length of 60 characters
        m_portNumber.SetLimitText(4); // Port numbers can only be a maximum of 4 characters

        // TD7027 - Try to populate the combobox, if we cannot the
        // use the default edit field instead		
        try
        {
            populateHostnamesCombo();

            // Since we have the database connection string 
            // hide the edit field and show the combobox
            m_hostnameEdit.ShowWindow(SW_HIDE);
            m_hostnameCombo.ShowWindow(SW_SHOW);
            m_hostnameEdit.EnableWindow( false);
            m_hostnameCombo.EnableWindow( true);

            // If using the combobox clear the edit field
            m_hostnameEdit.SetWindowText(_T(""));
        }
        catch (...)
        {
            // An error occured, default to the edit field
            m_hostnameEdit.ShowWindow(SW_SHOW);
            m_hostnameCombo.ShowWindow(SW_HIDE);
            m_hostnameEdit.EnableWindow( true);
            m_hostnameCombo.EnableWindow( false);

			std::string hostname = TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME).c_str();
            if (!hostname.empty())
            {
				TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(hostname.c_str());
                m_hostnameEdit.SetWindowText(tmpUniStr.c_str());
            }

            LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Could not populate hostname field automatically");
        }

        // We want to get the port number to set in the field if possible
		std::string portNumber = TA_Base_Core::RunParams::getInstance().get(RPARAM_PORTNUMBER).c_str();
		TAUnicodeString uniPortNumber = TAUnicodeString::CreateFromMultitByteString(portNumber.c_str());
        if (portNumber.empty())
        {
            // If no port number is currently set then we want to use the default port number
            uniPortNumber.str().Format(_T("%lu"),TA_Base_Core::DEFAULT_MGR_PORT);
        }

        m_portNumber.SetWindowText(uniPortNumber.c_str());

        refresh();
    }

    std::string SystemControllerDlg::getMyPModelName()
    {
        return PMODEL_SYSTEM_CONTROLLER_DLG;
    }


} // namespace TA_Base_App
