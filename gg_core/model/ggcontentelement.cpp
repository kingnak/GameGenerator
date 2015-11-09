#include "ggcontentelement.h"


QString GGAudioContent::audioFilePath() const
{
    return m_audioFilePath;
}

void GGAudioContent::setAudioFilePath(QString path)
{
    m_audioFilePath = path;
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

///////////////////////////////////////////////

QString GGImageContent::imageFilePath() const
{
    return m_imageFilePath;
}

void GGImageContent::setImageFilePath(QString path)
{
    m_imageFilePath = path;
}
