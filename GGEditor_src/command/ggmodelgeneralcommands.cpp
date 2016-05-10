#include "ggmodelgeneralcommands.h"
#include <model/ggeditmodel.h>
#include <model/ggvariable.h>
#include <style/ggabstractstyler.h>

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

////////////////////

GGSetBasicStyleCmd::GGSetBasicStyleCmd(GGEditModel *model, const GGBasicStyle &style)
    : GGAbstractModelCommand(model),
      m_new(style)
{

}

QString GGSetBasicStyleCmd::description() const
{
    return "Update basic style";
}

bool GGSetBasicStyleCmd::doExecute()
{
    m_old = m_model->getStyler()->basicStyle();
    m_model->getStyler()->setBasicStyle(m_new);
    return true;
}

bool GGSetBasicStyleCmd::doUndo()
{
    m_model->getStyler()->setBasicStyle(m_old);
    return true;
}

bool GGSetBasicStyleCmd::doRedo()
{
    m_model->getStyler()->setBasicStyle(m_new);
    return true;
}

////////////////////

GGAddStyleCmd::GGAddStyleCmd(GGEditModel *model, const GGStyle &style)
    : GGAbstractModelCommand(model),
      m_style(style)
{

}

QString GGAddStyleCmd::description() const
{
    return "Add style";
}

bool GGAddStyleCmd::doExecute()
{
    if (!m_model->getStyler()->addStyle(m_style)) {
        return false;
    }
    return true;
}

bool GGAddStyleCmd::doUndo()
{
    if (!m_model->getStyler()->removeStyle(m_style.name())) {
        return false;
    }
    return true;
}

bool GGAddStyleCmd::doRedo()
{
    return doExecute();
}

////////////////////


GGRemoveStyleCmd::GGRemoveStyleCmd(GGEditModel *model, const QString &style)
    : GGAbstractModelCommand(model),
      m_name(style)
{

}

QString GGRemoveStyleCmd::description() const
{
    return "Remove style";
}

bool GGRemoveStyleCmd::doExecute()
{
    bool ok = false;
    foreach (GGStyle s, m_model->getStyler()->styles()) {
        if (s.name() == m_name) {
            m_style = s;
            ok = true;
            break;
        }
    }
    if (!ok) return false;
    return doRedo();
}

bool GGRemoveStyleCmd::doUndo()
{
    return m_model->getStyler()->addStyle(m_style);
}

bool GGRemoveStyleCmd::doRedo()
{
    return m_model->getStyler()->removeStyle(m_name);
}
