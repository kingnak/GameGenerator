#include "ggcondition.h"

GGCondition::GGCondition(Type type, QString varName, QString value)
    : m_type(type),
      m_variableName(varName),
      m_value(value)
{

}

bool GGCondition::isValid() const
{
    return m_type != None && !m_variableName.isEmpty();
}

GGCondition::Type GGCondition::type() const
{
    return m_type;
}

void GGCondition::setType(const Type &type)
{
    m_type = type;
}

QString GGCondition::variableName() const
{
    return m_variableName;
}

void GGCondition::setVariableName(const QString &variableName)
{
    m_variableName = variableName;
}

QString GGCondition::value() const
{
    return m_value;
}

void GGCondition::setValue(const QString &value)
{
    m_value = value;
}

QString GGCondition::toString() const
{
    if (isValid())
        return QString("%1 %2 %3").arg(m_variableName, getConditionStringForType(m_type), m_value);
    return "";
}

QString GGCondition::getConditionStringForType(GGCondition::Type type)
{
    switch (type) {
    case None: return "";
    case Equals: return "==";
    case NotEquals: return "!=";
    case LessThen: return "<";
    case LessEqual: return "<=";
    case GreaterThen: return ">";
    case GreaterEquals: return ">=";
    default: return "";
    }
}

