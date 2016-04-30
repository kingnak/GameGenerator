#ifndef GGXMLUNSERIALIZER_H
#define GGXMLUNSERIALIZER_H

#include <QString>
#include <QVariantMap>
#include <QPair>
#include <QStack>
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

class QIODevice;

class GGXmlUnserializerHandler
{
public:
    enum HandleType {
        Push, PushList, Pop, Error
    };

    virtual ~GGXmlUnserializerHandler() {}
    virtual HandleType handleElement(QString &name, QVariant &data) = 0;
};

class GGXmlUnserializer : public QXmlDefaultHandler
{
public:
    GGXmlUnserializer(GGXmlUnserializerHandler *handler);

    bool load(QIODevice *device);
    virtual QString errorString() const;

private:
    virtual bool startDocument();
    virtual bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    virtual bool characters(const QString &ch);
    virtual bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    virtual bool endDocument();
    virtual void setDocumentLocator(QXmlLocator *locator);
    virtual bool skippedEntity(const QString &name);

    virtual bool error(const QXmlParseException &exception);
    virtual bool fatalError(const QXmlParseException &exception);

private:
    bool setError(const QString &error);

private:
    GGXmlUnserializerHandler *m_handler;
    QXmlLocator *m_locator;
    QString m_error;

    typedef QPair<QString, QVariant> Element;
    QStack<Element> m_elementStack;
};

#endif // GGXMLUNSERIALIZER_H
