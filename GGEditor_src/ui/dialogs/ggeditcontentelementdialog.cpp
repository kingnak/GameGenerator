#include "ggeditcontentelementdialog.h"
#include "ui_ggeditcontentelementdialog.h"


GGEditContentElementDialog::GGEditContentElementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditContentElementDialog)
{
    ui->setupUi(this);
}

GGEditContentElementDialog::~GGEditContentElementDialog()
{
    delete ui;
}

GGContentElement *GGEditContentElementDialog::getContentElement()
{
    return ui->wgtContent->getContentElement();
}

void GGEditContentElementDialog::setContentElement(GGContentElement *elem, GGAbstractMediaResolver *resolver)
{
    ui->wgtContent->setContentElement(elem, resolver);
}


