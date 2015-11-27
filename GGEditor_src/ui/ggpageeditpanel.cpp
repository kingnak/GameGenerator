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

void GGPageEditPanel::displayPage(GGPage *page)
{
    m_page = page;
    ui->grpAction->setVisible(false);
    ui->grpCondition->setVisible(false);
    ui->grpContent->setVisible(false);
    ui->grpDecision->setVisible(false);
    ui->grpMapping->setVisible(false);

    setCommon();
    setStart();
    setEnd();
    setCondition();
    setAction();
    setDecision();

    /*
    switch (page->type()) {
    case GGStartPage::Type:
        ui->grpContent->setVisible(true);
        break;
    case GGEndPage::Type:
        ui->grpContent->setVisible(true);
        break;
    case GGConditionPage::Type:
        ui->grpCondition->setVisible(true);
        break;
    case GGActionPage::Type:
        //ui->grpContent->setVisible(true);
        ui->grpAction->setVisible(true);
        ui->grpMapping->setVisible(true);
        break;
    case GGDecisionPage::Type:
        //ui->grpContent->setVisible(true);
        ui->grpDecision->setVisible(true);
        ui->grpMapping->setVisible(true);
        break;
    default:
        Q_ASSERT(false);
    }
    */
}

void GGPageEditPanel::pageUpdated(GGPage *page)
{
    if (page == m_page) {
        displayPage(page);
    }
}

void GGPageEditPanel::modelReset()
{
    disconnect(this, SLOT(pageUpdated(GGPage*)));
    if (m_ctrl->model() && m_ctrl->model()->editModel())
        connect(m_ctrl->model()->editModel(), SIGNAL(pageUpdated(GGPage*)), this, SLOT(pageUpdated(GGPage*)));
}

void GGPageEditPanel::on_txtName_editingFinished()
{
    m_ctrl->changePageName(m_page, ui->txtName->text());
}

void GGPageEditPanel::on_txtScene_editingFinished()
{
    m_ctrl->changePageScene(m_page, ui->txtScene->text());
}

void GGPageEditPanel::setCommon()
{
    ui->txtName->setText(m_page->name());
    ui->txtScene->setText(m_page->sceneName());
}

void GGPageEditPanel::setStart()
{
    if (GG::as<GGStartPage>(m_page))
        setContent();
}

void GGPageEditPanel::setEnd()
{
    if (GG::as<GGEndPage>(m_page))
        setContent();
}

void GGPageEditPanel::setCondition()
{
    if (GGConditionPage *cp = GG::as<GGConditionPage>(m_page)) {
        ui->wgtCondition->setPage(cp);
        ui->grpCondition->setVisible(true);
    }
}

void GGPageEditPanel::setAction()
{
    setMappedContent();
    if (GGActionPage *ap = GG::as<GGActionPage>(m_page)) {
        ui->wgtAction->setPage(ap);
        ui->grpAction->setVisible(true);
    }
}

void GGPageEditPanel::setDecision()
{
    setMappedContent();
    if (GGDecisionPage *dp = GG::as<GGDecisionPage>(m_page)) {
        ui->wgtDecision->setPage(dp);
        ui->grpDecision->setVisible(true);
    }
}

void GGPageEditPanel::setContent()
{
    if (GGContentPage *c = GG::as<GGContentPage>(m_page)) {
        ui->wgtContent->setContentPage(c);
        ui->grpContent->setVisible(true);
    }
}

void GGPageEditPanel::setMappedContent()
{
    if (GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(m_page)) {
        ui->wgtMapping->setMappedPage(mcp);
        ui->grpMapping->setVisible(true);
    }
}
