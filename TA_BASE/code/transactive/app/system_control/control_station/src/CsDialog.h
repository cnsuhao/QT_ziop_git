#pragma once
#include <QDialog>

struct CsDialog : QDialog
{
    Q_OBJECT

public:

    CsDialog();
    ~CsDialog();

    virtual void postMessage(unsigned int messageId, WPARAM wparam = 0, LPARAM lparam = 0)
    {
        emit postMessageSignal(messageId, wparam, lparam);
    }

    virtual void sendMessage(unsigned int messageId, WPARAM wparam = 0, LPARAM lparam = 0)
    {
        emit sendMessageSignal(messageId, wparam, lparam);
    }

    static void broadcastMessage(unsigned int messageId, WPARAM wparam = 0, LPARAM lparam = 0)
    {
        for (CsDialog* dialog : s_allDialogs)
        {
            emit dialog->postMessageSignal(messageId, wparam, lparam);
        }
    }

    virtual int doModal()
    {
        emit doModalSignal();
        return result();
    }

    virtual void showDialog()
    {
        emit showDialogSignal();
    }

    virtual void closeDialog()
    {
        emit closeDialogSignal();
    }

    virtual void setupMessageMap() {}

protected:

    virtual void onInitDialog() {}
    virtual void onCloseDialog() {}
    virtual void onCancelDialog() {}

protected:

    virtual void accept() override
    {
        onCloseDialog();
        QDialog::accept();
    }

    virtual void reject() override
    {
        onCancelDialog();
        QDialog::reject();
    }

signals:

    void sendMessageSignal(unsigned int messageId, WPARAM wparam, LPARAM lparam);
    void postMessageSignal(unsigned int messageId, WPARAM wparam, LPARAM lparam);
    void doModalSignal();
    void showDialogSignal();
    void closeDialogSignal();

public slots:

    virtual void onDialogClosed() {}

    virtual void onMessageSlot(unsigned int messageId, WPARAM wparam, LPARAM lparam);

    virtual void doModalSlot()
    {
        onInitDialog();
        exec();
    }

    virtual void showDialogSlot()
    {
        onInitDialog();
        show();
    }

    virtual void closeDialogSlot()
    {
        accept();
    }

public:

    static std::set<CsDialog*> s_allDialogs;
    typedef std::map< unsigned int, boost::function<void(WPARAM, LPARAM)> > MessageMap;
    MessageMap m_messageMap;
};
