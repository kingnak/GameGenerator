#include "ggaction.h"

GGAction::GGAction(Type type, QString varName, QString value)
    : m_type(type),
      m_variableName(varName),
      m_value(value)
{

}

bool GGAction::isValid() const
{
    return m_type != None && !m_variableName.isEmpty();
}

GGAction::Type GGAction::type() const
{
    return m_type;
}

void GGAction::setType(const Type &type)
{
    m_type = type;
}

QString GGAction::variableName() const
{
    return m_variableName;
}

void GGAction::setVariableName(const QString &variableName)
{
    m_variableName = variableName;
}

QString GGAction::value() const
{
    return m_value;
}

void GGAction::setValue(const QString &value)
{
    m_value = value;
}
