#include "ggbasicprojectserializer.h"
#include <io/ggserialization.hpp>
#include <io/ggserializationprocessor.h>
#include <io/ggabstractserializationwriter.h>
#include <model/ggeditproject.h>
#include <model/ggeditmodel.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggcontentelement.h>
#include <QVariantMap>

class DummyProcessor : public GGSerializationProcessor
{
public:
    virtual bool processProject(QVariant &) { return true; }
    virtual bool processScene(QVariant &) { return true; }
    virtual bool processPage(QVariant &) { return true; }
    virtual bool processConnection(QVariant &) { return true; }
    virtual bool processConnectionRef(QVariant &) { return true; }
    virtual bool processMediaRef(QVariant &) { return true; }
    virtual bool processContentElement(QVariant &) { return true; }
    virtual bool processMappedLink(QVariant &) { return true; }
    virtual bool processLink(QVariant &) { return true; }
};

GGBasicProjectSerializer::GGBasicProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor)
    : m_writer(writer),
      m_processor(processor)
{
    if (!processor) {
        m_processor = new DummyProcessor;
    }
}

GGBasicProjectSerializer::~GGBasicProjectSerializer()
{
    delete m_processor;
}

bool GGBasicProjectSerializer::saveProject(GGEditProject *project)
{
    bool ok = serializeProject(project);
    if (!ok) return false;
    foreach (GGScene *s, project->editModel()->getScenes()) {
        ok = serializeScene(s);
        if (!ok) return false;
        QList<GGConnection*> conns;
        foreach (GGPage *p, s->pages()) {
            ok = serializePage(p);
            if (!ok) return false;
            conns += p->getConnections();
        }
        foreach (GGConnection *c, conns) {
            ok = serializeConnection(c);
            if (!ok) return false;
        }
    }
    return true;
}

bool GGBasicProjectSerializer::serializeProject(GGEditProject *project)
{
    bool ok = true;
    QVariantMap m;
    m["title"] << project->title();
    // TODO: Variables
    QVariant v;
    v << m;
    ok &= m_processor->processProject(v);
    ok &= m_writer->writeProject(v);
    return ok;
}

bool GGBasicProjectSerializer::serializeScene(GGScene *scene)
{
    bool ok = true;
    QVariantMap m;
    m["id"] << scene->id();
    m["name"] << scene->name();
    m["mediaDir"] << scene->mediaDir();
    QVariant v;
    v << m;
    ok &= m_processor->processScene(v);
    ok &= m_writer->writeScene(v);
    return ok;
}

bool GGBasicProjectSerializer::serializePage(GGPage *page)
{
    bool ok = true;
    QVariantMap m;
    m["id"] << page->id();
    m["type"] << page->type();
    m["name"] << page->name();

    if (GGConditionPage *cp = GG::as<GGConditionPage>(page)) {
        m["condition"] << this->serializeCondition(cp->getCondition());
        m["true"] << cp->trueConnection()->id();
        ok &= m_processor->processConnectionRef(m["true"]);
        m["false"] << cp->falseConnection()->id();
        ok &= m_processor->processConnectionRef(m["false"]);
    }

    if (GGContentPage *cp = GG::as<GGContentPage>(page)) {
        m["caption"] << cp->caption();
        m["content"] = this->serializeContent(cp->content());
        ok &= m_processor->processContentElement(m["content"]);
    }

    if (GGStartPage *sp = GG::as<GGStartPage>(page)) {
        m["start"] << sp->startConnection()->id();
        ok &= m_processor->processConnectionRef(m["start"]);
    }

    if (GGEndPage *ep = GG::as<GGEndPage>(page)) {
        // Nothing to do
        Q_UNUSED(ep);
    }

    if (GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(page)) {
        QVariantMap map;
        for (int i = 0; i < mcp->getLinkMap().size(); ++i) {
            GGMappedLink l = mcp->getLinkMap()[i];
            QVariant v = this->serializeMappedLink(l);
            ok &= m_processor->processMappedLink(v);
            map[QString::number(i)] = v;
        }
        m["map"] << map;
    }

    if (GGDecisionPage *dp = GG::as<GGDecisionPage>(page)) {
        QVariantMap map;
        for (int i = 0; i < dp->getDecisionLinks().size(); ++i) {
            GGLink l = dp->getDecisionLinks()[i];
            QVariant v = this->serializeLink(l);
            ok &= m_processor->processLink(v);
            map[QString::number(i)] = v;
        }
        m["decisions"] << map;
    }

    QVariant v;
    v << m;
    ok &= m_processor->processPage(v);
    ok &= m_writer->writePage(v);
    return ok;
}

bool GGBasicProjectSerializer::serializeConnection(GGConnection *connection)
{
    bool ok = true;
    QVariantMap m;
    m["id"] << connection->id();
    m["destination"] << connection->destination()->id();
    QVariant v;
    v << m;
    ok &= m_processor->processConnection(v);
    ok &= m_writer->writeConnection(v);
    return ok;
}

QVariant GGBasicProjectSerializer::serializeContent(GGContentElement *elem)
{
    QVariantMap m;
    if (GGImageContent *ic = dynamic_cast<GGImageContent*>(elem)) {
        m["type"] << QString("image");
        m["media"] << ic->imageFilePath();
        m_processor->processMediaRef(m["media"]);
    } else if (GGTextContent *tc = dynamic_cast<GGTextContent*>(elem)) {
        m["type"] << QString("text");
        m["text"] << tc->textContent();
    }

    QVariant v;
    v << m;
    m_processor->processContentElement(v);
    return v;
}

QVariant GGBasicProjectSerializer::serializeMappedLink(const GGMappedLink &link)
{
    QVariantMap m;
    m["type"] << (quint32) link.type();
    switch (link.type()) {
    case GGMappedLink::Rectangle:
        m["rect"] << link.rectangle();
        break;
    case GGMappedLink::Ellipse:
        m["ellipse"] << link.ellipse();
        break;
    }

    m["link"] << this->serializeLink(link.link());

    QVariant v;
    v << m;
    return v;
}

QVariant GGBasicProjectSerializer::serializeLink(const GGLink &link)
{
    QVariantMap m;

    m["name"] << link.name();
    m["connection"] << link.connection()->id();
    m_processor->processConnectionRef(m["connection"]);
    m["action"] = this->serializeAction(link.action());

    QVariant v;
    v << m;
    return v;
}

QVariant GGBasicProjectSerializer::serializeCondition(const GGCondition &condition)
{
    QVariantMap m;

    m["operator"] << (quint32) condition.type();
    m["variable"] << condition.variableName();
    m["value"] << condition.value();

    QVariant v;
    v << m;
    return v;
}

QVariant GGBasicProjectSerializer::serializeAction(const GGAction &action)
{
    QVariantMap m;

    m["operator"] << (quint32) action.type();
    m["variable"] << action.variableName();
    m["value"] << action.value();

    QVariant v;
    v << m;
    return v;
}
