/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/AbstractToolBarManager.h $
* @author:  Hoa Le
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* This class implement common functionalities for Toolbars.
*/

#ifndef ABSTRACTTOOLBARMANAGER_H
#define ABSTRACTTOOLBARMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_planManagerFrame.h"

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// AbstractToolBarManager

	class AbstractToolBarManager
		: public QObject
	{
		Q_OBJECT
		// Operations
	public:
		AbstractToolBarManager();
		virtual ~AbstractToolBarManager();

		/**
		* initialise
		*
		* Initialise toolbar
		*
		* @param toolBarParent parent widget of toolbar
		*/
		virtual void initialise(Ui_PlanManagerFrame* toolBarParent);

		/**
		* toggleToolBarDisplay
		*
		* Toggle toolbar by show and hide
		*/
		virtual void toggleToolBarDisplay();

		/**
		* isToolBarVisible
		*
		* Check which toolbar is visible
		*
		* @return bool	true or false
		*/
		virtual bool isToolBarVisible() const;

		/**
		* isToolBarEnabled
		*
		* Check which toolbar is enabled
		*
		* @return bool	true or false
		*/
		virtual bool isToolBarEnabled() const;

	protected:

		/**
		* initialiseToolBarList
		*
		* Initialise vector list of QToolbar
		*
		* @param toolBarParent  Parent widget of toolbar
		*/
		virtual void initialiseToolBarList(Ui_PlanManagerFrame* toolBarParent) = 0;

		/**
		* addToolBarToList
		*
		* Add toolbar to vector list
		*
		* @param toolBar
		*
		* @return QToolBar  Added toolbar
		*/
		virtual QToolBar *addToolBarToList(QToolBar* toolBar);

		/**
		* addToolBarToList
		*
		* Add toolbar to vector list
		*
		* @param toolBarIdx  Index integer for toolbar list
		*/
		virtual void selectToolBar(int toolBarIdx);

	private:
		typedef std::vector<QToolBar*> ToolBarList; /// vector list of QToolBar objects

	protected:
		ToolBarList m_toolBars;	/// reference to ToolBarlist
		int m_selectedToolBarIdx; /// used to access toolbar

		bool m_isToolBarVisible; /// used to toggle toolbar display
	};
} // namespace TA_Base_App

#endif // ABSTRACTTOOLBARMANAGER_H
