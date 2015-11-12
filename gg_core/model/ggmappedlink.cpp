#include "ggmappedlink.h"


GGMappedLink GGMappedLink::rectangle(QRect r)
{
    GGMappedLink m(Rectangle);
    m.setRectangle(r);
    return m;
}

GGMappedLink GGMappedLink::ellipse(QRect e)
{
    GGMappedLink m(Ellipse);
    m.setEllipse(e);
    return m;
}

GGMappedLink GGMappedLink::polygon(QList<QPoint> p)
{
    GGMappedLink m(Polygon);
    m.setPolygon(p);
    return m;
}

bool GGMappedLink::setRectangle(QRect r)
{
    if (m_type == Rectangle) {
        m_data = r;
        return true;
    }
    return false;
}

bool GGMappedLink::setEllipse(QRect e)
{
    if (m_type == Ellipse) {
        m_data = e;
        return true;
    }
    return false;
}

bool GGMappedLink::setPolygon(QList<QPoint> p)
{
    if (m_type == Polygon && p.size() > 2) {
        m_data = QVariant::fromValue(p);
        return true;
    }
    return false;
}

QRect GGMappedLink::rectangle() const
{
    if (m_type == Rectangle) {
        return qvariant_cast<QRect>(m_data);
    }
    return QRect();
}

QRect GGMappedLink::ellipse() const
{
    if (m_type == Ellipse) {
        return qvariant_cast<QRect>(m_data);
    }
    return QRect();
}

QList<QPoint> GGMappedLink::polygon() const
{
    if (m_type == Polygon) {
        return qvariant_cast<QList<QPoint> >(m_data);
    }
    return QList<QPoint>();
}
