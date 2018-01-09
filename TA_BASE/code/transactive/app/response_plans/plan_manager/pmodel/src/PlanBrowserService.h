//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanBrowserService.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/10/18 15:45:45 $
// Last modified by:  $Author: huirong.luo $
//
// This is the base class for all documents that have interest in Plan Agent
// updates. To receive the updates, derived document classes must register
// with PlanAgentUpdateProcessor. Use subscribeForActivePlanUpdates() to
// register for plan execution updates and/or subscribeForPlanConfigUpdates()
// to register for plan configuration change updates.

#if !defined(AFX_PlanBrowserService_H__EB8B17F9_6F22_4656_987A_7540E74E5152__INCLUDED_)
#define AFX_PlanBrowserService_H__EB8B17F9_6F22_4656_987A_7540E74E5152__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include <map>
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "core/utilities/src/RunParams.h"
//#include "PlanManagerCommonDefs.h"
#include "IActivePlanObserver.h"
#include "IPlanConfigObserver.h"
#include "IPlanCustomisationObserver.h"
//#include "ViewUpdate.h"
#include "PlanInstanceFilter.h"
//#include "SynchronisedQueue.h"

#include "TreeNodeFactory.h"
#include "PlanNode.h"
#include "PlanInstance.h"

#include "app\response_plans\plan_manager\pmodel\src\PlanManagerConsts.h"


namespace TA_Base_App
{
    class TreeNode;
    class PlanInstance;
    class PlanStep;

    /////////////////////////////////////////////////////////////////////////////
    // PlanBrowserService document

    class PlanBrowserService : public IActivePlanObserver,
                            public IPlanConfigObserver,
                            public IPlanCustomisationObserver,
                            public TA_Base_Core::RunParamUser,
							public TreeNodeFactory,
							public PlanNode,
							public PlanInstance
    {
    // Construction
    protected:
        PlanBrowserService();          // protected constructor used by dynamic creation

    // Operations
    public:
        // TD18095,jianghp
        virtual void OnGenericGuiInitCompleted(){};
        // TD18095,jianghp
        bool IsNodeAccessible(TreeNodeId nodeId);
        bool IsNodeOnLockedCategoryPath(TreeNodeId nodeId);
        bool IsPlanTreeLocked();
        TreeNodeId GetLockedCategoryId();

        virtual void SetCurrentNode(TreeNode *node);
        TreeNode *GetCurrentNode() const;
		std::string getNodePathTitle();

        void setCurrentInstance(PlanInstanceSharedPtr& instance);
        //PlanInstanceSharedPtr& GetCurrentInstance();

        virtual PlanInstanceFilter* getPlanInstanceFilter();

        //void AddToCurrentSteps(PlanStep *planStep);
        //void RemoveFromCurrentSteps(PlanStep *planStep);
        //void PurgeCurrentSteps();
        //PlanStep* GetCurrentStep();
        //PlanStepList GetCurrentSteps();

		virtual bool editingEnabled() const;
		virtual bool controlEnabled() const;

        // IActivePlanObserver interface
        void activeStateChanged(const TA_Base_Core::ActivePlanDetail &activePlanDetail);

        // IPlanConfigObserver interface
        void planConfigChanged(const TA_Base_Core::PlanConfigUpdateData &planConfigData);
        void planTreeNodeDeleted(const TA_Base_Core::PlanConfigUpdateData &planConfigData);

        // IPlanCustomisationObserver interface
        void planCustomised(const TA_Base_Core::PlanCustomisationUpdate &planCustomDetail);
		
		/**
		* registerPlanSettings
		*
		* Register for active plan, plan customisation and plan configuration updates.
		* Register for ExecPlanId, LockCategory and SessionId runtime parameter updates.
		*/
		void registerPlanSettings();

		/**
		* deregisterPlanSettings
		*
		* Deregister for active plan, plan customisation and plan configuration updates.
		* Deregister for ExecPlanId, LockCategory and SessionId runtime parameter updates.
		*/
		void deregisterPlanSettings();

        // RunParamUser interface
        void onRunParamChange(const std::string& name, const std::string& value);

		bool canViewNonLocalActivePlans();



    // Attributes
    private:
        TreeNodeIdList m_lockCategoryPathNodes; // Node IDs on the path leading to the lock category.

        TreeNode* m_currentNode;              // Tree node associated with this document's view.

        // Used only when the view is a plan node view.
        PlanInstanceSharedPtr  m_currentInstance;      // Active instance currently selected in this document's view.
                                              // Only one instance may be selected at a time.

        PlanInstanceFilter m_planInstanceFilter; // Defines rules for displaying active instances in this
                                                 // document's view.

        //typedef std::map<PlanStepId, PlanStep *> CurrentStepMap;
        //CurrentStepMap m_currentStepMap;      // Plan step(s) currently selected in the view's step list.
                                              // In editing mode multiple steps may be selected.


        // Used in processing updates sent by the Plan Agent
        //typedef CViewUpdate<TA_Base_Core::ActivePlanDetail> ActivePlanDetailUpdate;
        //typedef CViewUpdate<TA_Base_Core::PlanConfigUpdateData> PlanConfigDataUpdate;
        //typedef CViewUpdate<TA_Base_Core::PlanCustomisationUpdate> PlanCustomisationUpdate;

        //SynchronisedQueue<ActivePlanDetailUpdate*> m_activePlanDetailUpdateQueue;
        //SynchronisedQueue<PlanConfigDataUpdate*> m_planConfigDataUpdateQueue;
        //SynchronisedQueue<PlanCustomisationUpdate*> m_planCustomisationUpdateQueue;

        //HWND m_parentFrameHWND;

    //public:
    //    virtual BOOL OnNewDocument();
    //    virtual void OnCloseDocument();

    // Implementation
    public:
        virtual ~PlanBrowserService();

    protected:
        //afx_msg void OnActivePlanDetailUpdate();
        //afx_msg void OnPlanConfigDataUpdate();
        //afx_msg void OnPlanCustomisationUpdate();
        //afx_msg void OnRparamExecPlanIdUpdate();
        //afx_msg void OnRparamViewPlanIdUpdate();
        //afx_msg void OnRparamAssocAlarmIdUpdate();
        //afx_msg void OnRparamLockCategoryUpdate();
        //afx_msg void OnRparamSessionIdUpdate();

    };

    /////////////////////////////////////////////////////////////////////////////

    // {{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

} // namespace TA_Base_App

#endif // !defined(AFX_PlanBrowserService_H__EB8B17F9_6F22_4656_987A_7540E74E5152__INCLUDED_)
