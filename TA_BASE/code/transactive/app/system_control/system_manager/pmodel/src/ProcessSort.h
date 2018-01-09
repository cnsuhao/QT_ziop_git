#pragma once
#include "CommonDefs.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/function.hpp>

namespace TA_Base_Core
{
    class ProcessDataSeq;
}

namespace TA_Base_App
{
    struct ProcessSort
    {
        enum { COLUMN_COUNT = 8 };
        enum EColumn { NAME, LOCATION, MODE, STATE, REQUESTED, STARTED, RESTARTS, DEBUG_LEVEL, NO_SORT, SEQUENCE_NUMBER };

    public:

        ProcessSort();
        void reset();
        void setupSort(TA_Base_Core::ProcessDataSeq& data);

        void onServiceProcessList(const ProcessList& processes);
        void onServiceUpdateProcessList(const ProcessUpdates& updates);
        void onSelectionChanged(const IndexSet& selection);
        void onSectionClicked(int index);

    private:

        void sort();
        void restoreToOriginal();
        void addSequenceNumber(ProcessList& processes);
        void setupCompare(TA_Base_Core::ProcessDataSeq& data);
        void buildIndexMap(const ProcessList& processes, IndexMap& viewToModel, IndexMap& modelToView);
        IndexSet convertIndexFromViewToModel(const IndexSet& selection);
        IndexSet convertIndexFromModelToView(const IndexSet& selection);
        IndexSet convertIndex(const IndexSet& selection, IndexMap& indexMap);
        ProcessUpdates buildUpdates(const ProcessList& old, const ProcessList& latest);

    public:

        size_t m_index;
        bool m_enabled;
        bool m_asscending;
        IndexSet ui_selection;
        IndexSet pm_selection;
        ProcessList m_original;
        ProcessList m_processes;
        ProcessUpdates m_updates;

    private:

        IndexMap m_viewToModelIndexMap;
        IndexMap m_modelToViewIndexMap;
        std::vector<int> m_orders;
        TA_Base_Core::ProcessDataSeq* m_data;
        boost::function<bool(const Process&, const Process&)> m_comparator;
        std::map<EColumn, boost::function<bool(const Process&, const Process&)> > m_comparatorMap;
    };
}
