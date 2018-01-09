/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in
  * any form.
  *
  * Source:		$File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scada/OpcBridgeLib/attributes/DnShortLabel.h $
  * @author:	Darren Sampson
  * @version:	$Revision: #1 $
  *
  * Last modification:	$DateTime: 2015/10/09 15:42:35 $
  * Last modified by:	$Author: CM $
  *
  * Description:
  * Class representing the Short Label attribute of a DataNode
  *
  */

#ifndef DNSHORTLABEL_H
#define DNSHORTLABEL_H

#include "app/scada/opcbridgelib/src/Attribute.h"

namespace TA_Base_App
{
    class DataNodeAttributes;

	class DnShortLabel : public Attribute
	{
	public:

		//------------------------------------------------------------------------------------------
		// Operation: DnShortLabel
		//	Constructor
		//------------------------------------------------------------------------------------------
		DnShortLabel(DataNodeAttributes* dnAttribute, const std::string& tagName, const std::string& defaultValue);

		//------------------------------------------------------------------------------------------
		// Operation: ~DnShortLabel
		//	Destructor
		//------------------------------------------------------------------------------------------
		virtual ~DnShortLabel();

		virtual EOPCATTRIBUTETYPE getAttributeType() {return E_OPC_DN_SHORTLABEL;}

	private:
		//DisplayDataNodeProxy* m_proxy;
		DataNodeAttributes*		m_dnAttribute;
	};
}


#endif
