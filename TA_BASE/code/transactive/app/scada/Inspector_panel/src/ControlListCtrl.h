/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/ControlListCtrl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic control information in
  * the CControlDialog class.
  *
  */

#ifndef CONTROLLISTCTRL_H
#define CONTROLLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "bus/scada/common_library/src/CommonDefs.h"
#include "app/scada/Inspector_Panel/src/DataPointListCtrl.h"
#include "../pmodel/src/Controllistctrlmodel.h"

namespace TA_Base_Bus
{
	class DataPointProxySmartPtr;
}

namespace TA_Base_App
{
	class CEquipment;

	/////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl window

	//TD15649, simplified this class 
	//by just responsing to the control state events of the visiable output datapoints
	//instead of using triggering entity of all datapoints to refresh the control state of visiable output datapoints
	
	class CControlListCtrl : public CAbstractDataPointListCtrl, ICellManager
	{
		// Construction
		public:
			CControlListCtrl();
		public:
			virtual ~CControlListCtrl();

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual TAUnicodeString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value);
			
			void updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType);
			bool isOkToChangeTab();
			bool isAnyDataPointsDisplayedOnScreen() const;
			void refreshDataPoints(); //wenching++ (TD13353)
			//TD15438, override the function to get a chance to adjust columns width.
			virtual BOOL SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );
			
			// Generated message map functions
		protected:
			//{{AFX_MSG(CControlListCtrl)
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			//}}AFX_MSG
        	//afx_msg void onCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult );

			DECLARE_MESSAGE_MAP()

			void setupList();
			void displayDataPoints();
			void updateDataPointDisplay ( int iPoint, int nItem);


			//add by tercel
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

			std::string		getMyPModelName();
			void			setupPModel();
	private:
			ControlListCtrlModel*			m_realPModel;
			volatile bool					m_dataPointsDisplayedOnScreen;
			volatile bool					m_initialState; //wenching++ (TD12608)
			
			//CEquipment*						m_equipment;
			//std::map<unsigned int, bool>	m_setValueActionAllowed;

            static const COLORREF       COLOUR_ALARM;  // colour to be used for showing alarms
            static const COLORREF       COLOUR_DISABLE; // colour to be used for showing disabled data points
            static const COLORREF       COLOUR_NORMAL; // colour to be used for showing normal data points
	};
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // ifndef CONTROLLISTCTRL_H
