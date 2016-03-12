#include "ggmappingscene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "ggresizableitem.h"
#include <model/ggpage.h>

struct LinkItem {
    GGConnectionSlot slt;
    LinkItem() : slt(GGConnectionSlot::NoConnection){}
};

class LinkRectItem : public GGResizableRectItem, public LinkItem
{
public:
    LinkRectItem(QGraphicsItem *parent = 0) : GGResizableRectItem(parent) {
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    }
};

///////////////////////////////////

class GGXorGraphicsRectItem : public QGraphicsRectItem
{
public:
    GGXorGraphicsRectItem(QGraphicsItem *parent = 0) : QGraphicsRectItem(parent) {}
    GGXorGraphicsRectItem(const QRectF &rect, QGraphicsItem *parent = 0) : QGraphicsRectItem(rect, parent) {}
    GGXorGraphicsRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0) : QGraphicsRectItem(x,y,w,h,parent) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
        QGraphicsRectItem::paint(painter, option, widget);
    }
};

///////////////////////////////////

const QColor GGMappingScene::s_defaultColor = Qt::red;
const QColor GGMappingScene::s_highColor = Qt::blue;

GGMappingScene::GGMappingScene(QObject *parent)
    : QGraphicsScene(parent),
      m_selItem(NULL),
      m_createItem(NULL),
      m_pixItem(NULL),
      m_lastSelIdx(-1)
{
    initSelectionItem();
    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

GGMappingScene::~GGMappingScene()
{

}

GGConnectionSlot GGMappingScene::getSelectedSlot()
{
    LinkRectItem *itm = qgraphicsitem_cast<LinkRectItem *> (selectedItems().at(0));
    if (itm) {
        return itm->slt;
    }
    return GGConnectionSlot(GGConnectionSlot::NoConnection);
}

void GGMappingScene::setMappedElement(QPixmap p)
{
    delete m_pixItem;
    QGraphicsPixmapItem *itm = addPixmap(p);
    itm->setZValue(-1);
    itm->setFlag(QGraphicsItem::ItemIsSelectable, false);
    itm->setFlag(QGraphicsItem::ItemIsMovable, false);
    itm->setFlag(QGraphicsItem::ItemIsFocusable, false);
    m_pixItem = itm;
}

void GGMappingScene::setConnections(GGMappedContentPage *page, QList<GGConnectionSlot> slts)
{
    clearSelection();
    qDeleteAll(m_mapItems);
    m_mapItems.clear();
    foreach (GGConnectionSlot s, slts) {
        if (s.type() == GGConnectionSlot::MappedConnection) {
            GGMappedLink lnk = page->getLinkMap()[s.index()];
            LinkRectItem *r = new LinkRectItem;
            QRectF bounds = lnk.rectangle();
            r->resizeToRect(bounds);
            r->setPen(QPen(s_defaultColor, s_penWidth));
            r->slt = s;
            addItem(r);
            m_mapItems << r;
        }
    }
    initSelectionItem();
    if (m_lastSelIdx >= 0 && m_lastSelIdx < m_mapItems.size()) {
        m_mapItems[m_lastSelIdx]->setSelected(true);
    }
}

void GGMappingScene::hoverItem(int idx)
{
    foreach (GGResizableRectItem *itm, m_mapItems) {
        itm->setPen(QPen(s_defaultColor, s_penWidth));
    }

    if (idx >=0 && idx < m_mapItems.size()) {
        m_mapItems[idx]->setPen(QPen(s_highColor, s_penWidth));
    }
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
        m_createItem = new GGXorGraphicsRectItem(QRectF(m_createStart, QSizeF()));
        addItem(m_createItem);
    } else {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void GGMappingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_createItem) {
        QPointF diff = mouseEvent->scenePos()-m_createStart;
        m_createItem->setRect(QRectF(m_createStart, QSizeF(diff.x(), diff.y())).normalized());
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GGMappingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_createItem) {
        QRectF bounds = m_createItem->boundingRect().toRect().normalized();

        delete m_createItem;
        m_createItem = NULL;

        // Minimum 3x3 px
        if (bounds.width() > 3 && bounds.height() > 3) {
            m_lastSelIdx = m_mapItems.size();
            emit addedItem(bounds.toRect());
        } else {
            QGraphicsScene::mouseReleaseEvent(mouseEvent);
            clearSelection();
        }
    } else {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        if (selectedItems().size() == 1) {
            LinkRectItem *itm = qgraphicsitem_cast<LinkRectItem*>(selectedItems()[0]);
            if (itm->hasMoved()) {
                itm->commitMove();
                this->itemMoved(itm);
            }
        }
    }
}

void GGMappingScene::updateSelection()
{
    initSelectionItem();
    if (selectedItems().size() == 1) {
        if (selectedItems()[0] != m_selItem) {
            GGResizableRectItem *itm = qgraphicsitem_cast<GGResizableRectItem*> (selectedItems()[0]);
            //clearSelection();
            m_selItem->setWrappedItem(itm);
            m_lastSelIdx = m_mapItems.indexOf(itm);
            //m_selItem->setSelected(true);
        }
    } else {
        m_selItem->setWrappedItem(NULL);
//        m_lastSelIdx = -1;
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

void GGMappingScene::itemMoved(LinkRectItem *itm)
{
    m_lastSelIdx = m_mapItems.indexOf(itm);
    emit movedItem(itm->slt.index(), itm->mapToScene(itm->boundingRect()).boundingRect().toRect());
}
