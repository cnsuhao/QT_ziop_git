#include "QtUtilities.h"
#include <QString>
#include <QTextCodec>
#include <QList>
#include <QListWidget>
#include <vector>
#include <string>
#include <boost/foreach.hpp>
#include <set>


namespace QtUtilities
{
    template<> QStringList convertTo(const std::vector<std::string>& vs)
    {
        QStringList qs;
        BOOST_FOREACH(const std::string& s, vs)
        {
            qs << QString::fromStdString(s);
        }
        return qs;
    }


    template<> QStringList convertTo(const std::vector<std::wstring>& vs)
    {
        QStringList qs;
        BOOST_FOREACH(const std::wstring& s, vs)
        {
            qs << QString::fromStdWString(s);
        }
        return qs;
    }


    template<> QStringList convertToUnicode(const std::vector<std::string>& vs)
    {
        QStringList qs;
        BOOST_FOREACH(const std::string& s, vs)
        {
            qs << QTextCodec::codecForLocale()->toUnicode(s.c_str());
        }
        return qs;
    }


    template<> void disableItems(QListWidget* widget, const std::set<size_t>& indexes)
    {
        for (size_t i = 0; i < widget->count(); ++i)
        {
            bool disable = indexes.find(i) != indexes.end();
            QListWidgetItem* item = widget->item(i);
            Qt::ItemFlags flags = item->flags() | Qt::ItemIsEnabled;
            item->setFlags(disable ? flags ^ Qt::ItemIsEnabled : flags);
        }
    }
}
