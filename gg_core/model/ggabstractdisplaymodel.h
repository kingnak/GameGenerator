#ifndef GGABSTRACTDISPLAYMODEL_H
#define GGABSTRACTDISPLAYMODEL_H

#include <gg_core_global.h>

class GGAbstractMediaResolver;
class GGStyler;

class GG_CORESHARED_EXPORT GGAbstractDisplayModel
{
public:
    virtual ~GGAbstractDisplayModel() {}

    virtual GGAbstractMediaResolver *getMediaResolver() = 0;
    virtual GGStyler *getStyler() = 0;
};

#endif // GGABSTRACTDISPLAYMODEL_H
