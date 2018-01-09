#pragma once

#include <QTableWidget>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QHeaderView>


enum EProcessColumn
{
    ProcessNameColumn,
    LocationColumn,
    OperationModeColumn,
    StatusColumn,
    RequestStatusColumn,
    StartTimeColumn,
    RestartsColumn,
    DebugLevelColumn,
    DefaultSortColumn,
    MaxColumns
};


class ProcessList
{
public:

    ProcessList(QTableWidget* table, const QList<QTableWidgetItem*>& items);
    ~ProcessList();
    QStringList getText(int column);
    QString getFirstText(int column);
    void start();
    void stop();
    void setDebugLevel(const QString& debugLevel);
    void changeOperationMode(const QString& mode);
    int size() {
        return m_processes.size();
    }
    int empty() {
        return m_processes.empty();
    }

private:

    QMap<int, QVector<QTableWidgetItem*> > m_processes;
    QTableWidget* m_table;
    QMap<QTableWidgetItem*, QString> m_cache;
};
