#ifndef GGABSTRACTSTYLER_H
#define GGABSTRACTSTYLER_H

#include "gg_core_global.h"
#include "ggstyle.h"
#include <QList>

class QTextDocument;

class GG_CORESHARED_EXPORT GGAbstractStyler
{
public:
    GGAbstractStyler();
    virtual ~GGAbstractStyler() {}


    GGBasicStyle basicStyle() const;
    void setBasicStyle(const GGBasicStyle &basicStyle);

    QList<GGStyle> styles() const;
    bool addStyle(const GGStyle &style);
    bool removeStyle(const QString &name);

    virtual QTextDocument *styleString(const QString &doc) const = 0;
    virtual QString parseDocument(const QTextDocument *doc) const = 0;
    virtual QString plainText(const QString &doc) const = 0;

protected:
    GGBasicStyle m_basic;
    QList<GGStyle> m_styles;
};

#endif // GGABSTRACTSTYLER_H
