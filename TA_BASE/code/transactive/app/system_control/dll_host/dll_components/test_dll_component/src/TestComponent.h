#ifndef TEST_COMPONENT_H
#define TEST_COMPONENT_H

#include "app/system_control/dll_host/dll_components/dll_component_helper/src/AbstractDllComponent.h"
#include "app/system_control/dll_host/dll_components/test_dll_component/IDL/src/ITestCorbaDef.h"
#include "core/threads/src/Thread.h"
#include "core/corba/src/ServantBase.h"


class TestComponent : public TA_Base_App::AbstractDllComponent,
                      public TA_Base_Core::Thread
{
public:

    TestComponent();
    ~TestComponent();

    virtual void init( TA_Base_App::IDllMgrCallBack* callback );
    virtual void uninit();
    virtual std::string getDLLName() const;

    virtual void run();
    virtual void terminate();

    class TestCorbaDef : public virtual POA_ITestCorbaDef,
                         public TA_Base_Core::ServantBase
    {
    public:

        TestCorbaDef()
        {
            activateServant();
        }

        virtual void echo(const char* str);
    };

private:

    volatile bool m_running;
    TA_Base_App::IDllMgrCallBack* m_callback;

    TestCorbaDef* m_object_1;
    TestCorbaDef* m_object_2;
};


#endif
