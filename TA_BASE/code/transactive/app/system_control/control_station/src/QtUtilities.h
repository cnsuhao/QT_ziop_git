#pragma once
#include <QObject>
#include <QLabel>

namespace QtUtilities
{
    template<typename to, typename from> to convertTo(const from&);
    template<typename to, typename from> to convertToUnicode(const from&);
    QString convertToUnicode(const char*);
    template<typename T, typename C> void disableItems(T* widget, const C& indexes);

    void startMovie(QLabel* label, const std::string& resource);

    struct QtBlockSignalsGuard
    {
        QtBlockSignalsGuard(QObject* obj) : m_obj(obj) { m_obj->blockSignals(true); }
        ~QtBlockSignalsGuard() { m_obj->blockSignals(false); }
        QObject* m_obj;
    };
}

#define BLOCK_SIGNALS(obj) QtUtilities::QtBlockSignalsGuard BOOST_PP_CAT(block_signal_, __line__)(obj)
