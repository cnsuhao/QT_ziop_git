/*
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/MainToolBarManager.h $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* Main toolbar manager for Plan Manager.
*/

#ifndef MAINTOOLBARMANAGER_H)
#define MAINTOOLBARMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlanManagerCommonDefs.h"

#include "AbstractToolBarManager.h"
#include "ui_planManagerFrame.h"

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// MainToolBarManager

	class MainToolBarManager
		: public AbstractToolBarManager
	{
		Q_OBJECT

	public:
		/// toolBarType may be TBT_CATEGORY, TBT_PLAN, or TBT_NONE
		typedef enum
		{
			TBT_CATEGORY = 0,
			TBT_PLAN,
			TBT_NONE
		}
		EToolBarType;

	public:
		MainToolBarManager();
		virtual ~MainToolBarManager();

		virtual void selectToolBar(EToolBarType toolBarType);

		/**
		* hideToolBar
		*
		* Hide toolbar
		*/
		void hideToolBar();

	protected:
		virtual void initialiseToolBarList(Ui_PlanManagerFrame* toolBarParent);
	};
} // namespace TA_Base_App

#endif // MAINTOOLBARMANAGER_H
