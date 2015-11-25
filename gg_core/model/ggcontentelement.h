#ifndef GGCONTENTELEMENT_H
#define GGCONTENTELEMENT_H

#include "gg_core_global.h"
#include <QString>
#include <QSize>

class QPixmap;

class GG_CORESHARED_EXPORT GGContentElement
{
protected:
    GGContentElement() {}
public:
    virtual ~GGContentElement() {}

    virtual QPixmap preview(QSize sz) const = 0;

    // Maybe needed by Copy Page
    //virtual GGContentElement *clone() = 0;
};

///////////////////////////////////////////////

class GG_CORESHARED_EXPORT GGAudioContent : public GGContentElement
{
public:
    enum PlaybackMode {
        Once, Looped
    };

    QString audioFilePath() const;
    void setAudioFilePath(QString path);
    PlaybackMode playbackMode() const;
    void setPlaybackMode(PlaybackMode mode);

protected:
    GGAudioContent(PlaybackMode mode = Looped) : m_mode(mode) {}

protected:
    QString m_audioFilePath;
    PlaybackMode m_mode;
};

///////////////////////////////////////////////

class GG_CORESHARED_EXPORT GGTextContent : public GGAudioContent
{
public:
    GGTextContent() {}
    QString textContent() const;
    void setTextContent(QString content);

protected:
    QString m_textContent;
};

///////////////////////////////////////////////

class GG_CORESHARED_EXPORT GGImageContent : public GGContentElement
{
public:
    GGImageContent() {}
    QString imageFilePath() const;
    void setImageFilePath(QString path);

    QPixmap preview(QSize sz) const;

protected:
    QString m_imageFilePath;
};

///////////////////////////////////////////////

#endif // GGCONTENTELEMENT_H
