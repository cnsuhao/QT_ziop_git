//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/ITreeNodeFilter.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if !defined(AFX_ITREENODEFILTER_H__87C87CF2_1969_4CC7_9E16_C6A24E9D3B96__INCLUDED_)
#define AFX_ITREENODEFILTER_H__87C87CF2_1969_4CC7_9E16_C6A24E9D3B96__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000


namespace TA_Base_App
{
    class TreeNode;


    /////////////////////////////////////////////////////////////////////////////
    // ITreeNodeFilter Interface

    class ITreeNodeFilter
    {
    public:
        virtual bool isRelevant(TreeNode &treeNode) = 0;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_ITREENODEFILTER_H__87C87CF2_1969_4CC7_9E16_C6A24E9D3B96__INCLUDED_)