#include "ggmappingeditorpane.h"
#include "ui_ggmappingeditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include "../dialogs/ggeditcontentmappingdialog.h"
#include <view/gguicontroller.h>
#include <model/ggpage.h>
#include <model/ggcontentelement.h>

GGMappingEditorPane::GGMappingEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGMappingEditorPane),
    m_ctrl(NULL)
{
    ui->setupUi(this);
}

GGMappingEditorPane::~GGMappingEditorPane()
{
    delete ui;
}

void GGMappingEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGMappingEditorPane::setMappedPage(GGMappedContentPage *p)
{
    m_page = p;
    ui->txtCaption->setText(p->caption());
    QPixmap pix;
    if (p->content())
        pix = p->content()->preview(ui->lblPreview->minimumSize());
    ui->lblPreview->setPixmap(pix);
}

void GGMappingEditorPane::on_txtCaption_editingFinished()
{
    m_ctrl->changeContentPageCaption(m_page, ui->txtCaption->text());
}

void GGMappingEditorPane::on_btnChangeContent_clicked()
{
    GGEditContentElementDialog dlg;
    dlg.setContentElement(m_page->content());
    if (dlg.exec() == QDialog::Accepted) {
        GGContentElement *e = dlg.getContentElement();
        m_ctrl->changeContentElement(m_page, e);
    }
}

void GGMappingEditorPane::on_btnChangeMapping_clicked()
{
    GGEditContentMappingDialog dlg;
    dlg.setMappedPage(m_page);
    dlg.exec();
}
