//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanGeneralDetailTab.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_PLANGENERALDETAILPAGE_H__27738064_C8A4_4CDA_B629_C82925359FE4__INCLUDED_)
#define AFX_PLANGENERALDETAILPAGE_H__27738064_C8A4_4CDA_B629_C82925359FE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/response_plans/plan_manager/GeneratedFiles/ui_planGeneralDetailTab.h"

#include "app/response_plans/plan_manager/src/PlanDetailTab.h"
#include "app/response_plans/plan_manager/src/PlanInstanceListCtrl.h"
//#include "YSplitterWnd.h" //Bind to splitter bar Need???


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // PlanGeneralDetailTab dialog

    class PlanGeneralDetailTab : public PlanDetailTab
								, public Ui::PlanGeneralDetailTab
    {
		Q_OBJECT
    // Construction
    public:
		PlanGeneralDetailTab(QWidget* parent);   // standard constructor
        virtual ~PlanGeneralDetailTab();


    //// Operations
    protected:
		virtual void showRelevantControls();
        virtual void refreshControlContents();
		void updatePlanInstanceList(PlanNode* planNode, const PlanInstanceFilter &filter);
    //    virtual void RefreshActiveInstances();
    //    virtual void PurgeActiveInstances();
		void updatePlanView();
		virtual CtlCheckBox* getAllOwnersCheck();
		virtual CtlCheckBox* getAllLocationsCheck();


    // Attributes
    private:
        bool m_isLoadingControls;

    //// Overrides
    //protected:
    //   // ClassWizard generated virtual function overrides
    //    // {{AFX_VIRTUAL(PlanGeneralDetailTab)
    //    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //    // }}AFX_VIRTUAL


    //    // Generated message map functions
    //protected:
    //    // {{AFX_MSG(PlanGeneralDetailTab)
    //    virtual BOOL OnInitDialog();
    //    virtual BOOL OnSetActive();
    //    afx_msg void OnSize(UINT nType, int cx, int cy);
    //    afx_msg void OnPlanInstanceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    //    afx_msg void OnChangeNameEdit();
    //    afx_msg void OnChangeDescriptionEdit();
    //    // }}AFX_MSG
    //    afx_msg LRESULT OnRefreshPlanGeneralDetail(WPARAM wParam, LPARAM lParam);
    //    DECLARE_MESSAGE_MAP()


    //// Dialog Data
    protected:
    //    // {{AFX_DATA(PlanGeneralDetailTab)
    //    enum { IDD = IDD_PLAN_GENERAL_DETAIL_DLG };
    //    CEdit    m_planModByEdit;
    //    CEdit    m_planModTimeEdit;
    //    CEdit    m_approvalModByEdit;
    //    CEdit    m_approvalModTimeEdit;
    //    CEdit    m_nameEdit;
    //    CEdit    m_descriptionEdit;
    //    CEdit    m_approvalStatusEdit;
    //    CButton    m_activeInstancesGroupBox;
    //    CButton    m_detailsGroupBox;
        PlanInstanceListCtrl    m_planInstanceListCtrl;
    //    // }}AFX_DATA
    //    CySplitterWnd m_splitterBar;

        friend class PlanNode;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_PLANGENERALDETAILPAGE_H__27738064_C8A4_4CDA_B629_C82925359FE4__INCLUDED_)
