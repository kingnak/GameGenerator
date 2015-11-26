#include "ggcontenteditorpane.h"
#include "ui_ggcontenteditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include <model/ggpage.h>
#include <model/ggcontentelement.h>
#include <view/gguicontroller.h>

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
        pix = p->content()->preview(ui->lblPreview->minimumSize());
    ui->lblPreview->setPixmap(pix);
}

void GGContentEditorPane::on_txtCaption_editingFinished()
{
    m_ctrl->changeContentPageCaption(m_page, ui->txtCaption->text());
}

void GGContentEditorPane::on_btnChange_clicked()
{
    GGEditContentElementDialog dlg;
    dlg.setContentElement(m_page->content());
    if (dlg.exec() == QDialog::Accepted) {
        GGContentElement *e = dlg.getContentElement();
        m_ctrl->changeContentElement(m_page, e);
    }
}
