#include "ggpageitem.h"
#include "ggeditorscene.h"
#include <viewmodel/ggviewpage.h>
#include "ggconnectionitem.h"
#include <model/ggpage.h>
#include <QtGui>
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

QPixmap GGPageItem::getPageTypeIcon(int type, QSize drawSize, QSize outSize)
{
    if (outSize.isEmpty()) outSize = drawSize;
    QPixmap pix(drawSize);

    QRectF rect(QPointF(), pix.size());
    rect.adjust(1,1,-1,-1);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    QRectF rectTrans = rect.translated(-pix.width()/2, -pix.height()/2);
    p.setPen(QPen(Qt::black, 3));

    switch (type) {
    case GGStartPage::Type:
        p.translate(pix.width()/2,pix.height()/2);
        paintStart(&p, rect, Qt::black, Qt::white);
        break;
    case GGEndPage::Type:
        p.translate(pix.width()/2,pix.height()/2);
        paintEnd(&p, rect, Qt::black, Qt::white);
        break;
    case GGConditionPage::Type:
        p.translate(pix.width()/2,pix.height()/2);
        paintCondition(&p, rectTrans, Qt::black, Qt::white);
        break;
    case GGActionPage::Type:
        paintAction(&p, rect, Qt::black, Qt::white);
        break;
    case GGDecisionPage::Type:
        paintDecision(&p, rect, Qt::black, Qt::white);
        break;
    }

    return pix.scaled(outSize, Qt::KeepAspectRatio);
}

void GGPageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(editScene()->pageColor(m_page), editScene()->pageBorderWidth()));

    bool withCaption = true;
    painter->save();
    switch (m_page->page()->type()) {
    case GGStartPage::Type:
        paintStart(painter, innerBoundingRect(), editScene()->pageColor(m_page), editScene()->pageFillColor());
        withCaption = false;
        break;
    case GGEndPage::Type:
        paintEnd(painter, innerBoundingRect(), editScene()->pageColor(m_page), editScene()->pageFillColor());
        withCaption = false;
        break;
    case GGConditionPage::Type:
        paintCondition(painter, innerBoundingRect(), editScene()->pageColor(m_page), editScene()->pageFillColor());
        break;
    case GGActionPage::Type:
        paintAction(painter, boundingRect(), editScene()->pageColor(m_page), editScene()->pageFillColor());
        break;
    case GGDecisionPage::Type:
        paintDecision(painter, boundingRect(), editScene()->pageColor(m_page), editScene()->pageFillColor());
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
        return roundShape(innerBoundingRect());
    case GGConditionPage::Type:
        return diamondShape(innerBoundingRect());
    case GGActionPage::Type:
    case GGDecisionPage::Type:
        return roundedRectShape(boundingRect());
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
        updateConnectionPositions();
    }
    return GGResizableItem::itemChange(change, value);
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

void GGPageItem::paintStart(QPainter *painter, const QRectF &rect, QColor border, QColor fill)
{
    Q_UNUSED(fill);
    QPainterPath path = roundShape(rect);
    painter->fillPath(path, border);
}

void GGPageItem::paintEnd(QPainter *painter, const QRectF &rect, QColor border, QColor fill)
{
    QPainterPath path = roundShape(rect);
    painter->fillPath(path, fill);
    painter->drawPath(path);

    QRectF rectPath = path.boundingRect();
    rectPath.adjust(5, 5, -5, -5);
    QPainterPath path2;
    path2.addEllipse(rectPath);
    painter->fillPath(path2, border);
}

void GGPageItem::paintCondition(QPainter *painter, const QRectF &rect, QColor border, QColor fill)
{
    Q_UNUSED(border);
    QPainterPath path = diamondShape(rect);
    painter->fillPath(path, fill);
    painter->drawPath(path);
}

void GGPageItem::paintAction(QPainter *painter, const QRectF &rect, QColor border, QColor fill)
{
    paintDecision(painter, rect, border, fill);
    QRectF ir = rect;
    QLineF l(ir.left()+20, ir.top(), ir.left()+20, ir.bottom());
    painter->drawLine(l);
    l.translate(ir.width()-40,0);
    painter->drawLine(l);
}

void GGPageItem::paintDecision(QPainter *painter, const QRectF &rect, QColor border, QColor fill)
{
    Q_UNUSED(border);
    QPainterPath path = roundedRectShape(rect);
    painter->fillPath(path, fill);
    painter->drawPath(path);
}

QPainterPath GGPageItem::roundShape(const QRectF &rect)
{
    QPainterPath p;
    qreal r = qMin(rect.width(), rect.height())/2;
    p.addEllipse(QPointF(), r, r);
    return p;
}

QPainterPath GGPageItem::roundedRectShape(const QRectF &rect)
{
    QPainterPath p;
    p.addRoundedRect(rect, 20, 20);
    return p;
}

QPainterPath GGPageItem::diamondShape(const QRectF &rect)
{
    QPainterPath p;
    QVector<QPointF> points;
    points
            << QPointF(0,rect.top())
            << QPointF(rect.right(), 0)
            << QPointF(0, rect.bottom())
            << QPointF(rect.left(), 0);

    p.addPolygon(points);
    p.closeSubpath();
    return p;
}
