#ifndef GGMAINWINDOW_H
#define GGMAINWINDOW_H

#include <QMainWindow>

#include <gg_definitions.h>

class QActionGroup;

class GGEditProject;
class GGUIController;
class GGEditorScene;
class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGScene;
class GGPage;
class GGConnectionSlot;
class GGSearchDialog;
class GGSearchRequest;
class GGGraphPanel;
class GGSceneTreeModel;

namespace Ui {
class GGMainWindow;
}

class GGMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GGMainWindow(QWidget *parent = 0);
    ~GGMainWindow();

    GGViewModel *currentModel();
    GGUIController *controller();

    GGGraphPanel *currentSceneView();
    GGGraphPanel *sceneViewForId(GG::SceneID id);

public slots:
    void newProject();
    void closeProject();

    void openSceneView(GGScene *scene);
    void closeSceneView(GG::SceneID id);

    void highlightPage(GG::PageID id);
    void selectPage(GGViewPage *page);
    void selectConnection(GGViewConnection *conn);
    void clearSelection();

    void executeSearch(const GGSearchRequest &req);
    void showSearchDialog(bool reset);
    void showSearchDialog();

    void showStartPage();

private slots:
    void showError(QString err);
    void setClickMode(QAction *act);
    void setPointerMode();
    void handleAction(QAction *act);
    void setCreationMode();
    void handleConnectDirect(GGPage *page, const GGConnectionSlot &slot);
    void showVariables();
    void closeTab(int idx);
    void changeTab(int idx);
    void sceneTreeActivated(const QModelIndex &idx);

private:
    void updateWindowTitle();

private:
    QString m_baseTitle;
    Ui::GGMainWindow *ui;
    GGUIController *m_ctrl;
    GGEditProject *m_project;
    GGViewModel *m_viewModel;
    QActionGroup *m_createActions;
    GGSearchDialog *m_searchDlg;
    GGSceneTreeModel *m_sceneTree;
};

#endif // GGMAINWINDOW_H
