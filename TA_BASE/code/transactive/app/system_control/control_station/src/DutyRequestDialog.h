/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/DutyRequestDialog.h $
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
#pragma once
#include "ui_DutyRequestDialog.h"
#include "CsDialog.h"
#include "app/system_control/control_station/src/DutyManager.h"
#include <QTimer>

/////////////////////////////////////////////////////////////////////////////
// DutyRequestDialog dialog

namespace TA_Base_Bus
{
    class SoundManagerProxy;    // forward declaration
}

class DutyRequestDialog : public CsDialog
{
    Q_OBJECT

public:

    /**
      * DutyRequestDialog
      *
      * Constructor.  Takes in the DutyDetail which it will use
      * to initialise the text on the dialog.
      *
      * @param pParent The parent of this dialog.  DON'T pass in NULL as we're going
      *                to pass back the user response through this
      * @param detail The details of the duty
      *
      */
    DutyRequestDialog(DutyManager::DutyRequestDetail* detail);


    /**
      * ~DutyRequestDialog
      *
      * Destructor
      *
      */
    ~DutyRequestDialog();


    /**
      * create
      *
      * Call this to create a modeless instance
      *
      * @return True if successfully created the instance
      */
    bool create();

    // Implementation
public slots:

    void onInitDialog();
    void OnTimer();
    void OnAccept();
    void OnDeny();
    void OnCancel(WPARAM, LPARAM);

private:

    /**
      * formatContent
      *
      * Formats the content of the duty text.
      *
      * @return A string that describes what is happening to a set of duty.
      *
      */
    CString formatContent();

    // Parent of this dialog.  We'll post the user response back to this window
    CWnd* m_parent;

    // Stores the duty details
    DutyManager::DutyRequestDetail* m_detail;

    // The count down value that will be displayed on the dialog box
    unsigned int m_countDown;

    // This is the id of the timeout timer in the dialog box
    static const UINT TIMEOUT_TIMER;

    // default timout
    static const unsigned int DutyRequestDialog::DEFAULT_TIMER_TIMEOUT;

    // Pointer to the sound interface
    TA_Base_Bus::SoundManagerProxy* m_soundInterface;

    // Format for the text
    CString PRE_TIMEOUT_FORMAT;
    CString POST_TIMEOUT_FORMAT;

    //TD15112++
    // Format for the text
    static const std::string LOCATION_FORMAT;
    static const std::string SUBSYSTEM_FORMAT;
    //++TD15112

    Ui::DutyRequestDialog ui;
    QTimer m_timer;
};
