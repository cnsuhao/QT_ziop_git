/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_manager/src/ComponentLibraryAccessor.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class accesses the component libraries to retrieve and set up the Component structure.
  * These can then be retrieved for processing.
  *
  * Adapted from Configuration Editor
  *
  */

#pragma warning(disable:4786)

#include "app/scheduling/scheduling_manager/src/StdAfx.h"
#include "app/scheduling/scheduling_manager/src/ComponentLibraryAccessor.h"
#include "app/scheduling/scheduling_manager/src/IJobDetailsPlugin.h"
#include "app/scheduling/scheduling_manager/src/SchedulingManagerConstants.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "core/utilities/src/DllHandler.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"

using TA_Base_Core::DebugUtil;
using namespace TA_Base_App::SchedulingManager;

namespace TA_Base_App
{
    ComponentLibraryAccessor::ComponentLibraryAccessor(std::vector<std::string>& jobTypes)
    {
        for (unsigned int i = 0; i < jobTypes.size(); i++)
        {
            Component comp;
            TAUnicodeString temp;
			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(jobTypes[i].c_str());
            AfxFormatString1(temp.str(), IDS_DLL_PATH_FORMAT, tmpUniStr.c_str());  // set its library path
            comp.path = temp.toMultiByteStdString();

            AfxFormatString1(temp.str(), IDS_HELP_PATH_FORMAT, tmpUniStr.c_str());  // set its help path
            comp.help = temp.toMultiByteStdString();

            comp.plugin = NULL;

            m_componentMap.insert(ComponentMap::value_type(jobTypes[i], comp));
        }
    }


    ComponentLibraryAccessor::~ComponentLibraryAccessor()
    {
    }


    IJobDetailsPlugin* ComponentLibraryAccessor::getComponent(std::string& name)
    {
        FUNCTION_ENTRY("getComponent");

        // See if we could find the component.  
        TA_ASSERT(m_componentMap.find(name) != m_componentMap.end(), "Couldn't find the component");

        Component comp = m_componentMap[name];
        // if we've already loaded the component, then just return it.
        if (comp.plugin != NULL)
        {
            return comp.plugin;
        }

        // Load the DLL
		TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(comp.path.c_str());
        comp.instance = AfxLoadLibrary(tmpUniStr.c_str());
        if (NULL == comp.instance)
	    {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Could not load the plugin %s", name.c_str());
            return NULL;
	    }

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "%s plugin loaded successfully", name.c_str());

	    // Load the DLL function.
        typedef IJobDetailsPlugin* (*pfnCreateComponent)();

	    pfnCreateComponent DLLCreateComponent = NULL;
	    DLLCreateComponent = reinterpret_cast<pfnCreateComponent>(::GetProcAddress(comp.instance, "createComponent"));
	    if (NULL == DLLCreateComponent)
	    {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Could not load function in the plugin %s", name.c_str());
            FreeLibrary(comp.instance);
		    comp.instance = NULL;
            return NULL;
	    }

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "createComponent() function loaded successfully for plugin %s", name.c_str());

        // Now call the DLL function to get the IJobDetailsPlugin interface object
        comp.plugin = DLLCreateComponent();
        TA_ASSERT(comp.plugin != NULL, "The component created is NULL");

        // Construct the DLL handler that we can pass when initialise the singletons in the DLLs.
        TA_Base_Core::DllHandler dllHandler;
        comp.plugin->setUpSingletons(dllHandler, name);

        FUNCTION_EXIT;
        return comp.plugin;
    }
};
