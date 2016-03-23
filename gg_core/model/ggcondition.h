#ifndef GGCONDITION_H
#define GGCONDITION_H

#include <gg_core_global.h>
#include <QString>

class GG_CORESHARED_EXPORT GGCondition
{
public:
    enum Type {
        None, Equals, NotEquals, LessThen, LessEqual, GreaterThen, GreaterEquals
    };

    GGCondition() : m_type(None) {}
    GGCondition(Type type, QString varName, QString value);

    bool isValid() const;

    Type type() const;
    void setType(const Type &type);

    QString variableName() const;
    void setVariableName(const QString &variableName);

    QString value() const;
    void setValue(const QString &value);

    QString toString() const;

    static QString getConditionStringForType(Type type);

private:
    Type m_type;
    QString m_variableName;
    QString m_value;
};

#endif // GGCONDITION_H
