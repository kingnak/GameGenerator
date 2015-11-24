#ifndef GGSELECTIONITEM_H
#define GGSELECTIONITEM_H

#include <QGraphicsItem>

class GGPageItem;

class GGSelectionItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const { return Type; }

    GGSelectionItem(QGraphicsItem *parent = 0);

    void setWrappedItem(GGPageItem *item);
    GGPageItem *wrappedItem() const { return m_wrapped; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void init();

protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

private:
    void updateCornerGrabberPos();
    QRectF wrappedRect() const;

private:
    GGPageItem *m_wrapped;

    class GGCornerGrabber;
    QList<GGCornerGrabber *> m_grabbers;
};

#endif // GGSELECTIONITEM_H
