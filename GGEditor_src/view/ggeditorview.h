#ifndef GGEDITORVIEW_H
#define GGEDITORVIEW_H

#include <QGraphicsView>

class GGEditorView : public QGraphicsView
{
public:
    GGEditorView(QWidget *parent = 0);
    QPoint getScenePosition() const;

protected:
    //void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GGEDITORVIEW_H
