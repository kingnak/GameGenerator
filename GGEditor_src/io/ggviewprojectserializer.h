#ifndef GGVIEWPROJECTSERIALIZER_H
#define GGVIEWPROJECTSERIALIZER_H

#include <io/ggbasicprojectserializer.h>
#include <gg_definitions.h>
#include <QMap>

class GGViewModel;
class GGViewPage;
class GGViewConnection;

class GGViewProjectSerializer : public GGBasicProjectSerializer
{
public:
    GGViewProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor);

    virtual bool saveProject(GGEditProject *project, GGViewModel *viewModel);

protected:
    virtual bool injectPageData(GGPage *page, QVariantMap &v);
    virtual bool injectConnectionData(GGConnection *connection, QVariantMap &v);
    virtual bool finalizeScene(GGScene *scene);

protected:
    QVariantMap serializeViewPage(GGViewPage *vp, bool &ok);
    QVariantMap serializeViewConnection(GGViewConnection *vc, bool &ok);

    GGViewModel *m_viewModel;

    QMap<GG::PageID, QList<GGViewPage*> > m_foreignPages;
    QMap<GG::ConnectionID, QList<GGViewConnection*> > m_foreignConns;
};

#endif // GGVIEWPROJECTSERIALIZER_H
