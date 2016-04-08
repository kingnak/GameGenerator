#ifndef GGEDITCONTENTELEMENTDIALOG_H
#define GGEDITCONTENTELEMENTDIALOG_H

#include <QDialog>

class GGContentElement;
class GGAbstractMediaResolver;

namespace Ui {
class GGEditContentElementDialog;
}

class GGEditContentElementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGEditContentElementDialog(QWidget *parent = 0);
    ~GGEditContentElementDialog();

    GGContentElement *getContentElement();

public slots:
    void setContentElement(GGContentElement *elem, GGAbstractMediaResolver *resolver);

private:
    Ui::GGEditContentElementDialog *ui;
};

#endif // GGEDITCONTENTELEMENTDIALOG_H
