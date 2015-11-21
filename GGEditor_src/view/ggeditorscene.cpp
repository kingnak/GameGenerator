#include "ggeditorscene.h"
#include "../viewmodel/ggviewmodel.h"
#include "ggpageitem.h"
#include "ggselectionitem.h"
#include "gguicontroller.h"
#include <viewmodel/ggviewpage.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

GGEditorScene::GGEditorScene(GGViewModel *model, QObject *parent)
    : QGraphicsScene(parent),
      m_model(model)
{
    m_selItem = new GGSelectionItem;
    m_selItem->setVisible(false);
    m_selItem->setZValue(100);
    addItem(m_selItem);
    m_selItem->init();

    connect(m_model, SIGNAL(viewPageRegistered(GGViewPage*)), this, SLOT(pageReg(GGViewPage*)));
    connect(m_model, SIGNAL(viewPageUnregistered(GGViewPage*)), this, SLOT(pageUnreg(GGViewPage*)));
    connect(m_model, SIGNAL(viewConnectionRegistered(GGViewConnection*)), this, SLOT(connReg(GGViewConnection*)));
    connect(m_model, SIGNAL(viewConnectionUnregistered(GGViewConnection*)), this, SLOT(connUnreg(GGViewConnection*)));
    connect(m_model, SIGNAL(pageUpdated(GGViewPage*)), this, SLOT(pageUpd(GGViewPage*)));
    connect(m_model, SIGNAL(viewPageUpdated(GGViewPage*)), this, SLOT(pageViewUpd(GGViewPage*)));

    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelectionItem()));
}

void GGEditorScene::itemMoved(GGPageItem *item)
{
    if (item == m_selItem->wrappedItem()) {
        m_selItem->setWrappedItem(item);
    }
}

void GGEditorScene::connectToController(GGUIController *ctrl)
{
    connect(this, SIGNAL(pageMoved(GGViewPage*,QRect)), ctrl, SLOT(changePageGeometry(GGViewPage*,QRect)));
    connect(this, SIGNAL(multiplePagesMoved(QList<QPair<GGViewPage*,QRect> >)), ctrl, SLOT(changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> >)));
}

void GGEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QList<QPair<GGViewPage*,QRect> > movs;
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
    QGraphicsScene::mouseReleaseEvent(event);
}

void GGEditorScene::pageReg(GGViewPage *p)
{
    GGPageItem *item = new GGPageItem(p);
    m_pageMap[p] = item;
    this->addItem(item);
}

void GGEditorScene::pageUnreg(GGViewPage *p)
{
    delete m_pageMap.take(p);
}

void GGEditorScene::connReg(GGViewConnection *c)
{
    // TODO
}

void GGEditorScene::connUnreg(GGViewConnection *c)
{
    m_connMap.remove(c);
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
    if (itms.size() != 1) {
        m_selItem->setVisible(false);
        m_selItem->setWrappedItem(NULL);
    } else {
        m_selItem->setWrappedItem(qgraphicsitem_cast<GGPageItem*>(itms[0]));
        m_selItem->setVisible(true);
    }
}
