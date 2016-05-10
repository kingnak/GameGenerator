#ifndef GGSTYLE_H
#define GGSTYLE_H

#include "gg_core_global.h"
#include <QColor>

class GG_CORESHARED_EXPORT GGBasicStyle
{
public:
    GGBasicStyle();

    QString font() const;
    void setFont(const QString &font);

    quint8 pointSize() const;
    void setPointSize(quint8 ptSize);

    QColor background() const;
    void setBackground(const QColor &background);

    QColor foreground() const;
    void setForeground(const QColor &foreground);

private:
    QString m_font;
    quint8 m_ptSize;
    QColor m_background;
    QColor m_foreground;
};

class GG_CORESHARED_EXPORT GGStyle
{
public:
    GGStyle();

    QString name() const;
    void setName(const QString &name);

    QColor foreground() const;
    void setForeground(const QColor &foreground);

private:
    QString m_name;
    QColor m_foreground;
};

#endif // GGSTYLE_H
