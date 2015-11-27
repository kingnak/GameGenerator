#ifndef GGDECISIONEDITORPANE_H
#define GGDECISIONEDITORPANE_H

#include <QWidget>

class GGUIController;
class GGPage;
class GGDecisionPage;
class GGConnectionSlot;

namespace Ui {
class GGDecisionEditorPane;
}

class GGDecisionEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGDecisionEditorPane(QWidget *parent = 0);
    ~GGDecisionEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setPage(GGDecisionPage *page);

private slots:
    void deleteLink(GGPage *, const GGConnectionSlot &slt);
    void connectLink(GGPage *, const GGConnectionSlot &slt);
    void on_btnAdd_clicked();

private:
    Ui::GGDecisionEditorPane *ui;
    GGUIController *m_ctrl;
    GGDecisionPage *m_page;
};

#endif // GGDECISIONEDITORPANE_H
