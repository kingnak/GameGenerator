#include "ggeditcontentelementwidget.h"
#include "ui_ggeditcontentelementwidget.h"
#include <model/ggcontentelement.h>
#include <model/ggscenemediamanager.h>
#include <model/ggmediaresolver.h>
#include <model/ggscene.h>
#include <ui/dialogs/ggmediamanagerdialog.h>

#define IMAGE_ID_PROPERTY "IMAGE_ID"

GGEditContentElementWidget::GGEditContentElementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGEditContentElementWidget),
    m_manager(NULL),
    m_curScene(NULL)
{
    ui->setupUi(this);
    ui->radImg->setChecked(true);
}

GGEditContentElementWidget::~GGEditContentElementWidget()
{
    delete ui;
}

void GGEditContentElementWidget::setManager(GGSceneMediaManager *manager)
{
    m_manager = manager;
}

GGContentElement *GGEditContentElementWidget::getContentElement()
{
    if (ui->radText->isChecked()) {
        GGTextContent *t = new GGTextContent;
        t->setTextContent(ui->txtText->document()->toHtml());
        return t;
    } else if (ui->radImg->isChecked()) {
        GGImageContent *i = new GGImageContent;
        i->setImageFilePath(ui->txtPathImage->property(IMAGE_ID_PROPERTY).toString());
        return i;
    }

    return NULL;
}

void GGEditContentElementWidget::setContentElement(GGContentElement *elem, GGScene *scene)
{
    if (elem) {
        if (GGImageContent *i = dynamic_cast<GGImageContent*>(elem)) {
            ui->radImg->setChecked(true);
            QString path = m_manager->resolver()->resolveName(i->imageFilePath());
            ui->txtPathImage->setText(path);
            ui->txtPathImage->setProperty(IMAGE_ID_PROPERTY, i->imageFilePath());
            loadPreviewImage(i->imageFilePath(), m_manager->resolver());
        } else if (GGTextContent *t = dynamic_cast<GGTextContent*>(elem)) {
            ui->radText->setChecked(true);
            QTextDocument *doc = new QTextDocument;
            doc->setHtml(t->textContent());
            ui->txtText->setDocument(doc);
        }
    } else {
        ui->radImg->setChecked(true);
    }
    m_curScene = scene;
    changePage();
}

void GGEditContentElementWidget::on_btnBrowseImage_clicked()
{
    GGMediaManagerDialog dlg(m_manager);
    if (m_curScene) {
        QString baseDir = m_curScene->mediaDir();
        if (ui->radAnim->isChecked() || ui->radImg->isChecked()) {
            baseDir += "/" + GGSceneMediaManager::PATH_IMAGE;
        } else if (ui->radVideo->isChecked()) {
            baseDir += "/" + GGSceneMediaManager::PATH_VIDEO;
        }
        dlg.setSelectedDirectory(baseDir);
    }

    if (dlg.exec() == QDialog::Accepted) {
        QString id = dlg.getSelectedMediaId();
        QString path = m_manager->resolver()->resolveName(id);
        ui->txtPathImage->setText(path);
        ui->txtPathImage->setProperty(IMAGE_ID_PROPERTY, id);
        loadPreviewImage(id, m_manager->resolver());
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
