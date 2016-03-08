#include "ggmappingscene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "ggresizableitem.h"

GGMappingScene::GGMappingScene(QObject *parent)
    : QGraphicsScene(parent),
      m_selItem(NULL),
      m_createItem(NULL)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

GGMappingScene::~GGMappingScene()
{

}

void GGMappingScene::setMappedElement(QPixmap p)
{
    clear();
    m_selItem = NULL;
    initSelectionItem();
    QGraphicsPixmapItem *itm = addPixmap(p);
    itm->setZValue(-1);
    itm->setFlag(QGraphicsItem::ItemIsSelectable, false);
    itm->setFlag(QGraphicsItem::ItemIsMovable, false);
    itm->setFlag(QGraphicsItem::ItemIsFocusable, false);
}

void GGMappingScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(mouseEvent);
        if (!selectedItems().isEmpty()) {
            return;
        }

        QGraphicsItem *itm = this->itemAt(mouseEvent->scenePos(), QTransform());
        if (itm && itm->type() != QGraphicsPixmapItem::Type) {
            return;
        }


        m_createStart = mouseEvent->scenePos();
        m_createItem = new QGraphicsRectItem(QRectF(m_createStart, QSizeF()));
        addItem(m_createItem);

    } else {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void GGMappingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_createItem) {
        QPointF diff = mouseEvent->scenePos()-m_createStart;
        m_createItem->setRect(QRectF(m_createStart, QSizeF(diff.x(), diff.y())));
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GGMappingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_createItem) {
        QRectF bounds = m_createItem->boundingRect().toRect();

        GGResizableRectItem *r = new GGResizableRectItem;
        r->resizeToRect(bounds);
        r->setPen(QPen(Qt::red, 3));
        addItem(r);

        delete m_createItem;
        m_createItem = NULL;
    } else {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}

void GGMappingScene::updateSelection()
{
    initSelectionItem();
    if (selectedItems().size() == 1) {
        if (selectedItems()[0] != m_selItem) {
            GGResizableItem *itm = qgraphicsitem_cast<GGResizableItem*> (selectedItems()[0]);
            clearSelection();
            m_selItem->setWrappedItem(itm);
        }
    } else {
        m_selItem->setWrappedItem(NULL);
    }
}

void GGMappingScene::initSelectionItem()
{
    if (!m_selItem) {
        m_selItem = new GGSelectionItem;
        m_selItem->setVisible(false);
        m_selItem->setZValue(5);
        addItem(m_selItem);
        m_selItem->init();
    }
}
