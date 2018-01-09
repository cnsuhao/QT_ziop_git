/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerFrame.h $
* @author:  Bart Golab
* @version: $Revision: #5 $
*
* Last modification: $DateTime: 2012/10/08 17:44:33 $
* Last modified by:  $Author: qi.huang $
*
* This class is the main Transactive QMainWindow for Plan Manager.
*/

#ifndef PLANMANAGERMAINFRAME_H
#define PLANMANAGERMAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

#include <qprogressbar.h>

#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/GeneratedFiles/ui_planManagerFrame.h"
#include "app/response_plans/plan_manager/src/MainToolBarManager.h"

class QAction;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
class QToolBar;
class QProgressBar;

namespace TA_Base_Bus
{
	class ITransActiveApp;
}

namespace TA_Base_App
{
	class PlanBrowserFrame;
	class PlanManagerFrameModel;
	class TreeNode;

	/////////////////////////////////////////////////////////////////////////////
	// The main frame of the application

	class PlanManagerFrame : public TA_Base_Bus::AbstractTransActiveDialog
	{
		Q_OBJECT

	public:
		PlanManagerFrame(TA_Base_Bus::ITransActiveApp * app);
		virtual ~PlanManagerFrame();

		/**
		* addPlanBrowserFrame
		*
		* Save PlanBrowserFrame instance pointers to map list
		*
		* @param planView  Pointer to QAction plan type
		*
		* @param planBrowserFrame  Pointer to PlanBrowserFrame's instance
		*/
		void addPlanBrowserFrame(QAction* planView, PlanBrowserFrame* planBrowserFrame);

		/**
		* addPlanBrowserFrame
		*
		* Retreive PlanBrowserFrame instance pointers from map list
		*
		* @param planView  Pointer to QAction plan type
		*
		* @return PlanBrowserFrame  Pointer to PlanBrowserFrame's instance
		*/
		PlanBrowserFrame* getPlanBrowserFrame(QAction* planView);

	public slots :

		/**
		* onAppAbout
		*
		* Display about dialog
		*/
		void onAppAbout();
		
		/**
		* onViewToolbar
		*
		* Toggle display of toolbar
		*/
		void onViewToolbar();

		/**
		* onViewStatusbar
		*
		* Toggle display of statusbar
		*/
		void onViewStatusbar();

		/**
		* onViewPlan
		*
		* Select Plan Type to display
		*/
		void onViewPlan();

		/**
		* onTreeNodeSelectionChanged
		*
		* Change the menu items and toolbar buttons based on the selected tree node from PlanTreeView
		*
		* @param treeNode  Pointer to TreeNode
		*/
		void onTreeNodeSelectionChanged(TreeNode* treeNode);

		/**
		* onCategoryViewSelectionChanged
		*
		* Change the menu items and toolbar buttons based on the selected tree node from CategoryNodeView
		*
		* @param treeNode  Pointer to TreeNode
		*/
		void onCategoryViewSelectionChanged(TreeNode* treeNode);

	protected:
		/// Overrided TA methods
		virtual std::string getMyPModelName();
		void setupPModel();
		virtual void initUIControl();
		virtual void updateView();
		void doNotifyMessage(unsigned long messageID, TA_Base_Bus::TRA_WPARAM wParam, TA_Base_Bus::TRA_LPARAM lParam);
		virtual void allocAppResource();
		virtual void cleanup();

		/**
		* keyPressEvent
		*
		* Capture key press and toggle indicator display in status bar
		*
		* @param event  Pointer to QKeyEvent
		*/
		void keyPressEvent(QKeyEvent* event);

		/**
		* sessionIdChanged
		*
		* Bring main windows to foreground when session ID changed
		*/
		void sessionIdChanged();

	private:
		/// Overrided notifyUpdate for receiving notify view notifications
		virtual void notifyUpdate(const char* updateType, TA_Base_Bus::EventArgs* args = NULL);

		/**
		* initialiseKeyLockSize
		*
		* Initialise lock key indicators' labels
		*/
		QLabel * initialiseKeyLockSize();
		
		/**
		* initialiseKeyLockState
		*
		* Set initial lock key indicator's state
		*/		
		void initialiseKeyLockState();

		/**
		* toggleKeyLock
		*
		* Set and clear indicators' labels
		*
		* @param lblLock  Pointer to QLabel
		*
		* @param strLock  Reference to QString
		*/
		void toggleKeyLock(QLabel* lblLock, const QString strLock);

		/**
		* toggleKeyCapsLock
		*
		* Toggle Caps Lock indicator
		*/
		void toggleKeyCapsLock();

		/**
		* toggleKeyNumLock
		*
		* Toggle Num Lock indicator
		*/
		void toggleKeyNumLock();

		/**
		* toggleKeyScrollLock
		*
		* Toggle Scroll lock indicator
		*/
		void toggleKeyScrollLock();

		/**
		* onRegisterPlanManagerProgress
		*
		* Initialise status bar on startup
		*
		* @param wParam  Reference to TA_Base_Bus::TRA_WPARAM
		*
		* @param lParam  Reference to TA_Base_Bus::TRA_LPARAM
		*/
		void onRegisterPlanManagerProgress(TA_Base_Bus::TRA_WPARAM wParam, TA_Base_Bus::TRA_LPARAM lParam);

		/**
		* setMainTitle
		*
		* DESCRIPTION
		*
		* @param strTitle  String title of Plan Manager
		*/
		void setMainTitle(std::string strTitle);

		/**
		* selectStartupView
		*
		* Select the startup plan type view
		*/
		void selectStartupView();

		/**
		* setUpConnectionPoints
		*
		* Setpu signal and slots
		*/
		void setUpConnectionPoints();

		/**
		* addNewDocument
		*
		* Add document object to mdiArea
		*
		* @param ePlanType  Plan Type enum
		*/
		void addNewDocument(EPlanTreeType ePlanType);

		/**
		* togglePlanNodeActions
		*
		* Toggle menu items and toolbar button actions for plan node
		*
		* @param treeNode  Pointer to TreeNode
		*
		* @param isMixed  True when plan node selections are both approved and unapproved
		*/
		void togglePlanNodeActions(TreeNode* treeNode, bool isMixed = false);

		/**
		* toggleCategoryNodeActions
		*
		* Toggle menu items and toolbar button actions for category node
		*
		* @param treeNode  Pointer to TreeNode
		*/
		void toggleCategoryNodeActions(TreeNode* treeNode);

	private:
		QProgressBar* m_progressBar; /// Pointer QProgressBar
		MainToolBarManager m_toolBarManager; /// Reference to  MainToolBarManager
		Ui_PlanManagerFrame ui; /// Reference to Ui_PlanManagerFrame
		QLabel* m_lblReady; /// Pointer to Ready QLabel 
		QLabel* m_lblCapsLock; /// Pointer to Caps lock QLabel 
		QLabel* m_lblNumLock; /// Pointer to Num lock QLabel 
		QLabel* m_lblScrollLock; /// Pointer to Scroll QLabel 
		QStatusBar* m_statusBar; /// Pointer to QStatusBar
		QMdiSubWindow* subWindow; /// Pointer to QMdiSubWindow 
		QActionGroup* planGroup; /// Pointer to QActionGroup 

		EPlanTreeType m_currentPlanType; /// Reference to current plan type

		typedef std::map<QAction*, PlanBrowserFrame*> PlanBrowserFrameMap; /// Type define std::map for m_planBrowserFrameMap
		PlanBrowserFrameMap m_planBrowserFrameMap; /// std::map of PlanBrowserFrameMap

		PlanManagerFrameModel* m_realPModel; /// Pointer to PlanManagerFrameModel
	};

	/////////////////////////////////////////////////////////////////////////////
} // namespace TA_Base_App

#endif // PLANMANAGERMAINFRAME_H
