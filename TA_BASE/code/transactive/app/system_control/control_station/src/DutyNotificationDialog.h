/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyNotificationDialog.h $
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

#pragma once
#include "ui_DutyNotificationDialog.h"
#include "CsDialog.h"
#include "app/system_control/control_station/src/DutyManager.h"

/////////////////////////////////////////////////////////////////////////////
// DutyNotificationDialog dialog

namespace TA_Base_Bus
{
    class SoundManagerProxy;
}

class DutyNotificationDialog : public CsDialog
{
    Q_OBJECT

public:

    /**
      * DutyNotificationDialog
      *
      * Constructor.  Takes in the DutyNotificationDetail which it will use
      * to initialise the text on the dialog.
      *
      * @param detail The details of the duty
      */
    DutyNotificationDialog(DutyManager::DutyNotificationDetail* detail);


    /**
      * ~DutyNotificationDialog
      *
      * Destructor
      *
      */
    ~DutyNotificationDialog();


    /**
      * create
      *
      * Call this to create a modeless instance
      *
      * @return True if successfully created the instance
      */
    bool create();


    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(DutyNotificationDialog)
    void onInitDialog();

public slots:

    void OnCancel();
    void OnOK();

private:

    void setView(int cnt); // TD19075

    enum EMessageType
    {
        GAINED,
        DENIED,
        LOST,
        CHANGED // TD19075
    };

    /**
      * formatContent
      *
      * Formats the content of the duty text.
      *
      * @param format The heading for the section which describes what is happening
      *               to the duty
      * @param data The duty details to be used in the text.
      *
      * @return A string that describes what is happening to a set of duty.
      *
      */
    CString formatContent(const EMessageType& format, const std::vector<DutyManager::DutyDetail>& data);

    // Stores the duty details
    DutyManager::DutyNotificationDetail* m_detail;

    // Pointer to the sound interface
    TA_Base_Bus::SoundManagerProxy* m_soundInterface;

    // Format for the text
    static const std::string GAIN_FORMAT;
    static const std::string GAIN_DUTYLESS_FORMAT;
    static const std::string DENIED_FORMAT;
    static const std::string LOST_FORMAT;
    static const std::string CHANGED_FORMAT; // TD19075
    static const std::string LOST_DUTYLESS_FORMAT;
    static const std::string LOCATION_FORMAT;
    static const std::string SUBSYSTEM_FORMAT;
    static const std::string CHANGE_DUTYLESS_FORMAT; // TD19075

    Ui::DutyNotificationDialog ui;
};
