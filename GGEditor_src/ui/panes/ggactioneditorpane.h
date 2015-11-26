#ifndef GGACTIONEDITORPANE_H
#define GGACTIONEDITORPANE_H

#include <QWidget>

class GGUIController;
class GGActionPage;

namespace Ui {
class GGActionEditorPane;
}

class GGActionEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGActionEditorPane(QWidget *parent = 0);
    ~GGActionEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setPage(GGActionPage *page);

private slots:
    void updateLink();
    void connectAction();

private:
    Ui::GGActionEditorPane *ui;
    GGUIController *m_ctrl;
    GGActionPage *m_page;
};

#endif // GGACTIONEDITORPANE_H
