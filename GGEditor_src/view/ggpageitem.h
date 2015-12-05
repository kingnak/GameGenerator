#ifndef GGPAGEITEM_H
#define GGPAGEITEM_H

#include "ggresizableitem.h"
#include <QSet>

class GGViewPage;
class GGEditorScene;
class GGConnectionItem;

class GGPageItem : public GGResizableItem
{
public:
    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const { return Type; }

    GGPageItem(GGViewPage *page, QGraphicsItem *parent = 0);

    GGViewPage *page() { return m_page; }
    QRect modelPosition() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void addConnection(GGConnectionItem *itm);
    void removeConnection(GGConnectionItem *itm);
    QSet<GGConnectionItem *> connectionItems();
    void clearConnections();

    void updateConnectionPositions();

    void resizeToRect(QRectF &rect);
    QRectF resizeRect() const;
    void updateDrawingGeometry();
    QRectF innerBoundingRect() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    GGEditorScene *editScene();
    const GGEditorScene *editScene() const;

    void paintStart(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintAction(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintDecision(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    GGViewPage *m_page;
    QRectF m_geo;
    QSet<GGConnectionItem *> m_connections;
};

#endif // GGPAGEITEM_H
