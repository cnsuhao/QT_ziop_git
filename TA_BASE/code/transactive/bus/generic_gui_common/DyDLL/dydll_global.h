#ifndef DYDLL_GLOBAL_H
#define DYDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef DYDLL_LIB
# define DYDLL_EXPORT Q_DECL_EXPORT
#else
# define DYDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // DYDLL_GLOBAL_H
