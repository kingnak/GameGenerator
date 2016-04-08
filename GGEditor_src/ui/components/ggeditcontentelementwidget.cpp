#include "ggeditcontentelementwidget.h"
#include "ui_ggeditcontentelementwidget.h"
#include <model/ggcontentelement.h>
#include <model/ggmediaresolver.h>
#include <QFileDialog>

GGEditContentElementWidget::GGEditContentElementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGEditContentElementWidget)
{
    ui->setupUi(this);
    ui->radImg->setChecked(true);
}

GGEditContentElementWidget::~GGEditContentElementWidget()
{
    delete ui;
}

GGContentElement *GGEditContentElementWidget::getContentElement()
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

void GGEditContentElementWidget::setContentElement(GGContentElement *elem, GGAbstractMediaResolver *resolver)
{
    if (elem) {
        if (GGImageContent *i = dynamic_cast<GGImageContent*>(elem)) {
            ui->radImg->setChecked(true);
            QString path = resolver->resolveName(i->imageFilePath());
            ui->txtPathImage->setText(path);
            loadPreviewImage(i->imageFilePath(), resolver);
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

void GGEditContentElementWidget::on_btnBrowseImage_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select image"), QString(), tr("Images (*.png *.gif *.jpg *.jpeg)"));
    if (!path.isNull()) {
        ui->txtPathImage->setText(path);
        // TODO: This should be done in MediaManager in future.
        // For now, use a local FileSystemResolver
        GGFileSystemResolver rev;
        loadPreviewImage(path, &rev);
    }
}

void GGEditContentElementWidget::loadPreviewImage(const QString &media, GGAbstractMediaResolver *resolver)
{
    QIODevice *dev = resolver->resolve(media);
    QPixmap p;
    if (dev) {
        QImage img;
        img.load(dev, resolver->resolveTypeHint(media).toUtf8());
        delete dev;
        p = QPixmap::fromImage(img);
    }

    ui->lblPreviewImage->setPixmap(p);
}

void GGEditContentElementWidget::changePage()
{
    if (ui->radText->isChecked()) {
        ui->stkPages->setCurrentWidget(ui->pageText);
    } else if (ui->radImg->isChecked()) {
        ui->stkPages->setCurrentWidget(ui->pageImage);
    }
}
