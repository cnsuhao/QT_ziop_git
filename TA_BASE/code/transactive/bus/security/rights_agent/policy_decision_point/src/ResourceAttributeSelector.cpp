/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/rights_agent/policy_decision_point/src/ResourceAttributeSelector.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "PolicyDecisionPoint.h"

namespace TA_Base_Bus
{
	ResourceAttributeSelector::ResourceAttributeSelector( const std::string& attribName ) :
		m_attribName(attribName)
	{
	}

	ValueSet ResourceAttributeSelector::getValues( const Context& context ) const
	{
		return( context.select( *this ) );
	}

} // TA_Base_Bus
