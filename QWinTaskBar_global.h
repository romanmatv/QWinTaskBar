#ifndef QWINTASKBAR_GLOBAL_H
#define QWINTASKBAR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QWINTASKBAR_LIBRARY)
#  define QWINTASKBAR_EXPORT Q_DECL_EXPORT
#else
#  define QWINTASKBAR_EXPORT Q_DECL_IMPORT
#endif

#endif // QWINTASKBAR_GLOBAL_H