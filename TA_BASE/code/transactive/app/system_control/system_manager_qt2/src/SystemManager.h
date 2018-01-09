#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <QtGui/QMainWindow>
#include "ui_systemmanager.h"

class SystemManager : public QMainWindow
{
    Q_OBJECT

public:
    SystemManager(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SystemManager();

private:
    Ui::SystemManagerClass ui;
};

#endif // SYSTEMMANAGER_H
