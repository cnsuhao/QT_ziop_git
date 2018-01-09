#include "ProcessSort.h"
#include "ProcessUtilities.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    namespace
    {
        bool operator < (const CORBA::String_member& lhs, const CORBA::String_member& rhs)
        {
            return ::strcmp(lhs.in(), rhs.in()) < 0;
        }

        bool operator == (const CORBA::String_member& lhs, const CORBA::String_member& rhs)
        {
            return 0 == ::strcmp(lhs.in(), rhs.in());
        }

        template<typename T>
        struct CompareByElement
        {
            typedef T(TA_Base_Core::ProcessData::*Pointer);
            Pointer m_ptr;
            TA_Base_Core::ProcessDataSeq& data;
            CompareByElement(TA_Base_Core::ProcessDataSeq& d, Pointer ptr) : data(d), m_ptr(ptr) {}
            bool operator()(const Process& lhs, const Process& rhs) const
            {
                size_t ln = boost::lexical_cast<size_t>(lhs.back());
                size_t rn = boost::lexical_cast<size_t>(rhs.back());
                const T& lv = data[ln].*m_ptr;
                const T& rv = data[rn].*m_ptr;
                return (lv < rv) || (lv == rv && ln < rn);
            }
        };

        template<typename T> CompareByElement<T> makeCompareByElement(TA_Base_Core::ProcessDataSeq& d, T(TA_Base_Core::ProcessData::*ptr))
        {
            return CompareByElement<T>(d, ptr);
        }

        struct CompareByDebugLevel
        {
            TA_Base_Core::ProcessDataSeq& data;
            CompareByDebugLevel(TA_Base_Core::ProcessDataSeq& d) : data(d) {}
            bool operator()(const Process& lhs, const Process& rhs) const
            {
                size_t ln = boost::lexical_cast<size_t>(lhs.back());
                size_t rn = boost::lexical_cast<size_t>(rhs.back());
                size_t lv = ProcessUtilities::getDebugLevelIndex(data[ln].debugLevel.in());
                size_t rv = ProcessUtilities::getDebugLevelIndex(data[rn].debugLevel.in());
                return (lv < rv) || (lv == rv && ln < rn);
            }
        };
    }


    ProcessSort::ProcessSort() :
        m_index(NO_SORT),
        m_enabled(false),
        m_orders(SEQUENCE_NUMBER),
        m_data(NULL)
    {
        BOOST_FOREACH(int& order, m_orders)
        {
            order = true;
        }
    }


    void ProcessSort::setupSort(TA_Base_Core::ProcessDataSeq& data)
    {
        m_data = &data;
        setupCompare(data);
    }


    void ProcessSort::setupCompare(TA_Base_Core::ProcessDataSeq& data)
    {
        m_comparatorMap[NAME] = makeCompareByElement(data, &TA_Base_Core::ProcessData::ProcessId);
        m_comparatorMap[LOCATION] = makeCompareByElement(data, &TA_Base_Core::ProcessData::host);
        m_comparatorMap[MODE] = makeCompareByElement(data, &TA_Base_Core::ProcessData::operationMode);
        m_comparatorMap[STATE] = makeCompareByElement(data, &TA_Base_Core::ProcessData::state);
        m_comparatorMap[REQUESTED] = makeCompareByElement(data, &TA_Base_Core::ProcessData::requestedState);
        m_comparatorMap[STARTED] = makeCompareByElement(data, &TA_Base_Core::ProcessData::started);
        m_comparatorMap[RESTARTS] = makeCompareByElement(data, &TA_Base_Core::ProcessData::restarts);
        m_comparatorMap[DEBUG_LEVEL] = CompareByDebugLevel(data);
    }


    void ProcessSort::reset()
    {
        m_index = NO_SORT;
        m_enabled = false;

        BOOST_FOREACH(int& order, m_orders)
        {
            order = true;
        }

        m_original.clear();
        m_processes.clear();
        m_updates.clear();
        ui_selection.clear();
        pm_selection.clear();
    }


    void ProcessSort::onServiceProcessList(const ProcessList& processes)
    {
        m_processes = m_original = processes;
        addSequenceNumber(m_processes);
        addSequenceNumber(m_original);

        if (m_enabled)
        {
            sort();
        }
    }


    void ProcessSort::onServiceUpdateProcessList(const ProcessUpdates& updates)
    {
        for (ProcessUpdates::const_iterator it = updates.begin(); it != updates.end(); ++it)
        {
            size_t row = it->first.first;
            size_t col = it->first.second;
            m_original[row][col] = it->second;
        }

        if (m_enabled)
        {
            sort();
        }
        else
        {
            m_updates = buildUpdates(m_processes, m_original);
            m_processes = m_original;
        }
    }


    void ProcessSort::onSelectionChanged(const IndexSet& selection)
    {
        ui_selection = pm_selection = selection;

        if (m_enabled)
        {
            pm_selection = convertIndexFromViewToModel(selection);
        }
    }


    void ProcessSort::onSectionClicked(int index)
    {
        if (NO_SORT == index)
        {
            if (m_index != NO_SORT)
            {
                m_index = NO_SORT;
                m_enabled = false;
                restoreToOriginal();
            }

            return;
        }

        if (index == m_index)
        {
            m_orders[index] = !m_orders[index];
        }

        m_index = index;
        m_enabled = true;
        m_asscending = m_orders[index];
        m_comparator = m_comparatorMap[(EColumn)index];

        if (!m_asscending)
        {
            m_comparator = std::not2(m_comparator);
        }

        sort();
    }


    void ProcessSort::sort()
    {
        ProcessList latested = m_original;
        std::sort(latested.begin(), latested.end(), m_comparator);
        buildIndexMap(latested, m_viewToModelIndexMap, m_modelToViewIndexMap);
        m_updates = buildUpdates(m_processes, latested);
        ui_selection = convertIndexFromModelToView(pm_selection);
        m_processes = latested;
    }


    void ProcessSort::restoreToOriginal()
    {
        m_updates = buildUpdates(m_processes, m_original);
        m_processes = m_original;
        ui_selection = pm_selection;
    }


    void ProcessSort::addSequenceNumber(ProcessList& processes)
    {
        for (size_t i = 0; i < processes.size(); i++)
        {
            processes[i].push_back(boost::lexical_cast<std::string>(i));
        }
    }


    void ProcessSort::buildIndexMap(const ProcessList& processes, IndexMap& viewToModel, IndexMap& modelToView)
    {
        viewToModel.clear();
        modelToView.clear();

        for (size_t i = 0; i < processes.size(); i++)
        {
            size_t sequenceNumber = boost::lexical_cast<size_t>(processes[i].back());
            viewToModel[i] = sequenceNumber;
            modelToView[sequenceNumber] = i;
        }
    }


    IndexSet ProcessSort::convertIndexFromViewToModel(const IndexSet& selection)
    {
        return convertIndex(selection, m_viewToModelIndexMap);
    }


    IndexSet ProcessSort::convertIndexFromModelToView(const IndexSet& selection)
    {
        return convertIndex(selection, m_modelToViewIndexMap);
    }


    IndexSet ProcessSort::convertIndex(const IndexSet& selection, IndexMap& indexMap)
    {
        IndexSet result;
        BOOST_FOREACH(size_t i, selection)
        {
            result.insert(indexMap[i]);
        }
        return result;
    }


    ProcessUpdates ProcessSort::buildUpdates(const ProcessList& old, const ProcessList& latest)
    {
        ProcessUpdates updates;

        for (size_t i = 0; i < old.size(); i++)
        {
            for (size_t j = 0; j < COLUMN_COUNT; j++)
            {
                const std::string& ov = old[i][j];
                const std::string& nv = latest[i][j];

                if (ov != nv)
                {
                    updates[std::make_pair(i, j)] = nv;
                }
            }
        }

        return updates;
    }
}
