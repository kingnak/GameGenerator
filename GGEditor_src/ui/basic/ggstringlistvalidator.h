#ifndef GGSTRINGLISTVALIDATOR_H
#define GGSTRINGLISTVALIDATOR_H

#include <QValidator>

class GGStringListValidator : public QValidator {
public:
    GGStringListValidator(QObject *parent = NULL) : QValidator(parent) {}

    void setStrings(const QStringList &strings) { m_strings = strings; }

    State validate(QString &input, int &pos) const;

private:
    QStringList m_strings;
};

#endif // GGSTRINGLISTVALIDATOR_H
