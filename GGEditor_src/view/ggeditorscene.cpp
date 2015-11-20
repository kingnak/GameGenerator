#include "ggeditorscene.h"
#include "../viewmodel/ggviewmodel.h"
#include "ggpageitem.h"

GGEditorScene::GGEditorScene(GGViewModel *model, QObject *parent)
    : QGraphicsScene(parent),
      m_model(model)
{
    connect(m_model, SIGNAL(viewPageRegistered(GGViewPage*)), this, SLOT(pageReg(GGViewPage*)));
    connect(m_model, SIGNAL(viewPageUnregistered(GGViewPage*)), this, SLOT(pageUnreg(GGViewPage*)));
    connect(m_model, SIGNAL(viewConnectionRegistered(GGViewConnection*)), this, SLOT(connReg(GGViewConnection*)));
    connect(m_model, SIGNAL(viewConnectionUnregistered(GGViewConnection*)), this, SLOT(connUnreg(GGViewConnection*)));
    connect(m_model, SIGNAL(pageUpdated(GGViewPage*)), this, SLOT(pageUpd(GGViewPage*)));
    connect(m_model, SIGNAL(viewPageUpdated(GGViewPage*)), this, SLOT(pageViewUpd(GGViewPage*)));
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
    // TODO: Later more?
    pageUpd(p);
}

