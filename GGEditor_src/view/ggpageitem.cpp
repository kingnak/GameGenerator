#include "ggpageitem.h"
#include "ggeditorscene.h"
#include <viewmodel/ggviewpage.h>
#include "ggconnectionitem.h"
#include <model/ggpage.h>
#include <QtGui>
#include <QtWidgets>
#include <QVector>
#include <QDebug>

GGPageItem::GGPageItem(GGViewPage *page, QGraphicsItem *parent)
    : GGResizableItem(parent),
      m_page(page)
{
    QRectF rf = page->bounds();
    resizeToRect(rf);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRect GGPageItem::modelPosition() const
{
    return mapToScene(m_geo).boundingRect().toRect();
}

QRectF GGPageItem::boundingRect() const
{
    const qreal penWidth = editScene()->pageBorderWidth();
    QRectF ret = innerBoundingRect();
    ret.adjust(-penWidth/2, -penWidth/2, penWidth/2, penWidth/2);
    return ret;
}

void GGPageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(editScene()->pageColor(), editScene()->pageBorderWidth()));

    bool withCaption = true;
    painter->save();
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
        paintStart(painter, option, widget);
        withCaption = false;
        break;
    case GGEndPage::Type:
        paintEnd(painter, option, widget);
        withCaption = false;
        break;
    case GGConditionPage::Type:
        paintCondition(painter, option, widget);
        break;
    case GGActionPage::Type:
        paintAction(painter, option, widget);
        break;
    case GGDecisionPage::Type:
        paintDecision(painter, option, widget);
        break;
    }
    painter->restore();

    if (withCaption) {
        QString name = m_page->page()->name();
        painter->drawText(innerBoundingRect(), Qt::AlignHCenter | Qt::AlignCenter, name);
    }

    /*
    // TEST CENTER
    painter->setPen(Qt::red);
    painter->drawEllipse(0,0,2,2);
    // TEST BOUNDS
    painter->drawRect(boundingRect());
    */

    if (isSelected()) {
        painter->setPen(editScene()->pageHighlightColor());
        painter->drawRect(boundingRect());
    }
}

QPainterPath GGPageItem::shape() const
{
    QPainterPath p;
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
    case GGEndPage::Type:
    {
        qreal r = qMin(innerBoundingRect().width(), innerBoundingRect().height())/2;
        p.addEllipse(QPointF(), r, r);
    }
        break;
    case GGConditionPage::Type:
    {
        QRectF ir = innerBoundingRect();
        QVector<QPointF> points;
        points
                << QPointF(0,ir.top())
                << QPointF(ir.right(), 0)
                << QPointF(0, ir.bottom())
                << QPointF(ir.left(), 0);

        p.addPolygon(points);
        p.closeSubpath();
    }
        break;
    case GGActionPage::Type:
    case GGDecisionPage::Type:
        p.addRoundedRect(boundingRect(), 20, 20);
        break;
    }
    return p;
}

void GGPageItem::addConnection(GGConnectionItem *itm)
{
    m_connections.insert(itm);
}

void GGPageItem::removeConnection(GGConnectionItem *itm)
{
    m_connections.remove(itm);
}

QSet<GGConnectionItem *> GGPageItem::connectionItems()
{
    return m_connections;
}

void GGPageItem::clearConnections()
{
    m_connections.clear();
}

void GGPageItem::updateConnectionPositions()
{
    foreach (GGConnectionItem *c, m_connections) {
        c->updatePosition();
    }
}

void GGPageItem::resizeToRect(QRectF &f)
{
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
    case GGEndPage::Type:
    {
        qreal s = qMax(30., f.height());
        if (f.width() != m_geo.width()) {
            s = qMax(30., f.width());
        }
        f.setSize(QSizeF(s,s));
    }
        break;
    default:
        f.setWidth(qMax(f.width(), 50.));
        f.setHeight(qMax(f.height(), 30.));
    }

    if (m_geo != f) {
        prepareGeometryChange();
        setPos(f.center());
        m_geo = mapFromScene(f).boundingRect();
    }

    updateConnectionPositions();
}

QRectF GGPageItem::resizeRect() const
{
    return innerBoundingRect();
}

void GGPageItem::updateDrawingGeometry()
{
    QRectF rf = m_page->bounds();
    resizeToRect(rf);
}

QVariant GGPageItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged && scene()) {
        editScene()->itemMoved(this);
    }
    return value;
}

GGEditorScene *GGPageItem::editScene()
{
    return static_cast<GGEditorScene*> (scene());
}

const GGEditorScene *GGPageItem::editScene() const
{
    return static_cast<const GGEditorScene*> (scene());
}

QRectF GGPageItem::innerBoundingRect() const
{
    QRectF ret = QRectF(QPointF(), QSizeF(m_geo.size()));
    ret.translate(-ret.center());
    return ret;
}

void GGPageItem::paintStart(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path = shape();
    painter->fillPath(path, editScene()->pageColor());
}

void GGPageItem::paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPainterPath path = shape();
    painter->fillPath(path, editScene()->pageFillColor());
    painter->drawPath(path);

    QRectF rect = path.boundingRect();
    rect.adjust(5, 5, -5, -5);
    QPainterPath path2;
    path2.addEllipse(rect);
    painter->fillPath(path2, editScene()->pageColor());
}

void GGPageItem::paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path = shape();
    painter->fillPath(path, editScene()->pageFillColor());
    painter->drawPath(path);
}

void GGPageItem::paintAction(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    paintDecision(painter, option, widget);
    QRectF ir = innerBoundingRect();
    QLineF l(ir.left()+20, ir.top(), ir.left()+20, ir.bottom());
    painter->drawLine(l);
    l.translate(ir.width()-40,0);
    painter->drawLine(l);
}

void GGPageItem::paintDecision(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path = shape();
    painter->fillPath(path, editScene()->pageFillColor());
    painter->drawPath(path);
}
