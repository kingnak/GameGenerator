#ifndef GGABSTRACTSERIALIZATIONWRITER_H
#define GGABSTRACTSERIALIZATIONWRITER_H

#include <QVariant>

class GGAbstractSerializationWriter
{
public:
    virtual ~GGAbstractSerializationWriter() {}

    virtual bool writeHeader() = 0;
    virtual bool writeFooter() = 0;

    virtual bool writeProjectStart() = 0;
    virtual bool writeProject(const QVariant &project) = 0;
    virtual bool writeProjectEnd() = 0;

    virtual bool writeSceneStart() = 0;
    virtual bool writeScene(const QVariant &scene) = 0;
    virtual bool writeSceneEnd() = 0;

    virtual bool writePagesStart() = 0;
    virtual bool writePage(const QVariant &page) = 0;
    virtual bool writePagesEnd() = 0;

    virtual bool writeConnectionsStart() = 0;
    virtual bool writeConnection(const QVariant &connection) = 0;
    virtual bool writeConnectionsEnd() = 0;

    virtual bool writeForeignPagesStart() = 0;
    virtual bool writeForeignPage(const QVariant &page) = 0;
    virtual bool writeForeignPagesEnd() = 0;

    virtual bool writeForeignConnectionsStart() = 0;
    virtual bool writeForeignConnection(const QVariant &connection) = 0;
    virtual bool writeForeignConnectionsEnd() = 0;
};

#endif // GGABSTRACTSERIALIZATIONWRITER_H
