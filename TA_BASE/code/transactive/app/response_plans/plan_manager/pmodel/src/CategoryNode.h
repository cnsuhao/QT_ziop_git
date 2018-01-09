    // 
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/CategoryNode.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// This is a representation of a category node within the plan tree.

#if !defined(AFX_CATEGORYNODE_H__5D70BCA1_A160_4D0A_8772_2B9EF0128C84__INCLUDED_)
#define AFX_CATEGORYNODE_H__5D70BCA1_A160_4D0A_8772_2B9EF0128C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "app\response_plans\plan_manager\pmodel\src\SynchronisedMap.h"
#include "TreeNode.h"
#include "PlanNode.h"

//#include <QTreeWidget>

//#include <QMetaType>

namespace TA_Base_App
{
	class CategoryNodeView;
    /////////////////////////////////////////////////////////////////////////////
    // CategoryNode

    class CategoryNode : public TreeNode
    {
    // Operations
    public:
        // Constructors and destructors
        CategoryNode(const TA_Base_Core::NodeSummary &nodeSummary);
		CategoryNode(const CategoryNode &other);
        virtual ~CategoryNode();

        // Summary info getters
        virtual const std::string &getNodeType();   // Returned by reference to enable setting CategoryNodeView item labels by callback
        virtual const std::string &getNodeStatus(); // Returned by reference to enable setting CategoryNodeView item labels by callback

        // Action methods
        //virtual void importPlan (TA_Base_Core::AuxPlanDetail& plan);
        //virtual void newPlan();
        //virtual void copyPlan(PlanNode &planNode, bool overwrite);
        //virtual void movePlan(PlanNode &planNode, bool overwrite);
        //virtual void newCategory();
        //virtual void deleteCategory();

        // Enable actions that can be performed on this category
        //virtual void updateCmdUI(CCmdUI &cmdUI);

        // Return an image type for the corresponding tree item
		virtual const std::string getNodeImageType(bool isExpanded);

        // Facilitate in-tree renaming of nodes.
        virtual void renameNode(std::string nodeName);

        // Plan config data update handler
        //virtual void updateChildNodeList(const TA_Base_Core::PlanConfigUpdateData &planConfigData);

          virtual void getChildren(TreeNodeList &childNodes);

		  virtual bool hasPrecedence(TreeNode &siblingNode);
    protected:
        virtual void invalidateDescendantInfo(); // Reset summary information about descendants. Recurses up the ancestry line.        
        virtual void createChildren(TreeNodeList &childNodes);
        virtual void addToChildNodeList(const TA_Base_Core::NodeSummary &nodeSummary);
        virtual void removeFromChildNodeList(TreeNodeId nodeId);
        virtual TA_Base_Core::CategoryDetail &getCategoryDetail();

    private:
        CategoryNode &operator=(const CategoryNode &other);


    // Attributes
    private:
		SynchronisedMap<TreeNodeId, TreeNode *> *m_childNodeMap;
		CategoryNodeView* m_pCategoryNodeView;


    };

} // namespace TA_Base_App

//Q_DECLARE_METATYPE(TA_Base_App::CategoryNode*)

#endif // !defined(AFX_CATEGORYNODE_H__5D70BCA1_A160_4D0A_8772_2B9EF0128C84__INCLUDED_)
