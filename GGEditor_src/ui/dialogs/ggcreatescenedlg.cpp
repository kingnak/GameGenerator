#include "ggcreatescenedlg.h"
#include "ui_ggcreatescenedlg.h"

GGCreateSceneDlg::GGCreateSceneDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGCreateSceneDlg)
{
    ui->setupUi(this);
}

GGCreateSceneDlg::~GGCreateSceneDlg()
{
    delete ui;
}
