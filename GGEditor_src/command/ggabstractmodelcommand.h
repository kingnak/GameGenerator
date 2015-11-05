#ifndef GGABSTRACTMODELCOMMAND_H
#define GGABSTRACTMODELCOMMAND_H

#include "ggabstractcommand.h"

class GGEditModel;

class GGAbstractModelCommand : public GGAbstractCommand
{
public:
    GGAbstractModelCommand(GGEditModel *model)
        : m_model(model) {}
protected:
    GGEditModel *m_model;
};

#endif // GGABSTRACTMODELCOMMAND_H

