#ifndef GGMAPPEDLINK_H
#define GGMAPPEDLINK_H

#include "gg_core_global.h"
#include <QList>
#include <QPoint>
#include <QRect>
#include <QVariant>
#include "gglink.h"

class GGConnection;

class GG_CORESHARED_EXPORT GGMappedLink
{
public:
    enum Type {
        Rectangle, Ellipse, Polygon
    };
    GGMappedLink() : m_type(Rectangle) {}

    static GGMappedLink rectangle(QRect r);
    static GGMappedLink ellipse(QRect e);
    static GGMappedLink polygon(QList<QPoint> p);

    Type type() const { return m_type; }

    void convertToRectangle();
    void convertToEllipse();
    void convertToPolygon();

    bool setRectangle(QRect r);
    bool setEllipse(QRect e);
    bool setPolygon(QList<QPoint> p);

    QRect rectangle() const;
    QRect ellipse() const;
    QList<QPoint> polygon() const;

    void setLink(GGLink link) { m_link = link; }
    GGLink link() const { return m_link; }

private:
    GGMappedLink(Type t) : m_type(t)  {}

private:
    Type m_type;
    QVariant m_data;
    GGLink m_link;
};

#endif // GGMAPPEDLINK_H
