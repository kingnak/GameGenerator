#include "ggabstractstyler.h"

GGAbstractStyler::GGAbstractStyler()
{

}

GGBasicStyle GGAbstractStyler::basicStyle() const
{
    return m_basic;
}

void GGAbstractStyler::setBasicStyle(const GGBasicStyle &basic)
{
    m_basic = basic;
}

QList<GGStyle> GGAbstractStyler::styles() const
{
    return m_styles;
}

bool GGAbstractStyler::addStyle(const GGStyle &style)
{
    foreach (const GGStyle &s, m_styles) {
        if (s.name() == style.name()) {
            return false;
        }
    }

    m_styles << style;
    return true;
}

bool GGAbstractStyler::removeStyle(const QString &name)
{
    QList<GGStyle>::iterator it = m_styles.begin();
    for (; it != m_styles.end(); ++it) {
        if (it->name() == name) {
            m_styles.erase(it);
            return true;
        }
    }
    return false;
}
