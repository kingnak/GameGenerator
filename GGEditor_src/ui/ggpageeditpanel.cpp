#include "ggpageeditpanel.h"
#include "ui_ggpageeditpanel.h"
#include <model/ggpage.h>
#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <model/ggeditmodel.h>

GGPageEditPanel::GGPageEditPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGPageEditPanel),
    m_ctrl(NULL),
    m_page(NULL)
{
    ui->setupUi(this);
}

void GGPageEditPanel::setController(GGUIController *ctrl)
{
    if (m_ctrl) disconnect(m_ctrl, 0, 0, 0);
    m_ctrl = ctrl;
    connect(m_ctrl, SIGNAL(modelReset(GGViewModel*)), this, SLOT(modelReset()));
    modelReset();

    ui->wgtEntryAction->setController(m_ctrl);
    ui->wgtCaption->setController(m_ctrl);
    ui->wgtContent->setController(m_ctrl);
    ui->wgtCondition->setController(m_ctrl);
    ui->wgtAction->setController(m_ctrl);
    ui->wgtDecision->setController(m_ctrl);
    ui->wgtMapping->setController(m_ctrl);
}

GGPageEditPanel::~GGPageEditPanel()
{
    delete ui;
}

GGPage *GGPageEditPanel::displayedPage() const
{
    return m_page;
}

void GGPageEditPanel::clearPage()
{
    displayPage(NULL);
}

void GGPageEditPanel::displayPage(GGPage *page, GGAbstractModel::PageSections updateSections)
{
    if (page != m_page) {
        m_page = page;
        ui->grpEntryAction->setVisible(false);
        ui->grpCaption->setVisible(false);
        ui->grpAction->setVisible(false);
        ui->grpCondition->setVisible(false);
        ui->grpContent->setVisible(false);
        ui->grpDecision->setVisible(false);
        ui->grpMapping->setVisible(false);
    }

    setCommon(updateSections);
    setEntryAction(updateSections);
    setCaption(updateSections);
    setStart(updateSections);
    setEnd(updateSections);
    setContent(updateSections);
    setCondition(updateSections);
    setAction(updateSections);
    setDecision(updateSections);
    setMappedContent(updateSections);
}

void GGPageEditPanel::editPageTitle()
{
    if (!m_page) return;
    ui->txtName->setFocus();
    ui->txtName->selectAll();
}

void GGPageEditPanel::editPageCaption()
{
    ui->wgtCaption->startEditing();
}

void GGPageEditPanel::editPageContent()
{
    ui->wgtContent->openContentEditor();
    ui->wgtMapping->openContentEditor();
}

void GGPageEditPanel::editPageContentMap()
{
    ui->wgtMapping->openMappingEditor();
}

void GGPageEditPanel::pageUpdated(GGPage *page, GGAbstractModel::PageSections sections)
{
    if (page == m_page) {
        displayPage(page, sections);
    }
}

void GGPageEditPanel::modelReset()
{
    disconnect(this, SLOT(pageUpdated(GGPage*,GGAbstractModel::PageSections)));
    if (m_ctrl->model() && m_ctrl->model()->editModel())
        connect(m_ctrl->model()->editModel(), SIGNAL(pageUpdated(GGPage*,GGAbstractModel::PageSections)), this, SLOT(pageUpdated(GGPage*,GGAbstractModel::PageSections)));
}

void GGPageEditPanel::on_txtName_editingFinished()
{
    m_ctrl->changePageName(m_page, ui->txtName->text());
}

//void GGPageEditPanel::on_txtScene_editingFinished()
//{
//    m_ctrl->changePageScene(m_page, ui->txtScene->text());
//}

void GGPageEditPanel::setCommon(GGAbstractModel::PageSections updateSections)
{
    // No performance issue yet
    Q_UNUSED(updateSections)
    if (m_page) {
        ui->txtName->setText(m_page->name());
    } else {
        ui->txtName->clear();
    }
}

void GGPageEditPanel::setEntryAction(GGAbstractModel::PageSections updateSections)
{
    if (GGEntryActionPage *eap = GG::as<GGEntryActionPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::EntryAction))
            ui->wgtEntryAction->setPage(eap);

        ui->grpEntryAction->setVisible(true);
    } else {
        ui->wgtEntryAction->setPage(NULL);
    }
}

void GGPageEditPanel::setCaption(GGAbstractModel::PageSections updateSections)
{
    if (GGContentPage *cp = GG::as<GGContentPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::Caption))
            ui->wgtCaption->setPage(cp);

        ui->grpCaption->setVisible(true);
    } else {
        ui->wgtCaption->setPage(NULL);
    }
}

void GGPageEditPanel::setStart(GGAbstractModel::PageSections updateSections)
{
    Q_UNUSED(updateSections);
    //if (GG::as<GGStartPage>(m_page))
}

void GGPageEditPanel::setEnd(GGAbstractModel::PageSections updateSections)
{
    Q_UNUSED(updateSections);
    //if (GG::as<GGEndPage>(m_page))
}

void GGPageEditPanel::setCondition(GGAbstractModel::PageSections updateSections)
{
    if (GGConditionPage *cp = GG::as<GGConditionPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::Connections) || updateSections.testFlag(GGAbstractModel::Condition))
            ui->wgtCondition->setPage(cp);

        ui->grpCondition->setVisible(true);
    } else {
        ui->wgtCondition->setPage(NULL);
    }
}

void GGPageEditPanel::setAction(GGAbstractModel::PageSections updateSections)
{
    if (GGActionPage *ap = GG::as<GGActionPage>(m_page)) {
        setMappedContent(updateSections);
        if (updateSections.testFlag(GGAbstractModel::Connections) || updateSections.testFlag(GGAbstractModel::PageData))
            ui->wgtAction->setPage(ap);

        ui->grpAction->setVisible(true);
    } else {
        ui->wgtAction->setPage(NULL);
    }
}

void GGPageEditPanel::setDecision(GGAbstractModel::PageSections updateSections)
{

    if (GGDecisionPage *dp = GG::as<GGDecisionPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::DecisionLinks))
            ui->wgtDecision->setPage(dp);

        ui->grpDecision->setVisible(true);
    } else {
        ui->wgtDecision->setPage(NULL);
    }
}

void GGPageEditPanel::setContent(GGAbstractModel::PageSections updateSections)
{
    // Mapped content is also a content, but that we don't need here
    GGContentPage *c = GG::as<GGContentPage>(m_page);
    if (c && !GG::as<GGMappedContentPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::Content))
            ui->wgtContent->setContentPage(c);
        ui->grpContent->setVisible(true);
    } else {
        ui->wgtContent->setContentPage(NULL);
    }
}

void GGPageEditPanel::setMappedContent(GGAbstractModel::PageSections updateSections)
{
    if (GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(m_page)) {
        if (updateSections.testFlag(GGAbstractModel::MappedLinks) ||
                updateSections.testFlag(GGAbstractModel::Content))
        {
            ui->wgtMapping->setMappedPage(mcp);
        }
        ui->grpMapping->setVisible(true);
    } else {
        ui->wgtMapping->setMappedPage(NULL);
    }
}
