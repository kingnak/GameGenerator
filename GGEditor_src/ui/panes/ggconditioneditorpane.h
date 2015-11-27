#ifndef GGCONDITIONEDITORPANE_H
#define GGCONDITIONEDITORPANE_H

#include <QWidget>

class GGUIController;
class GGConditionPage;

namespace Ui {
class GGConditionEditorPane;
}

class GGConditionEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGConditionEditorPane(QWidget *parent = 0);
    ~GGConditionEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setPage(GGConditionPage *page);

private slots:
    void connectPage();
    void deleteConn();

private:
    Ui::GGConditionEditorPane *ui;
    GGUIController *m_ctrl;
    GGConditionPage *m_page;
};

#endif // GGCONDITIONEDITORPANE_H
