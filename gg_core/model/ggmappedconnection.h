#ifndef GGMAPPEDCONNECTION_H
#define GGMAPPEDCONNECTION_H

#include "gg_core_global.h"
#include <QList>
#include <QPoint>
#include <QRect>
#include <QVariant>

class GGConnection;

class GG_CORESHARED_EXPORT GGMappedConnection
{
public:
    enum Type {
        Rectangle, Ellipse, Polygon
    };

    static GGMappedConnection rectangle(QRect r, GGConnection *conn);
    static GGMappedConnection ellipse(QRect e, GGConnection *conn);
    static GGMappedConnection polygon(QList<QPoint> p, GGConnection *conn);

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

    void setConnection(GGConnection *conn) { m_connection = conn; }

    GGConnection *connection() const { return m_connection; }

private:
    GGMappedConnection(Type t, GGConnection *conn) : m_type(t), m_connection(conn) {}

private:
    Type m_type;
    QVariant m_data;
    GGConnection *m_connection;
};

#endif // GGMAPPEDCONNECTION_H
