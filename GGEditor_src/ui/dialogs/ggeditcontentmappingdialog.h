#ifndef GGEDITCONTENTMAPPINGDIALOG_H
#define GGEDITCONTENTMAPPINGDIALOG_H

#include <QDialog>
#include <model/ggconnectionslot.h>

class GGEditModel;
class GGPage;
class GGMappedContentPage;
class GGMappingScene;
class GGMappingUIController;
class GGCommandStack;
class GGAction;

namespace Ui {
class GGEditContentMappingDialog;
}

class GGEditContentMappingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGEditContentMappingDialog(GGEditModel *model, QWidget *parent = 0);
    ~GGEditContentMappingDialog();

    void setMappedPage(GGMappedContentPage *page);

    GGCommandStack *getExecutedCommands();

public slots:
    void reject();

private slots:
    void addLink(QRect rect);
    void moveLink(int idx, QRect rect);
    void updateLinkCaption(GGPage *, const GGConnectionSlot &slt, const QString &cap);
    void updateLinkAction(GGPage *, const GGConnectionSlot &slt, const GGAction &act);
    void updatePage(GGPage *page);
    void deleteConnect(GGPage *page, GGConnectionSlot slt);
    void handleHoverEnter(GGPage *page, GGConnectionSlot slt);
    void handleHoverLeave(GGPage *page, GGConnectionSlot slt);
    void deleteSelectedItem();

private:
    Ui::GGEditContentMappingDialog *ui;
    GGMappedContentPage *m_page;
    GGMappingScene *m_scene;
    GGMappingUIController *m_ctrl;
};

#endif // GGEDITCONTENTMAPPINGDIALOG_H
