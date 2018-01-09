#include "app/system_control/dll_host/dll_components/test_dll_component/src/stdafx.h"
#include "app/system_control/dll_host/dll_components/test_dll_component/src/TestComponent.h"
#include "app/system_control/dll_host/dll_components/test_dll_component/src/TestDialog.h"
#include "app/system_control/dll_host/dll_components/dll_component_helper/src/MessageDialogHelper.h"
#include "core/utilities/src/DebugUtil.h"


TestComponent::TestComponent()
    : m_running(false),
      m_callback(NULL),
      m_object_1(NULL),
      m_object_2(NULL)
{
    // DO NOT create corba object here, the CorbaUtil instance in this dll hasn't initialized by dll host yet.
}


TestComponent::~TestComponent()
{
    if ( m_object_1 )
    {
        m_object_1->deactivateAndDeleteServant();
        m_object_1 = NULL;
    }

    if ( m_object_2 )
    {
        m_object_2->deactivateAndDeleteServant();
        m_object_2 = NULL;
    }
}


std::string TestComponent::getDLLName() const
{
    return "TA_TestDll";
}


void TestComponent::init( TA_Base_App::IDllMgrCallBack* callback  )
{
    m_callback = callback;

    m_object_1 = new TestCorbaDef;
    m_object_2 = new TestCorbaDef;

    m_callback->registerObject( "Test Object 1", m_object_1->_this() );
    m_callback->registerObject( "Test Object 2", m_object_2->_this() );

    start();
}


void TestComponent::uninit()
{
    terminateAndWait();
}


void TestComponent::run()
{
    m_running = true;
    static int cnt = 0;

    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    TestDialog dlg;
    dlg.DoModal();

    while ( m_running )
    {
        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TA_TestDll is running %d", cnt++ );

        for ( int i = 0; i < 100 && m_running; ++i) // sleep 10 seconds
        {
            sleep( 100 );
        }
    }
}


void TestComponent::terminate()
{
    m_running = false;
}


void TestComponent::TestCorbaDef::echo(const char* str)
{
    LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "%p: %s", this, str );
}
