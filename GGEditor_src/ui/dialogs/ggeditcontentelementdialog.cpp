#include "ggeditcontentelementdialog.h"
#include "ui_ggeditcontentelementdialog.h"
#include <QCloseEvent>

GGEditContentElementDialog::GGEditContentElementDialog(GGSceneMediaManager *manager, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    ui(new Ui::GGEditContentElementDialog),
    m_windowInfo(this, "ContentEditor")
{
    ui->setupUi(this);
    ui->wgtContent->setManager(manager);
    m_windowInfo.restore();
}

GGEditContentElementDialog::~GGEditContentElementDialog()
{
    delete ui;
}

GGContentElement *GGEditContentElementDialog::getContentElement()
{
    return ui->wgtContent->getContentElement();
}

void GGEditContentElementDialog::setContentElement(GGContentElement *elem, GGScene *scene)
{
    ui->wgtContent->setContentElement(elem, scene);
}

void GGEditContentElementDialog::accept()
{
    m_windowInfo.backup();
    QDialog::accept();
}

void GGEditContentElementDialog::reject()
{
    m_windowInfo.backup();
    QDialog::reject();
}

void GGEditContentElementDialog::closeEvent(QCloseEvent *event)
{
    m_windowInfo.backup();
    event->accept();
}
