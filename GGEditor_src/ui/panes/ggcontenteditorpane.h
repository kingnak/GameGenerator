#ifndef GGCONTENTEDITORPANE_H
#define GGCONTENTEDITORPANE_H

#include <QWidget>

class GGContentPage;
class GGUIController;

namespace Ui {
class GGContentEditorPane;
}

class GGContentEditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGContentEditorPane(QWidget *parent = 0);
    ~GGContentEditorPane();

    void setController(GGUIController *ctrl);

public slots:
    void setContentPage(GGContentPage *p);

private slots:
    void on_btnChange_clicked();
    void on_btnEditCaption_clicked();

private:
    Ui::GGContentEditorPane *ui;
    GGUIController *m_ctrl;
    GGContentPage *m_page;
};

#endif // GGCONTENTEDITORPANE_H
