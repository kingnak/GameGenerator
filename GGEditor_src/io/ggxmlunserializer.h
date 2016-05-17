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

class GGDefaultXmlUnserializationHandler : public GGXmlUnserializerHandler
{
public:
    virtual HandleType handleElement(QString &name, QVariant &data);
    void addListType(const QString &name);
    void setRootElement(const QString &name);
    QVariantMap getDocument() const;

protected:
    HandleType handleText(QVariantMap &map, QString &name, QVariant &data);
    HandleType handleColor(QVariantMap &map, QString &name, QVariant &data);
    HandleType handlePoint(QVariantMap &map, QString &name, QVariant &data);
    HandleType handleRect(QVariantMap &map, QString &name, QVariant &data);
    HandleType handleLists(QVariantMap &map, QString &name, QVariant &data);
    HandleType handleDefaults(QVariantMap &map, QString &name, QVariant &data);

private:
    QStringList m_listTypes;
    QString m_root;
    QVariantMap m_doc;
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
