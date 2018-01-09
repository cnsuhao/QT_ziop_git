#pragma once
#include "IView.h"
#include <string>
#include <vector>
#include <map>
#include "FormattedString.h"

typedef std::vector<std::string> Process;
typedef std::vector<Process> ProcessList;
typedef std::map<std::pair<size_t, size_t>, std::string> ProcessUpdates;

namespace TA_Base_App
{
    struct ISystemManagerDlg : virtual IView
    {
        //TODO: exit process
        virtual void display() {}
        virtual void onModelClearProcessList() = 0;
        virtual void onModelProcessList(const ProcessList& processes) = 0;
        virtual void onModelUpdateProcessList(const ProcessUpdates& updates) = 0;
        virtual void onModelUpdateStatus(const FormattedString& status) = 0;
        virtual void onModelUpdateWindowTitle(const FormattedString& title) = 0;
        virtual void onModelEnableLogin(bool enable) = 0;
        virtual void onModelEnableLogout(bool enable) = 0;
        virtual void onModelEnableStartProcess(bool enable) = 0;
        virtual void onModelEnableStopProcess(bool enable) = 0;
        virtual void onModelEnableRefresh(bool enable) = 0;
        virtual void onModelEnableChangeMode(bool enable) = 0;
        virtual void onModelEnableShutdown(bool enable) = 0;
        virtual void onModelEnablSetParams(bool enable) = 0;
        virtual void onModelEnableOption(bool enable) = 0;
        virtual void onModelEnableProcess(bool enable) = 0;
        virtual void onModelSetSelection(const std::set<size_t>& selection) = 0;
        virtual void onModelSetSortIndicator(int column, bool asscending) = 0;
        virtual void onModelSetSortIndicatorShown(bool show) = 0;
    };
}
