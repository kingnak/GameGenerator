#include "gggraphpanel.h"
#include <QtWidgets>
#include <view/ggeditorscene.h>
#include <view/ggeditorview.h>
#include <view/gguicontroller.h>

GGGraphPanel::GGGraphPanel(GGUIController *ctrl, GGViewScene *scene, QWidget *parent)
    : QWidget(parent)
{
    m_editScene = new GGEditorScene(ctrl, this);
    m_editScene->resetModel(ctrl->model(), scene);
    m_editView = new GGEditorView(this);
    m_editView->setScene(m_editScene);
    QHBoxLayout *l = new QHBoxLayout(this);
    l->addWidget(m_editView);
}
