#ifndef GGCONNECTIONITEM_H
#define GGCONNECTIONITEM_H

#include <QGraphicsItem>

class GGViewConnection;
class GGEditorScene;
class GGPageItem;

class GGConnectionItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const { return Type; }

    GGConnectionItem(GGViewConnection *conn, QGraphicsItem *parent = 0);

    GGViewConnection *connection() { return m_conn; }
    GGPageItem *sourceItem();
    GGPageItem *destinationItem();

    void updatePosition();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    GGEditorScene *editScene();
    const GGEditorScene *editScene() const;
    void fetchPageItems();

private:
    GGViewConnection *m_conn;
    GGPageItem *m_source;
    GGPageItem *m_dest;
    QPainterPath m_head;
};

#endif // GGCONNECTIONITEM_H
