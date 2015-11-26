#include "ggeditcontentelementdialog.h"
#include "ui_ggeditcontentelementdialog.h"
#include <model/ggcontentelement.h>
#include <QFileDialog>

GGEditContentElementDialog::GGEditContentElementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditContentElementDialog)
{
    ui->setupUi(this);
    ui->radImg->setChecked(true);
}

GGEditContentElementDialog::~GGEditContentElementDialog()
{
    delete ui;
}

GGContentElement *GGEditContentElementDialog::getContentElement()
{
    if (ui->radText->isChecked()) {
        GGTextContent *t = new GGTextContent;
        t->setTextContent(ui->txtText->document()->toHtml());
        return t;
    } else if (ui->radImg->isChecked()) {
        GGImageContent *i = new GGImageContent;
        i->setImageFilePath(ui->txtPathImage->text());
        return i;
    }

    return NULL;
}

void GGEditContentElementDialog::setContentElement(GGContentElement *elem)
{
    if (elem) {
        if (GGImageContent *i = dynamic_cast<GGImageContent*>(elem)) {
            ui->radImg->setChecked(true);
            ui->txtPathImage->setText(i->imageFilePath());
            loadImagePreview();
        } else if (GGTextContent *t = dynamic_cast<GGTextContent*>(elem)) {
            ui->radText->setChecked(true);
            QTextDocument *doc = new QTextDocument;
            doc->setHtml(t->textContent());
            ui->txtText->setDocument(doc);
        }
    } else {
        ui->radImg->setChecked(true);
    }
    changePage();
}

void GGEditContentElementDialog::on_btnBrowseImage_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select image"), QString(), tr("Images (*.png *.gif *.jpg *.jpeg)"));
    if (!path.isNull()) {
        ui->txtPathImage->setText(path);
        loadImagePreview();
    }
}

void GGEditContentElementDialog::loadImagePreview()
{
    QPixmap p;
    p.load(ui->txtPathImage->text());
    ui->lblPreviewImage->setPixmap(p);
}

void GGEditContentElementDialog::changePage()
{
    if (ui->radText->isChecked()) {
        ui->stkPages->setCurrentWidget(ui->pageText);
    } else if (ui->radImg->isChecked()) {
        ui->stkPages->setCurrentWidget(ui->pageImage);
    }
}
