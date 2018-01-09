#ifndef INHIBITLISTCTRL_H
#define INHIBITLISTCTRL_H

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/InhibitListCtrl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic information in
  * the CInformationDialog class.
  *
  */


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/scada/inspector_panel/src/DataPointListCtrl.h"
#include "AppLogicExceptionConverter.h"
#include "../pmodel/src/inhibitlistctrlmodel.h"

namespace TA_Base_Bus
{
	class DataPointProxySmartPtr;
}

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CInhibitListCtrl window

	class CInhibitListCtrl : public CAbstractDataPointListCtrl, ICellManager
	{
		// Construction
		public:
			CInhibitListCtrl();
			void refreshDataPoints();

		
		public:

			void updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType);
			
		// Implementation
		public:
			virtual ~CInhibitListCtrl();

			virtual bool isAnyDataPointsDisplayedOnScreen() const;

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual TAUnicodeString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value);
			//TD15438, override the function to get a chance to adjust columns width.
			virtual BOOL SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );
			

		protected:

			bool inhibitOverrideConfirm(const int inhibitOverrideItem,unsigned int iPoint, bool enable);
			void displayItem(int nItem);
			bool isCellEnabled (int nItem, int nSubItem);

			//
			// CAbstractDataPointListCtrl methods
			//
			void setupList();
			void displayDataPoints();
			void updateDataPointDisplay(int iPoint, int nItem);			
			std::string		getMyPModelName();
			void			setupPModel();
		protected:
			InhibitListCtrlModel*			m_realPModel;
			volatile bool					m_dataPointsDisplayedOnScreen;
	};

	/////////////////////////////////////////////////////////////////////////////

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_DATAPOINTLISTCTRL_H__8C394273_7B6C_11D7_B175_0050BAB1D931__INCLUDED_)
