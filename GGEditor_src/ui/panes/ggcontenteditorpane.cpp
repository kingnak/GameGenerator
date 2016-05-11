#include "ggcontenteditorpane.h"
#include "ui_ggcontenteditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include <model/ggpage.h>
#include <model/ggcontentelement.h>
#include <view/gguicontroller.h>
#include <model/ggeditproject.h>
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
    ui->wgtCaptionEdit->setStyler(m_ctrl->model()->editModel()->getStyler());
    ui->wgtCaptionEdit->setFormattedText(p->caption());
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

void GGContentEditorPane::onCaptionUpdate(const QString &caption)
{
    m_ctrl->changeContentPageCaption(m_page, caption);
}
