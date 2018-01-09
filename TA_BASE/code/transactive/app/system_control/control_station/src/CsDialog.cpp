#include "StdAfx.h"
#include "CsDialog.h"
#include <QThread>

// Cs = ControlStation
std::set<CsDialog*> CsDialog::s_allDialogs;

CsDialog::CsDialog()
{
    s_allDialogs.insert(this);
    setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setupMessageMap();

    connect(this, SIGNAL(sendMessageSignal(unsigned int, WPARAM, LPARAM)), this, SLOT(onMessageSlot(unsigned int, WPARAM, LPARAM)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(postMessageSignal(unsigned int, WPARAM, LPARAM)), this, SLOT(onMessageSlot(unsigned int, WPARAM, LPARAM)), Qt::QueuedConnection);
    connect(this, SIGNAL(doModalSignal(void)), this, SLOT(doModalSlot()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(showDialogSignal(void)), this, SLOT(showDialogSlot()));
    connect(this, SIGNAL(closeDialogSignal(void)), this, SLOT(closeDialogSlot()));
    connect(this, SIGNAL(finished(int)), this, SLOT(onDialogClosed()));
}

CsDialog::~CsDialog()
{
    s_allDialogs.erase(this);
}

void CsDialog::onMessageSlot(unsigned int messageId, WPARAM wparam, LPARAM lparam)
{
    MessageMap::iterator findIt = m_messageMap.find(messageId);

    if (findIt != m_messageMap.end())
    {
        findIt->second(wparam, lparam);
    }
}
