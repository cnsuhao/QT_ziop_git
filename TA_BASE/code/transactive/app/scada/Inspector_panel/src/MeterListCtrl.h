/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/MeterListCtrl.h $
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

#ifndef METERLISTCTRL_H
#define METERLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "app/scada/Inspector_Panel/src/DataPointListCtrl.h"
#include "../pmodel/src/meterlistctrlmodel.h"

namespace TA_Base_Bus
{
	class DataPointProxySmartPtr;
}

namespace TA_Base_App
{
	class CEquipment;

	/////////////////////////////////////////////////////////////////////////////
	// CMeterListCtrl window

	class CMeterListCtrl : public CAbstractDataPointListCtrl, ICellManager
	{
		// Construction
		public:
			CMeterListCtrl();			

		// Attributes
		public:

		// Implementation
		public:			
			virtual ~CMeterListCtrl();

			virtual bool isAnyDataPointsDisplayedOnScreen() const;

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual TAUnicodeString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value);

			void updatePermissions();
			//TD15438, override the function to get a chance to adjust columns width.
			virtual BOOL SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );
			
		protected:

			void displayItem(int nItem);

			// Generated message map functions
			//{{AFX_MSG(CMeterListCtrl)
			afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
			//}}AFX_MSG
			DECLARE_MESSAGE_MAP()			


			//
			// CAbstractDataPointListCtrl methods
			//
			void setupList();
			void displayDataPoints();
			void updateDataPointDisplay(int iPoint, int nItem);
			std::string			getMyPModelName();
			void				setupPModel();

	protected:
			MeterListCtrlModel*					m_realPModel;
			volatile bool						m_dataPointsDisplayedOnScreen;
			bool								m_isInMeterAdjust;


            static const COLORREF       COLOUR_ALARM;  // colour to be used for showing alarms
            static const COLORREF       COLOUR_DISABLE; // colour to be used for showing disabled data points
            static const COLORREF       COLOUR_NORMAL; // colour to be used for showing normal data points
	};

	/////////////////////////////////////////////////////////////////////////////

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // #ifndef METERLISTCTRL_H
