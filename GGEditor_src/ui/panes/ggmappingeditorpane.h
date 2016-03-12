#ifndef GGMAPPINGEDITORPANE_H
#define GGMAPPINGEDITORPANE_H

#include <QWidget>
#include <model/ggconnectionslot.h>

class GGMappedContentPage;
class GGUIController;
class GGPage;

namespace Ui {
class GGMappingEditorPane;
}

class GGMappingEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGMappingEditorPane(QWidget *parent = 0);
    ~GGMappingEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setMappedPage(GGMappedContentPage *p);

private slots:
    void deleteLink(GGPage *, const GGConnectionSlot &slt);
    void connectLink(GGPage *, const GGConnectionSlot &slt);
    void on_txtCaption_editingFinished();
    void on_btnChangeContent_clicked();
    void on_btnChangeMapping_clicked();

private:
    Ui::GGMappingEditorPane *ui;
    GGUIController *m_ctrl;
    GGMappedContentPage *m_page;
};

#endif // GGMAPPINGEDITORPANE_H
