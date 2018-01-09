#pragma once

namespace TA_Base_Core
{
    class IGui;
}

class Application
{
public:

    virtual ~Application() {}
    virtual TA_Base_Core::IGui& getGui() = 0;
    static Application* create(TA_Base_Core::IGui* gui);
};
