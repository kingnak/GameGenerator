#include "ggmodelverifier.h"
#include <model/ggabstractmodel.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggconnectionslot.h>
#include <model/ggcontentelement.h>
#include <model/ggmediaresolver.h>

GGModelErrorList GGModelErrorList::filtered(GGModelError::Severity level, bool andHigher) const
{
    GGModelErrorList ret;
    foreach (GGModelError e, *this) {
        if (e.severity() == level) {
            ret << e;
        } else if (andHigher) {
            if (e.severity() > level) {
                ret << e;
            }
        }
    }
    return ret;
}


const GGModelErrorList GGModelVerifier::NoError;

GGModelVerifier::GGModelVerifier()
    : m_model(NULL),
      m_hasEndPage(false)
{

}

GGModelVerifier::~GGModelVerifier()
{

}

GGModelErrorList GGModelVerifier::verify(const GGAbstractModel *model)
{
    clear();
    m_model = model;
    GGModelErrorList errs;
    QStringList vars = model->variableNames();

    foreach (GGPage *p, model->getPages()) {
        errs += checkPageName(p);
        errs += checkStartPage(p);
        errs += checkEndPage(p);
        errs += checkDeadEnd(p);
        errs += checkLooseConnection(p);
        errs += checkVariables(p, vars);
        errs += checkCondition(p);
        errs += checkLinkTitles(p);
        errs += checkContent(p);
    }

    errs += postCheckStartPage();
    errs += postCheckEndPage();
    errs += postCheckUnreachable();

    m_model = NULL;
    return errs;
}

void GGModelVerifier::clear()
{
    m_model = NULL;
    m_uniquePageNames.clear();
    m_startPages.clear();
    m_hasEndPage = false;
}

const GGAbstractModel *GGModelVerifier::model()
{
    return m_model;
}

GGModelErrorList GGModelVerifier::checkPageName(const GGPage *page)
{
    QString pn = page->scene()->name() + "::" + page->name();
    if (m_uniquePageNames.contains(pn)) {
        return GGModelError(GGModelError::Error, QString("Duplicate page name: ") + page->name(), page);
    } else {
        m_uniquePageNames << pn;
        return NoError;
    }
}

GGModelErrorList GGModelVerifier::checkStartPage(const GGPage *page)
{
    if (GG::as<const GGStartPage>(page)) {
        if (!m_startPages.isEmpty()) {
            return GGModelError(GGModelError::Fatal, "Multiple start pages", page);
        }
        m_startPages << page;
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::checkEndPage(const GGPage *page)
{
    if (GG::as<const GGEndPage>(page)) {
        m_hasEndPage = true;
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::checkDeadEnd(const GGPage *page)
{
    if (!GG::as<const GGEndPage>(page)) {
        QList<GGConnectionSlotData> slts = GGConnectionSlotData::enumerateConnections(page);
        if (slts.isEmpty()) {
            return GGModelError(GGModelError::Warning, "Dead End", page);
        }
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::checkLooseConnection(const GGPage *page)
{
    GGModelErrorList errors;
    QList<GGConnectionSlotData> slts = GGConnectionSlotData::enumerateConnections(page);
    foreach (GGConnectionSlotData s, slts) {
        GGConnectionSlot sl(s);
        if (sl.getExistingConnection(page) == NULL) {
            errors << GGModelError(GGModelError::Warning, "Unconnected Link", page, NULL, s);
        }
    }
    return errors;
}

GGModelErrorList GGModelVerifier::checkVariables(const GGPage *page, const QStringList &validVariables)
{
    GGModelErrorList errors;
    QList<GGConnectionSlotData> slts = GGConnectionSlotData::enumerateConnections(page);
    foreach (GGConnectionSlotData s, slts) {
        if (s.isLink()) {
            GGConnectionSlot sl(s);
            GGLink l = sl.getLink(page);
            if (l.action().isValid()) {
                if (!validVariables.contains(l.action().variableName())) {
                    errors << GGModelError(GGModelError::Error, QString("Undefined variable in link: ") + l.action().variableName(), page, NULL, s);
                }
            }
        }
    }

    if (const GGConditionPage *cp = GG::as<const GGConditionPage>(page)) {
        if (cp->getCondition().isValid()) {
            if (!validVariables.contains(cp->getCondition().variableName())) {
                errors << GGModelError(GGModelError::Error, QString("Undefined variable in condition: ") + cp->getCondition().variableName(), page);
            }
        }
    }

    if (const GGEntryActionPage *eap = GG::as<const GGEntryActionPage>(page)) {
        if (eap->entryAction().isValid()) {
            if (!validVariables.contains(eap->entryAction().variableName())) {
                errors << GGModelError(GGModelError::Error, QString("Undefined variable in entry action: ") + eap->entryAction().variableName(), page);
            }
        }
    }

    return errors;
}

GGModelErrorList GGModelVerifier::checkCondition(const GGPage *page)
{
    if (const GGConditionPage *cp = GG::as<const GGConditionPage>(page)) {
        if (!cp->getCondition().isValid()) {
            return GGModelError(GGModelError::Fatal, "No condition set", page);
        }
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::checkLinkTitles(const GGPage *page)
{
    GGModelErrorList errors;
    QList<GGConnectionSlotData> slts = GGConnectionSlotData::enumerateConnections(page);
    foreach (GGConnectionSlotData s, slts) {
        if (s.isLink()) {
            GGConnectionSlot sl(s);
            GGLink l = sl.getLink(page);
            if (l.name().isEmpty()) {
                errors << GGModelError(GGModelError::Warning, "Link has no title", page, NULL, s);
            }
        }
    }
    return errors;
}

GGModelErrorList GGModelVerifier::checkContent(const GGPage *page)
{
    GGModelErrorList errors;
    if (const GGContentPage *cp = GG::as<const GGContentPage>(page)) {
        if (cp->content()) {
            if (const GGImageContent *ic = dynamic_cast<const GGImageContent*> (cp->content())) {
                if (!model()->mediaResolver()->isValid(ic->imageFilePath())) {
                    errors << GGModelError(GGModelError::Error, "Media not found", page);
                }
            }
        } else {
            errors << GGModelError(GGModelError::Error, "Page has no content", page);
        }
    }
    return errors;
}

GGModelErrorList GGModelVerifier::postCheckStartPage()
{
    if (m_startPages.isEmpty()) {
        return GGModelError(GGModelError::Fatal, "No start page");
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::postCheckEndPage()
{
    if (!m_hasEndPage) {
        return GGModelError(GGModelError::Notice, "No end page");
    }
    return NoError;
}

GGModelErrorList GGModelVerifier::postCheckUnreachable()
{
    if (m_startPages.isEmpty()) {
        return GGModelError(GGModelError::Fatal, "No start pages");
    }

    const GGPage *cur = m_startPages.first();
    QSet<const GGPage*> visited;
    recursiveCheckUnreachable(cur, visited);

    QList<GGPage*> allList = model()->getPages();
    QSet<const GGPage*> all;
    foreach (GGPage *p, allList) all.insert(p);

    QSet<const GGPage*> diff = all - visited;
    if (!diff.isEmpty()) {
        const GGPage *p = *diff.begin();
        return GGModelError(GGModelError::Error, "Unreachable page", p);
    }
    return NoError;
}

void GGModelVerifier::recursiveCheckUnreachable(const GGPage *page, QSet<const GGPage *> &visited)
{
    if (visited.contains(page)) return;
    visited.insert(page);
    foreach (GGConnection *c, page->getConnections()) {
        recursiveCheckUnreachable(c->destination(), visited);
    }
}
