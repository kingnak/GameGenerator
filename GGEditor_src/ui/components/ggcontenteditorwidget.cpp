#include "ggcontenteditorwidget.h"
#include "ui_ggcontenteditorwidget.h"
#include <model/ggpage.h>
#include <model/ggcontentelement.h>
#include <view/gguicontroller.h>

GGContentEditorWidget::GGContentEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGContentEditorWidget)
{
    ui->setupUi(this);
}

GGContentEditorWidget::~GGContentEditorWidget()
{
    delete ui;
}

void GGContentEditorWidget::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGContentEditorWidget::setContentPage(GGContentPage *p)
{
    m_page = p;
    ui->txtCaption->setText(p->caption());
    QPixmap pix;
    if (p->content())
        pix = p->content()->preview(ui->lblPreview->minimumSize());
    ui->lblPreview->setPixmap(pix);
}

void GGContentEditorWidget::on_txtCaption_editingFinished()
{
    m_ctrl->changeContentPageCaption(m_page, ui->txtCaption->text());
}

void GGContentEditorWidget::on_btnChange_clicked()
{
    // TODO
}
