#ifndef GG_GLOBAL_H
#define GG_GLOBAL_H

#include <QtGlobal>

namespace GG {
typedef quint32 PageID;
typedef quint32 ConnectionID;

static const PageID InvalidPageId = 0xFFFFFFFF;
static const ConnectionID InvalidConnectionId = 0xFFFFFFFF;
}

#endif // GG_GLOBAL_H

