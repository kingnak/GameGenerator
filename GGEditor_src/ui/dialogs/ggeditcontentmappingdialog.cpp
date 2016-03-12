#include "ggeditcontentmappingdialog.h"
#include "ui_ggeditcontentmappingdialog.h"
#include <QAction>
#include <model/ggeditmodel.h>
#include <model/ggpage.h>
#include <model/gglink.h>
#include <model/ggcontentelement.h>
#include <view/ggmappingscene.h>
#include <view/ggmappinguicontroller.h>

GGEditContentMappingDialog::GGEditContentMappingDialog(GGEditModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditContentMappingDialog),
    m_page(NULL)
{
    ui->setupUi(this);
    m_scene = new GGMappingScene(this);
    ui->graphicsView->setScene(m_scene);
    m_ctrl = new GGMappingUIController(model, this);

    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionDelete->setShortcut(QKeySequence::Delete);

    this->addAction(ui->actionUndo);
    this->addAction(ui->actionRedo);
    ui->graphicsView->addAction(ui->actionDelete);

    ui->wgtLinks->setFields(GGConnectionEditorWidget::Action | GGConnectionEditorWidget::Caption | GGConnectionEditorWidget::Page | GGConnectionEditorWidget::Number);
    ui->wgtLinks->setActions(GGConnectionEditorWidget::Edit | GGConnectionEditorWidget::Delete | GGConnectionEditorWidget::Hover);

    connect(ui->actionUndo, SIGNAL(triggered(bool)), m_ctrl, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered(bool)), m_ctrl, SLOT(redo()));
    connect(ui->actionDelete, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedItem()));

    connect(model, SIGNAL(pageUpdated(GGPage*)), this, SLOT(updatePage(GGPage*)));
    connect(m_scene, SIGNAL(addedItem(QRect)), this, SLOT(addLink(QRect)));
    connect(m_scene, SIGNAL(movedItem(int,QRect)), this, SLOT(moveLink(int,QRect)));

    connect(ui->wgtLinks, SIGNAL(hoverEnteredConnection(GGPage*,GGConnectionSlot)), this, SLOT(handleHoverEnter(GGPage*,GGConnectionSlot)));
    connect(ui->wgtLinks, SIGNAL(hoverLeftConnection(GGPage*,GGConnectionSlot)), this, SLOT(handleHoverLeave(GGPage*,GGConnectionSlot)));

    connect(ui->wgtLinks, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SLOT(deleteConnect(GGPage*,GGConnectionSlot)));
}

GGEditContentMappingDialog::~GGEditContentMappingDialog()
{
    delete ui;
}

void GGEditContentMappingDialog::setMappedPage(GGMappedContentPage *page)
{
    m_page = page;
    updatePage(page);
}

GGCommandStack *GGEditContentMappingDialog::getExecutedCommands()
{
    return m_ctrl->getCommandStack();
}

void GGEditContentMappingDialog::reject()
{
    disconnect(this, SLOT(updatePage(GGPage*)));
    m_ctrl->undoAll();

    QDialog::reject();
}

void GGEditContentMappingDialog::addLink(QRect rect)
{
    m_ctrl->createLink(m_page, rect);
}

void GGEditContentMappingDialog::moveLink(int idx, QRect rect)
{
    m_ctrl->moveLink(m_page, idx, rect);
}

void GGEditContentMappingDialog::updatePage(GGPage *page)
{
    if (page && page == m_page) {
        if (m_page->content()) {
            QPixmap p = m_page->content()->preview(QSize());
            m_scene->setMappedElement(p);
        }

        QList<GGConnectionSlot> slts = GGConnectionSlot::enumerateConnections(m_page, GGConnectionSlot::MappedConnection);
        ui->wgtLinks->setConnections(m_page, slts);
        m_scene->setConnections(m_page, slts);
    }
}

void GGEditContentMappingDialog::deleteConnect(GGPage *page, GGConnectionSlot slt)
{
    Q_UNUSED(page);
    m_ctrl->deleteLink(m_page, slt.index());
}

void GGEditContentMappingDialog::handleHoverEnter(GGPage *page, GGConnectionSlot slt)
{
    Q_UNUSED(page);
    m_scene->hoverItem(slt.index());
}

void GGEditContentMappingDialog::handleHoverLeave(GGPage *page, GGConnectionSlot slt)
{
    Q_UNUSED(page);
    Q_UNUSED(slt);
    m_scene->hoverItem(-1);
}

void GGEditContentMappingDialog::deleteSelectedItem()
{
    GGConnectionSlot slt = m_scene->getSelectedSlot();
    if (slt.type() == GGConnectionSlot::MappedConnection) {
        m_ctrl->deleteLink(m_page, slt.index());
    }
}
