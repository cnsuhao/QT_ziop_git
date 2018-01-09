#include "ProcessList.h"


ProcessList::ProcessList(QTableWidget* table, const QList<QTableWidgetItem*>& items)
    : m_table(table)
{
    for (QList<QTableWidgetItem*>::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        QTableWidgetItem* item = *it;
        int row = item->row();
        int col = item->column();
        QVector<QTableWidgetItem*>& items = m_processes[row];

        if (items.empty())
        {
            items.resize(MaxColumns);
        }

        items[col] = item;
    }
}


ProcessList::~ProcessList()
{
    if (m_cache.empty())
    {
        return;
    }

    bool sortingEnabled = m_table->isSortingEnabled();
    bool shouldDisableFirst = false;

    if (sortingEnabled)
    {
        int sortSection = m_table->horizontalHeader()->sortIndicatorSection();

        for (QMap<QTableWidgetItem*, QString>::iterator it = m_cache.begin(); it != m_cache.end(); ++it)
        {
            if (it.key()->column() == sortSection)
            {
                m_table->setSortingEnabled(false);
                shouldDisableFirst = true;
                break;
            }
        }
    }

    for (QMap<QTableWidgetItem*, QString>::iterator it = m_cache.begin(); it != m_cache.end(); ++it)
    {
        it.key()->setText(it.value());
    }

    if (sortingEnabled && shouldDisableFirst)
    {
        m_table->setSortingEnabled(true);
    }
}


QStringList ProcessList::getText(int column)
{
    QStringList strings;

    for (QMap<int, QVector<QTableWidgetItem*> >::iterator it = m_processes.begin(); it != m_processes.end(); ++it)
    {
        strings.push_back(it.value()[column]->text());
    }

    return strings;
}


QString ProcessList::getFirstText(int column)
{
    return m_processes.begin().value()[column]->text();
}


void ProcessList::start()
{
    for (QMap<int, QVector<QTableWidgetItem*> >::iterator it = m_processes.begin(); it != m_processes.end(); ++it)
    {
        QVector<QTableWidgetItem*>& p = it.value();
        QTableWidgetItem* statusItem = p[StatusColumn];

        if ("Stopped" == statusItem->text())
        {
            QTableWidgetItem* modeItem = p[OperationModeColumn];
            QTableWidgetItem* requestItem = p[RequestStatusColumn];

            if ("Control" == modeItem->text())
            {
                m_cache[statusItem] = "Running Control";
                m_cache[requestItem] = "Running Control";
            }
            else
            {
                m_cache[statusItem] = "Running Monitor";
                m_cache[requestItem] = "Running Monitor";
            }

            QTableWidgetItem* restartsItem = p[RestartsColumn];
            m_cache[restartsItem] = QString::number(restartsItem->text().toInt() + 1);
        }
    }
}


void ProcessList::stop()
{
    for (QMap<int, QVector<QTableWidgetItem*> >::iterator it = m_processes.begin(); it != m_processes.end(); ++it)
    {
        QVector<QTableWidgetItem*>& p = it.value();
        QTableWidgetItem* statusItem = p[StatusColumn];
        QTableWidgetItem* requestItem = p[RequestStatusColumn];

        if ("Stopped" != statusItem->text())
        {
            m_cache[statusItem] = "Stopped";
            m_cache[requestItem] = "Stopped";
        }
    }
}


void ProcessList::setDebugLevel(const QString& debugLevel)
{
    for (QMap<int, QVector<QTableWidgetItem*> >::iterator it = m_processes.begin(); it != m_processes.end(); ++it)
    {
        QVector<QTableWidgetItem*>& p = it.value();
        QTableWidgetItem* item = p[DebugLevelColumn];

        if (item->text() != debugLevel)
        {
            m_cache[item] = debugLevel;
        }
    }
}


void ProcessList::changeOperationMode(const QString& mode)
{
    for (QMap<int, QVector<QTableWidgetItem*> >::iterator it = m_processes.begin(); it != m_processes.end(); ++it)
    {
        QVector<QTableWidgetItem*>& p = it.value();
        QTableWidgetItem* modeItem = p[OperationModeColumn];

        if (modeItem->text() != mode)
        {
            m_cache[modeItem] = mode;
            QTableWidgetItem* statusItem = p[StatusColumn];
            QTableWidgetItem* requestItem = p[RequestStatusColumn];

            if (statusItem->text() != "Stopped")
            {
                if ("Control" == mode)
                {
                    m_cache[statusItem] = "Running Control";
                    m_cache[requestItem] = "Running Control";
                }
                else
                {
                    m_cache[statusItem] = "Running Monitor";
                    m_cache[requestItem] = "Running Monitor";
                }
            }
        }
    }
}
