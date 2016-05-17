#include "ggxmlunserializer.h"
#include <io/ggserialization.hpp>
#include <QColor>
#include <QRect>

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleElement(QString &name, QVariant &data)
{
    QVariantMap map;
    data >> map;

    if (name == m_root) {
        m_doc = map;
        return Pop;
    }

    HandleType t = handleDefaults(map, name, data);
    if (t == Pop) return Push;
    return t;
}

void GGDefaultXmlUnserializationHandler::addListType(const QString &name)
{
    m_listTypes << name;
}

void GGDefaultXmlUnserializationHandler::setRootElement(const QString &name)
{
    m_root = name;
}

QVariantMap GGDefaultXmlUnserializationHandler::getDocument() const
{
    return m_doc;
}

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleText(QVariantMap &map, QString &name, QVariant &data)
{
    // Set Text as only content
    if (map.contains(" text ")) {
        if (map.size() != 1) {
            name = "Only text allowed as content";
            return Error;
        }
        data = map[" text "];
        return Push;
    }
    return Pop;
}

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleColor(QVariantMap &map, QString &name, QVariant &data)
{
    Q_UNUSED(name);
    // Fix Colors
    if (map.size() == 1 && map.contains("rgb")) {
        bool ok;
        quint32 v = map["rgb"].toString().toUInt(&ok, 16);
        if (!ok) return Error;
        v &= 0xFFFFFF;
        QColor c = QColor::fromRgb((QRgb) v);
        data = QVariant::fromValue(c);
        return Push;
    }
    return Pop;
}

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleRect(QVariantMap &map, QString &name, QVariant &data)
{
    // Fix Rects
    //if (name == "rect" && map.size() == 4) {
    if (map.size() == 4 && map.contains("x") && map.contains("y") && map.contains("w") && map.contains("h")) {
        // Assume rect
        bool ok;
        int x = map["x"].toInt(&ok);
        if (!ok) return Error;
        int y = map["y"].toInt(&ok);
        if (!ok) return Error;
        int w = map["w"].toInt(&ok);
        if (!ok) return Error;
        int h = map["h"].toInt(&ok);
        if (!ok) return Error;

        QRect r(x,y,w,h);
        data = r;
        return Push;
    }
    return Pop;
}

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleLists(QVariantMap &map, QString &name, QVariant &data)
{
    if (m_listTypes.contains(name)) {
        // Special case: text lists
        handleText(map, name, data);
        return PushList;
    }
    foreach (QString i, m_listTypes) {
        if (i+"s" == name) {
            name = i;
            data = map[i];
            return Push;
        }
    }
    return Pop;
}

GGXmlUnserializerHandler::HandleType GGDefaultXmlUnserializationHandler::handleDefaults(QVariantMap &map, QString &name, QVariant &data)
{
    HandleType t = handleLists(map, name, data);
    if (t != Pop) return t;
    t = handleRect(map, name, data);
    if (t != Pop) return t;
    t = handleColor(map, name, data);
    if (t != Pop) return t;
    t = handleText(map, name, data);
    if (t != Pop) return t;

    return t;
}

////////////////////

GGXmlUnserializer::GGXmlUnserializer(GGXmlUnserializerHandler *handler)
    : m_handler(handler),
      m_locator(NULL)
{

}

bool GGXmlUnserializer::load(QIODevice *device)
{
    QXmlInputSource src(device);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    reader.setFeature("http://qt-project.org/xml/features/report-whitespace-only-CharData", false);
    return reader.parse(src);
}

bool GGXmlUnserializer::startDocument()
{
    m_error = QString::null;
    m_elementStack.clear();
    m_elementStack.push(qMakePair(QString(" ROOT "), QVariantMap()));
    return true;
}

bool GGXmlUnserializer::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

    QVariantMap map;
    for (int i = 0; i < atts.count(); ++i) {
        map[atts.localName(i)] = atts.value(i);
    }

    QVariant v;
    v << map;
    m_elementStack.push(qMakePair(localName, v));
    return true;
}

bool GGXmlUnserializer::characters(const QString &ch)
{
    QVariant v = m_elementStack.top().second;
    QVariantMap m;
    v >> m;
    QString t = m[" text "].toString();
    t += ch;
    m[" text "] = t;
    v = m;
    m_elementStack.top().second = v;
    return true;
}

bool GGXmlUnserializer::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

    Q_ASSERT(localName == m_elementStack.top().first);
    if (localName != m_elementStack.top().first) {
        return setError("Element nesting mismatch");
    }

    Element e = m_elementStack.pop();
    QString n = e.first;
    QVariant v = e.second;

    GGXmlUnserializerHandler::HandleType h = m_handler->handleElement(n, v);
    switch (h) {
    case GGXmlUnserializerHandler::Push:
        // Push as child of parent
    {
        QVariant parent = m_elementStack.top().second;
        QVariantMap m;
        parent >> m;
        m[n] = v;
        parent = m;
        m_elementStack.top().second = parent;
    }
        break;
    case GGXmlUnserializerHandler::PushList:
        // Push as list in parent
    {
        QVariant parent = m_elementStack.top().second;
        QVariantMap m;
        parent >> m;
        QVariantList lst;
        if (m.contains(n)) {
            if (m[n].canConvert<QVariantList>()) {
                lst = qvariant_cast<QVariantList> (m[n]);
            } else {
                return setError(QString("Element %1 must be a list").arg(n));
            }
        }
        lst.append(v);
        v = lst;
        m[n] = v;
        parent = m;
        m_elementStack.top().second = parent;
    }
        break;
    case GGXmlUnserializerHandler::Pop:
        // Handled, nothing to do
        break;
    case GGXmlUnserializerHandler::Error:
        return setError(n);
        break;
    }

    return true;
}

bool GGXmlUnserializer::endDocument()
{
    Element e = m_elementStack.pop();
    Q_ASSERT(m_elementStack.empty());
    m_locator = NULL;
    if (m_handler->handleElement(e.first, e.second) == GGXmlUnserializerHandler::Error) {
        return setError(e.first);
    }
    return true;
}

QString GGXmlUnserializer::errorString() const
{
    return m_error;
}

void GGXmlUnserializer::setDocumentLocator(QXmlLocator *locator)
{
    m_locator = locator;
}

bool GGXmlUnserializer::skippedEntity(const QString &name)
{
    return setError("Skipped Entity: " + name);
}

bool GGXmlUnserializer::error(const QXmlParseException &exception)
{
    return setError(exception.message());
}

bool GGXmlUnserializer::fatalError(const QXmlParseException &exception)
{
    return setError(exception.message());
}

bool GGXmlUnserializer::setError(const QString &error)
{
    if (m_locator) {
        m_error = QString("Error in line %1, column %2: %3")
                .arg(m_locator->lineNumber())
                .arg(m_locator->columnNumber())
                .arg(error);
    } else {
        m_error = error;
    }
    return false;
}
