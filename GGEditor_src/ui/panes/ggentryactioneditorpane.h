#ifndef GGENTRYACTIONEDITORPANE_H
#define GGENTRYACTIONEDITORPANE_H

#include <QWidget>

class GGUIController;
class GGEntryActionPage;

namespace Ui {
class GGEntryActionEditorPane;
}

class GGEntryActionEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGEntryActionEditorPane(QWidget *parent = 0);
    ~GGEntryActionEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setPage(GGEntryActionPage *page);

private slots:
    void updateEntryAction();

private:
    Ui::GGEntryActionEditorPane *ui;
    GGUIController *m_ctrl;
    GGEntryActionPage *m_page;
};

#endif // GGENTRYACTIONEDITORPANE_H
