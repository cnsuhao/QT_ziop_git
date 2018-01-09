/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/MainTabControl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class handles the tab control displayed on the main dialog screen.  It manages the 
  * three associated dialogs CInformationDialog, CControlDialog and CTagDialog.
  */

#ifndef MAINTABCONTROL_H
#define MAINTABCONTROL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "bus/mfc_extensions/src/tab_dialog_control/TabDialogCtrl.h"

#include "bus/scada/common_library/src/CommonDefs.h"
#include "../pmodel/src/inspectorpanelconsts.h"
#include "bus\generic_gui_view\src\AbstractTransActiveView.h"
#include "../pmodel/src/maintabcontrolmodel.h"
#include "bus\generic_gui_pmod\src\PModelInterface.h"

namespace TA_Base_Core
{
	class RightsLibrary;
}

namespace TA_Base_App
{
	class CEquipment;
	class ITabDialog;

	/////////////////////////////////////////////////////////////////////////////
	// CMainTabControl window

	class CMainTabControl : public CTabDialogCtrl,public TA_Base_Bus::AbstractTransActiveView
	{
	// Construction
	public:
		CMainTabControl();

		void initChildTabs();
	// Attributes
	public:

	// Operations
	public:

		virtual void setupPModel();
		virtual std::string getMyPModelName();
		virtual void notifyPModelDataChange(const char * updateType, const TA_Base_Bus::EventArgs * args=NULL) {};
		virtual HWND getViewHandle(){return 0;}
		// Tab numbers for the required pages
		// Note:  This will need to be updated if the pages are loaded in a different order


		//void setEquipment(CEquipment* equipment);
        //void setStatusTabEquipment(CEquipment* equipment);
		void refreshAllTab();
		void refreshStatusTab();
		void updateDataPoints(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
		void updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType);
		bool isUnsavedChangeOnTab(ETabNumbers tab);
		bool isOkToChangeTab(ETabNumbers tab);
		bool isAnyDataPointsDisplayedInTab(ETabNumbers tab);
		int resize();
		unsigned int getMinimumDialogHeight();
		void updatePermissions();
		void updateControlTab();
		void resetList();
		
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMainTabControl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CMainTabControl();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMainTabControl)
		virtual void PreSubclassWindow();
		//}}AFX_MSG
		std::map<ETabNumbers, ITabDialog*>	m_tabDialogs;

		CEquipment*							m_equipment;

		unsigned int						m_heightDifference;
		MainTabControlModel*				m_realPModel;
		TA_Base_Bus::ITransActiveWinApp*	m_pTransactiveApp;

		// Rights Library Manager to get permissions for actions within this application
		//TA_Base_Bus::RightsLibrary*				m_rightsMgr;

		DECLARE_MESSAGE_MAP()
		void CMainTabControl::clearChildTabs();
	};

	/////////////////////////////////////////////////////////////////////////////

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

}

#endif // #ifndef MAINTABCONTROL_H
