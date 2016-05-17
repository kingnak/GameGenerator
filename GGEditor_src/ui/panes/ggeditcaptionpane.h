#ifndef GGEDITCAPTIONPANE_H
#define GGEDITCAPTIONPANE_H

#include <QWidget>

class GGUIController;
class GGContentPage;

namespace Ui {
class GGEditCaptionPane;
}

class GGEditCaptionPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGEditCaptionPane(QWidget *parent = 0);
    ~GGEditCaptionPane();

public slots:
    void setController(GGUIController *ctrl);
    void setPage(GGContentPage *page);
    void startEditing();

private slots:
    void captionUpdated(const QString &caption);

private:
    Ui::GGEditCaptionPane *ui;
    GGUIController *m_ctrl;
    GGContentPage *m_page;
};

#endif // GGEDITCAPTIONPANE_H
