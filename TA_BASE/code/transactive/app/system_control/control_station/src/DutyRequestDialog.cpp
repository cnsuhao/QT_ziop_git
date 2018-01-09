/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyRequestDialog.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * A dialog which lists requested duties and allow the operator to accept or
  * deny requests.  The dialog will automatically accept the request if the
  * operator does not reply before timeout.
  *
  */

#include "StdAfx.h"
#include "ControlStationRootDialog.h"
#include "QtUtilities.h"
#include "resourceQt.h"
#include "app/system_control/control_station/src/DutyRequestDialog.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/SoundPlayerException.h"
using TA_Base_Core::RunParams;

const UINT DutyRequestDialog::TIMEOUT_TIMER = 124;
const unsigned int DutyRequestDialog::DEFAULT_TIMER_TIMEOUT = 30;
// This is the id of the clock timer that updates the time and date in the launch bar

//TD15112++
const std::string DutyRequestDialog::LOCATION_FORMAT = "\t%s\n";
const std::string DutyRequestDialog::SUBSYSTEM_FORMAT = "\t%s\n";
//++TD15112

/////////////////////////////////////////////////////////////////////////////
// DutyRequestDialog dialog

DutyRequestDialog::DutyRequestDialog(DutyManager::DutyRequestDetail* detail)
    : m_detail(detail),
      m_soundInterface(NULL)
{
    PRE_TIMEOUT_FORMAT = "剩余时间: %us";
    POST_TIMEOUT_FORMAT = "剩余时间: 超时";

    ui.setupUi(this);

    connect(ui.m_okCtrl, SIGNAL(clicked(bool)), this, SLOT(OnAccept()));
    connect(ui.m_cancelCtrl, SIGNAL(clicked(bool)), this, SLOT(OnDeny()));
}

DutyRequestDialog::~DutyRequestDialog()
{
    if (m_soundInterface != NULL)
    {
        delete m_soundInterface;
        m_soundInterface = NULL;
    }

    delete m_detail;
}

void DutyRequestDialog::onInitDialog()
{
    //// disable the 'x' button for now
    //CMenu* pSysMenu = GetSystemMenu(FALSE);

    //if (pSysMenu != NULL)
    //{
    //    pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
    //}

    // format the content
    CString out = formatContent();
    //    out.TrimRight(_T("\n\r \t"));
    ui.m_textCtrl->setText((const char*)out);
    //ui.m_textCtrl->SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));

    // Set the timer to go off every 1000 milliseconds to update the clock
    CString time;
    unsigned int timeout = TA_Base_Core::getRunParamValue(TA_Base_App::TA_ControlStation::ACCESS_CONTROL_POPUP_TIMEOUT, DEFAULT_TIMER_TIMEOUT);

    time.Format(PRE_TIMEOUT_FORMAT, timeout);
    ui.m_timeCtrl->setText(QtUtilities::convertToUnicode((const char*)time));
    m_countDown = timeout;

    // Call the sound player
    try
    {
        m_soundInterface = new TA_Base_Bus::SoundManagerProxy();
    }
    catch (TA_Base_Core::SoundPlayerException& ex)
    {
        std::string error = "Unable to initialise SoundManager: ";
        error += ex.what();
        LOG_EXCEPTION("SoundPlayerException", error.c_str());

        m_soundInterface = NULL;
    }

    if (m_soundInterface != NULL)
    {
        try
        {
            m_soundInterface->setPriority(TA_Base_App::TA_ControlStation::CS_DUTY_SOUND_CATEGORY,
                                          TA_Base_App::TA_ControlStation::CS_DUTY_SOUND_PRIORITY);
        }
        // If there are any exceptions, just log and continue
        catch (TA_Base_Core::SoundPlayerException& ex)
        {
            std::string message("Caught while attempting to initialise sound for Duty Request dialog, with error: ");
            message += ex.what();
            LOG_EXCEPTION("SoundPlayerException", message);
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown Exception", "Caught an exception while attempting to initialise sound. No sound will be played");
        }
    }

    // set the timer to go off every 1 second so that we could update the count down value in the dialog
    //SetTimer(TIMEOUT_TIMER, 1000, 0);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    m_timer.start(1000);
}

CString DutyRequestDialog::formatContent()
{
    DutyManager::DutyDetail& data = m_detail->duty;

    //TD15112++

    std::string format, temp, out;

    if (m_detail->duty.locationName.empty())
    {
        temp = str(boost::format(IDS_REQUEST_FORMAT_WITHOUT_LOCATION) % data.operatorName % data.profileName);
    }
    else
    {
        temp = str(boost::format(IDS_REQUEST_FORMAT_WITH_LOCATION) % data.operatorName % data.profileName % data.locationName);
    }

    out += temp;

    for (unsigned int j = 0; j < data.subsystem.size(); j++)
    {
        out += "\n\nfor:";

        for (unsigned int k = 0; k < data.subsystem[j].subsystemName.size(); k++)
        {
            temp = str(boost::format(SUBSYSTEM_FORMAT) % data.subsystem[j].subsystemName[k]);
            out += temp;
        }

        out += "\nat:";
        temp = str(boost::format(LOCATION_FORMAT) % data.subsystem[j].regionName);
        out += temp;
    }

    //out += "\nsubsystem that you are currently controlling?";
    out += "\n";

    //++TD15112

    return out.c_str();
}

void DutyRequestDialog::OnTimer()
{
    if (m_countDown > 0)
    {
        m_countDown--;
        CString time;
        time.Format(PRE_TIMEOUT_FORMAT, m_countDown);
        ui.m_timeCtrl->setText(QtUtilities::convertToUnicode((const char*)time));
    }
    else
    {
        // we've hit 0, so need to kill timer, disable buttons and act as though the
        // operator has clicked OK but leave the dialog open

        // stop the clock timer
        //KillTimer(TIMEOUT_TIMER);
        m_timer.stop();

        // disable buttons
        ui.m_cancelCtrl->setEnabled(FALSE);
        ui.m_okCtrl->setEnabled(FALSE);

        //// enable the 'x' button since that's the only way user could close the dialog now
        //CMenu* pSysMenu = GetSystemMenu(FALSE);

        //if (pSysMenu != NULL)
        //{
        //    pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
        //}

        std::string* uniqueId = new std::string(m_detail->uniqueId);
        // tell our parent the user has failed to respond
        ControlStationRootDialog::getInstance().postMessage(WM_DUTY_RESPONSE, reinterpret_cast<WPARAM>(uniqueId), static_cast<LPARAM>(TA_Base_App::TA_ControlStation::TIMEOUT));

        ui.m_timeCtrl->setText(QtUtilities::convertToUnicode((const char*)POST_TIMEOUT_FORMAT));
    }

    //CDialog::OnTimer(nIDEvent);
    m_timer.start();
}

bool DutyRequestDialog::create()
{
    onInitDialog();
    show();
    return true;
}

void DutyRequestDialog::OnCancel(WPARAM, LPARAM)
{
    // Disable the sound

    if (m_soundInterface != NULL)
    {
        try
        {
            m_soundInterface->setPriority(TA_Base_App::TA_ControlStation::CS_DUTY_SOUND_CATEGORY, 0);
        }
        // If there are any exceptions, just log and continue
        catch (TA_Base_Core::SoundPlayerException& ex)
        {
            std::string message("Caught while attempting to clear sound for Duty Request dialog, with error: ");
            message += ex.what();
            LOG_EXCEPTION("SoundPlayerException", message);
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown Exception", "Caught an exception while attempting to clear sound.");
        }
    }

    ControlStationRootDialog::getInstance().postMessage(WM_CANCEL_CHILD, reinterpret_cast<WPARAM>(this));
    CsDialog::close();
}

void DutyRequestDialog::OnAccept()
{
    // tell our parent that user has accepted the request
    std::string* uniqueId = new std::string(m_detail->uniqueId);
    ControlStationRootDialog::getInstance().postMessage(WM_DUTY_RESPONSE, reinterpret_cast<WPARAM>(uniqueId), static_cast<LPARAM>(TA_Base_App::TA_ControlStation::ACCEPT));
    OnCancel(0, 0);
}

void DutyRequestDialog::OnDeny()
{
    // tell our parent that user has rejected the request
    std::string* uniqueId = new std::string(m_detail->uniqueId);
    ControlStationRootDialog::getInstance().postMessage(WM_DUTY_RESPONSE, reinterpret_cast<WPARAM>(uniqueId), static_cast<LPARAM>(TA_Base_App::TA_ControlStation::DENY));
    OnCancel(0, 0);
}
