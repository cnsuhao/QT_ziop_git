/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/DirectControlDialog.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_DIRECTCONTROLDIALOG_H__9E492B98_83B7_4205_9655_99F4DC39D41F__INCLUDED_)
#define AFX_DIRECTCONTROLDIALOG_H__9E492B98_83B7_4205_9655_99F4DC39D41F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectControlDialog.h : header file
//

#include "bus/scada/proxy_library/src/IEntityUpdateEventProcessor.h"
#include "../pmodel/src/directcontroldialogmodel.h"
#include "bus\generic_gui_view\src\TransActiveDialog.h"

#define	MESSAGE_REFRESH_ALL_DATA	                (WM_USER + 30)
#define	MESSAGE_UPDATE_DATA							(WM_USER + 31)
#define	MESSAGE_UPDATE_PERMISSION					(WM_USER + 32)

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // CDirectControlDialog dialog

    class CDirectControlDialog : public AbstractTransActiveDialog
    {
    // Construction
    public:
	    CDirectControlDialog(TA_Base_Bus::ITransActiveWinApp*app,CWnd* pParent = NULL);   // standard constructor
        virtual ~CDirectControlDialog();

    // Dialog Data
	    //{{AFX_DATA(CDirectControlDialog)
	enum { IDD = IDD_CONFIRM_DIRECT_CONTROL_DIALOG };
		
		CButton	m_confirmButton;
	    CButton	m_test;
	    CStatic	m_newValueLabel;
	    CStatic	m_equipmentLabel;
	    CStatic	m_dataPointLabel;
	//}}AFX_DATA

		virtual void setupPModel();
		virtual std::string getMyPModelName();

        void setActionAllowed(bool isEnabled);
        void setEquipmentLabelInMainThread(const std::string& label);
        void setDataPointLabelInMainThread(const std::string& label);
        void setNewValueLabelInMainThread(const std::string& label);

        void resetDialog();

        //void setEquipment(CEquipment* equipment);
		void refreshAllData();
	    void updatePermissions();
        void destroyDataPoint();

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CDirectControlDialog)
	protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void	initUIControl();
	    //}}AFX_VIRTUAL

    // Implementation
	protected:
		virtual void	notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

		DirectControlDialogModel	*m_realPModel;

	    // Generated message map functions
	    //{{AFX_MSG(CDirectControlDialog)
	    afx_msg void OnConfirm();
        afx_msg LRESULT OnUpdateLabelInMainThread(UINT wParam, LONG lParam);
		afx_msg LRESULT onRefreshAllData(WPARAM wparam, LPARAM lparam);
		afx_msg LRESULT onUpdatePermission(WPARAM wparam, LPARAM lparam);

	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()


	};

}
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTCONTROLDIALOG_H__9E492B98_83B7_4205_9655_99F4DC39D41F__INCLUDED_)
