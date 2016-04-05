#ifndef GGGRAPHPANEL_H
#define GGGRAPHPANEL_H

#include <QWidget>

class GGViewScene;
class GGUIController;
class GGEditorView;
class GGEditorScene;

class GGGraphPanel : public QWidget
{
    Q_OBJECT
public:
    explicit GGGraphPanel(GGUIController *ctrl, GGViewScene *scene, QWidget *parent = 0);

    GGEditorScene *editorScene() { return m_editScene; }
    GGEditorView *editorView() { return m_editView; }

signals:

private:
    GGEditorView * m_editView;
    GGEditorScene *m_editScene;
};

#endif // GGGRAPHPANEL_H
