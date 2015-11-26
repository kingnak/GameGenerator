#include "gglink.h"

GGLink::GGLink()
    : m_connection(NULL)
{

}

GGAction GGLink::action() const
{
    return m_action;
}

void GGLink::setAction(const GGAction &action)
{
    m_action = action;
}

