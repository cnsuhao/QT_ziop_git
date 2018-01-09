/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/OptionsDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * OptionsDlg is associated with the dialog that allows
  * operators to set the data refresh rate.
  *
  */

#pragma warning(disable:4786)

#include "StdAfx.h"
#include "OptionsDlg.h"
#include "../pmodel/src/OptionsDlgModel.h"
#include "../pmodel/src/SystemManagerConsts.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include <vector>

namespace TA_Base_App
{

    OptionsDlg::OptionsDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent /*=NULL*/)
        : TA_Base_Bus::AbstractDialog(app,OptionsDlg::IDD, pParent)
		, m_realPModel(NULL)
    {
        //{{AFX_DATA_INIT(OptionsDlg)
            // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void OptionsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(OptionsDlg)
        DDX_Control(pDX, IDC_REFRESH_RATE_COMBO, m_RefreshRate);
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(OptionsDlg, CDialog)
        //{{AFX_MSG_MAP(OptionsDlg)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    void OptionsDlg::OnOK()
    {
        m_realPModel->setRefreshRate(m_RefreshRate.GetCurSel());

        CDialog::OnOK();
    }

	void OptionsDlg::setupPModel()
	{
		m_realPModel = dynamic_cast<OptionsDlgModel *> (m_pModel);
	}

    void OptionsDlg::initUIControl()
    {
        std::vector<int> intervals = m_realPModel->getRefreshIntervals();
        for (unsigned int i = 0; i < intervals.size(); i++)
        {
            TAUnicodeString tmpString;
            tmpString.str().Format(_T("%d s"), intervals[i]);
            m_RefreshRate.SetItemData(m_RefreshRate.AddString(tmpString.c_str()),i);
        }
        m_RefreshRate.SetCurSel(m_realPModel->getCurrentRefreshRate());
    }

    std::string OptionsDlg::getMyPModelName()
    {
        return PMODEL_OPTIONS_DLG;
    }
} // namespace TA_Base_App

