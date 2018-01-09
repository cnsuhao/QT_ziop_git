#pragma once

namespace TA_Base_App
{
    template<typename Model>
    struct IService
    {
        typedef Model Model;
        virtual ~IService() {}
        virtual void setModel(Model* model) { m_model = model; }
        Model* m_model;
    };
}
