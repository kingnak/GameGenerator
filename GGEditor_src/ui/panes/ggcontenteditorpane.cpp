#include "ggcontenteditorpane.h"
#include "ui_ggcontenteditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include <model/ggpage.h>
#include <model/ggcontentelement.h>
#include <view/gguicontroller.h>
#include <model/ggeditproject.h>
#include <ui/dialogs/ggedittextdialog.h>
#include <viewmodel/ggviewmodel.h>
#include <model/ggeditmodel.h>

GGContentEditorPane::GGContentEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGContentEditorPane)
{
    ui->setupUi(this);
}

GGContentEditorPane::~GGContentEditorPane()
{
    delete ui;
}

void GGContentEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGContentEditorPane::setContentPage(GGContentPage *p)
{
    m_page = p;
    ui->txtCaption->setText(p->caption());
    QPixmap pix;
    if (p->content())
        pix = p->content()->preview(p->model()->mediaResolver(), ui->lblPreview->minimumSize());
    ui->lblPreview->setPixmap(pix);
}

void GGContentEditorPane::on_btnChange_clicked()
{
    GGEditContentElementDialog dlg(this->m_ctrl->project()->mediaManager());
    dlg.setContentElement(m_page->content(), m_page->scene());
    if (dlg.exec() == QDialog::Accepted) {
        GGContentElement *e = dlg.getContentElement();
        m_ctrl->changeContentElement(m_page, e);
    }
}

void GGContentEditorPane::on_btnEditCaption_clicked()
{
    GGEditTextDialog dlg(this);
    dlg.setStyler(m_ctrl->model()->editModel()->getStyler());
    dlg.setFormattedDocument(m_page->caption());
    if (dlg.exec() == QDialog::Accepted) {
        m_ctrl->changeContentPageCaption(m_page, dlg.getFormattedDocument());
    }
}
