#ifndef GGPAGEITEM_H
#define GGPAGEITEM_H

#include <QGraphicsItem>

class GGViewPage;

class GGPageItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const { return Type; }

    GGPageItem(GGViewPage *page, QGraphicsItem *parent = 0);

    GGViewPage *page() { return m_page; }
    QRect modelPosition() const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDrawingGeometry(QRectF f);
//    void commitPageGeometry();
    void updateDrawingGeometry();
    QRectF innerBoundingRect() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

    void paintStart(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintAction(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintDecision(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    GGViewPage *m_page;
    QRectF m_geo;
    static const qreal penWidth;
};

#endif // GGPAGEITEM_H
