#ifndef GGEDITCONTENTELEMENTDIALOG_H
#define GGEDITCONTENTELEMENTDIALOG_H

#include <QDialog>
#include <utils/ggglobaluserinfo.h>

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
    void accept();
    void reject();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::GGEditContentElementDialog *ui;
    GGWindowHelper m_windowInfo;
};

#endif // GGEDITCONTENTELEMENTDIALOG_H
