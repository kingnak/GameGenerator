#include "ggeditcaptionpane.h"
#include "ui_ggeditcaptionpane.h"
#include <model/ggpage.h>
#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <model/ggeditmodel.h>


GGEditCaptionPane::GGEditCaptionPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGEditCaptionPane),
    m_ctrl(NULL)
{
    ui->setupUi(this);
    connect(ui->txtCaption, SIGNAL(textChanged(QString)), this, SLOT(captionUpdated(QString)));
}

GGEditCaptionPane::~GGEditCaptionPane()
{
    delete ui;
}

void GGEditCaptionPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGEditCaptionPane::setPage(GGContentPage *page)
{
    m_page = page;
    if (page) {
        ui->txtCaption->setStyler(m_ctrl->model()->editModel()->getStyler());
        ui->txtCaption->setFormattedText(page->caption());
    }
}

void GGEditCaptionPane::captionUpdated(const QString &caption)
{
    m_ctrl->changeContentPageCaption(m_page, caption);
}
