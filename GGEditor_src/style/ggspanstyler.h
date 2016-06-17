#ifndef GGSPANSTYLER_H
#define GGSPANSTYLER_H

#include <style/ggabstractstyler.h>

class GGSpanStyler : public GGAbstractStyler
{
public:
    GGSpanStyler();

    virtual QTextDocument *styleString(const QString &doc) const;
    virtual QString parseDocument(const QTextDocument *doc) const;
    virtual QString plainText(const QString &doc) const;
    virtual QString previewText(const QString &doc) const;
    virtual QString htmlText(const QString &doc) const;

    virtual SupplementalStyleOptions supplementalOptions() const;

    virtual QTextFormat formatForBasicStyle() const;
    virtual QTextFormat formatForDefaultStyle() const;
    virtual QTextFormat formatForStyle(const GGStyle &style) const;

    virtual QString styleNameForFormat(const QTextFormat &format) const;

    QString sanitizeStyleName(const QString &name) const;
};

#endif // GGSPANSTYLER_H
