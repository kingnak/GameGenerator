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

bool GGLink::match(const GGSearchRequest &req, GGSearchResult &results, GG::PageID pageId, GGConnectionSlotData asSlot) const
{
    bool res = false;
    if (req.what().testFlag(GGSearchRequest::LinkName) && req.matches(m_name)) {
        results << GGSearchResultItem(GGSearchResultItem::LinkName, GGSearchRequest::LinkName, m_name, pageId, asSlot);
        res = true;
    }
    if (req.what().testFlag(GGSearchRequest::Variable) && req.matches(m_action.variableName())) {
        results << GGSearchResultItem(GGSearchResultItem::Action, GGSearchRequest::Variable, m_action.toString(), pageId, asSlot);
        res = true;
    }
    return res;
}

