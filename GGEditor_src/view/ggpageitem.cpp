#include "ggpageitem.h"
#include "ggeditorscene.h"
#include <viewmodel/ggviewpage.h>
#include <model/ggpage.h>
#include <QtGui>
#include <QtWidgets>
#include <QVector>
#include <QDebug>

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

QRect GGPageItem::modelPosition() const
{
    return mapToScene(m_geo).boundingRect().toRect();
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

    if (isSelected()) {
        painter->setPen(Qt::blue);
        painter->drawRect(boundingRect());
    }
}

void GGPageItem::setDrawingGeometry(QRectF f)
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
}

//void GGPageItem::commitPageGeometry()
//{
//    QRect r = mapToScene(m_geo).boundingRect().toRect();
//    qDebug() << "Page" << m_page->page()->id() << "position" << r;
//    m_page->setBounds(r);
//}

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
    Q_UNUSED(option)
    Q_UNUSED(widget)
    qreal r = qMin(innerBoundingRect().width(), innerBoundingRect().height())/2;
    QPainterPath path;
    path.addEllipse(QPointF(), r, r);
    painter->fillPath(path, Qt::black);
}

void GGPageItem::paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
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
    Q_UNUSED(option)
    Q_UNUSED(widget)
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
    QPainterPath path;
    path.addRoundedRect(innerBoundingRect(), 15, 15);
    painter->fillPath(path, Qt::white);
    painter->drawPath(path);
}
