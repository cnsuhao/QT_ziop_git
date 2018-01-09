/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/banner/pmodel/src/BannerAppController.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __BANNER_APPLICATION_CONTROLLER_H_INCLUDED__
#define __BANNER_APPLICATION_CONTROLLER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"

// forward declaration
namespace TA_Base_Core
{
	class Banner;
}

namespace TA_Base_App
{
	class BannerAppController
		: public TA_Base_Bus::AbstractGUIAppController
	{
	public: //Methods
		/**
          * Constructor
          */
		BannerAppController(TA_Base_Bus::ITransActiveApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~BannerAppController();


        /**
          * getApplicationType
          * 
          * This returns the application type for this GUI. GenericGUI will then
          * save this in the RunParams and will use it to pass to the Control Station. 
          * In the existing system the application type would need to be hard-coded 
          * into here. This method must be implemented by the lowest level class.
          *
          * @return unsigned long representing the application type
          *
          */
	    virtual unsigned long getApplicationType();

        virtual TA_Base_Bus::ITransActivePModel * createPModel(const std::string & pModelName);

		virtual  void checkEntity(TA_Base_Core::IEntityData* guiEntity);

		TA_Base_Core::Banner* getBannerEntity() { return m_bannerEntity; };
    protected:
        void cleanup();

        void allocAppResource();
       

	public: // Variables

	private: // Methods
		// Copy constructor and operator= made private to prevent them being used.
		BannerAppController(const BannerAppController&);
		BannerAppController& operator=(const BannerAppController&);

	private: // Variables
		TA_Base_Core::Banner* m_bannerEntity; // banner configuration
	};
}

#endif //__BANNER_APPLICATION_CONTROLLER_H_INCLUDED__
