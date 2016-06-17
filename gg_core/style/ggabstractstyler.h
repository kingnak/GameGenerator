#ifndef GGABSTRACTSTYLER_H
#define GGABSTRACTSTYLER_H

#include "gg_core_global.h"
#include "ggstyle.h"
#include <QList>

class QTextDocument;
class QTextFormat;

class GG_CORESHARED_EXPORT GGAbstractStyler
{
public:
    GGAbstractStyler();
    virtual ~GGAbstractStyler() {}

    enum SupplementalStyleOption {
        None    = 0x0000,
        Italic  = 0x0001
    };
    Q_DECLARE_FLAGS(SupplementalStyleOptions, SupplementalStyleOption)

    GGBasicStyle basicStyle() const;
    void setBasicStyle(const GGBasicStyle &basicStyle);

    QList<GGStyle> styles() const;
    bool addStyle(const GGStyle &style);
    bool removeStyle(const QString &name);

    virtual QTextDocument *styleString(const QString &doc) const = 0;
    virtual QString parseDocument(const QTextDocument *doc) const = 0;
    virtual QString plainText(const QString &doc) const = 0;
    virtual QString previewText(const QString &doc) const = 0;
    virtual QString htmlText(const QString &doc) const = 0;

    virtual SupplementalStyleOptions supplementalOptions() const = 0;

    virtual QTextFormat formatForBasicStyle() const = 0;
    virtual QTextFormat formatForDefaultStyle() const = 0;
    virtual QTextFormat formatForStyle(const GGStyle &style) const = 0;
    virtual QString styleNameForFormat(const QTextFormat &format) const = 0;

protected:
    GGBasicStyle m_basic;
    QList<GGStyle> m_styles;
};

#endif // GGABSTRACTSTYLER_H
