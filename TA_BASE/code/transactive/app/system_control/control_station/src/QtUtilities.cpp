#include "StdAfx.h"
#include "QtUtilities.h"
#include <QString>
#include <QTextCodec>
#include <QList>
#include <QListWidget>
#include <QMovie>

namespace QtUtilities
{
    template<> QStringList convertTo(const std::vector<std::string>& vs)
    {
        QStringList qs;

        for (const std::string& s : vs)
        {
            qs << QString::fromStdString(s);
        }

        return qs;
    }

    template<> QStringList convertTo(const std::vector<std::wstring>& vs)
    {
        QStringList qs;

        for (const std::wstring& s : vs)
        {
            qs << QString::fromStdWString(s);
        }

        return qs;
    }

    template<> QStringList convertToUnicode(const std::vector<std::string>& vs)
    {
        QStringList qs;

        for (const std::string& s : vs)
        {
            qs << QTextCodec::codecForLocale()->toUnicode(s.c_str());
        }

        return qs;
    }

    template<> QString convertToUnicode(const std::string& s)
    {
        return QTextCodec::codecForLocale()->toUnicode(s.c_str());
    }

    QString convertToUnicode(const char* s)
    {
        return QTextCodec::codecForLocale()->toUnicode(s);
    }

    template<> void disableItems(QListWidget* widget, const std::set<size_t>& indexes)
    {
        for (int i = 0; i < widget->count(); ++i)
        {
            bool disable = indexes.find(i) != indexes.end();
            QListWidgetItem* item = widget->item(i);
            Qt::ItemFlags flags = item->flags() | Qt::ItemIsEnabled;
            item->setFlags(disable ? flags ^ Qt::ItemIsEnabled : flags);
        }
    }

    void startMovie(QLabel* label, const std::string& resource)
    {
        QMovie* movie = new QMovie(QString::fromStdString(resource));
        movie->start();
        label->setMovie(movie);
        label->setScaledContents(true);
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}
