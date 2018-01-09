#include "StdAfx.h"
#include "Application.h"
#include "Application.h"
#include "core/data_access_interface/src/IGui.h"

Application* Application::create(TA_Base_Core::IGui* gui)
{
    return new Application(gui);
}
