/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGui.h $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * main header file for the ALARMSGUI application
  */
#pragma once
#include "Singleton.h"
#include <QObject>
#include <QString>

class QLocalServer;

struct ControlStationInstanceChecker : public QObject, public Singleton<ControlStationInstanceChecker>
{
    Q_OBJECT

public:

    void check();

private slots:

    void newLocalConnection();

protected:

    ControlStationInstanceChecker();
    void newLocalServer();

protected:

    QLocalServer* m_localServer;
    QString m_serverName;
};
