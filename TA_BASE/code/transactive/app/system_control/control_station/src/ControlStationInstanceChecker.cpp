/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/control_station/src/ControlStationInstanceChecker.cpp $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * Defines the class behaviors for the application. Launches the Alarm Manager as
  * a managed application using Generic GUI.
  */
#include "StdAfx.h"
#include "resource.h"
#include "ControlStationInstanceChecker.h"
#include "bus/generic_gui_view/src/transactivemessage.h"
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include <QFileInfo>
#include <QApplication>

#define TIME_OUT (500) // 500ms

ControlStationInstanceChecker::ControlStationInstanceChecker()
    : m_localServer(NULL)
{
    m_serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
}

void ControlStationInstanceChecker::newLocalConnection()
{
    QLocalSocket* socket = m_localServer->nextPendingConnection();

    if (socket)
    {
        socket->waitForReadyRead(2 * TIME_OUT);
        delete socket;
    }
}

void ControlStationInstanceChecker::check()
{
    QLocalSocket socket;
    socket.connectToServer(m_serverName);

    if (socket.waitForConnected(TIME_OUT))
    {
        showMsgBox(IDS_UE_010009, m_serverName.toStdString());
        ::exit(0);
    }

    newLocalServer();
}

void ControlStationInstanceChecker::newLocalServer()
{
    m_localServer = new QLocalServer(this);
    connect(m_localServer, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

    if (!m_localServer->listen(m_serverName))
    {
        if (m_localServer->serverError() == QAbstractSocket::AddressInUseError)
        {
            QLocalServer::removeServer(m_serverName);
            m_localServer->listen(m_serverName);
        }
    }
}
