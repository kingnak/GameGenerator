#ifndef GGVARIABLE_H
#define GGVARIABLE_H

#include <gg_core_global.h>
#include <QString>

class GG_CORESHARED_EXPORT GGVariable
{
public:
    enum Type {
        Transient, Persistent
    };

    GGVariable() : m_type(Transient) {}
    GGVariable(QString name, QString initValue = QString::null, Type type = Transient);

    bool isValid() const;
    void reinitialize(bool force = false);

    QString name() const;
    void setName(const QString &name);

    QString value() const;
    void setValue(const QString &value);

    Type type() const;
    void setType(Type type);

    QString initValue() const;
    void setInitValue(const QString &initValue);

    static QString sanitizeName(QString name);

private:
    QString m_name;
    QString m_value;
    Type m_type;
    QString m_initValue;
};

bool operator ==(const GGVariable &v1, const GGVariable &v2);
uint qHash(const GGVariable &v);

#endif // GGVARIABLE_H
