#ifndef GGEDITCONTENTMAPPINGDIALOG_H
#define GGEDITCONTENTMAPPINGDIALOG_H

#include <QDialog>

class GGMappedContentPage;
class GGMappingScene;

namespace Ui {
class GGEditContentMappingDialog;
}

class GGEditContentMappingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGEditContentMappingDialog(QWidget *parent = 0);
    ~GGEditContentMappingDialog();

    void setMappedPage(GGMappedContentPage *page);

private:
    Ui::GGEditContentMappingDialog *ui;
    GGMappedContentPage *m_page;
    GGMappingScene *m_scene;
};

#endif // GGEDITCONTENTMAPPINGDIALOG_H
