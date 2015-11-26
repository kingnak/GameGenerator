#ifndef GGACTION_H
#define GGACTION_H

#include <gg_core_global.h>
#include <QString>

class GG_CORESHARED_EXPORT GGAction
{
public:
    enum Type {
        None, Add, Sub, Multiply, Divide, Set, Unset
    };

    GGAction() : m_type(None) {}
    GGAction(Type type, QString varName, QString value);

    bool isValid() const;

    Type type() const;
    void setType(const Type &type);

    QString variableName() const;
    void setVariableName(const QString &variableName);

    QString value() const;
    void setValue(const QString &value);

private:
    Type m_type;
    QString m_variableName;
    QString m_value;
};

#endif // GGACTION_H
