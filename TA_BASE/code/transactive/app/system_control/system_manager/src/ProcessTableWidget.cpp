#include "ProcessTableWidget.h"


ProcessTableWidget::ProcessTableWidget(QWidget* parent)
    : QTableWidget(parent)
{
    init();
}


ProcessTableWidget::ProcessTableWidget(int rows, int columns, QWidget* parent)
    : QTableWidget(rows, columns, parent)
{
    init();
}


void ProcessTableWidget::init()
{
    setColumnCount(MaxColumns);
    setHorizontalHeaderLabels(QStringList() << "Process Name" << "Location" << "Operation Mode" << "Status" << "Request Status" << "Start Time" << "Restart Number" << "Debug Level" << "");
    setColumnWidth(ProcessNameColumn, 200);
    setColumnWidth(StartTimeColumn, 150);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setShowGrid(false);
    setFocusPolicy(Qt::NoFocus);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setHighlightSections(false);
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));
    m_currentSortingSection = DefaultSortColumn;
}


ProcessList ProcessTableWidget::selectedProcesses()
{
    return ProcessList(this, selectedItems());
}


void ProcessTableWidget::sectionClicked(int logicalIndex)
{
    if (DefaultSortColumn == logicalIndex)
    {
        if (logicalIndex != m_currentSortingSection)
        {
            sortByColumn(DefaultSortColumn, Qt::AscendingOrder);
            setSortingEnabled(false);
        }
    }
    else
    {
        if (DefaultSortColumn == m_currentSortingSection)
        {
            setSortingEnabled(true);
        }
    }

    m_currentSortingSection = logicalIndex;
}
