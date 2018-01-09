/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/StatusDialog.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the dialog which displays the equiment information tab shown
  * by the inspector panel.  It allows the operator to view the values of the data points.
  */

#include "stdafx.h"

#include <sstream>
#include "ace/OS.h"

#include "app/scada/inspector_panel/src/StatusDialog.h"
#include "app/scada/inspector_panel/src/InspectorPanel.h"

#include "core/utilities/src/DebugUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // CStatusDialog dialog


    CStatusDialog::CStatusDialog(TA_Base_Bus::ITransActiveWinApp*app, CWnd* pParent /*=NULL*/)
       : TA_Base_Bus::AbstractTransActiveDialog(app, CStatusDialog::IDD, NULL),
	   m_realPModel(NULL)
    {
        FUNCTION_ENTRY ("CStatusDialog()");

        //{{AFX_DATA_INIT(CStatusDialog)
    //}}AFX_DATA_INIT

        FUNCTION_EXIT;
    }

    void CStatusDialog::refreshAllData()
    {
        FUNCTION_ENTRY ("setEquipment()");
        
		updateDataNode(TA_Base_Bus::ConfigStateUpdate);
        
		FUNCTION_EXIT;
    }

    void CStatusDialog::DoDataExchange(CDataExchange* pDX)
    {
        FUNCTION_ENTRY ("DoDataExchange()");

        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CStatusDialog)
    DDX_Control(pDX, IDC_STATUS_INFO, m_statusInfo);
    //}}AFX_DATA_MAP

        FUNCTION_EXIT;
    }

	void CStatusDialog::setupPModel()
	{
		m_realPModel = dynamic_cast<StatusDialogModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	}


	std::string CStatusDialog::getMyPModelName()
	{
		return PMODEL_STATUS_DIALOG;
	}


    BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
        //{{AFX_MSG_MAP(CStatusDialog)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CStatusDialog message handlers



    void CStatusDialog::updateDataPoint(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
    {
        FUNCTION_ENTRY ("updateDataPoints()");

        updateDataNode(updateType);

        FUNCTION_EXIT;
    }

    void CStatusDialog::updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType)
    {
        FUNCTION_ENTRY ("updateDataNode()");

        if (m_hWnd==0 || m_realPModel == NULL)
        {
            return;
        }
		m_statusInfo.SetWindowText(_T(""));
		//this->m_realPModel->updateDataNode(updateType);
		std::string statusText = m_realPModel->getStatusInfo();
		m_statusInfo.SetWindowText(CONVERT_FROM_STD(statusText));
        FUNCTION_EXIT;
    }

    bool CStatusDialog::isUnsavedChangeOnTab()
    {
        FUNCTION_ENTRY ("isUnsavedChangeOnTab()");
        FUNCTION_EXIT;
        return false;
    }

    bool CStatusDialog::isOkToChangeTab()
    {
        FUNCTION_ENTRY ("isOkToChangeTab()");
        FUNCTION_EXIT;

        return true;
    }


    void CStatusDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
    {
        CDialog::OnShowWindow(bShow, nStatus);
        
        if (bShow)
        {
            updateDataNode(TA_Base_Bus::ConfigStateUpdate);
        }
    }

    void CStatusDialog::getWindowRect(CRect* rect)
    {
        GetWindowRect(rect);
    }

    unsigned int CStatusDialog::getMinimumHeight()
    {
        // No real minimum height - one of the other dialogs will be bigger.
        return 0;
    }       

    void CStatusDialog::resize(unsigned int height)
    {
        CRect editRect, dialogRect;
        m_statusInfo.GetWindowRect(&editRect);
        GetWindowRect(&dialogRect);

        editRect.NormalizeRect();
        dialogRect.NormalizeRect();

        int diff = height - dialogRect.Height();

        if (diff > 0)
        {
            editRect.InflateRect(0, 0, 0, diff);
            dialogRect.InflateRect(0, 0, 0, diff);
        }
        else
        {
            editRect.DeflateRect(0, 0, 0, -diff);
            dialogRect.DeflateRect(0, 0, 0, -diff);
        }

        m_statusInfo.SetWindowPos(NULL, 0, 0, editRect.Width(), editRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
        SetWindowPos(NULL, 0, 0, dialogRect.Width(), dialogRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
    }

    void CStatusDialog::updatePermissions(/*TA_Base_Bus::RightsLibrary* rightsMgr*/)
    {
    }

    bool CStatusDialog::isAnyDataPointsDisplayedInTab() const
    {
        return true;
    }

	void CStatusDialog::resetList()
	{

	}
}
