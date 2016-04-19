#ifndef GGEDITCONTENTELEMENTDIALOG_H
#define GGEDITCONTENTELEMENTDIALOG_H

#include <QDialog>

class GGContentElement;
class GGAbstractMediaResolver;
class GGSceneMediaManager;
class GGScene;

namespace Ui {
class GGEditContentElementDialog;
}

class GGEditContentElementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGEditContentElementDialog(GGSceneMediaManager *manager, QWidget *parent = 0);
    ~GGEditContentElementDialog();

    GGContentElement *getContentElement();

public slots:
    void setContentElement(GGContentElement *elem, GGScene *scene);

private:
    Ui::GGEditContentElementDialog *ui;
};

#endif // GGEDITCONTENTELEMENTDIALOG_H
