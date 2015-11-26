#include "ggcondition.h"

GGCondition::GGCondition(Type type, QString varName, QString value)
    : m_type(type),
      m_variableName(varName),
      m_value(value)
{

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

