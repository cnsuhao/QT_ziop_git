/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanTreeModel.h $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2016/10/18 15:45:45 $
* Last modified by:  $Author: huirong.luo $
*
* <description>
*/

#ifndef PLANTREEDOC_H
#define PLANTREEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"
#include "PlanBrowserService.h"
//#include "PlanNode.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
	class TreeNode;
	//class PlanNode;
	class RootNode;

	/////////////////////////////////////////////////////////////////////////////
	// PlanTreeModel document

	class PlanTreeModel : public PlanBrowserService,
		public TA_Base_Bus::AbstractPModel
	{
		// Construction
	public:
		PlanTreeModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);        // protected constructor used by dynamic creation
		virtual ~PlanTreeModel();

		std::string& GetRootNodeLabel(EPlanTreeType eTreeType);

		//void AddToCategoryViewNodes(TreeNode *treeNode);
		//void RemoveFromCategoryViewNodes(TreeNode *treeNode);
		void PurgeCategoryViewNodes();
		bool ArePlanNodesSelected();
		TreeNodeList GetSelectedNodes();

		RootNode* createRootNode();

		bool isRelevant(TreeNode &treeNode);

		void addToCategoryViewNodes(TreeNode *treeNode);
		void removeFromCategoryViewNodes(TreeNode *treeNode);

	protected:
		virtual void allocViewResource();
		virtual void releaseViewResource();

		// private:
		//     // replace all new-line chars with spaces (implemented in OnExportPlan())
		//     void ReplaceNLChars (CString& value);

		//// Attributes
	private:
		TreeNodeList m_copyNodes;

		typedef std::map<TreeNodeId, TreeNode *> TreeNodeMap;
		TreeNodeMap m_categoryViewNodeMap;  // Contains the nodes selected in the CategoryNodeView shown for
		//                                        // a category selected in the plan tree.
		EPlanTreeType m_ePlanType;
	};

	/////////////////////////////////////////////////////////////////////////////

	// {{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
} // namespace TA_Base_App

#endif // !defined(AFX_PLANTREEDOC_H__C12FC01D_7368_4455_88DA_15412D4B3105__INCLUDED_)
