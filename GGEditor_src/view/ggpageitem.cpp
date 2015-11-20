#include "ggpageitem.h"
#include <viewmodel/ggviewpage.h>
#include <model/ggpage.h>
#include <QtGui>
#include <QVector>

const qreal GGPageItem::penWidth = 1;

GGPageItem::GGPageItem(GGViewPage *page, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_page(page)
{
    setFlag(QGraphicsItem::ItemIsMovable);
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
    painter->drawRect(innerBoundingRect());
}

QRectF GGPageItem::innerBoundingRect() const
{
    QRectF ret = QRectF(QPointF(), QSizeF(m_page->bounds().size()));
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
    painter->drawEllipse(rect);
    rect.adjust(5, 5, -5, -5);
    QPainterPath path;
    path.addEllipse(rect);
    painter->fillPath(path, Qt::black);
}

void GGPageItem::paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF ir = innerBoundingRect();
    QVector<QPointF> points;
    points
            << QPointF(0,ir.top())
            << QPointF(ir.right(), 0)
            << QPointF(0, ir.bottom())
            << QPointF(ir.left(), 0)
            << QPointF(0,ir.top());

    painter->drawPolyline(points);
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
    painter->drawRoundedRect(innerBoundingRect(), 15, 15);
}

