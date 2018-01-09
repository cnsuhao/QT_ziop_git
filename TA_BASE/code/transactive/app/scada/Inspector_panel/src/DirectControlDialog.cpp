/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/DirectControlDialog.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */
// DirectControlDialog.cpp : implementation file
//

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InspectorPanelDlg.h"
#include "app/scada/inspector_panel/src/DirectControlDialog.h"
#include "AppLogicExceptionConverter.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace TA_Base_App
{

    /////////////////////////////////////////////////////////////////////////////
    // CDirectControlDialog dialog


    CDirectControlDialog::CDirectControlDialog(TA_Base_Bus::ITransActiveWinApp*app,CWnd* pParent /*=NULL*/)
	 :TA_Base_Bus::AbstractTransActiveDialog(app, CDirectControlDialog::IDD, NULL)
    {
	    //{{AFX_DATA_INIT(CDirectControlDialog)
	    //}}AFX_DATA_INIT
    }

    CDirectControlDialog::~CDirectControlDialog()
    {

    }


	void	CDirectControlDialog::setupPModel()
	{
		m_realPModel = dynamic_cast<DirectControlDialogModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");

		m_realPModel->attachView(this,PMODEL_NOTIFY_REFRESH_ALL_DATA);
		//m_realPModel->attachView(this,PMODEL_NOTIFY_UPDATE_DATA);
		m_realPModel->attachView(this,PMODEL_NOTIFY_PERTMISSION);
	}


	std::string CDirectControlDialog::getMyPModelName()
	{
		return  PMODEL_DIRECT_CONTROL_DIALOG;
	}

	void CDirectControlDialog::initUIControl()
	{
		CRect windowSize;
		GetWindowRect(&windowSize);

		TA_Base_Bus::ViewResizeProperty properties;
		properties.canMaximise = false;
		properties.maxHeight = -1;
		properties.maxWidth = -1;
		properties.minHeight = windowSize.bottom - windowSize.top;
		properties.minWidth = windowSize.right - windowSize.left;
		m_resizeViewHelper->setViewResizeProperty(properties);
	}

	void	CDirectControlDialog::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if( _stricmp(updateType, PMODEL_NOTIFY_REFRESH_ALL_DATA) == 0 )
		{
			BOOL err = this->PostMessage(MESSAGE_REFRESH_ALL_DATA);
		}
		else if( _stricmp(updateType, PMODEL_NOTIFY_PERTMISSION) == 0 )
		{
			BOOL err = this->PostMessage(MESSAGE_UPDATE_PERMISSION);
		}
	}

	LRESULT CDirectControlDialog::onRefreshAllData(WPARAM wparam, LPARAM lparam)
	{
		this->refreshAllData();
		return 0;
	}

	LRESULT CDirectControlDialog::onUpdatePermission(WPARAM wparam, LPARAM lparam)
	{
		this->updatePermissions();;
		return 0;
	}

    void CDirectControlDialog::DoDataExchange(CDataExchange* pDX)
    {
	    CDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(CDirectControlDialog)
	DDX_Control(pDX, IDCONFIRM, m_confirmButton);
	    DDX_Control(pDX, IDC_NEW_VALUE, m_newValueLabel);
	    DDX_Control(pDX, IDC_EQUIPMENT, m_equipmentLabel);
	    DDX_Control(pDX, IDC_DATA_POINT, m_dataPointLabel);
	//}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CDirectControlDialog, AbstractTransActiveDialog)
	    //{{AFX_MSG_MAP(CDirectControlDialog)
	    ON_BN_CLICKED(IDCONFIRM, OnConfirm)
		ON_MESSAGE( MESSAGE_REFRESH_ALL_DATA, onRefreshAllData)
		ON_MESSAGE( MESSAGE_UPDATE_PERMISSION, onUpdatePermission)
        ON_MESSAGE(UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE, OnUpdateLabelInMainThread)

	    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CDirectControlDialog message handlers



    void CDirectControlDialog::resetDialog()
    {
        setDataPointLabelInMainThread("");
        setNewValueLabelInMainThread("");
		setEquipmentLabelInMainThread("");
		setActionAllowed(false);
    }


	void CDirectControlDialog::refreshAllData()
	{
		FUNCTION_ENTRY ("refreshAllData()");

        std::string tmpString("");
        if ( NULL != m_realPModel)
        {
			if(!m_realPModel->isConfigValid())
			{
				m_realPModel->init();
			}

			if(m_realPModel->isConfigValid())
			{
				tmpString = m_realPModel->getEquipmentLabel();
				//m_equipment = tmpString.c_str();
				setEquipmentLabelInMainThread(tmpString);

				tmpString = m_realPModel->getControlDescription();
				setDataPointLabelInMainThread(tmpString);

				tmpString = m_realPModel->getControlValue();
				setNewValueLabelInMainThread(tmpString);
				this->setActionAllowed(m_realPModel->isActionAllowd());
			}
        }
		
	}


	void CDirectControlDialog::updatePermissions()
	{
		if(m_realPModel->isConfigValid())
		{
			this->setActionAllowed(m_realPModel->isActionAllowd());
		}
		else
		{
			this->setActionAllowed(false);
		}
	}

    void CDirectControlDialog::setActionAllowed(bool isEnabled)
    {
		 m_confirmButton.EnableWindow(isEnabled);
    }

    void CDirectControlDialog::setEquipmentLabelInMainThread(const std::string& label)
    {
        std::string* temporalString = new std::string(label);
        PostMessage(UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE, reinterpret_cast<UINT>(&m_equipmentLabel), reinterpret_cast<LONG>(temporalString));
    }

    void CDirectControlDialog::setDataPointLabelInMainThread(const std::string& label)
    {
        std::string* temporalString = new std::string(label);
        PostMessage( UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE, reinterpret_cast<UINT>(&m_dataPointLabel), reinterpret_cast<LONG>(temporalString));
    }

    void CDirectControlDialog::setNewValueLabelInMainThread(const std::string& label)
    {
        std::string* temporalString = new std::string(label);
		PostMessage( UPDATE_LABEL_IN_MAIN_THREAD_MESSAGE,reinterpret_cast<UINT>(&m_newValueLabel), reinterpret_cast<LONG>(temporalString));
    }

    LRESULT CDirectControlDialog::OnUpdateLabelInMainThread(UINT wParam, LONG lParam)
    {
        std::string* temporalString = reinterpret_cast<std::string*>(lParam);
        CWnd* desiredLabel = reinterpret_cast<CWnd*>(wParam);

        if (temporalString && desiredLabel)
        {
            desiredLabel->SetWindowText(CONVERT_FROM_STD((*temporalString)));
        }

		if(temporalString)
		{
			delete temporalString;
		}
        return 0;
    }

    void CDirectControlDialog::OnConfirm() 
    {
		try
		{
			m_realPModel->directControl();
			TAUnicodeString *statusLineStr = new TAUnicodeString();
			*statusLineStr = TALoadString(STRING_ID_INSPECTOR_PANEL_0110);
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
		}
		catch(OutputLogicException&ex)
		{
			TAUnicodeString *statusLineStr = new TAUnicodeString();
			*statusLineStr = TALoadString(STRING_ID_INSPECTOR_PANEL_0111);
			AfxGetApp()->m_pMainWnd->PostMessage(UPDATE_STATUS_WINDOW_MESSAGE, reinterpret_cast<UINT>(statusLineStr), 0);
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "%s", "Unknown Exception" );
		} 
    }
}


