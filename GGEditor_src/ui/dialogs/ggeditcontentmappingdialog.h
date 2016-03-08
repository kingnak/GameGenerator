#ifndef GGEDITCONTENTMAPPINGDIALOG_H
#define GGEDITCONTENTMAPPINGDIALOG_H

#include <QDialog>

class GGEditModel;
class GGPage;
class GGMappedContentPage;
class GGMappingScene;
class GGMappingUIController;
class GGCommandStack;

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
    void updatePage(GGPage *page);

private:
    Ui::GGEditContentMappingDialog *ui;
    GGMappedContentPage *m_page;
    GGMappingScene *m_scene;
    GGMappingUIController *m_ctrl;
};

#endif // GGEDITCONTENTMAPPINGDIALOG_H
