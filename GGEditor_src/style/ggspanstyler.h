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
};

#endif // GGSPANSTYLER_H
