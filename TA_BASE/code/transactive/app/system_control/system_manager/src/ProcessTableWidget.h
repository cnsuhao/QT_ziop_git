#pragma once
#include <QTableWidget>
#include "ProcessList.h"


class ProcessTableWidget : public QTableWidget
{
    Q_OBJECT

public:

    ProcessTableWidget(QWidget* parent = 0);
    ProcessTableWidget(int rows, int columns, QWidget* parent = 0);

    // new functions
    ProcessList selectedProcesses();

private slots:

    void sectionClicked(int logicalIndex);

private:

    void init();

private:

    int m_currentSortingSection;
};
