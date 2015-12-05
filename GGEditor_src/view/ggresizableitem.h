#ifndef GGRESIZABLEITEM_H
#define GGRESIZABLEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>

class GGResizableItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 0x100 };
    int type() const { return Type; }

    GGResizableItem(QGraphicsItem *parent = 0) : QGraphicsItem(parent) {}

    virtual void resizeToRect(QRectF &r) = 0;
    virtual QRectF resizeRect() const { return boundingRect(); }
};

class GGResizableRectItem : public GGResizableItem
{
public:
    GGResizableRectItem(QGraphicsItem *parent = 0) : GGResizableItem(parent) {
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    }
    virtual void resizeToRect(QRectF &r) {
        prepareGeometryChange();
        m_rect = r;
        setPos(m_rect.center());
        m_rect.translate(-m_rect.center());
    }
    void setPen(QPen p) {
        m_pen = p;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setPen(m_pen);
        painter->drawRect(boundingRect());
    }
    QRectF boundingRect() const {
        return m_rect;
    }
private:
    QRectF m_rect;
    QPen m_pen;
};

#endif // GGRESIZABLEITEM_H
