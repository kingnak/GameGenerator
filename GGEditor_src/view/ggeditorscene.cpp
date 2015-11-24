#include "ggeditorscene.h"
#include "../viewmodel/ggviewmodel.h"
#include "ggpageitem.h"
#include "ggconnectionitem.h"
#include "ggselectionitem.h"
#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <viewmodel/ggviewpage.h>
#include <viewmodel/ggviewconnection.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QtWidgets>

GGEditorScene::GGEditorScene(GGUIController *ctrl, QObject *parent)
    : QGraphicsScene(parent),
      m_model(NULL),
      m_selItem(NULL)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelectionItem()));
    initSelItem();
    if (ctrl)
        connectToController(ctrl);
}

void GGEditorScene::itemMoved(GGPageItem *item)
{
    if (item == m_selItem->wrappedItem()) {
        m_selItem->setWrappedItem(item);
    }

    item->updateConnectionPositions();
}

void GGEditorScene::connectToController(GGUIController *ctrl)
{
    connect(ctrl, SIGNAL(modelReset(GGViewModel*)), this, SLOT(resetModel(GGViewModel*)));
    connect(this, SIGNAL(pageMoved(GGViewPage*,QRect)), ctrl, SLOT(changePageGeometry(GGViewPage*,QRect)));
    connect(this, SIGNAL(multiplePagesMoved(QList<QPair<GGViewPage*,QRect> >)), ctrl, SLOT(changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> >)));
    connect(this, SIGNAL(multipleObjectsDeleted(QList<GGViewPage*>,QList<GGViewConnection*>)), ctrl, SLOT(deleteMultipleObjects(QList<GGViewPage*>,QList<GGViewConnection*>)));
}

GGPageItem *GGEditorScene::itemForPage(GGViewPage *page)
{
    return m_pageMap.value(page);
}

void GGEditorScene::resetModel(GGViewModel *model)
{
    if (m_model) {
        m_model->disconnect();
    }
    this->clearSelection();
    this->clear();
    m_model = model;
    m_pageMap.clear();
    m_connMap.clear();

    if (m_model) {
        connect(m_model, SIGNAL(viewPageRegistered(GGViewPage*)), this, SLOT(pageReg(GGViewPage*)));
        connect(m_model, SIGNAL(viewPageUnregistered(GGViewPage*)), this, SLOT(pageUnreg(GGViewPage*)));
        connect(m_model, SIGNAL(viewConnectionRegistered(GGViewConnection*)), this, SLOT(connReg(GGViewConnection*)));
        connect(m_model, SIGNAL(viewConnectionUnregistered(GGViewConnection*)), this, SLOT(connUnreg(GGViewConnection*)));
        connect(m_model, SIGNAL(pageUpdated(GGViewPage*)), this, SLOT(pageUpd(GGViewPage*)));
        connect(m_model, SIGNAL(viewPageUpdated(GGViewPage*)), this, SLOT(pageViewUpd(GGViewPage*)));
    }

    // MUST Refresh, to ensure selitem
    refresh();
}

void GGEditorScene::refresh()
{
    this->clear();
    m_selItem = NULL;
    initSelItem();
    // Populate with items
    if (m_model) {
        foreach (GGPage *p, m_model->editModel()->getPages()) {
            if (GGViewPage *vp = m_model->getViewPageForPage(p)) {
                pageReg(vp);
            } else {
                qCritical("No ViewPage for Page");
            }
        }
        foreach (GGConnection *c, m_model->editModel()->getConnections()) {
            if (GGViewConnection *vc = m_model->getViewConectionForConnection(c)) {
                connReg(vc);
            } else {
                qCritical("No ViewConnection for Connection");
            }
        }
    }
}

void GGEditorScene::selectPage(GGViewPage *page)
{
    if (GGPageItem *i = m_pageMap.value(page)) {
        i->setSelected(true);
    } else {
        clearSelection();
    }
}

void GGEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_clickedOnEmpty = true;
    if (event->button() != Qt::LeftButton || !selectedItems().isEmpty()) {
        m_clickedOnEmpty = false;
    }
    QGraphicsScene::mousePressEvent(event);
    if (event->button() != Qt::LeftButton || !selectedItems().isEmpty()) {
        m_clickedOnEmpty = false;
    }
}

void GGEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        QList<QPair<GGViewPage*,QRect> > movs;
        if (selectedItems().isEmpty() && m_clickedOnEmpty) {
            // clicked on empty space.
            emit clickedEmptySpace(event->scenePos());
        } else {
            foreach (QGraphicsItem *i, selectedItems()) {
                if (GGPageItem *p = qgraphicsitem_cast<GGPageItem*> (i)) {
                    if (p->page()->bounds() != p->modelPosition())
                        movs << qMakePair(p->page(), p->modelPosition());
                }
            }
            if (movs.size() > 1)
                emit multiplePagesMoved(movs);
            else if (movs.size() == 1)
                emit pageMoved(movs[0].first, movs[0].second);
        }
    }
    m_clickedOnEmpty = false;
}

void GGEditorScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        QList<GGViewConnection *> delCons;
        QList<GGViewPage *> delPages;
        foreach (QGraphicsItem *i, selectedItems()) {
            if (GGPageItem *p = qgraphicsitem_cast<GGPageItem*>(i)) {
                delPages << p->page();
            } else if (GGConnectionItem *c = qgraphicsitem_cast<GGConnectionItem*>(i)) {
                delCons << c->connection();
            }
        }

        if (!delCons.isEmpty() || !delPages.isEmpty()) {
            emit multipleObjectsDeleted(delPages, delCons);
        }
    }
}

void GGEditorScene::pageReg(GGViewPage *p)
{
    GGPageItem *item = new GGPageItem(p);
    item->setZValue(PAGE_ZVALUE);
    m_pageMap[p] = item;
    this->addItem(item);

    // TODO: Needed? Added page with connections, but they are added separately later...
    /*
    foreach (GGConnection *c, m_model->editModel()->getPageAllConnections(p->page())) {
        if (GGViewConnection *vc = m_model->getViewConectionForConnection(c)) {
            if (GGConnectionItem *it = m_connMap.value(vc)) {
                item->addConnection(it);
            }
        }
    }
    */
}

void GGEditorScene::pageUnreg(GGViewPage *p)
{
    GGPageItem *it = m_pageMap.take(p);
    if (it) {
        // TODO: needed? When removing page, connections are removed fist
        foreach (GGConnectionItem *cit, it->connectionItems()) {
            qDebug("Removing page from scene that has connections");
            if (cit) connUnreg(cit->connection());
        }
        delete it;
    }
}

void GGEditorScene::connReg(GGViewConnection *c)
{
    GGConnectionItem *item = new GGConnectionItem(c);
    item->setZValue(CONNECTION_ZVALUE);
    m_connMap[c] = item;
    this->addItem(item);

    if (GGViewPage *vp = m_model->getViewPageForPage(c->connection()->source())) {
        if (GGPageItem *it = m_pageMap.value(vp)) {
            it->addConnection(item);
        }
    }
    if (GGViewPage *vp = m_model->getViewPageForPage(c->connection()->destination())) {
        if (GGPageItem *it = m_pageMap.value(vp)) {
            it->addConnection(item);
        }
    }
}

void GGEditorScene::connUnreg(GGViewConnection *c)
{
    GGConnectionItem *it = m_connMap.take(c);
    if (it) {
        if (it->sourceItem()) it->sourceItem()->removeConnection(it);
        if (it->destinationItem()) it->destinationItem()->removeConnection(it);
    }
    delete it;
}

void GGEditorScene::pageUpd(GGViewPage *p)
{
    if (GGPageItem *item = m_pageMap.value(p)) {
        item->update();
    }
}

void GGEditorScene::pageViewUpd(GGViewPage *p)
{
    if (GGPageItem *item = m_pageMap.value(p)) {
        item->updateDrawingGeometry();
        if (item == m_selItem->wrappedItem()) {
            m_selItem->setWrappedItem(item);
        }
    }
}

void GGEditorScene::updateSelectionItem()
{
    QList<QGraphicsItem *> itms = selectedItems();
    m_selItem->setVisible(false);
    m_selItem->setWrappedItem(NULL);
    if (itms.size() != 1) {
        emit otherSelected();
    } else if (GGPageItem *itm = qgraphicsitem_cast<GGPageItem*>(itms[0])) {
        m_selItem->setWrappedItem(itm);
        m_selItem->setVisible(true);
        emit pageSelected(itm->page());
    } else if (GGConnectionItem *itm = qgraphicsitem_cast<GGConnectionItem*>(itms[0])) {
        emit connectionSelected(itm->connection());
    }
}

void GGEditorScene::initSelItem()
{
    if (!m_selItem) {
        m_selItem = new GGSelectionItem;
        m_selItem->setVisible(false);
        m_selItem->setZValue(SELECTION_ZVALUE);
        addItem(m_selItem);
        m_selItem->init();
    }
}
