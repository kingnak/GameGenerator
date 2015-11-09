#include "ggmodelconnectioncommands.h"
#include <model/ggeditmodel.h>
#include <model/ggabstractfactory.h>
#include <model/ggconnection.h>
#include <model/ggpage.h>

GGCreateConnectionCmd::GGCreateConnectionCmd(GGEditModel *model, GGConnectionSlot slot, GGPage *src, GGPage *dest, Type type)
:   GGAbstractModelCommand(model),
    m_createdConn(NULL),
    m_type(type),
    m_slot(slot),
    m_src(src),
    m_dest(dest)
{

}

GGCreateConnectionCmd::~GGCreateConnectionCmd()
{
    if (m_state != Executed) {
        delete m_createdConn;
    }
}

QString GGCreateConnectionCmd::description() const
{
    return "Create Connection";
}

GGConnection *GGCreateConnectionCmd::createdConnection()
{
    return m_createdConn;
}

bool GGCreateConnectionCmd::doExecute()
{
    Q_ASSERT(m_src && m_src->model() == m_model);
    Q_ASSERT(m_dest && m_dest->model() == m_model);

    if (m_type == Connection) {
        m_createdConn = m_model->factory()->createConnection(m_src->id(), m_dest->id());
    } else if (m_type == Link) {
        m_createdConn = m_model->factory()->createLink(m_src->id(), m_dest->id());
    } else {
        Q_ASSERT_X(false, "GGCreateConnectionCmd::doExecute", "Unknown Connection Type");
        return setError("INTERNAL ERROR");
    }

    if (!m_model->registerNewConnection(m_createdConn)) {
        return setError("Cannot register connection");
    }

    if (!m_slot.connect(m_src, m_createdConn)) {
        if (!m_model->unregisterConnection(m_createdConn->id())) {
            Q_ASSERT_X(false, "GGCreateConnectionCmd::doExecute", "Cannot unregister connection after failing to set connection slot");
        }
        return setError("Cannot set Connection in source page");
    }

    return true;
}

bool GGCreateConnectionCmd::doUndo()
{
    if (!m_model->unregisterConnection(m_createdConn->id())) {
        return setError("Cannot unregister connection");
    }
    return true;
}

bool GGCreateConnectionCmd::doRedo()
{
    if (!m_model->registerConnectionWithId(m_createdConn)) {
        return setError("Cannot re-register connection");
    }

    if (!m_slot.connect(m_src, m_createdConn)) {
        if (!m_model->unregisterConnection(m_createdConn->id())) {
            Q_ASSERT_X(false, "GGCreateConnectionCmd::doExecute", "Cannot unregister connection after failing to set connection slot");
        }
        return setError("Cannot set Connection in source page");
    }

    return true;
}

///////////////////////////

GGDeleteConnectionCmd::GGDeleteConnectionCmd(GGEditModel *model, GGConnection *conn)
    : GGAbstractModelCommand(model), m_deletedConn(conn), m_slot(GGConnectionSlot::NoConnection)
{

}

GGDeleteConnectionCmd::~GGDeleteConnectionCmd()
{
    if (m_state == Executed) {
        delete m_deletedConn;
    }
}

QString GGDeleteConnectionCmd::description() const
{
    return "Delete Connection";
}

GGConnection *GGDeleteConnectionCmd::deletedConnection()
{
    return m_deletedConn;
}

bool GGDeleteConnectionCmd::doExecute()
{
    m_slot = GGConnectionSlot::findConnection(m_deletedConn->source(), m_deletedConn);
    Q_ASSERT(m_slot.type() != GGConnectionSlot::NoConnection);
    if (m_slot.type() == GGConnectionSlot::NoConnection) {
        return setError("Connection is not in the source page");
    }

    if (!m_model->unregisterConnection(m_deletedConn->id())) {
        return setError("Cannot unregister connection");
    }

    return true;
}

bool GGDeleteConnectionCmd::doUndo()
{
    if (!m_model->registerConnectionWithId(m_deletedConn)) {
        return setError("Cannot re-register connection");
    }

    if (!m_slot.connect(m_deletedConn->source(), m_deletedConn)) {
        if (!m_model->unregisterConnection(m_deletedConn->id())) {
            Q_ASSERT_X(false, "GGDeleteConnectionCmd::doUndo", "Cannot unregister connection after failing to set connection slot");
        }
        return setError("Cannot set Connection in source page");
    }

    return true;
}

bool GGDeleteConnectionCmd::doRedo()
{
    if (!m_model->unregisterConnection(m_deletedConn->id())) {
        return setError("Cannot unregister connection");
    }

    return true;
}

