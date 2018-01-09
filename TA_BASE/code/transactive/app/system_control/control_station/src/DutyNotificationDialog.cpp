/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyNotificationDialog.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * A dialog which lists:
  *  - the duties that this operator has previously requested and gained from operators holding the duties,
  *  - the duties that this operator has previously requested and denied by operators holding the duties,
  *  - the duties that are lost to another operator.
  *
  * It is possible for the dialog to display the gained and denied requests together in a single dialog.
  */

#include "StdAfx.h"
#include "ControlStationRootDialog.h"
#include "app/system_control/control_station/src/DutyNotificationDialog.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/SoundPlayerException.h"

const std::string DutyNotificationDialog::GAIN_FORMAT = "You have gained rights from\n\t%s/%s\n";
const std::string DutyNotificationDialog::GAIN_DUTYLESS_FORMAT = "You have gained rights\n";
const std::string DutyNotificationDialog::DENIED_FORMAT = "You have been denied rights by\n\t%s/%s\n";
const std::string DutyNotificationDialog::LOST_FORMAT = "You have lost rights to\n\t%s/%s\n";
const std::string DutyNotificationDialog::CHANGED_FORMAT = "You have rights changed to\n\t%s/%s\n"; // TD19075
const std::string DutyNotificationDialog::CHANGE_DUTYLESS_FORMAT = "You have changed rights\n"; // TD19075
const std::string DutyNotificationDialog::LOST_DUTYLESS_FORMAT = "You have lost rights\n";
const std::string DutyNotificationDialog::LOCATION_FORMAT = "\t%s\n";
const std::string DutyNotificationDialog::SUBSYSTEM_FORMAT = "\t%s\n";

/////////////////////////////////////////////////////////////////////////////
// DutyNotificationDialog dialog

DutyNotificationDialog::DutyNotificationDialog(DutyManager::DutyNotificationDetail* detail)
    : m_detail(detail),
      m_soundInterface(NULL)
{
    ui.setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(OnOK()));
    connect(this, SIGNAL(rejected()), this, SLOT(OnCancel()));
}

DutyNotificationDialog::~DutyNotificationDialog()
{
    if (m_soundInterface != NULL)
    {
        delete m_soundInterface;
        m_soundInterface = NULL;
    }

    delete m_detail;
}

void DutyNotificationDialog::onInitDialog()
{
    // TD19075
    // set View counts
    int nViewCnt = 0;

    // format the content
    CString out;

    if (0 != m_detail->gained.size())
    {
        ++nViewCnt;
        out.Empty();
        out = formatContent(GAINED, m_detail->gained);
        ui.m_textCtrl->setText((const char*)out);
        //ui.m_textCtrl.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
    }

    if (0 != m_detail->lost.size())
    {
        ++nViewCnt;
        out.Empty();
        out = formatContent(LOST, m_detail->lost);

        if (1 == nViewCnt)
        {
            ui.m_textCtrl->setText((const char*)out);
            //ui.m_textCtrl.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else
        {
            ui.m_textCtrl2->setText((const char*)out);
            //ui.m_textCtrl2.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
    }

    if (0 != m_detail->changed.size())
    {
        ++nViewCnt;
        out.Empty();
        out = formatContent(CHANGED, m_detail->changed);

        if (1 == nViewCnt)
        {
            ui.m_textCtrl->setText((const char*)out);
            //ui.m_textCtrl.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else if (2 == nViewCnt)
        {
            ui.m_textCtrl2->setText((const char*)out);
            //ui.m_textCtrl2.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else
        {
            ui.m_textCtrl3->setText((const char*)out);
            //ui.m_textCtrl3.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
    }

    // some denied exist
    if (0 != m_detail->denied.size())
    {
        ++nViewCnt;
        out.Empty();
        out = formatContent(DENIED, m_detail->denied);

        if (1 == nViewCnt)
        {
            ui.m_textCtrl->setText((const char*)out);
            //ui.m_textCtrl.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else if (2 == nViewCnt)
        {
            ui.m_textCtrl2->setText((const char*)out);
            //ui.m_textCtrl2.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else if (3 == nViewCnt)
        {
            ui.m_textCtrl3->setText((const char*)out);
            //ui.m_textCtrl3.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
        else
        {
            ui.m_textCtrl4->setText((const char*)out);
            //ui.m_textCtrl4.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
        }
    }

    setView(nViewCnt);

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
            std::string message("Caught while attempting to initialise sound for Duty Notification dialog, with error: ");
            message += ex.what();
            LOG_EXCEPTION("SoundPlayerException", message);
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown Exception", "Caught an exception while attempting to initialise sound. No sound will be played");
        }
    }
}

CString DutyNotificationDialog::formatContent(const EMessageType& type, const std::vector<DutyManager::DutyDetail>& data)
{
    std::string format, temp, out;

    static std::map<EMessageType, std::string> theMap =
    {
        { GAINED, GAIN_FORMAT },
        { DENIED, DENIED_FORMAT },
        { LOST, LOST_FORMAT },
        { CHANGED, CHANGED_FORMAT }
    };

    format = theMap[type];

    for (unsigned int i = 0; i < data.size(); i++)
    {
        if (!data[i].operatorName.empty())
        {
            temp = str(boost::format(format) % data[i].operatorName % data[i].profileName);
        }
        else
        {
            if (type == GAINED)
            {
                temp = GAIN_DUTYLESS_FORMAT;
            }
            else if (type == LOST)
            {
                temp = LOST_DUTYLESS_FORMAT;
            }
            else if (CHANGED == type) // 19075++
            {
                temp = CHANGE_DUTYLESS_FORMAT;
            }
        }

        out += temp;

        for (unsigned int j = 0; j < data[i].subsystem.size(); j++)
        {
            out += "\nfor:";

            for (unsigned int k = 0; k < data[i].subsystem[j].subsystemName.size(); k++)
            {
                temp = str(boost::format(SUBSYSTEM_FORMAT) % data[i].subsystem[j].subsystemName[k]);
                out += temp;
            }

            out += "at:";
            temp = str(boost::format(LOCATION_FORMAT) % data[i].subsystem[j].regionName);
            out += temp;
        }

        out += "\n";
    }

    return out.c_str();
}

bool DutyNotificationDialog::create()
{
    onInitDialog();
    show();
    return true;
}

void DutyNotificationDialog::OnCancel()
{
    // If the sound interface exists, then silence it
    if (NULL != m_soundInterface)
    {
        try
        {
            m_soundInterface->setPriority(TA_Base_App::TA_ControlStation::CS_DUTY_SOUND_CATEGORY, 0);
        }
        // If there are any exceptions, just log and continue
        catch (TA_Base_Core::SoundPlayerException& ex)
        {
            std::string message("Caught while attempting to clear sound for Duty Notification, with error: ");
            message += ex.what();
            LOG_EXCEPTION("SoundPlayerException", message);
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown Exception", "Caught an exception while attempting to clear Duty Notificaiton sound.");
        }
    }

    //TODO
    ControlStationRootDialog::getInstance().postMessage(WM_CANCEL_CHILD, reinterpret_cast<WPARAM>(this));
    CsDialog::close();
}

void DutyNotificationDialog::OnOK()
{
    // We want OK and Cancel to do the same thing (i.e. close and delete the dialog), so call OnCancel from within
    // OnOK.
    OnCancel();
}

// TD19075
void DutyNotificationDialog::setView(int cnt)
{
    //TODO
    //if (0 == cnt)
    //{
    //    return;
    //}

    //CRect rectAll, rectLine3, rectLine2, rectLine1, rectB;
    //GetWindowRect(&rectAll);
    //GetDlgItem(IDC_LINE3)->GetWindowRect(&rectLine3);
    //GetDlgItem(IDC_LINE2)->GetWindowRect(&rectLine2);
    //GetDlgItem(IDC_LINE1)->GetWindowRect(&rectLine1);
    //GetDlgItem(IDOK)->GetWindowRect(&rectB);
    //int nbtPos = (rectB.right - rectB.left + 1) / 2;

    //if (3 == cnt)
    //{
    //    int nPosA = (rectLine3.right - rectAll.left + 1) / 2;
    //    ScreenToClient(&rectLine3);
    //    SetWindowPos(NULL, rectAll.left, rectAll.top, rectLine3.right + 1, rectAll.Height(), SWP_NOZORDER);
    //    ScreenToClient(&rectB);
    //    rectB.left = nPosA - nbtPos;
    //    rectB.right = nPosA + nbtPos;
    //    GetDlgItem(IDOK)->MoveWindow(&rectB);
    //}
    //else if (2 == cnt)
    //{
    //    int nPosA = (rectLine2.right - rectAll.left + 1) / 2;
    //    ScreenToClient(&rectLine2);
    //    SetWindowPos(NULL, rectAll.left, rectAll.top, rectLine2.right + 1, rectAll.Height(), SWP_NOZORDER);
    //    ScreenToClient(&rectB);
    //    rectB.left = nPosA - nbtPos;
    //    rectB.right = nPosA + nbtPos;
    //    GetDlgItem(IDOK)->MoveWindow(&rectB);
    //}
    //else if (1 == cnt)
    //{
    //    int nPosA = (rectLine1.right - rectAll.left + 1) / 2;
    //    ScreenToClient(&rectLine1);
    //    SetWindowPos(NULL, rectAll.left, rectAll.top, rectLine1.right + 1, rectAll.Height(), SWP_NOZORDER);
    //    ScreenToClient(&rectB);
    //    rectB.left = nPosA - nbtPos;
    //    rectB.right = nPosA + nbtPos;
    //    GetDlgItem(IDOK)->MoveWindow(&rectB);
    //}
    //else
    //{
    //    int nPosA = (rectAll.right - rectAll.left + 1) / 2;
    //    ScreenToClient(&rectB);
    //    rectB.left = nPosA - nbtPos;
    //    rectB.right = nPosA + nbtPos;
    //    GetDlgItem(IDOK)->MoveWindow(&rectB);
    //}
}
// ++TD19075
