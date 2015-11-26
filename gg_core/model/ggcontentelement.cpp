#include "ggcontentelement.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QTextDocument>

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

QPixmap GGTextContent::preview(QSize sz) const
{
    QPixmap ret(sz);
    ret.fill(Qt::transparent);
    QPainter p(&ret);
    QTextDocument doc;
    doc.setHtml(m_textContent);
    doc.drawContents(&p, QRect(QPoint(), sz));
    //p.drawText(QRect(QPoint(), sz), m_textContent);
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

QPixmap GGImageContent::preview(QSize sz) const
{
    QImage img(m_imageFilePath);
    return QPixmap::fromImage(img.scaled(sz, Qt::KeepAspectRatio));
}
