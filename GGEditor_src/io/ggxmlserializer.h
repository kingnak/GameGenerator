#ifndef GGXMLSERIALIZER_H
#define GGXMLSERIALIZER_H

#include <Qvariant>
#include <QSet>

class QXmlStreamWriter;

class GGXmlSerializer
{
public:
    GGXmlSerializer();

    bool writeData(QXmlStreamWriter *writer, const QString &tag, const QVariant &data, const QSet<QString> &attributes = QSet<QString>(), const QSet<QString> &exclude = QSet<QString>());

};

#endif // GGXMLSERIALIZER_H
