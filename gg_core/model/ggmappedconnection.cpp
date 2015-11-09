#include "ggmappedconnection.h"


GGMappedConnection GGMappedConnection::rectangle(QRect r, GGConnection *conn)
{
    GGMappedConnection m(Rectangle, conn);
    m.setRectangle(r);
    return m;
}

GGMappedConnection GGMappedConnection::ellipse(QRect e, GGConnection *conn)
{
    GGMappedConnection m(Ellipse, conn);
    m.setEllipse(e);
    return m;
}

GGMappedConnection GGMappedConnection::polygon(QList<QPoint> p, GGConnection *conn)
{
    GGMappedConnection m(Polygon, conn);
    m.setPolygon(p);
    return m;
}

bool GGMappedConnection::setRectangle(QRect r)
{
    if (m_type == Rectangle) {
        m_data = r;
        return true;
    }
    return false;
}

bool GGMappedConnection::setEllipse(QRect e)
{
    if (m_type == Ellipse) {
        m_data = e;
        return true;
    }
    return false;
}

bool GGMappedConnection::setPolygon(QList<QPoint> p)
{
    if (m_type == Polygon && p.size() > 2) {
        m_data = QVariant::fromValue(p);
        return true;
    }
    return false;
}

QRect GGMappedConnection::rectangle() const
{
    if (m_type == Rectangle) {
        return qvariant_cast<QRect>(m_data);
    }
    return QRect();
}

QRect GGMappedConnection::ellipse() const
{
    if (m_type == Ellipse) {
        return qvariant_cast<QRect>(m_data);
    }
    return QRect();
}

QList<QPoint> GGMappedConnection::polygon() const
{
    if (m_type == Polygon) {
        return qvariant_cast<QList<QPoint> >(m_data);
    }
    return QList<QPoint>();
}
