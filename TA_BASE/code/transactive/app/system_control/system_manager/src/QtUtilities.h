#pragma once
#include <QObject>
#include <boost/preprocessor/cat.hpp>

namespace QtUtilities
{
    template<typename to, typename from> to convertTo(const from&);
    template<typename to, typename from> to convertToUnicode(const from&);
    template<typename T, typename C> void disableItems(T* widget, const C& indexes);

    struct QtBlockSignalsGuard
    {
        QtBlockSignalsGuard(QObject* obj) : m_obj(obj) { m_obj->blockSignals(true); }
        ~QtBlockSignalsGuard() { m_obj->blockSignals(false); }
        QObject* m_obj;
    };
}

#define BLOCK_SIGNALS(obj) QtUtilities::QtBlockSignalsGuard BOOST_PP_CAT(block_signal_, __line__)(obj)
