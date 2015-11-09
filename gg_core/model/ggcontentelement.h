#ifndef GGCONTENTELEMENT_H
#define GGCONTENTELEMENT_H

#include "gg_core_global.h"
#include <QString>

class GG_CORESHARED_EXPORT GGContentElement
{
protected:
    GGContentElement() {}
public:
    virtual ~GGContentElement() {}

    // Maybe needed by Copy Page
    //virtual GGContentElement *clone() = 0;
};

///////////////////////////////////////////////

class GG_CORESHARED_EXPORT GGAudioContent : public GGContentElement
{
protected:
    GGAudioContent() {}
public:
    QString audioFilePath() const;
    void setAudioFilePath(QString path);

protected:
    QString m_audioFilePath;
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

protected:
    QString m_imageFilePath;
};

///////////////////////////////////////////////

#endif // GGCONTENTELEMENT_H
