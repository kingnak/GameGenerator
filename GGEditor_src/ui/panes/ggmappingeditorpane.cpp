#include "ggmappingeditorpane.h"
#include "ui_ggmappingeditorpane.h"
#include "../dialogs/ggeditcontentelementdialog.h"
#include "../dialogs/ggeditcontentmappingdialog.h"
#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <model/ggpage.h>
#include <model/ggcontentelement.h>
#include <model/ggeditproject.h>


GGMappingEditorPane::GGMappingEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGMappingEditorPane),
    m_ctrl(NULL),
    m_page(NULL)
{
    ui->setupUi(this);
    connect(ui->wgtMappedConnections, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)), this, SLOT(connectLink(GGPage*,GGConnectionSlot)));
    connect(ui->wgtMappedConnections, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SLOT(deleteLink(GGPage*,GGConnectionSlot)));
    connect(ui->wgtMappedConnections, SIGNAL(updateLinkCaption(GGPage*,GGConnectionSlot,QString)), this, SLOT(updateLinkCaption(GGPage*,GGConnectionSlot,QString)));
    connect(ui->wgtMappedConnections, SIGNAL(updateLinkAction(GGPage*,GGConnectionSlot,GGAction)), this, SLOT(updateLinkAction(GGPage*,GGConnectionSlot,GGAction)));
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
    QPixmap pix;
    QList<GGConnectionSlot> slts;
    if (p) {
        if (p->content())
            pix = p->content()->preview(p->model()->mediaResolver(), ui->lblPreview->minimumSize());
        slts = GGConnectionSlot::enumerateConnections(p, GGConnectionSlot::MappedConnection);
    }
    ui->wgtMappedConnections->setConnections(p, slts);
    ui->lblPreview->setPixmap(pix);
}

void GGMappingEditorPane::openContentEditor()
{
    if (!m_page) return;
    GGEditContentElementDialog dlg(m_ctrl->project()->mediaManager());
    dlg.setContentElement(m_page->content(), m_page->scene());
    if (dlg.exec() == QDialog::Accepted) {
        GGContentElement *e = dlg.getContentElement();
        m_ctrl->changeContentElement(m_page, e);
    }
}

void GGMappingEditorPane::openMappingEditor()
{
    if (!m_page) return;
    GGEditContentMappingDialog dlg(m_ctrl->model()->editModel());
    dlg.setMappedPage(m_page);
    int res = dlg.exec();
    if (res == QDialog::Accepted) {
        m_ctrl->applySubcommandsAsSingle(dlg.getExecutedCommands());
    }
}

void GGMappingEditorPane::updateLinkCaption(GGPage *, const GGConnectionSlot &slt, const QString &cap)
{
    GGLink l = m_page->getLinkMap()[slt.index()].link();
    l.setName(cap);
    m_ctrl->changeLink(m_page, slt, l);
}

void GGMappingEditorPane::updateLinkAction(GGPage *, const GGConnectionSlot &slt, const GGAction &act)
{
    GGLink l = m_page->getLinkMap()[slt.index()].link();
    l.setAction(act);
    m_ctrl->changeLink(m_page, slt, l);
}

void GGMappingEditorPane::deleteLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->removeMappedLink(m_page, slt.index());
}

void GGMappingEditorPane::connectLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->connectPageDirect(m_page, slt);
}

void GGMappingEditorPane::on_btnChangeContent_clicked()
{
    openContentEditor();
}

void GGMappingEditorPane::on_btnChangeMapping_clicked()
{
    openMappingEditor();
}

void GGMappingEditorPane::onUpdateCaption(const QString &caption)
{
    m_ctrl->changeContentPageCaption(m_page, caption);
}
