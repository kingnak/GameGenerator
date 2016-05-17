#include "ggeditorview.h"

GGEditorView::GGEditorView(QWidget *parent)
    : QGraphicsView(parent)
{

}

QPoint GGEditorView::getScenePosition() const
{
    return this->sceneRect().center().toPoint();
}

