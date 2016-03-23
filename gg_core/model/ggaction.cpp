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

QString GGAction::toString() const
{
    QString val;
    if (isValid()) {
        if (m_type == GGAction::Unset) {
            val = QString("Unset %1").arg(m_variableName);
        } else {
            val = QString("%1 %2 %3").arg(m_variableName, getActionStringForType(m_type), m_value);
        }
    }
    return val;
}

QString GGAction::getActionStringForType(GGAction::Type type)
{
    switch (type) {
    case GGAction::None: return "";
    case GGAction::Add: return "+=";
    case GGAction::Sub: return "-=";
    case GGAction::Multiply: return "*=";
    case GGAction::Divide: return "/=";
    case GGAction::Set: return "=";
    case GGAction::Unset: return "X";
    }
    return "";
}
