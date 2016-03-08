#ifndef GGMAPPINGSCENE_H
#define GGMAPPINGSCENE_H

#include <QGraphicsScene>
#include "ggselectionitem.h"

class GGMappingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGMappingScene(QObject *parent = 0);
    ~GGMappingScene();

public slots:
    void setMappedElement(QPixmap p);

signals:
    void addedItem(QRect pos);
    void movedItem();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private slots:
    void updateSelection();

private:
    void initSelectionItem();

private:
    GGSelectionItem *m_selItem;
    QGraphicsRectItem *m_createItem;
    QPointF m_createStart;
};

#endif // GGMAPPINGSCENE_H
