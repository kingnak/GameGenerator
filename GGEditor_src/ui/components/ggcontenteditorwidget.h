#ifndef GGCONTENTEDITORWIDGET_H
#define GGCONTENTEDITORWIDGET_H

#include <QWidget>

class GGContentPage;
class GGUIController;

namespace Ui {
class GGContentEditorWidget;
}

class GGContentEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGContentEditorWidget(QWidget *parent = 0);
    ~GGContentEditorWidget();

    void setController(GGUIController *ctrl);

public slots:
    void setContentPage(GGContentPage *p);

private slots:
    void on_txtCaption_editingFinished();
    void on_btnChange_clicked();

private:
    Ui::GGContentEditorWidget *ui;
    GGUIController *m_ctrl;
    GGContentPage *m_page;
};

#endif // GGCONTENTEDITORWIDGET_H
