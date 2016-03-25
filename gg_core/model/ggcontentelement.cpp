#include "ggcontentelement.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QTextDocument>
#include <model/ggmediaresolver.h>

QString GGAudioContent::audioFilePath() const
{
    return m_audioFilePath;
}

void GGAudioContent::setAudioFilePath(QString path)
{
    m_audioFilePath = path;
}

GGAudioContent::PlaybackMode GGAudioContent::playbackMode() const
{
    return m_mode;
}

void GGAudioContent::setPlaybackMode(GGAudioContent::PlaybackMode mode)
{
    m_mode = mode;
}

///////////////////////////////////////////////

QString GGTextContent::textContent() const
{
    return m_textContent;
}

void GGTextContent::setTextContent(QString content)
{
    m_textContent = content;
}

QPixmap GGTextContent::preview(GGAbstractMediaResolver *resolver, QSize sz) const
{
    Q_UNUSED(resolver);
    QRect bound;
    if (!sz.isEmpty()) {
        bound = QRect(QPoint(), sz);
    }
    QPixmap ret(sz);
    ret.fill(Qt::transparent);
    QPainter p(&ret);
    QTextDocument doc;
    doc.setHtml(m_textContent);
    doc.drawContents(&p, bound);
    return ret;
}

///////////////////////////////////////////////

QString GGImageContent::imageFilePath() const
{
    return m_imageFilePath;
}

void GGImageContent::setImageFilePath(QString path)
{
    m_imageFilePath = path;
}

QPixmap GGImageContent::preview(GGAbstractMediaResolver *resolver, QSize sz) const
{
    QImage img;
    QIODevice *data = resolver->resolve(m_imageFilePath);
    if (data) {
        img.load(data, NULL);
        delete data;
    }
    if (!sz.isEmpty()) {
        img = img.scaled(sz, Qt::KeepAspectRatio);
    }
    return QPixmap::fromImage(img);
}
