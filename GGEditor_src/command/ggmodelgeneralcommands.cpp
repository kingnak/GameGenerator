#include "ggmodelgeneralcommands.h"
#include <model/ggeditmodel.h>
#include <model/ggvariable.h>

GGAddVariableCmd::GGAddVariableCmd(GGEditModel *model, const QString &name)
    : GGAbstractModelCommand(model),
      m_name(name)
{
}

QString GGAddVariableCmd::description() const
{
    return QString("Add Variable %1").arg(m_name);
}

bool GGAddVariableCmd::doExecute()
{
    return m_model->addVariable(GGVariable(m_name));
}

bool GGAddVariableCmd::doUndo()
{
    return m_model->removeVariable(GGVariable(m_name));
}

bool GGAddVariableCmd::doRedo()
{
    return doExecute();
}

////////////////////

GGRemoveVariableCmd::GGRemoveVariableCmd(GGEditModel *model, const QString &name)
    : GGAbstractModelCommand(model),
      m_name(name)
{

}

QString GGRemoveVariableCmd::description() const
{
    return QString("Remove Variable %1").arg(m_name);
}

bool GGRemoveVariableCmd::doExecute()
{
    m_old = m_model->variableByName(m_name);
    return m_model->removeVariable(GGVariable(m_name));
}

bool GGRemoveVariableCmd::doUndo()
{
    return m_model->addVariable(m_old);
}

bool GGRemoveVariableCmd::doRedo()
{
    return m_model->removeVariable(GGVariable(m_name));
}

////////////////////

GGChangeVariableCmd::GGChangeVariableCmd(GGEditModel *model, const QString &oldName, const GGVariable &var)
    : GGAbstractModelCommand(model),
      m_new(var)
{
    m_old = m_model->variableByName(oldName);
}

QString GGChangeVariableCmd::description() const
{
    return QString("Update variable %1").arg(m_old.name());
}

bool GGChangeVariableCmd::doExecute()
{
    return doExchange(m_old, m_new);
}

bool GGChangeVariableCmd::doUndo()
{
    return doExchange(m_new, m_old);
}

bool GGChangeVariableCmd::doRedo()
{
    return doExchange(m_old, m_new);
}

bool GGChangeVariableCmd::doExchange(const GGVariable &o, const GGVariable &n)
{
    if (o.name() != n.name()) {
        if (m_model->addVariable(n)) {
            return m_model->removeVariable(o);
        } else {
            return false;
        }
    } else {
        m_model->removeVariable(o);
        return m_model->addVariable(n);
    }
}
