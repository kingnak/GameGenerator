#ifndef GGMAINWINDOW_H
#define GGMAINWINDOW_H

#include <QMainWindow>

class QActionGroup;

class GGUIController;
class GGEditorScene;
class GGViewModel;
class GGViewPage;
class GGViewConnection;

namespace Ui {
class GGMainWindow;
}

class GGMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GGMainWindow(QWidget *parent = 0);
    ~GGMainWindow();

public slots:
    void newModel();
    void closeModel();

    void selectPage(GGViewPage *page);
    void selectConnection(GGViewConnection *conn);
    void clearSelection();

private slots:
    void showError(QString err);
    void setClickMode(QAction *act);
    void setPointerMode();
    void handleAction(QAction *act);

private:
    Ui::GGMainWindow *ui;
    GGUIController *m_ctrl;
    GGEditorScene *m_editorScene;
    GGViewModel *m_viewModel;
    QActionGroup *m_createActions;

};

#endif // GGMAINWINDOW_H
