#ifndef GG_CORE_GLOBAL_H
#define GG_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GG_CORE_LIBRARY)
#  define GG_CORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define GG_CORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GG_CORE_GLOBAL_H
