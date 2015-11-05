#include "ggconnection.h"

GGConnection::GGConnection(GG::PageID srcId, GG::PageID destId)
:   m_id(GG::InvalidConnectionId),
    m_srcId(srcId),
    m_destId(destId),
    m_src(NULL),
    m_dest(NULL),
    m_model(NULL)
{
}

GGConnection::~GGConnection()
{
    // Required??
    //if (m_model) m_model->unregisterConnection(m_id);
}
