#ifndef GGABSTRACTSERIALIZATIONWRITER_H
#define GGABSTRACTSERIALIZATIONWRITER_H

#include <QVariant>

class GGAbstractSerializationWriter
{
public:
    virtual ~GGAbstractSerializationWriter() {}

    virtual bool writeHeader() = 0;
    virtual bool writeFooter() = 0;

    virtual bool writeProject(QVariant project) = 0;
    virtual bool writeScene(QVariant scene) = 0;
    virtual bool writePage(QVariant page) = 0;
    virtual bool writeConnection(QVariant connection) = 0;
};

#endif // GGABSTRACTSERIALIZATIONWRITER_H
