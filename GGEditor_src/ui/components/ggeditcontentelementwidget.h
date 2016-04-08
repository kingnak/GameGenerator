#ifndef GGEDITCONTENTELEMENTWIDGET_H
#define GGEDITCONTENTELEMENTWIDGET_H

#include <QWidget>

class GGContentElement;
class GGAbstractMediaResolver;

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
    void setContentElement(GGContentElement *elem, GGAbstractMediaResolver *resolver);

private slots:
    void changePage();
    void on_btnBrowseImage_clicked();

    void loadPreviewImage(const QString &media, GGAbstractMediaResolver *resolver);

private:
    Ui::GGEditContentElementWidget *ui;
};

#endif // GGEDITCONTENTELEMENTWIDGET_H
