/**
  * The source code in this file is the property of
  * Combuilder PTE LTD and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/banner/pmodel/src/BannerAppController.cpp $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#include "app/system_control/banner/pmodel/src/BannerAppController.h"
#include "app/system_control/banner/pmodel/src/BannerPModelConst.h"
#include "app/system_control/banner/pmodel/src/BannerDlgPModel.h"

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/application_types/src/apptypes.h"
#include "core/utilities/src/DebugUtil.h"

#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

//DTL-Bannner-PositionWrong - start
#include "core/data_access_interface/entity_access/src/Banner.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"
#include "core\exceptions\src\EntityTypeException.h"
#include "core\data_access_interface\entity_access\src\EntityAccessFactory.h"

using TA_Base_Core::TransactiveException;
using TA_Base_Core::DatabaseException;
using TA_Base_Core::DataException;
using TA_Base_Core::EntityTypeException;
using TA_Base_Core::IEntityData;
//DTL-Bannner-PositionWrong - end

using TA_Base_Core::Banner;
using TA_Base_Core::GenericGUIException;
using TA_Base_Core::DebugUtil;

namespace TA_Base_App
{

	BannerAppController::BannerAppController(TA_Base_Bus::ITransActiveApp * pApp, const char * appName)
		: TA_Base_Bus::AbstractGUIAppController(pApp, appName)
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "BannerAppController::Constructor");
	}

	BannerAppController::~BannerAppController()
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "BannerAppController::Destructor");
	}

	unsigned long BannerAppController::getApplicationType()
	{
		return BANNER_GUI_APPTYPE;
	}

	void BannerAppController::checkEntity(TA_Base_Core::IEntityData* guiEntity)
	{
		m_guiEntity = guiEntity;

		m_bannerEntity = dynamic_cast<Banner*>(guiEntity);
		if(m_bannerEntity == NULL)
		{
			TA_THROW(GenericGUIException("Entity configuration is not of type Banner!", GenericGUIException::ENTITY_CONFIGURATION_INVALID));
		}
	}

	void BannerAppController::allocAppResource()
	{
		// DTL-Bannner-PositionWrong - start
		// replace code to directly access the banner entity from database and position the banner window
		// to its correct position and avoid displaying the banner on the center of the screen.

		if (m_bannerEntity!=NULL) return;

		//
		// Retrieve the entity name from RunParams to ensure it was on the command line.
		//
		std::string entityName = TA_Base_Core::RunParams::getInstance().get( RPARAM_ENTITYNAME );
		if ( entityName.empty() )
		{
			TA_THROW ( GenericGUIException( "No entity specified on command line (ie --entity-name not found)", GenericGUIException::COMMAND_LINE_INVALID ) );
		}

		//
		// Load the entity and check the configuration is correct
		//
		try
		{
			TA_Base_Core::IEntityData* guiEntity;
			guiEntity = TA_Base_Core::EntityAccessFactory::getInstance().getEntity( entityName );
			TA_ASSERT ( guiEntity != NULL, "EntityAccessFactory returned a NULL entity and yet did not throw an exception." );

			m_bannerEntity = dynamic_cast<Banner*>(guiEntity);
			if(m_bannerEntity == NULL)
			{
				TA_THROW(GenericGUIException("Entity configuration is not of type Banner!", GenericGUIException::ENTITY_CONFIGURATION_INVALID));
			}
// 			getApplicationWnd()->MoveWindow( m_bannerEntity->getLeftPosition(), m_bannerEntity->getTopPosition(), 
// 				m_bannerEntity->getWidth(), m_bannerEntity->getHeight() );
		}
		catch ( const DatabaseException& )
		{
			LOG( SourceInfo, DebugUtil::ExceptionCatch,"DatabaseException", "Will change this exception into a GenericGUIException. We don't care what happened just that we can't connect to the database" );
			TA_THROW ( GenericGUIException( GenericGUIException::NO_DATABASE_CONNECTION ) );
		}
		catch ( const EntityTypeException& )
		{
			LOG(SourceInfo,DebugUtil::ExceptionCatch,"EntityTypeException","Will change this exception into a GenericGUIException.");
			TA_THROW ( GenericGUIException( GenericGUIException::ENTITY_CONFIGURATION_INVALID ) );
		}
		catch ( const TA_Base_Core::DataException& ex )
		{
			LOG( SourceInfo, DebugUtil::ExceptionCatch,"DataException", "Will change this exception into a GenericGUIException." );
			switch ( ex.getFailType() )
			{
			case ( DataException::WRONG_TYPE ):
				TA_THROW ( GenericGUIException( GenericGUIException::ENTITY_CONFIGURATION_INVALID ) );
				break;
			case ( DataException::NO_VALUE ):
				TA_THROW ( GenericGUIException( GenericGUIException::ENTITY_DOES_NOT_EXIST ) );
				break;
			case ( DataException::NOT_UNIQUE ):
				TA_THROW ( GenericGUIException( GenericGUIException::ENTITY_NOT_UNIQUE ) );
				break;
			default:
				TA_ASSERT(false, "Do not recognise the DataException type");
			}
		}
		//setWindowPosition();
		// DTL-Bannner-PositionWrong - end 		
	}

	void BannerAppController::cleanup()
	{
		// Todo:
	}

	TA_Base_Bus::ITransActivePModel * BannerAppController::createPModel( const std::string & pModelName )
	{
		TA_Base_Bus::ITransActivePModel * pModel = NULL;
		if (pModelName == PMODEL_BANNER_DLG)
		{
			pModel = new BannerDlgPModel(m_bannerEntity->getLeftPosition(), m_bannerEntity->getTopPosition(), 
				m_bannerEntity->getWidth(), m_bannerEntity->getHeight(), this, pModelName.c_str());
		}
		else
		{
			std::string errorDesc = "presentation model name is not valid: " + pModelName;
			TA_ASSERT(FALSE, errorDesc.c_str());
		}
		return pModel;
	}

	
}