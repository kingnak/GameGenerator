#include "ggeditcontentmappingdialog.h"
#include "ui_ggeditcontentmappingdialog.h"
#include <model/ggpage.h>
#include <model/gglink.h>
#include <model/ggcontentelement.h>
#include <view/ggmappingscene.h>

GGEditContentMappingDialog::GGEditContentMappingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditContentMappingDialog),
    m_page(NULL)
{
    ui->setupUi(this);
    m_scene = new GGMappingScene(this);
    ui->graphicsView->setScene(m_scene);
}

GGEditContentMappingDialog::~GGEditContentMappingDialog()
{
    delete ui;
}

void GGEditContentMappingDialog::setMappedPage(GGMappedContentPage *page)
{
    m_page = page;
    if (page && page->content()) {
        QPixmap p = page->content()->preview(QSize());
        m_scene->setMappedElement(p);
    }
}
