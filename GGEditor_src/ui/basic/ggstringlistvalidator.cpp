#include "ggstringlistvalidator.h"

QValidator::State GGStringListValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    if (m_strings.contains(input)) {
        return Acceptable;
    }

    if (m_strings.indexOf(QRegExp(input + ".*")) >= 0) {
        return Intermediate;
    }

    return Invalid;
}
