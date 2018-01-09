#include "ProcessDataUser.h"
#include "SystemManagerDlgModelService.h"
#include "Factory.h"


namespace TA_Base_App
{
    ProcessDataUser::ProcessDataUser()
        : m_selection(Factory::instance().getSystemManagerDlgModelService()->getSelection()),
          m_data(Factory::instance().getSystemManagerDlgModelService()->getProcessData())
    {
        m_util.set(m_data, m_selection);
    }
}
