#include "ggxmlunserializer.h"
#include <io/ggserialization.hpp>

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

