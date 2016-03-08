#ifndef GGRESIZABLEITEM_H
#define GGRESIZABLEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <view/ggselectionitem.h>

class GGResizableItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 0x100 };
    int type() const { return Type; }

    GGResizableItem(QGraphicsItem *parent = 0) : QGraphicsItem(parent), m_selectionItem(NULL) {
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    }

    virtual void resizeToRect(QRectF &r) = 0;
    virtual QRectF resizeRect() const { return boundingRect(); }

    void setSelectionItem(GGSelectionItem *itm) { m_selectionItem = itm; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) {
        if (change == QGraphicsItem::ItemPositionHasChanged && m_selectionItem) {
            m_selectionItem->setPos(pos());
        }
        return value;
    }

protected:
    GGSelectionItem *m_selectionItem;
};

class GGResizableRectItem : public GGResizableItem
{
public:
    GGResizableRectItem(QGraphicsItem *parent = 0) : GGResizableItem(parent) {
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
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
        const qreal a = m_pen.widthF()/2;
        painter->drawRect(boundingRect().adjusted(a,a,-a,-a));
    }
    QRectF boundingRect() const {
        return m_rect;
    }
private:
    QRectF m_rect;
    QPen m_pen;
};

#endif // GGRESIZABLEITEM_H
