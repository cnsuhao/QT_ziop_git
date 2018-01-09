#include "ControlListCtrlModel.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include <atltime.h>
#include "core/exceptions/src/ScadaProxyException.h"
#include "core/exceptions/src/ObjectResolutionException.h"
#include "core/exceptions/IDL/src/CommonExceptionsCorbaDef.h"
#include "core/corba/src/CorbaUtil.h"
#include <atltime.h>
#include "DataPointControlHelper.h"
#include "inspectorpanelmodeleventargs.h"
#include "convertFunction.h"
namespace TA_Base_App
{

	ControlListCtrlModel::ControlListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: AbstractEquipmentPModel(appController, pmodelName)

	{

	}


	ControlListCtrlModel::~ControlListCtrlModel(void)
	{
		
	}


	ControlItem ControlListCtrlModel::getControlListCtrlItem(unsigned int iPoint)
	{
		return m_displayLogic->getControlDisplayItem(iPoint);
	}


	std::vector<ControlItem>	ControlListCtrlModel::getAllControlDisplayItem()
	{
		return m_displayLogic->getAllControlDisplayItem();
	}


	int	ControlListCtrlModel::updateControlState(std::vector<std::string>& parameters)
	{
		return m_displayLogic->constructControlStateString(parameters);
	}



	bool	ControlListCtrlModel::isControlPermit(unsigned int iPoint)
	{
		bool rtn = false;
		std::map<unsigned int, bool>::iterator it = m_setValueActionAllowed.find(iPoint);

		if (it != m_setValueActionAllowed.end())
		{
			rtn = it->second;
		}
		return rtn;
	}

	


	std::string ControlListCtrlModel::getControlStatusString ( TA_Base_Bus::EDataPointControlState state ) 
	{
		return ConvertFunction::convertDataPointControlStateToString(state);
	}

	void	ControlListCtrlModel::setValue(unsigned int iPoint, const std::string& strValue)
	{
		std::string responseString;
		//bool result = 
		m_outputLogic->setValue(iPoint,strValue,responseString);
		//this->postStatusLine(responseString);
		//return result;
	}


	bool ControlListCtrlModel::isOkToSendControl( unsigned int iPoint, const std::string& strValue)
	{
		return m_outputLogic->isOkToSendControl(iPoint,strValue);
	}

	void	ControlListCtrlModel::onInitStatusUpdate (EquipmentInitStatus status)
	{
		if(status == INIT_STEP5_COMPLETE)
		{
			m_setValueActionAllowed.clear();
			std::vector<unsigned long>& vectorIndex = m_displayLogic->getWriteableIndex();
			for(unsigned int index = 0;index < vectorIndex.size(); ++index)
			{
				m_setValueActionAllowed.insert( std::map<unsigned int, bool>::value_type(vectorIndex[index], false) );
			}
			this->notifyViews(PMODEL_NOTIFY_REFRESH_ALL_DATA,NULL);
		}
	}


	void	ControlListCtrlModel::onPertmissionUpdate()
	{
		m_setValueActionAllowed = m_displayLogic->getAllowedToWriteMap();
	}


	void	ControlListCtrlModel::onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		UINT iPoint = m_displayLogic->getChildDataPointIndexFromEntityKey(key);
		if(!m_displayLogic->isWritableDp(iPoint) )
		{
			return;
		}

		
		switch (updateType)
		{
		case InhibitStateUpdate:
			{
				//  Find the DataPoint item with the key
				
				std::map<unsigned int, bool>::iterator it = m_setValueActionAllowed.find(iPoint);
				// if found
				if (it != m_setValueActionAllowed.end())
				{
					bool isAllowedToWrite = m_displayLogic->getDpAllowedToWrite(iPoint);
					if( (*it).second != isAllowedToWrite)
					{
						(*it).second = isAllowedToWrite;
						TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoint);
						this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
					}
				}
			}
			break;

		case ControlStateUpdate:
			{
				std::vector<std::string> parameters;
				int MsgId = updateControlState(parameters);
				this->postStatusLine(MsgId,  parameters);
				
				TA_Base_Bus::EventArgs* pEventArgs = new InspectorPanelModelEventArgs(iPoint);
				this->notifyViews(PMODEL_NOTIFY_UPDATE_DATA,pEventArgs);
			}
			break;

		default:
			break;
		}
	}


	bool ControlListCtrlModel::isValid(unsigned int iPoint)
	{
		bool rtn = false;
		std::map<unsigned int, bool>::iterator it = m_setValueActionAllowed.find(iPoint);
		// if found
		if (it != m_setValueActionAllowed.end())
		{
			rtn = true;
		}
		return rtn;
	}

	std::string	ControlListCtrlModel::getEquipmentDescription()
	{
		std::string rtnString;

		rtnString	= m_displayLogic->getDataNodeAssetName();
		rtnString	+= "\r\n";
		rtnString	+= m_displayLogic->getDataNodeLabel();
		return rtnString;
	}


	EPModelCellType		ControlListCtrlModel::getPMCellTypeFromPoint(unsigned int iPoint)
	{
		// If pending, disable the cell to prevent the next control to be sent
		if (m_displayLogic->isControlStateInPending(iPoint) )
		{
			return PMCT_NOTEDITABLE;
		}
		// Check for rights
		if (!m_displayLogic->getDpAllowedToWrite(iPoint) )
		{
			return PMCT_NOTEDITABLE;
		}
		// Check if boolean, text or combo
		if (m_displayLogic->isBoolean(iPoint) || m_displayLogic->isDerived(iPoint))
		{
			return PMCT_SELECTION;
		}
		else if (m_displayLogic->isNumber(iPoint))
		{
			return PMCT_NUMBER;
		}
		else if (m_displayLogic->isText(iPoint))
		{
			return PMCT_TEXT;
		}
		return PMCT_NOTEDITABLE;
	}

	std::vector<std::string>	ControlListCtrlModel::getCellValues(unsigned int iPoint)
	{
		return m_displayLogic->getAllValues(iPoint);
	}

// 	void ControlListCtrlModel::updateControlState()
// 	{
// 		this->getLatestControlString();
// 		std::string latestControlStatus = this->getLatestControlString();
// 		if(latestControlStatus.size() > 0)
// 		{
// 			this->postStatusLine(latestControlStatus );
// 		}		
// 	}
}