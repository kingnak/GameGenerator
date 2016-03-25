#include "ggeditdisplaymodel.h"

GGEditDisplayModel::GGEditDisplayModel()
    : m_resolver(NULL)
{

}

void GGEditDisplayModel::setResolver(GGAbstractMediaResolver *resolver)
{
    m_resolver = resolver;
}

GGAbstractMediaResolver *GGEditDisplayModel::getMediaResolver()
{
    return m_resolver;
}

GGStyler *GGEditDisplayModel::getStyler()
{
    return NULL;
}

