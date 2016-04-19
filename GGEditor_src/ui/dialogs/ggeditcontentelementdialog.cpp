#include "ggeditcontentelementdialog.h"
#include "ui_ggeditcontentelementdialog.h"


GGEditContentElementDialog::GGEditContentElementDialog(GGSceneMediaManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditContentElementDialog)
{
    ui->setupUi(this);
    ui->wgtContent->setManager(manager);
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
