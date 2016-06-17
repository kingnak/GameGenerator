#include "ggspanstyler.h"
#include <style/ggstyle.h>
#include <QTextFormat>
#include <QTextDocument>
#include <QTextBlock>
#include <QXmlDefaultHandler>
#include <QXmlStreamWriter>
#include <QStack>
#include <QMap>

class SpanStyleParser : public QXmlDefaultHandler
{
public:
    SpanStyleParser(const GGSpanStyler *styler, QTextDocument *d);
    ~SpanStyleParser();
    bool startDocument();
    bool startElement(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
    bool endElement(const QString & namespaceURI, const QString & localName, const QString & qName);
    bool characters(const QString & ch);
private:
    QTextDocument *m_doc;
    QTextCursor *m_cur;
    QStack<QTextCharFormat> m_stack;
    QMap<QString, QTextCharFormat> m_styles;
};

GGSpanStyler::GGSpanStyler()
{

}

QTextDocument *GGSpanStyler::styleString(const QString &doc) const
{
    QTextDocument *d = new QTextDocument;
    QTextCursor cur(d);
    cur.setBlockFormat(formatForBasicStyle().toBlockFormat());
    cur.setBlockCharFormat(formatForDefaultStyle().toCharFormat());

    if (!doc.isEmpty()) {
        QXmlSimpleReader reader;
        SpanStyleParser parser(this, d);
        reader.setContentHandler(&parser);
        QXmlInputSource input;
        input.setData("<root>"+doc+"</root>");
        reader.parse(&input);
    }
    return d;
}

QString GGSpanStyler::parseDocument(const QTextDocument *doc) const
{
    QString ret;
    QXmlStreamWriter w(&ret);
    QTextBlock b = doc->firstBlock();
    while (b.isValid()) {
        QString prevStyle;
        bool prevItalic = false;
        QTextBlock::iterator it = b.begin();
        for (; !it.atEnd(); ++it) {
            QTextFragment frag = it.fragment();
            QTextCharFormat f = frag.charFormat();

            QString curStyle = f.stringProperty(QTextFormat::UserProperty);
            bool curItalic = f.fontItalic();

            if (prevStyle == curStyle) {
                if (prevItalic != curItalic) {
                    if (!curItalic) w.writeEndElement();
                    else w.writeStartElement("em");
                }
                // No change
            } else {
                if (prevItalic) w.writeEndElement();
                if (!prevStyle.isEmpty()) w.writeEndElement();
                if (!curStyle.isEmpty()) {
                    w.writeStartElement("span");
                    w.writeAttribute("class", curStyle);
                }
                if (curItalic) w.writeStartElement("em");
            }

            prevStyle = curStyle;
            prevItalic = curItalic;

            w.writeCharacters(frag.text());
        }

        b = b.next();
        if (b.isValid()) {
            w.writeEndDocument();
            w.writeEmptyElement("br");
        }
    }
    w.writeEndDocument();

    ret.replace("\n", "");

    return ret;
}

QString GGSpanStyler::plainText(const QString &doc) const
{
    QString plain = doc;
    plain.replace(QRegExp("<br\\s*/>"), " ");
    plain.replace(QRegExp("<[^>]*>"), "");
    plain.replace("&lt;", "<");
    plain.replace("&gt;", ">");
    plain.replace("&amp;", "&");
    return plain;
}

QString GGSpanStyler::previewText(const QString &doc) const
{
    QString prev = doc;
    prev.replace(QRegExp("</?span\\b[^>]*>"), "");
    return prev;
}

QString GGSpanStyler::htmlText(const QString &doc) const
{
    return doc;
}

GGAbstractStyler::SupplementalStyleOptions GGSpanStyler::supplementalOptions() const
{
    return Italic;
}

QTextFormat GGSpanStyler::formatForBasicStyle() const
{
    QTextBlockFormat f;
    f.setBackground(m_basic.background());
    f.setForeground(m_basic.foreground());
    return f;
}

QTextFormat GGSpanStyler::formatForDefaultStyle() const
{
    QTextCharFormat f;
    QString sanName = "";
    f.setProperty(QTextFormat::UserProperty, sanName);
    f.setForeground(m_basic.foreground());
    QFont font(m_basic.font(), m_basic.pointSize());
    f.setFont(font);
    return f;
}

QTextFormat GGSpanStyler::formatForStyle(const GGStyle &style) const
{
    QTextCharFormat f;
    QString sanName = sanitizeStyleName(style.name());
    f.setProperty(QTextFormat::UserProperty, sanName);
    f.setForeground(style.foreground());
    return f;
}

QString GGSpanStyler::sanitizeStyleName(const QString &name) const
{
    QString n = name;
    return n.replace(QRegExp("[^_A-Za-z1-9]"), "_");
}

QString GGSpanStyler::styleNameForFormat(const QTextFormat &format) const
{
    QString styleName = format.stringProperty(QTextFormat::UserProperty);
    if (styleName.isEmpty()) return QString::null;

    foreach (const GGStyle &s, m_styles) {
        if (styleName == sanitizeStyleName(s.name())) {
            return s.name();
        }
    }

    return QString::null;
}

////////////

SpanStyleParser::SpanStyleParser(const GGSpanStyler *styler, QTextDocument *d)
    : m_doc(d)
{
    m_cur = new QTextCursor(d);

    foreach (GGStyle s, styler->styles()) {
        m_styles[styler->sanitizeStyleName(s.name())] = styler->formatForStyle(s).toCharFormat();
    }
    m_stack.push(styler->formatForDefaultStyle().toCharFormat());
}

SpanStyleParser::~SpanStyleParser() {
    delete m_cur;
}

bool SpanStyleParser::startDocument()
{
    return true;
}

bool SpanStyleParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

    if (localName == "root") return true;
    if (localName == "em") {
        QTextCharFormat f = m_stack.top();
        QTextCharFormat f2;
        f2.setFontItalic(true);
        f.merge(f2);
        m_stack.push(f);
    } else if (localName == "span") {
        QString cls = atts.value("class");
        QTextCharFormat f = m_stack.top();
        QTextCharFormat f2 = m_styles.value(cls);
        f.merge(f2);
        m_stack.push(f);
    } else if (localName == "br") {
        m_cur->insertBlock();
    }
    return true;
}

bool SpanStyleParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

    if (localName == "root") return true;
    if (localName == "br") return true;

    m_stack.pop();
    return true;
}

bool SpanStyleParser::characters(const QString &ch)
{
    m_cur->insertText(ch, m_stack.top());
    return true;
}
