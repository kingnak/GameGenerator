#ifndef GGPAGEITEM_H
#define GGPAGEITEM_H

#include <QGraphicsItem>

class GGViewPage;

class GGPageItem : public QGraphicsItem
{
public:
    GGPageItem(GGViewPage *page, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF innerBoundingRect() const;

    void paintStart(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintEnd(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCondition(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintAction(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintDecision(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    GGViewPage *m_page;
    static const qreal penWidth;
};

#endif // GGPAGEITEM_H
