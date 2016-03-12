#include "ggmappingeditorpane.h"
#include "ui_ggmappingeditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include "../dialogs/ggeditcontentmappingdialog.h"
#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <model/ggpage.h>
#include <model/ggcontentelement.h>

GGMappingEditorPane::GGMappingEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGMappingEditorPane),
    m_ctrl(NULL)
{
    ui->setupUi(this);
    connect(ui->wgtMappedConnections, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)), this, SLOT(connectLink(GGPage*,GGConnectionSlot)));
    connect(ui->wgtMappedConnections, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SLOT(deleteLink(GGPage*,GGConnectionSlot)));
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
    QList<GGConnectionSlot> slts = GGConnectionSlot::enumerateConnections(p, GGConnectionSlot::MappedConnection);
    ui->wgtMappedConnections->setConnections(p, slts);
}

void GGMappingEditorPane::deleteLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->removeMappedLink(m_page, slt.index());
}

void GGMappingEditorPane::connectLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->connectPageDirect(m_page, slt);
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
    GGEditContentMappingDialog dlg(m_ctrl->model()->editModel());
    dlg.setMappedPage(m_page);
    int res = dlg.exec();
    if (res == QDialog::Accepted) {
        m_ctrl->applySubcommandsAsSingle(dlg.getExecutedCommands());
    }
}
