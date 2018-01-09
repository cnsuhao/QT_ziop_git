/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusListCtrl.h $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Class is responsible for displaying the ListCtrl in the main window of the application.
 * It handles the column preparation and supports list sorting, column hiding/unhiding and
 * the image list (icons) to display.
 *
 */

#if !defined(AFX_EQUIPMENTSTATUSLISTCTRL_H__0060C665_8803_4C5B_942D_D84E8B454E33__INCLUDED_)
#define AFX_EQUIPMENTSTATUSLISTCTRL_H__0060C665_8803_4C5B_942D_D84E8B454E33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <set>

#include "app/scada/EquipmentStatusViewer/src/StdAfx.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentItemEventArg.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerDlgModel.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"

namespace TA_Base_App
{
	class CEquipmentStatusListCtrl : public CListCtrl, public TA_Base_Bus::AbstractTransActiveView
	{

	public:

        /** 
          * CEquipmentStatusListCtrl
          *
          * Constructor
          *
          */
		CEquipmentStatusListCtrl(TA_Base_Bus::ITransActiveWinApp * pApp);
		/**
  		 * GetSortOrder()
		 *
		 * Gets the sort order of a particular column
  		 *
  		 * @param	aCol	the 0-based index of the column sort order to retrieve
		 * @return			bool specifying if column sorting set to Ascending (true)
		 *					or Descending (false)
		 *
  		 */
		bool GetSortOrder(int aCol);

		/**
  		 * SetSortOrder()
  		 *
		 * Sets sort order of a particular column
		 *
  		 * @param	aCol		the 0-based index of the column sort order to set
		 * @param	aAscending	bool specifying if the column sort order to set is
		 *						to be Ascending (true) or Descending (false)
		 *
  		 */
		void SetSortOrder(int aCol, bool aAscending);

		/**
		 * Default virtual destructor
		 *
		 */
		virtual ~CEquipmentStatusListCtrl();

		/**
  		 * SortColumn()
		 *
		 * Sorts a particular column
  		 *
  		 * @param	aCol	the 0-based index of the column to sort
		 *
  		 */
		void SortColumn(int aCol, bool isUpdateUI = true);

        /** 
          * prepareColumns
          *
          * Sets the column widths and header text depending on the type of report to load.
          *
          *
          */
        void prepareColumns();


        //TD16889 Mintao 
	    /**
	      * getCurrentSortCol
	      * 
	      * get current sort col
	      * 
	      * @return int 
	      * 
	      * @exception <exceptions> Optional
	      */
        int getCurrentSortCol() { return m_currentSortCol;}
		
		/** 
          * CompareFunc
          *
          * Called when the CListCtrl is sorting to determine the relative order
          * of two items.
          *
          * @param lParam1      First item's data (entitykey)
          * @param lParam2      Second item's data
          * @param lParamSort   struct Sort_Parameters containing the column number to sort
          *                     by, the sort direction and the pointer to the parent dialog
          *                     where the proxies are kept
          *
          * @return             -1 if first item should come before second item
          *                     0  if items are the same sort position (ie. they match on this column)
          *                     +1 if first item should come after second item
          */
        static int CALLBACK CompareFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
		/** 
          * UpdateInList
          *
          * Called in the UI thread to update the on-screen display. This is usually
          * in response to this object handling the processEntityUpdate callback in
          * a pooled thread and posting it to the UI as a Windows user message.
          *
          *
          * @return             The index of the list control item that has been updated
          *
          */
	    int UpdateInList(EquipmentItemUpdateEvent* updateEvent);

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEquipmentStatusListCtrl)
		protected:
		virtual void PreSubclassWindow();
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
        //}}AFX_VIRTUAL

	protected:
		virtual void setupPModel();
		virtual std::string getMyPModelName();
		virtual HWND getViewHandle();
	protected:
		//{{AFX_MSG(CEquipmentStatusListCtrl)
        afx_msg void OnDestroy();
		afx_msg void OnSize(UINT nType, int cx, int cy);
	    //}}AFX_MSG

		DECLARE_MESSAGE_MAP()

    private:

        /** 
          * hideColumn
          *
          * Removes column from display by setting its width to zero.
          *
          * @param colIndex     The index number of the column to hide.
          */
        void hideColumn(int colIndex);

        /** 
          * showColumn
          *
          * Restores the column to the display.
          *
          * @param colIndex     The index number of the column to restore.
          * @param width        How wide the column should be now.
          *
          */
        void showColumn(int colIndex, int width);

        /**
         * @return icon handle for specified icon ID, or NULL if not found
         */
        HICON getIconHandle(const EIcon& icon);

		/** 
		 * An array of bool identifying whether a column is to be sorted in 
		 * Ascending (true) order or Descending (false) order
		 *
  		 */
	private:
		bool m_bAscending[ESL_COLUMN_MAX];
		int m_currentSortCol;

        typedef std::map<EIcon, HICON> IconMap;
        IconMap     m_icons;
        CImageList	m_imageList;

        // Defines all the hidden columns in this control
        std::set<int>   m_hiddenCols;

		Sort_Parameters	m_sortParameters;
		EquipmentStatusViewerDlgModel* m_realModel;
	};

} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTSTATUSLISTCTRL_H__0060C665_8803_4C5B_942D_D84E8B454E33__INCLUDED_)
