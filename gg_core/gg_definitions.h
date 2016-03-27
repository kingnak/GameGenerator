#ifndef GG_DEFINITIONS_H
#define GG_DEFINITIONS_H

#include <QtGlobal>
#include <QMetaType>

namespace GG {
typedef quint32 SceneID;
typedef quint32 PageID;
typedef quint32 ConnectionID;

static const SceneID InvalidSceneId = 0xFFFFFFFF;
static const PageID InvalidPageId = 0xFFFFFFFF;
static const ConnectionID InvalidConnectionId = 0xFFFFFFFF;
}

//Q_DECLARE_METATYPE(GG::PageID)
//Q_DECLARE_METATYPE(GG::ConnectionID)

#endif // GG_DEFINITIONS_H

