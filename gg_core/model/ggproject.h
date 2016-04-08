#ifndef GGPROJECT_H
#define GGPROJECT_H

#include <gg_core_global.h>

class GGAbstractModel;
class GGAbstractMediaResolver;

class GG_CORESHARED_EXPORT GGProject
{
public:
    GGProject() {}
    virtual ~GGProject() {}

    virtual GGAbstractModel *model() = 0;
    virtual GGAbstractMediaResolver *resolver() = 0;
};

#endif // GGPROJECT_H
