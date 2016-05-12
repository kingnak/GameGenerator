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

    static QPixmap getPageTypeIcon(int type, QSize drawSize = QSize(48,32), QSize outSize = QSize());

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    GGEditorScene *editScene();
    const GGEditorScene *editScene() const;

    void paintDecoration(QPainter *painter) const;
    QRectF getActionIndicatorPosition() const;

    static void paintStart(QPainter *painter, const QRectF &rect, QColor border, QColor fill);
    static void paintEnd(QPainter *painter, const QRectF &rect, QColor border, QColor fill);
    static void paintCondition(QPainter *painter, const QRectF &rect, QColor border, QColor fill);
    static void paintAction(QPainter *painter, const QRectF &rect, QColor border, QColor fill);
    static void paintDecision(QPainter *painter, const QRectF &rect, QColor border, QColor fill);

    static QPainterPath roundShape(const QRectF &rect);
    static QPainterPath roundedRectShape(const QRectF &rect);
    static QPainterPath diamondShape(const QRectF &rect);

private:
    GGViewPage *m_page;
    QRectF m_geo;
    QSet<GGConnectionItem *> m_connections;
};

#endif // GGPAGEITEM_H
