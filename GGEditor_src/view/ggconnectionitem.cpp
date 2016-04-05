#include "ggconnectionitem.h"
#include <QtGui>
#include <QGraphicsSceneMoveEvent>
#include <viewmodel/ggviewconnection.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewscene.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include "ggeditorscene.h"
#include "ggpageitem.h"

GGConnectionItem::GGConnectionItem(GGViewConnection *conn, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_conn(conn),
      m_source(NULL),
      m_dest(NULL)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

GGPageItem *GGConnectionItem::sourceItem()
{
    return m_source;
}

GGPageItem *GGConnectionItem::destinationItem()
{
    return m_dest;
}

void GGConnectionItem::updatePosition()
{
    prepareGeometryChange();
}

QRectF GGConnectionItem::boundingRect() const
{
    qreal extra = (editScene()->connectionWidth() + 20) / 2;
    QRectF f;
    f.setTopLeft(m_source->pos());
    f.setBottomRight(m_dest->pos());
    return f.normalized().adjusted(-extra,-extra,extra,extra);
}

void GGConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Don't draw if colliding
    if (m_source->collidesWithItem(m_dest)) return;

    // Draw actual line
    QLineF line(mapFromItem(m_source, 0, 0), mapFromItem(m_dest, 0, 0));
    painter->setPen(QPen(editScene()->connectionColor(), editScene()->connectionWidth()));
    painter->drawLine(line);

    if (isSelected()) {
        QPainterPathStroker ps;
        //ps.setDashPattern(Qt::DashLine);
        ps.setWidth(20);
        QPainterPath linePath;
        linePath.moveTo(line.p1());
        linePath.lineTo(line.p2());
        QPainterPath sel = ps.createStroke(linePath);
        painter->save();
        painter->setPen(QPen(editScene()->connectionHighlightColor(), 1, Qt::DashLine));
        painter->drawPath(sel);
        painter->restore();
    }

    // Get intersection point with destination's shape
    QPolygonF poly = mapFromItem(m_dest, m_dest->shape().toFillPolygon());
    QPointF p1 = poly.at(0);
    QPointF intersect;
    for (int i = 1; i < poly.length(); ++i) {
        QLineF pLine(p1, poly.at(i));
        if (line.intersect(pLine, &intersect) == QLineF::BoundedIntersection) {
            break;
        }
        p1 = poly.at(i);
    }

    // Draw head
    const qreal headSize = 25;
    const qreal PI = 3.14;
    const qreal deltaAngle = PI/6;
    qreal angle = ::atan2(line.dy(), line.dx());
    QPointF a1 = intersect;
    QPointF a2 = intersect-QPointF(headSize*::cos(angle+deltaAngle),headSize*::sin(angle+deltaAngle));
    QPointF a3 = intersect-QPointF(headSize/2*::cos(angle),headSize/2*::sin(angle));
    QPointF a4 = intersect-QPointF(headSize*::cos(angle-deltaAngle),headSize*::sin(angle-deltaAngle));
    QPolygonF head = QPolygonF(QVector<QPointF>() << a1 << a2 << a3 << a4);
    QPainterPath headPath;
    headPath.addPolygon(head);
    painter->fillPath(headPath, editScene()->connectionColor());
    m_head = headPath;
}

QPainterPath GGConnectionItem::shape() const
{
    QPainterPath p;
    p.moveTo(m_source->pos());
    p.lineTo(m_dest->pos());
    QPainterPathStroker ps;
    ps.setWidth(editScene()->connectionWidth());
    p = ps.createStroke(p);
    p.addPath(m_head);
    return p;
}

QVariant GGConnectionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSceneHasChanged) {
        fetchPageItems();
    }
    return value;
}

GGEditorScene *GGConnectionItem::editScene()
{
    return static_cast<GGEditorScene*> (scene());
}

const GGEditorScene *GGConnectionItem::editScene() const
{
    return static_cast<const GGEditorScene*> (scene());
}

void GGConnectionItem::fetchPageItems()
{
    if (scene()) {
        GGPage *s = m_conn->connection()->source();
        m_source = editScene()->itemForPage(editScene()->model()->getViewPageForPage(s, editScene()->modelScene()->scene()->id()));
        GGPage *d = m_conn->connection()->destination();
        m_dest = editScene()->itemForPage(editScene()->model()->getViewPageForPage(d, editScene()->modelScene()->scene()->id()));
    } else {
        m_source = m_dest = NULL;
    }
}

