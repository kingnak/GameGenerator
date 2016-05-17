#ifndef GGVIEWPROJECTUNSERIALIZER_H
#define GGVIEWPROJECTUNSERIALIZER_H

#include <io/ggbasicprojectunserializer.h>

class GGViewModel;
class GGViewPage;

class GGViewProjectUnserializer : public GGBasicProjectUnserializer
{
public:
    GGViewProjectUnserializer(const QString &basePath, const QString &fileName, GGAbstractUnserializationReader *reader, GGUnserializationProcessor *processor);
    virtual ~GGViewProjectUnserializer();

    virtual bool load(QIODevice *device);

    GGViewModel *loadedViewModel();
    GGViewModel *takeViewModel();

    bool unserializeScene(QVariant scene);
    bool unserializePage(QVariant page);
    bool unserializeConnection(QVariant connection);
    bool unserializeForeignPage(QVariant page);
    bool unserializeForeignConnection(QVariant connection);
    bool finalizeUnserialization();

protected:
    bool unserializeViewPage(QVariant viewPage, GG::PageID pid);
    bool unserializeViewConnection(QVariant viewConnection, GG::ConnectionID cid);

protected:
    GGViewModel *m_viewModel;
    typedef QPair<GG::SceneID, QRect> SceneRectPair;
    typedef QList<SceneRectPair> SceneRectPairList;
    QMap<GG::PageID, SceneRectPairList> m_vpMap;
    QMap<GG::ConnectionID, QList<GG::SceneID> > m_vcMap;
    QMap<GG::SceneID, QPoint> m_scenePos;
};

#endif // GGVIEWPROJECTUNSERIALIZER_H
