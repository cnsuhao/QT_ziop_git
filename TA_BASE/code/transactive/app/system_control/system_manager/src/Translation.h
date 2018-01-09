#pragma once
#include "FormattedString.h"
#include <QString>
#include <QCoreApplication>

namespace TA_Base_App
{
    struct Translation
    {
        Q_DECLARE_TR_FUNCTIONS(Translation)

    public:

        virtual QString tr(const std::string& s);
        virtual QString tr(const FormattedString& s);
        virtual QStringList tr(const std::vector<std::string>& strs);

    private:

        void empty();
    };
}
