#ifndef GGMAINWINDOW_H
#define GGMAINWINDOW_H

#include <QMainWindow>

#include <gg_definitions.h>

class QActionGroup;

class GGUIController;
class GGEditorScene;
class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGPage;
class GGConnectionSlot;
class GGSearchDialog;
class GGSearchRequest;

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

public slots:
    void newModel();
    void closeModel();

    void highlightPage(GG::PageID id);
    void selectPage(GGViewPage *page);
    void selectConnection(GGViewConnection *conn);
    void clearSelection();

    void executeSearch(const GGSearchRequest &req);
    void showSearchDialog(bool reset);
    void showSearchDialog();

private slots:
    void showError(QString err);
    void setClickMode(QAction *act);
    void setPointerMode();
    void handleAction(QAction *act);
    void setCreationMode();
    void handleConnectDirect(GGPage *page, const GGConnectionSlot &slot);
    void showVariables();

private:
    Ui::GGMainWindow *ui;
    GGUIController *m_ctrl;
    GGEditorScene *m_editorScene;
    GGViewModel *m_viewModel;
    QActionGroup *m_createActions;
    GGSearchDialog *m_searchDlg;

};

#endif // GGMAINWINDOW_H
