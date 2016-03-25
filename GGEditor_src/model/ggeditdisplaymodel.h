#ifndef GGEDITDISPLAYMODEL_H
#define GGEDITDISPLAYMODEL_H

#include <model/ggabstractdisplaymodel.h>

class GGEditDisplayModel : public GGAbstractDisplayModel
{
public:
    GGEditDisplayModel();

    void setResolver(GGAbstractMediaResolver *resolver);

    GGAbstractMediaResolver *getMediaResolver();
    GGStyler *getStyler();

private:
    GGAbstractMediaResolver *m_resolver;

};

#endif // GGEDITDISPLAYMODEL_H
