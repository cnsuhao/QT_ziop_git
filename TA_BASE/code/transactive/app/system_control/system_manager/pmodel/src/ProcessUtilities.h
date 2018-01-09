#pragma once
#include "Predicate.h"
#include "CommonDefs.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace TA_Base_App
{
    struct IDatabaseService;
    typedef Predicate1<size_t> Predicate;

    struct ProcessUtilities
    {
    public:

        ProcessUtilities();
        ProcessUtilities(const TA_Base_Core::ProcessDataSeq& data);
        ProcessUtilities(const TA_Base_Core::ProcessDataSeq& data, const IndexSet& selection);
        ProcessUtilities& set(const TA_Base_Core::ProcessDataSeq& data);
        ProcessUtilities& set(const IndexSet& selection);
        ProcessUtilities& set(const TA_Base_Core::ProcessDataSeq& data, const IndexSet& selection);
        ProcessList convertToVector();
        bool isMode(size_t i, TA_Base_Core::EOperationMode mode);
        bool isControl(size_t i);
        bool isMonitor(size_t i);
        bool isNotApplicable(size_t i);
        bool isStartAble(size_t i);
        bool isStopAble(size_t i);
        bool isDebugLevel(size_t i, const std::string& level);
        bool isService(size_t i);
        bool isAutoFailBack(size_t i);
        bool predicate(const Predicate& pred, bool isAny = false);
        bool isAllControl();
        bool isAllMonitor();
        bool isAllMode(TA_Base_Core::EOperationMode mode);
        bool isAnyStartAble();
        bool isAnyStopAble();
        bool isAllSameDebugLevel();
        bool isAnyCanChangeMode();
        bool isAllCannotChangeMode();
        bool isAllCannotChangeToControl();
        bool isAllCannotChangeToMonitor();
        bool isCanChangeToBothMode();
        IndexSet selectIndexes(const Predicate& pred);
        IndexSet selectStartAbleIndexes();
        IndexSet selectStopAbleIndexes();
        IndexSet selectCanChangeModeIndexes();
        StringList select(const Predicate& pred, bool description = false);
        StringList selectStartAble(bool description = false);
        StringList selectStopAble(bool description = false);
        StringList selectNotStartAble(bool description = false);
        StringList selectNotStopAble(bool description = false);
        StringList selectMonitor(bool description = false);
        StringList selectControl(bool description = false);
        StringList selectNotDebugLevel(const std::string& debugLevel, bool description = false);
        StringList selectNotService(bool description = false);
        StringList selectCanChangeMode(bool description = false);
        StringList selectCanChangeToControl(bool description = false);
        StringList selectCanChangeToMonitor(bool description = false);
        StringList selectCannotChangeMode(bool description = false);
        StringList selectCannotChangeToControl(bool description = false);
        StringList selectCannotChangeToMonitor(bool description = false);
        std::string getDebugLevel(size_t i);
        std::string getDescription(const std::string& name);
        std::string getDescription(size_t i);
        StringList getNames();
        StringList getAllNames();
        StringList getDescriptions();
        static std::string getTimeStr(time_t t);
        static int getDebugLevelIndex(const std::string& debugLevel);
        static const StringList& getDebugLevels();
        static ProcessUpdates getUpdates(const TA_Base_Core::ProcessDataSeq& old, const TA_Base_Core::ProcessDataSeq& newed);

    private:

        void initPredicates();

    private:

        Predicate control;
        Predicate monitor;
        Predicate notApplicable;
        Predicate service;
        Predicate autoFailBack;
        Predicate canChangeMode;
        Predicate canChangeToControl;
        Predicate canChangeToMonitor;
        Predicate startAble;
        Predicate stopAble;
        IndexSet m_selection;
        IDatabaseService& m_dbservice;
        const TA_Base_Core::ProcessDataSeq* m_data;
    };
}
