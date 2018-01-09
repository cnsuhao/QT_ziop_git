/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/configuration/configuration_editor/src/RightsChecker.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class is used to determine if a user has access rights for a certain item. This could be modified
  * later to be used in the config_plugin_helper by making it print out nice error messages to the user
  * (as well as logging) before returning the result
  */

#ifndef CE_RIGHTS_CHECKER
#define CE_RIGHTS_CHECKER
#include <boost/shared_ptr.hpp>

namespace TA_Base_Bus
{
    class RightsLibrary;
    typedef boost::shared_ptr<RightsLibrary> RightsLibraryPtr;
}

namespace TA_Base_App
{
    class RightsChecker
    {
    public:

        /**
         * Constructor
         *
         * @exception GenericGUIException - This may throw a GenericGUI exception of type
         *                                  UNKNOWN_ACCESS_RIGHTS if the rights cannnot be set up
         *                                  correctly.
         */
        RightsChecker();

        /**
         * Destructor
         */
        ~RightsChecker();

        /**
         * canPerformAction
         *
         * This checks if the specified action can be performed on the Configuration Editor
         * by the current session. If there is no session because we are running connected straight
         * to the database this will always return true
         *
         * @param unsigned long- The action to test
         *
         * @return bool - true if the action can be performed, false otherwise
         */
        bool canPerformAction(unsigned long action);

    private:

        TA_Base_Bus::RightsLibraryPtr m_rightsLibrary;
        unsigned long m_configurationEditorResourceId;
    };
}

#endif CE_RIGHTS_CHECKER
