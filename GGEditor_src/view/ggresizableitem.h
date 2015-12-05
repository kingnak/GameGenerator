#ifndef GGRESIZABLEITEM_H
#define GGRESIZABLEITEM_H

#include <QGraphicsItem>

class GGResizableItem : public QGraphicsItem
{
public:
    GGResizableItem(QGraphicsItem *parent = 0) : QGraphicsItem(parent) {}

    virtual void resizeToRect(QRectF &r) = 0;
    virtual QRectF resizeRect() const { return boundingRect(); }
};

#endif // GGRESIZABLEITEM_H
