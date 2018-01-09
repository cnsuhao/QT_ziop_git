#ifndef TRANSACTIVEABOUTBOXDLG_H
#define TRANSACTIVEABOUTBOXDLG_H

#include "transactiveaboutbox_global.h"
#include "ui_TransactiveAboutBox.h"
#include <QtGui/QMainWindow>


class TRANSACTIVEABOUTBOX_EXPORT TransactiveAboutBoxDlg : public QMainWindow
{
    Q_OBJECT

public:

    TransactiveAboutBoxDlg( const char buildVersion[],
                            const char buildDate[],
                            const char componentName[],
                            const char componentVersion[],
                            const int headerArraySize,
                            char* headerArray[] );

public slots:

    void on_okButton_clicked();
    void on_videoPlayer_finished();
    void replay();

protected:

    virtual void languageChange();

private:

    Ui_TransactiveAboutBox ui;

    const char* m_buildVersion;
    const char* m_buildDate;
    const char* m_componentName;
    const char* m_componentVersion;
    int m_headerArraySize;
    char** m_headerArray;
};

#endif
