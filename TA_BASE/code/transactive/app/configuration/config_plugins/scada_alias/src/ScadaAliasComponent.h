/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/configuration/config_plugins/scada_alias/src/ScadaAliasComponent.h $
  * @author:  Wu Mintao
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This is the component the main Configuration Editor framework communicates with.
  * It implements the IConfigurableComponent interface. It will handle all toolbar and menu
  * requests and handle them appropriately.
  */

#if !defined(ScadaAliasComponent_A4EEC36D_18C5_4d02_927B_EC8EFFC7172B__INCLUDED_)
#define ScadaAliasComponent_A4EEC36D_18C5_4d02_927B_EC8EFFC7172B__INCLUDED_

#include <string>
#include <map>

#include "app/configuration/config_plugins/config_plugin_helper/src/AbstractComponent.h"
#include "app/configuration/config_plugins/config_plugin_helper/src/MessageSender.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"

namespace TA_Base_App
{

    class ScadaAliasComponent : public AbstractComponent
    {

    public:
        /** 
         * Constructor
         *
         * @param const string& - This is the name of the component displaying in the selection bar so we
         *                        can use the same name in the DLL for sending audit messages etc.
         */
        ScadaAliasComponent(const std::string& componentName);


        /**
         * Destructor
         */
	    virtual ~ScadaAliasComponent();


    public:
	    
	    /**
         * getIcon
         *
         * This returns the icon to be displayed in the selection bar.
         *
         * @return HICON - A handle to an icon that has been created
         */
	    virtual HICON getIcon();


	    /**
         * getComponentView
         *
         * This returns runtime class for the view to be shown in the bottom-right pane
         * of the application.
         *
         * @return CRuntimeClass* - This will be used to create the view in the correct pane.
         */
	    virtual CRuntimeClass* getComponentView();


        /**
         * getRightsActionForAddingThisComponent
         *
         * This should return the rights action the user must have to be able to view and use this
         * component. This action id will be passed straight to the rights library.
         *
         * @return unsigned long - The action id to use when checking whether to add this component
         *                         to the Configuration Editor.
         */
        virtual unsigned long getRightsActionForAddingThisComponent()
        {
            //Todo: return the correct rights action
            return TA_Base_Bus::aca_CE_CONFIGURE_OPC_TAG_ALIAS;
        }

    
    protected:

        /**
         * getDLLName
         *
         * This returns the name of the DLL.
         */
        virtual std::string getDLLName() const;


    private:
        /**
         * Copy Constructor
         *
         * Hidden as a private method so it is not used.
         */
        ScadaAliasComponent( const ScadaAliasComponent&);


        /**
         * Operator =
         *
         * Hidden as a private method so it is not used.
         */
        ScadaAliasComponent& operator=(const ScadaAliasComponent&);



    public:
        // This is the name of the DLL and is used to load resources correctly.
        static const CString DLL_NAME;

    private:

        // The icon for this component
        HICON m_icon;
    };
}

#endif // !defined(ScadaAliasComponent_A4EEC36D_18C5_4d02_927B_EC8EFFC7172B__INCLUDED_)
