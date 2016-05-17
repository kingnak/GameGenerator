#include "ggxmlserializer.h"

#include <QXmlStreamWriter>
#include <QRect>
#include <QColor>

GGXmlSerializer::GGXmlSerializer()
{

}

bool GGXmlSerializer::writeData(QXmlStreamWriter *writer, const QString &tag, const QVariant &data, const QSet<QString> &attributes, const QSet<QString> &exclude)
{
    if (data.canConvert<QVariantMap>()) {
        writer->writeStartElement(tag);
        QSet<QString> handled = exclude;
        QVariantMap map = qvariant_cast<QVariantMap> (data);
        foreach (QString k, map.keys()) {
            if (!handled.contains(k)) {
                if (attributes.contains(k)) {
                    writer->writeAttribute(k, map[k].toString());
                    handled.insert(k);
                }
            }
        }

        foreach (QString k, map.keys()) {
            if (!handled.contains(k)) {
                writeData(writer, k, map[k]);
                handled.insert(k);
            }
        }
        writer->writeEndElement();
    } else if (data.canConvert<QVariantList>()){
        writer->writeStartElement(tag+"s");
        QVariantList lst = qvariant_cast<QVariantList> (data);
        foreach (QVariant v, lst) {
            writeData(writer, tag, v);
        }
        writer->writeEndElement();
    } else if (data.canConvert<QPoint>()) {
        writer->writeStartElement(tag);
        writer->writeAttribute("x", QString::number(data.value<QPoint>().x()));
        writer->writeAttribute("y", QString::number(data.value<QPoint>().y()));
        writer->writeEndElement();
    } else if (data.canConvert<QRect>()) {
        writer->writeStartElement(tag);
        writer->writeAttribute("x", QString::number(data.value<QRect>().x()));
        writer->writeAttribute("y", QString::number(data.value<QRect>().y()));
        writer->writeAttribute("w", QString::number(data.value<QRect>().width()));
        writer->writeAttribute("h", QString::number(data.value<QRect>().height()));
        writer->writeEndElement();
    } else if (data.type() == QVariant::Color) {
        writer->writeStartElement(tag);
        QString hex = QString::number((quint32) data.value<QColor>().rgb() & 0xFFFFFF, 16);
        while (hex.length() < 6) hex = "0" + hex;
        writer->writeAttribute("rgb", hex);
        writer->writeEndElement();
    } else {
        writer->writeTextElement(tag, data.toString());
    }
    return true;
}

