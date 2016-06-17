#ifndef GGPROJECT_H
#define GGPROJECT_H

#include <gg_core_global.h>
#include <QString>

class GGAbstractModel;
class GGAbstractMediaResolver;

class GG_CORESHARED_EXPORT GGProject
{
public:
    GGProject() {}
    virtual ~GGProject() {}

    void setTitle(const QString &title) { m_title = title; }
    QString title() const { return m_title; }

    virtual GGAbstractModel *model() = 0;
    virtual const GGAbstractModel *model() const = 0;
    virtual GGAbstractMediaResolver *resolver() = 0;
    virtual const GGAbstractMediaResolver *resolver() const = 0;

protected:
    QString m_title;
};

#endif // GGPROJECT_H
