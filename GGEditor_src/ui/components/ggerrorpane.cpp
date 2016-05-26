#include "ggerrorpane.h"
#include "ui_ggerrorpane.h"
#include <model/ggpage.h>

typedef QPair<const GGPage *, GGConnectionSlotData> ItemData;
Q_DECLARE_METATYPE(ItemData)

GGErrorPane::GGErrorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGErrorPane)
{
    ui->setupUi(this);
}

GGErrorPane::~GGErrorPane()
{
    delete ui;
}

void GGErrorPane::clear()
{
    ui->lstErrors->clear();
    ui->lblCount->setText("No errors");
}

void GGErrorPane::enableVerify(bool enable)
{
    ui->btnRefresh->setEnabled(enable);
}

void GGErrorPane::setErrors(GGModelErrorList list)
{
    clear();
    if (list.isEmpty()) return;

    ui->lblCount->setText(QString("%1 errors").arg(list.count()));
    foreach (GGModelError e, list) {
        QListWidgetItem *itm = new QListWidgetItem(e.message());
        switch (e.severity()) {
        case GGModelError::Fatal:
        case GGModelError::Error:
            itm->setIcon(QPixmap(":/icons/error"));
            break;
        case GGModelError::Warning:
            itm->setIcon(QPixmap(":/icons/warning"));
            break;
        case GGModelError::Notice:
            itm->setIcon(QPixmap(":/icons/info"));
            break;
        case GGModelError::None:
            break;
        }

        ItemData d = qMakePair(e.page(), e.slot());
        itm->setData(Qt::UserRole, QVariant::fromValue(d));
        ui->lstErrors->addItem(itm);
    }
}

void GGErrorPane::on_btnRefresh_clicked()
{
    emit reverify();
}

void GGErrorPane::showError(QListWidgetItem *item)
{
    if (item) {
        ItemData d = item->data(Qt::UserRole).value<ItemData>();
        if (d.first) {
            emit showPage(d.first, d.second);
        }
    }
}
