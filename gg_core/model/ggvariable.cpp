#include "ggvariable.h"
#include <QHash>

GGVariable::GGVariable(QString name, QString initValue, Type type)
    : m_name(name),
      m_value(initValue),
      m_type(type),
      m_initValue(initValue)
{

}

bool GGVariable::isValid() const
{
    return !m_name.isEmpty();
}

void GGVariable::reinitialize(bool force)
{
    if (m_type == Transient || force) {
        m_value = m_initValue;
    }
}

QString GGVariable::name() const
{
    return m_name;
}

void GGVariable::setName(const QString &name)
{
    // Sanitize name
    m_name = sanitizeName(name);
}

QString GGVariable::value() const
{
    return m_value;
}

void GGVariable::setValue(const QString &value)
{
    m_value = value;
}

GGVariable::Type GGVariable::type() const
{
    return m_type;
}

void GGVariable::setType(GGVariable::Type type)
{
    m_type = type;
}

QString GGVariable::initValue() const
{
    return m_initValue;
}

void GGVariable::setInitValue(const QString &initValue)
{
    m_initValue = initValue;
}

QString GGVariable::sanitizeName(QString name)
{
    name = name.trimmed();
    name.remove(QRegExp("[^\\d_\\w]"));
    if (name.isEmpty()) name = "var";
    if (name.at(0).isDigit()) name.prepend('_');
    return name;
}

bool operator ==(const GGVariable &v1, const GGVariable &v2)
{
    return v1.name() == v2.name();
}

uint qHash(const GGVariable &v)
{
    return qHash(v.name());
}
