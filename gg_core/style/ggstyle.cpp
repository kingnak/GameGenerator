#include "ggstyle.h"

GGBasicStyle::GGBasicStyle()
    : m_font("Times New Roman"),
      m_ptSize(12),
      m_background(Qt::black),
      m_foreground(Qt::yellow)
{

}

QString GGBasicStyle::font() const
{
    return m_font;
}

void GGBasicStyle::setFont(const QString &font)
{
    m_font = font;
}

quint8 GGBasicStyle::pointSize() const
{
    return m_ptSize;
}

void GGBasicStyle::setPointSize(quint8 ptSize)
{
    m_ptSize = ptSize;
}

QColor GGBasicStyle::background() const
{
    return m_background;
}

void GGBasicStyle::setBackground(const QColor &background)
{
    m_background = background;
}

QColor GGBasicStyle::foreground() const
{
    return m_foreground;
}

void GGBasicStyle::setForeground(const QColor &foreground)
{
    m_foreground = foreground;
}

QString GGBasicStyle::toCSS(const QString &selector) const
{
    QString ret;
    ret = selector + " {\n";
    ret += QString("\tfont-size: %1pt;\n").arg(m_ptSize);
    ret += QString("\tfont-family: \"%1\";\n").arg(m_font);
    ret += QString("\tcolor: rgb(%1,%2,%3);\n").arg(m_foreground.red()).arg(m_foreground.green()).arg(m_foreground.blue());
    ret += QString("\tbackground-color: rgb(%1,%2,%3);\n").arg(m_background.red()).arg(m_background.green()).arg(m_background.blue());
    ret += "}\n\n";
    return ret;
}

/////////////////////////

GGStyle::GGStyle()
{

}

QString GGStyle::name() const
{
    return m_name;
}

void GGStyle::setName(const QString &name)
{
    m_name = name;
}

QColor GGStyle::foreground() const
{
    return m_foreground;
}

void GGStyle::setForeground(const QColor &foreground)
{
    m_foreground = foreground;
}

QString GGStyle::toCSS() const
{
    QString ret;
    ret = QString(".%1 {\n").arg(m_name);
    ret += QString("\tcolor: rgb(%1,%2,%3);\n").arg(m_foreground.red()).arg(m_foreground.green()).arg(m_foreground.blue());
    ret += "}\n\n";
    return ret;
}
