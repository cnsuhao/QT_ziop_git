/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanNode.h $
  * @author:  Bart Golab
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/11/21 13:03:08 $
  * Last modified by:  $Author: hoa.le $
  *
  * This is a representation of a plan node within the plan tree.
  *
  **/

#if !defined(AFX_PLANNODE_H__7FC18459_AC18_4BBF_B5A7_39BA7FD76656__INCLUDED_)
#define AFX_PLANNODE_H__7FC18459_AC18_4BBF_B5A7_39BA7FD76656__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "TreeNode.h"
//#include "PlanInstanceNone.h"

//#include "bus/common_control/src/itemview/CtlTreeWidget.h"
//#include <QTreeWidget>

//#include <QMetaType>

// Command UI enable state tester
//struct CCmdUIProbe : public CCmdUI //QTTODO2
//{
//	CCmdUIProbe(int cmdID)
//	{
//		m_nID = cmdID;
//		m_enabled = TRUE;
//	}
//
//	void Enable(BOOL value)
//	{
//		m_enabled &= value;
//	}
//
//	BOOL m_enabled;
//};


namespace TA_Base_App
{
 //   class CPlanGeneralDetailPage;
 //   class CPlanStepDetailPage;
 //   class CPlanStepListCtrl;
 //   class CPlanInstanceComboBox;
 //   class InteractiveStep;
 //   class PlanStep;
 //   class PlanStepMap;
    class PlanInstanceMap;
    class PlanInstanceFilter;

	//class CPlanNodeView;

    /////////////////////////////////////////////////////////////////////////////
    // PlanNode

    class PlanNode : public TreeNode
    {
    // Operations
    public:
        // Constructors and destructors
		PlanNode(){};
        PlanNode(const TA_Base_Core::NodeSummary &nodeSummary);
        PlanNode(const PlanNode& planNode); // Copy constructor (deep copy)
        virtual ~PlanNode();

        // Summary info getters
        virtual const std::string &getNodeType();   // Returned by reference to enable setting CategoryNodeView item labels by callback
        virtual const std::string &getNodeStatus(); // Returned by reference to enable setting CategoryNodeView item labels by callback

        // Get plan step details
        PlanStep* getPlanStep(PlanStepId stepId);
        //PlanStepNumber getLastStepPosition();

        // Plan management functions
        //void approvePlan(bool isApproved);
        //void schedulePlan();
        //void deletePlan();
        //void savePlan();
          //TA_Base_Core::AuxPlanDetail* exportPlan ();
          //void insertPlanStep(TA_Base_Core::EStep stepType, PlanStepNumber position);
        //void deletePlanSteps(PlanStepList &deleteSteps);
        //void copyPlanSteps(const PlanStepList &copySteps, PlanStepNumber position);
        //void movePlanSteps(const PlanStepList &moveSteps, PlanStepNumber position);

        // Plan execution control
        //void startNewInstance();
        //void customiseNewInstance();
        //void executeInteractiveStep(InteractiveStep& interactiveStep);
        //void controlPlan(bool ignoreActivePlanControllerLimit = false);
        //void controlPlan(const PlanInstanceId &planInstanceId);

        // Plan editing
        //bool isCopiedPlan() { return m_isCopiedPlan; }
        //void editPlan();
        //void setPlanName(std::string  name);
        //void setPlanDescription(std::string description);
        //bool editPlanStep(PlanStep* planStep);
        //void notifyPlanGeneralDetailUpdate();
        //void notifyPlanStepUpdate(PlanStepNumber stepNumber, unsigned long stepCount);

        // Active instance access
        PlanInstanceSharedPtr getPlanInstance(const PlanInstanceId &planInstanceId);

        // Enable actions that can be performed on this plan
        //virtual void updateCmdUI(CCmdUI &cmdUI);

        // Return an image type for the corresponding tree item
		virtual const std::string getNodeImageType(bool isExpanded);

        // Window widgets modifiers
		/**
		* getAllLocationPlans
		*
		* Get List of all Plan instances from all locationss
		*
		* @param filter  DESCRIPTION
		*
		* @return PlanInstanceList  DESCRIPTION
		*/
		PlanInstanceList* getAllLocationPlans(const PlanInstanceFilter &filter);
        //virtual void updatePlanView(CPlanStepDetailPage &stepDetailPage, const PlanInstanceFilter &filter);
    
        //virtual void updatePlanInstanceCombo(CPlanInstanceComboBox &instanceComboBox, const PlanInstanceFilter &filter);
        //virtual void addStepsToStepList(CPlanStepListCtrl& planStepsListCtrl);

        // Facilitate in-tree renaming of nodes.
        //virtual void renameNode(std::string nodeName);

        // Active plan, plan config and plan customisation update handlers
        //virtual void updatePlanInstance(const TA_Base_Core::ActivePlanDetail &activePlanDetail);
        //virtual void updatePlanInstance(const TA_Base_Core::PlanCustomisationUpdate &planCustomDetail);
        //virtual void updateNode(const TA_Base_Core::PlanConfigUpdateData &planConfigData);
        //virtual void updateChildNodeList(const TA_Base_Core::PlanConfigUpdateData &planConfigData);

		virtual bool hasPrecedence(TreeNode &siblingNode); //Moved from protected

		virtual TA_Base_Core::PlanDetail &getPlanDetail();

    protected:                
        virtual PlanInstanceMap &getPlanInstanceMap(LocationKey location);
        virtual PlanInstanceMap &getLocalPlanInstanceMap();
        virtual PlanInstanceMap &getRemotePlanInstanceMap();
        //virtual PlanStepMap &getPlanStepMap();
        //virtual void invalidatePlanStepMap();

    private:
        PlanNode &operator=(const PlanNode &other);


    // Attributes
    private:
        //bool                    m_isCopiedPlan;
        //bool                    m_areStepsUpdated;
        PlanInstanceMap*        m_localPlanInstanceMap;   // instances at the profile locations
        PlanInstanceMap*        m_remotePlanInstanceMap;  // instances at all other locations
        //PlanStepMap*            m_planStepMap;
        //PlanInstanceNone        m_planInstanceNone;

		//CPlanNodeView* m_pPlanNodeView;
    };

} // namespace TA_Base_App

//Q_DECLARE_METATYPE(TA_Base_App::PlanNode*)

#endif // !defined(AFX_PLANNODE_H__7FC18459_AC18_4BBF_B5A7_39BA7FD76656__INCLUDED_)
