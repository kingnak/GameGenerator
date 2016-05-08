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
#include <model/ggscenemediamanager.h>
#include <QVariantMap>

GGBasicProjectSerializer::GGBasicProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor)
    : m_writer(writer),
      m_processor(processor)
{
}

GGBasicProjectSerializer::~GGBasicProjectSerializer()
{
    delete m_processor;
    delete m_writer;
}

bool GGBasicProjectSerializer::saveProject(GGEditProject *project)
{
    bool ok = m_writer->writeHeader();
    if (!ok) return false;

    ok &= m_writer->writeProjectStart();
    ok &= serializeProject(project);
    if (!ok) return false;

    foreach (GGScene *s, project->editModel()->getScenes()) {
        ok &= m_writer->writeSceneStart();
        ok &= serializeScene(s);
        if (!ok) return false;

        ok &= m_writer->writePagesStart();
        QList<GGConnection*> conns;
        // Serialize all pages in this scene
        foreach (GGPage *p, s->pages()) {
            ok &= serializePage(p);
            if (!ok) return false;
            conns += p->getConnections();
        }
        ok &= m_writer->writePagesEnd();

        ok &= m_writer->writeConnectionsStart();
        // Serialize all connections of the pages in this scene
        foreach (GGConnection *c, conns) {
            ok &= serializeConnection(c);
            if (!ok) return false;
        }
        ok &= m_writer->writeConnectionsEnd();

        ok &= finalizeScene(s);

        ok &= m_writer->writeSceneEnd();
    }

    ok &= m_writer->writeProjectEnd();
    ok &= m_writer->writeFooter();
    return ok;
}

bool GGBasicProjectSerializer::serializeProject(GGEditProject *project)
{
    bool ok = true;
    QVariantMap m;
    m["title"] << project->title();

    QVariantList varList;
    foreach (GGVariable var, project->runtimeModel()->variables()) {
        QVariant v;
        ok &= this->serializeVariable(v, var);
        varList << v;
    }
    m["variable"] << varList;

    QVariantList mediaList;
    foreach (QString path, project->mediaManager()->allMedia()) {
        QVariant v;
        ok &= this->serializeMedia(v, path, project);
        mediaList << v;
    }
    m["media"] << mediaList;

    ok &= injectProjectData(project, m);
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
    ok &= injectSceneData(scene, m);
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
    m["scene"] << (quint32) page->sceneId();
    ok &= m_processor->processSceneRef(m["scene"]);

    if (GGConditionPage *cp = GG::as<GGConditionPage>(page)) {
        QVariant cond;
        ok &= this->serializeCondition(cond, cp->getCondition());
        m["condition"] = cond;
        if (cp->trueConnection()) {
            m["true"] << cp->trueConnection()->id();
            ok &= m_processor->processConnectionRef(m["true"]);
        }
        if (cp->falseConnection()) {
            m["false"] << cp->falseConnection()->id();
            ok &= m_processor->processConnectionRef(m["false"]);
        }
    }

    if (GGContentPage *cp = GG::as<GGContentPage>(page)) {
        m["caption"] << cp->caption();
        QVariant v;
        ok &= this->serializeContent(v, cp->content());
        m["content"] = v;
        ok &= m_processor->processContentElement(m["content"]);
    }

    if (GGStartPage *sp = GG::as<GGStartPage>(page)) {
        if (sp->startConnection()) {
            m["start"] << sp->startConnection()->id();
            ok &= m_processor->processConnectionRef(m["start"]);
        }
    }

    if (GGEndPage *ep = GG::as<GGEndPage>(page)) {
        // Nothing to do
        Q_UNUSED(ep);
    }

    if (GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(page)) {
        QVariantList lst;
        for (int i = 0; i < mcp->getLinkMap().size(); ++i) {
            GGMappedLink l = mcp->getLinkMap()[i];
            QVariant v;
            ok &= this->serializeMappedLink(v, l, i);
            ok &= m_processor->processMappedLink(v);
            lst << v;
        }
        m["map"] << lst;
    }

    if (GGActionPage *ap = GG::as<GGActionPage>(page)) {
        QVariant v;
        ok &= this->serializeLink(v, ap->actionLink());
        ok &= m_processor->processLink(v);
        m["action"] << v;
    }

    if (GGDecisionPage *dp = GG::as<GGDecisionPage>(page)) {
        QVariantList lst;
        for (int i = 0; i < dp->getDecisionLinks().size(); ++i) {
            GGLink l = dp->getDecisionLinks()[i];
            QVariant v;
            ok &= this->serializeDecisionLink(v, l, i);
            ok &= m_processor->processLink(v);
            lst << v;
        }
        m["decision"] << lst;
    }

    ok &= injectPageData(page, m);
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
    m["source"] << connection->sourceId();
    m["destination"] << connection->destinationId();
    ok &= injectConnectionData(connection, m);
    QVariant v;
    v << m;
    ok &= m_processor->processConnection(v);
    ok &= m_writer->writeConnection(v);
    return ok;
}

bool GGBasicProjectSerializer::serializeMedia(QVariant &v, const QString &path, GGEditProject *project)
{
    QString id = project->mediaManager()->getIdForFilePath(path);
    Q_ASSERT(!id.isEmpty());
    QVariantMap media;
    media["id"] << id;
    media["path"] << path;
    v << media;
    return m_processor->processMedia(v);
}

bool GGBasicProjectSerializer::serializeContent(QVariant &v, GGContentElement *elem)
{
    bool ok = true;
    if (!elem) return true;

    QVariantMap m;
    if (GGImageContent *ic = dynamic_cast<GGImageContent*>(elem)) {
        m["type"] << QString("image");
        m["media"] << ic->imageFilePath();
        m_processor->processMediaRef(m["media"]);
    } else if (GGTextContent *tc = dynamic_cast<GGTextContent*>(elem)) {
        m["type"] << QString("text");
        m["text"] << tc->textContent();
    }

    v << m;
    ok &= m_processor->processContentElement(v);
    return ok;
}

bool GGBasicProjectSerializer::serializeMappedLink(QVariant &v, const GGMappedLink &link, int idx)
{
    bool ok = true;
    QVariantMap m;
    m["type"] << (quint32) link.type();
    m["index"] << idx;
    switch (link.type()) {
    case GGMappedLink::Rectangle:
        m["rect"] << link.rectangle();
        break;
    case GGMappedLink::Ellipse:
        m["ellipse"] << link.ellipse();
        break;
    }

    QVariant l;
    ok &= this->serializeLink(l, link.link());
    m["link"] = l;

    v << m;
    return ok;
}

bool GGBasicProjectSerializer::serializeDecisionLink(QVariant &v, const GGLink &link, int idx)
{
    bool ok = serializeLink(v, link);
    if (ok) {
        QVariantMap map;
        v >> map;
        map["index"] = idx;
        v << map;
        return true;
    }
    return false;
}

bool GGBasicProjectSerializer::serializeLink(QVariant &v, const GGLink &link)
{
    bool ok = true;
    QVariantMap m;

    m["name"] << link.name();

    if (link.connection()) {
        m["connection"] << link.connection()->id();
        ok &= m_processor->processConnectionRef(m["connection"]);
    }
    QVariant a;
    ok &= this->serializeAction(a, link.action());
    m["action"] = a;

    v << m;
    return ok;
}

bool GGBasicProjectSerializer::serializeCondition(QVariant &v, const GGCondition &condition)
{
    QVariantMap m;

    m["operator"] << (quint32) condition.type();
    m["variable"] << condition.variableName();
    m["value"] << condition.value();

    v << m;
    return m_processor->processCondition(v);
}

bool GGBasicProjectSerializer::serializeAction(QVariant &v, const GGAction &action)
{
    QVariantMap m;

    m["operator"] << (quint32) action.type();
    m["variable"] << action.variableName();
    m["value"] << action.value();

    v << m;
    return m_processor->processAction(v);
}

bool GGBasicProjectSerializer::serializeVariable(QVariant &v, const GGVariable &var)
{
    QVariantMap m;

    m["name"] << var.name();
    m["initial"] << var.initValue();
    m["type"] << (quint32) var.type();

    v << m;
    return m_processor->processVariable(v);
}

bool GGBasicProjectSerializer::injectProjectData(GGEditProject *project, QVariantMap &v)
{
    Q_UNUSED(project);
    Q_UNUSED(v);
    return true;
}

bool GGBasicProjectSerializer::injectSceneData(GGScene *scene, QVariantMap &v)
{
    Q_UNUSED(scene);
    Q_UNUSED(v);
    return true;
}

bool GGBasicProjectSerializer::injectPageData(GGPage *page, QVariantMap &v)
{
    Q_UNUSED(page);
    Q_UNUSED(v);
    return true;
}

bool GGBasicProjectSerializer::injectConnectionData(GGConnection *connection, QVariantMap &v)
{
    Q_UNUSED(connection);
    Q_UNUSED(v);
    return true;
}

bool GGBasicProjectSerializer::finalizeScene(GGScene *scene)
{
    Q_UNUSED(scene)
    return true;
}
