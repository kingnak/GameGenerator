#include "ggpageitem.h"
#include "ggeditorscene.h"
#include <viewmodel/ggviewpage.h>
#include <model/ggpage.h>
#include <QtGui>
#include <QVector>

const qreal GGPageItem::penWidth = 3;

GGPageItem::GGPageItem(GGViewPage *page, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_page(page)
{
    setDrawingGeometry(page->bounds());
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QRectF GGPageItem::boundingRect() const
{
    QRectF ret = innerBoundingRect();
    ret.adjust(-penWidth/2, -penWidth/2, penWidth/2, penWidth/2);
    return ret;
}

void GGPageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, penWidth));

    painter->save();;
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
        paintStart(painter, option, widget);
        break;
    case GGEndPage::Type:
        paintEnd(painter, option, widget);
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

    // TEST CENTER
    painter->setPen(Qt::red);
    painter->drawEllipse(0,0,2,2);
    // TEST BOUNDS
    painter->drawRect(boundingRect());
}

void GGPageItem::setDrawingGeometry(QRectF f)
{
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
    case GGEndPage::Type:
    {
        qreal s = qMax(30., qMax(f.width(), f.height()));
        f.setSize(QSizeF(s,s));
    }
        break;
    default:
        f.setWidth(qMax(f.width(), 50.));
        f.setHeight(qMax(f.height(), 30.));
    }

    if (m_geo != f) {
        prepareGeometryChange();
        QPointF cc = f.center();
        QPointF ccc = pos();
        setPos(f.center());
        cc = pos();
        f = mapFromScene(f).boundingRect();
        m_geo = f;
    }
}

void GGPageItem::updateDrawingGeometry()
{
    setDrawingGeometry(m_page->bounds());
}

QVariant GGPageItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged && scene()) {
        static_cast<GGEditorScene*>(scene())->itemMoved(this);
    }
    return value;
}

QRectF GGPageItem::innerBoundingRect() const
{
    QRectF ret = QRectF(QPointF(), QSizeF(m_geo.size()));
    ret.translate(-ret.center());
    return ret;
}

void GGPageItem::paintStart(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal r = qMin(innerBoundingRect().width(), innerBoundingRect().height())/2;
    QPainterPath path;
    path.addEllipse(QPointF(), r, r);
    painter->fillPath(path, Qt::black);
}

void GGPageItem::paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal r = qMin(innerBoundingRect().width(), innerBoundingRect().height());
    QRectF rect(QPointF(-r/2,-r/2), QSizeF(r,r));
    QPainterPath path;
    path.addEllipse(rect);
    painter->fillPath(path, Qt::white);
    painter->drawPath(path);
    rect.adjust(5, 5, -5, -5);
    QPainterPath path2;
    path2.addEllipse(rect);
    painter->fillPath(path2, Qt::black);
}

void GGPageItem::paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF ir = innerBoundingRect();
    QVector<QPointF> points;
    points
            << QPointF(0,ir.top())
            << QPointF(ir.right(), 0)
            << QPointF(0, ir.bottom())
            << QPointF(ir.left(), 0);

    QPainterPath path;
    path.addPolygon(points);
    path.closeSubpath();
    painter->fillPath(path, Qt::white);
    painter->drawPath(path);
}

void GGPageItem::paintAction(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintDecision(painter, option, widget);
    QRectF ir = innerBoundingRect();
    QLineF l(ir.left()+20, ir.top(), ir.left()+20, ir.bottom());
    painter->drawLine(l);
    l.translate(ir.width()-40,0);
    painter->drawLine(l);
}

void GGPageItem::paintDecision(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    path.addRoundedRect(innerBoundingRect(), 15, 15);
    painter->fillPath(path, Qt::white);
    painter->drawPath(path);
}
