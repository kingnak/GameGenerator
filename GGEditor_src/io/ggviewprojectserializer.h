#ifndef GGVIEWPROJECTSERIALIZER_H
#define GGVIEWPROJECTSERIALIZER_H

#include <io/ggbasicprojectserializer.h>

class GGViewModel;

class GGViewProjectSerializer : public GGBasicProjectSerializer
{
public:
    GGViewProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor);

    virtual bool saveProject(GGEditProject *project, GGViewModel *viewModel);

protected:
    virtual bool injectPageData(GGPage *page, QVariantMap &v);
    virtual bool injectConnectionData(GGConnection *connection, QVariantMap &v);

protected:
    GGViewModel *m_viewModel;
};

#endif // GGVIEWPROJECTSERIALIZER_H
