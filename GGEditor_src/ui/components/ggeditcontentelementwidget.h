#ifndef GGEDITCONTENTELEMENTWIDGET_H
#define GGEDITCONTENTELEMENTWIDGET_H

#include <QWidget>

class GGContentElement;

namespace Ui {
class GGEditContentElementWidget;
}

class GGEditContentElementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGEditContentElementWidget(QWidget *parent = 0);
    ~GGEditContentElementWidget();

    GGContentElement *getContentElement();

public slots:
    void setContentElement(GGContentElement *elem);

private slots:
    void changePage();
    void on_btnBrowseImage_clicked();

    void loadPreviewImage();

private:
    Ui::GGEditContentElementWidget *ui;
};

#endif // GGEDITCONTENTELEMENTWIDGET_H
