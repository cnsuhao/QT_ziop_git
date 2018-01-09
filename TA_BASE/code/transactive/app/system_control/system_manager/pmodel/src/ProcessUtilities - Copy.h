#pragma once
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/function.hpp>

namespace ProcessUtilities
{
    struct IPredicate
    {
        virtual bool operator()(const TA_Base_Core::ProcessDataSeq& data, size_t i) const = 0;
    };

    struct Predicate : IPredicate
    {
        boost::function<bool(const TA_Base_Core::ProcessDataSeq&, size_t)> m_function;
        Predicate(boost::function<bool(const TA_Base_Core::ProcessDataSeq&, size_t)> fun) : m_function(fun) {}
        bool operator()(const TA_Base_Core::ProcessDataSeq& data, size_t i) const {
            return m_function(data, i);
        }
    };

    struct NotPredicate : IPredicate
    {
        const IPredicate& m_pred;
        NotPredicate(const IPredicate& pred) : m_pred(pred) {}
        bool operator()(const TA_Base_Core::ProcessDataSeq& data, size_t i) const {
            return !m_pred(data, i);
        }
    };

    struct BinaryPredicate : IPredicate
    {
        const IPredicate& m_lhs;
        const IPredicate& m_rhs;
        BinaryPredicate(const IPredicate& lhs, const IPredicate& rhs) : m_lhs(lhs), m_rhs(rhs) {}
    };

    struct AndPredicate : BinaryPredicate
    {
        AndPredicate(const IPredicate& lhs, const IPredicate& rhs) : BinaryPredicate(lhs, rhs) {}
        bool operator()(const TA_Base_Core::ProcessDataSeq& data, size_t i) const {
            return m_lhs(data, i) && m_rhs(data, i);
        }
    };

    struct OrPredicate : BinaryPredicate
    {
        OrPredicate(const IPredicate& lhs, const IPredicate& rhs) : BinaryPredicate(lhs, rhs) {}
        bool operator()(const TA_Base_Core::ProcessDataSeq& data, size_t i) const {
            return m_lhs(data, i) || m_rhs(data, i);
        }
    };

    AndPredicate operator && (const IPredicate& lhs, const IPredicate& rhs);
    OrPredicate operator || (const IPredicate& lhs, const IPredicate& rhs);
    NotPredicate operator !(IPredicate& pred);

    std::vector< std::vector<std::string> > convertToVector(const TA_Base_Core::ProcessDataSeq& data);
    std::map<std::pair<size_t, size_t>, std::string> getUpdates(const TA_Base_Core::ProcessDataSeq& old, const TA_Base_Core::ProcessDataSeq& newed);
    bool isMode(const TA_Base_Core::ProcessDataSeq& data, size_t i, TA_Base_Core::EOperationMode mode);
    bool isControl(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isMonitor(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isNotApplicable(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isStartAble(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isStopAble(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isDebugLevel(const TA_Base_Core::ProcessDataSeq& data, size_t i, const std::string& level);
    bool isService(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool isAutoFailBack(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    bool predicate(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, IPredicate& predicate, bool isAny = false);
    bool isAllControl(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    bool isAllMonitor(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    bool isAllMode(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, TA_Base_Core::EOperationMode mode);
    bool isAnyStartAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    bool isAnyStopAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    bool isAllSameDebugLevel(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    std::vector<std::string> select(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const IPredicate& predicate, bool description = false);
    std::set<size_t> selectIndexes(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const IPredicate& predicate);
    std::vector<std::string> selectStartAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description = false);
    std::vector<std::string> selectStopAble(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description = false);
    std::vector<std::string> selectMonitor(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description = false);
    std::vector<std::string> selectControl(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description = false);
    std::vector<std::string> selectNotDebugLevel(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, const std::string& debugLevel, bool description = false);
    std::vector<std::string> selectNotService(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection, bool description = false);
    std::string getTimeStr(time_t t);
    std::string getDebugLevel(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    std::string getDescription(const std::string& name);
    std::string getDescription(const TA_Base_Core::ProcessDataSeq& data, size_t i);
    std::vector<std::string> getNames(const TA_Base_Core::ProcessDataSeq& data);
    std::vector<std::string> getNames(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
    std::vector<std::string> getDescriptions(const TA_Base_Core::ProcessDataSeq& data, const std::set<size_t>& selection);
}
