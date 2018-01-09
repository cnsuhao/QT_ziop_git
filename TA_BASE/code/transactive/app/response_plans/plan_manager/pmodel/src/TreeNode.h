//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/TreeNode.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// This is a representation of a node within the plan tree.

#if !defined(AFX_TREENODE_H__7D5CB743_CFCD_406D_AE34_E1C7F44685A5__INCLUDED_)
#define AFX_TREENODE_H__7D5CB743_CFCD_406D_AE34_E1C7F44685A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
//#include <qobject.h>

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
//#include "MainToolBarManager.h"
#include "app\response_plans\plan_manager\src\PlanManagerCommonDefs.h"

#include "app\response_plans\plan_manager\pmodel\src\PlanManagerConsts.h"

namespace TA_Base_App
{

    /////////////////////////////////////////////////////////////////////////////
    // TreeNode

	class TreeNode
    {
    // Operations
    public:
        // Constructors and destructors
		TreeNode(){};
        TreeNode(const TA_Base_Core::NodeSummary &nodeSummary);
        virtual ~TreeNode();

        // Summary info getters
        virtual TreeNode *getParentNode();
        virtual TreeNodeId getNodeId() const;
        virtual TreeNodeId getParentId() const;
        virtual TA_Base_Core::NodeSummary getNodeSummary();
        virtual const std::string &getNodeName();       // Returned by reference to enable setting PlanTreeView/CategoryNodeView item labels by callback
        virtual const std::string &getNodeType() = 0;   // Returned by reference to enable setting CategoryNodeView item labels by callback
        virtual const std::string &getNodeStatus() = 0; // Returned by reference to enable setting CategoryNodeView item labels by callback
        virtual bool isApproved();

        // Enable actions that can be performed on this node
        //virtual void updateCmdUI(CCmdUI &cmdUI);

        // Return an image type for the corresponding tree item
		virtual const std::string getNodeImageType(bool isExpanded) = 0;
		
        // Facilitate in-tree renaming of nodes.
        //virtual void renameNode(std::string nodeName) = 0; //QTTODO2:

        // Plan config data update handlers
        virtual void updateNode(const TA_Base_Core::NodeSummary &nodeSummary);
        //virtual void updateNode(const TA_Base_Core::PlanConfigUpdateData &planConfigData);
        //virtual void updateChildNodeList(const TA_Base_Core::PlanConfigUpdateData &planConfigData) = 0;

        // Tree node comparison callback used in sorting
        //static int CALLBACK compareSiblingNodes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //QTTODO3: implemented from CPlanTreeView::SortChildren(HTREEITEM parentItem) <- CPlanTreeView::ProcessModifyNodeUpdate <- CategoryNodeView::OnUpdate
        bool isVisible();
        void setVisible(bool bVisible, bool writeToDb = true);

		bool isPlanPaste();

		virtual bool hasPrecedence(TreeNode &siblingNode) = 0;

    protected:
        virtual TA_Base_Core::NodeDetail &getNodeDetail();
        virtual void invalidateNodeName();
        virtual void invalidateNodeSummary();
        virtual void invalidateNodeDetail();        

    private:
        TreeNode(const TreeNode &other);
        TreeNode &operator=(const TreeNode &other);

		//std::string& getRootNodeLabel(EPlanTreeType eTreeType); // custom not needed?


    // Attributes
    private:
        TreeNodeId m_nodeId;
        TreeNodeId m_parentId;
        std::string *m_nodeName;
        TA_Base_Core::NodeSummary *m_nodeSummary;
        TA_Base_Core::NodeDetail *m_nodeDetail;
        bool m_nodeVisible;

		typedef enum { CNA_COPY_PLAN, CNA_CUT_PLAN, CNA_NO_ACTION } ECopyNodeAction;
		ECopyNodeAction m_copyNodeAction;

	//signals:
	//	void labelChanged(TreeNode* pNode);
    };	

	

} // namespace TA_Base_App


#endif // !defined(AFX_TREENODE_H__7D5CB743_CFCD_406D_AE34_E1C7F44685A5__INCLUDED_)
