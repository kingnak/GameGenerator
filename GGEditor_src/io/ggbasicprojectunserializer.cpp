#include "ggbasicprojectunserializer.h"
#include <model/ggeditproject.h>
#include <io/ggserialization.hpp>
#include <io/ggabstractunserializationreader.h>
#include <io/ggunserializationprocessor.h>
#include <model/ggeditmodel.h>
#include <model/ggabstractfactory.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggconnectionslot.h>
#include <model/ggvariable.h>
#include <model/ggcontentelement.h>
#include <model/ggscenemediamanager.h>
#include <style/ggstyle.h>
#include <style/ggabstractstyler.h>

template<typename F, typename S>
bool pairFirstSorter(const QPair<F,S> &p1, const QPair<F,S> &p2) {
    return p1.first < p2.first;
}

GGBasicProjectUnserializer::GGBasicProjectUnserializer(const QString &basePath, const QString &fileName, GGAbstractUnserializationReader *reader, GGUnserializationProcessor *processor)
    : m_reader(reader),
      m_processor(processor),
      m_project(NULL)
{
    m_project = new GGEditProject(basePath, fileName, reader->serializationType());
}

GGBasicProjectUnserializer::~GGBasicProjectUnserializer()
{
    delete m_project;
}

bool GGBasicProjectUnserializer::load(QIODevice *device)
{
    m_conns.clear();
    m_connSlots.clear();
    // Do not emit signals while loading!
    m_project->editModel()->blockSignals(true);
    bool ret = m_reader->unserialize(device, this);
    m_project->editModel()->blockSignals(false);
    return ret;
}

QString GGBasicProjectUnserializer::error() const
{
    if (m_reader) return m_reader->error();
    return "Unknown error";
}

GGEditProject *GGBasicProjectUnserializer::loadedProject()
{
    return m_project;
}

GGEditProject *GGBasicProjectUnserializer::takeProject()
{
    GGEditProject *ret = m_project;
    m_project = NULL;
    return ret;
}

bool GGBasicProjectUnserializer::unserializeProject(QVariant project)
{
    if (!m_processor->processProject(project)) return false;

    if (!project.canConvert<QVariantMap>()) return false;

    QVariantMap map = project.value<QVariantMap>();
    if (!map.contains("title")) return false;
    m_project->setTitle(map["title"].toString());

    QVariantList lst;
    map["variable"] >> lst;
    bool ok = true;
    foreach (QVariant v, lst) {
        GGVariable var;
        ok &= unserializeVariable(v, var);
        m_project->editModel()->addVariable(var);
    }

    QVariantList lstMedia;
    map["media"] >> lstMedia;
    foreach (QVariant v, lstMedia) {
        ok &= unserializeMedia(v);
    }

    QString mediaSource;
    map["mediaSourceDir"] >> mediaSource;
    m_project->mediaManager()->setMediaSourceDir(mediaSource);

    QVariant bs;
    map["basicStyle"] >> bs;
    GGBasicStyle style;
    ok &= unserializeBasicStyle(bs, style);
    m_project->editModel()->getStyler()->setBasicStyle(style);

    QVariantList lstStyles;
    map["style"] >> lstStyles;
    foreach (QVariant v, lstStyles) {
        GGStyle style;
        ok &= unserializeStyle(v, style);
        m_project->editModel()->getStyler()->addStyle(style);
    }

    return ok;
}

bool GGBasicProjectUnserializer::unserializeScene(QVariant scene)
{
    if (!m_processor->processScene(scene)) return false;
    if (!scene.canConvert<QVariantMap>()) return false;
    QVariantMap map = scene.value<QVariantMap>();

    if (!map.contains("id") || !map.contains("name")) return false;

    GGScene *sc = m_project->editModel()->factory()->createScene();
    sc->setName(map["name"].toString());
    sc->setMediaDir(map["mediaDir"].toString());
    bool ok;
    GG::SceneID id = static_cast<GG::SceneID> (map["id"].toUInt(&ok));

    this->setSceneId(sc, id);
    ok &= m_project->model()->registerSceneWithId(sc);

    return ok;
}

bool GGBasicProjectUnserializer::unserializePage(QVariant page)
{
    if (!m_processor->processPage(page)) return false;
    if (!page.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    page >> map;

    if (!map.contains("name") || !map.contains("id") || !map.contains("type") || !map.contains("scene")) return false;

    bool ok;
    int type = map["type"].toInt(&ok);
    if (!ok) return false;

    ok = m_processor->processSceneRef(map["scene"]);
    if (!ok) return false;
    GG::SceneID sceneId = static_cast<GG::SceneID> (map["scene"].toInt(&ok));
    if (!ok) return false;
    if (!m_project->model()->getScene(sceneId)) return false;

    GGPage *p = NULL;
    switch (type) {
    case GGConditionPage::Type: p = m_project->editModel()->factory()->createConditionPage(sceneId); break;
    case GGStartPage::Type: p = m_project->editModel()->factory()->createStartPage(sceneId); break;
    case GGEndPage::Type: p = m_project->editModel()->factory()->createEndPage(sceneId); break;
    case GGActionPage::Type: p = m_project->editModel()->factory()->createActionPage(sceneId); break;
    case GGDecisionPage::Type: p = m_project->editModel()->factory()->createDecisionPage(sceneId); break;
    }
    if (!p) return false;

    p->setName(map["name"].toString());
    GG::PageID id = static_cast<GG::PageID> (map["id"].toInt(&ok));
    if (!ok) return false;
    this->setPageId(p, id);

    if (GGConditionPage *cp = GG::as<GGConditionPage>(p)) {
        GGCondition cond;
        ok &= this->unserializeCondition(map["condition"], cond);
        cp->setCondition(cond);

        ok &= storeConnection(map, "true", p, GGConnectionSlotData::TrueConnection);
        ok &= storeConnection(map, "false", p, GGConnectionSlotData::FalseConnection);
        if (!ok) return false;
    }

    if (GGContentPage *cp = GG::as<GGContentPage>(p)) {
        cp->setCaption(map["caption"].toString());
        GGContentElement *elem = NULL;
        ok &= this->unserializeContent(map["content"], elem);
        if (!ok) return false;
        cp->setContent(elem);
    }

    if (GGStartPage *sp = GG::as<GGStartPage>(p)) {
        Q_UNUSED(sp);
        ok &= storeConnection(map, "start", p, GGConnectionSlotData::StartConnection);
        if (!ok) return false;
    }

    if (GGEndPage *ep = GG::as<GGEndPage>(p)) {
        // Nothing to do
        Q_UNUSED(ep);
    }

    if (GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(p)) {
        if (map.contains("map") && map["map"].isValid()) {
            if (!map["map"].canConvert<QVariantList>()) return false;
            QVariantList lst;
            map["map"] >> lst;
            QList<QPair<int,GGMappedLink> > mappedLinks;
            foreach (QVariant l, lst) {
                GGMappedLink link;
                int idx;
                ok &= unserializeMappedLink(l, link, p, idx);
                mappedLinks << qMakePair(idx, link);
            }
            if (!ok) return false;

            // Guarantee sorting
            qSort(mappedLinks.begin(), mappedLinks.end(), pairFirstSorter<int, GGMappedLink>);
            typedef QPair<int, GGMappedLink> T_PAIR;
            foreach (T_PAIR l, mappedLinks) {
                mcp->addMappedLink(l.second);
            }
        }
    }

    if (GGActionPage *ap = GG::as<GGActionPage>(p)) {
        GGLink link;
        ok &= this->unserializeLink(map["action"], link, p, GGConnectionSlotData::ActionConnection);
        ap->setActionLink(link);
    }

    if (GGDecisionPage *dp = GG::as<GGDecisionPage>(p)) {
        if (map.contains("decision") && map["decision"].isValid()) {
            if (!map["decision"].canConvert<QVariantList>()) return false;
            QVariantList lst;
            map["decision"] >> lst;
            QList<QPair<int,GGLink> > decLinks;
            foreach (QVariant l, lst) {
                GGLink link;
                int idx;
                ok &= unserializeDecisionLink(l, link, p, idx);
                decLinks << qMakePair(idx, link);
            }
            if (!ok) return false;

            // Guarantee sorting
            qSort(decLinks.begin(), decLinks.end(), pairFirstSorter<int, GGLink>);
            typedef QPair<int, GGLink> T_PAIR;
            foreach (T_PAIR l, decLinks) {
                dp->addDecisionLink(l.second);
            }
        }
    }

    return m_project->editModel()->registerPageWithId(p);
}

bool GGBasicProjectUnserializer::unserializeConnection(QVariant connection)
{
    if (!m_processor->processConnection(connection)) return false;
    if (!connection.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    connection >> map;

    if (!map.contains("id") || !map.contains("source") || !map.contains("destination")) return false;

    bool ok = true;
    ok &= m_processor->processPageRef(map["source"]);
    ok &= m_processor->processPageRef(map["destination"]);
    if (!ok) return false;

    GG::ConnectionID id = static_cast<GG::ConnectionID> (map["id"].toInt(&ok));
    if (!ok) return false;
    GG::PageID sid = static_cast<GG::PageID> (map["source"].toInt(&ok));
    if (!ok) return false;
    GG::PageID did = static_cast<GG::PageID> (map["destination"].toInt(&ok));
    if (!ok) return false;

    GGConnection *c = m_project->editModel()->factory()->createConnection(sid, did);
    this->setConnectionId(c, id);
    m_conns << c;

    return true;
}

bool GGBasicProjectUnserializer::unserializeForeignPage(QVariant page)
{
    Q_UNUSED(page);
    return true;
}

bool GGBasicProjectUnserializer::unserializeForeignConnection(QVariant connection)
{
    Q_UNUSED(connection);
    return true;
}

bool GGBasicProjectUnserializer::finalizeUnserialization()
{
    // Resolve all connections
    foreach (GGConnection *conn, m_conns) {
        // Resolve pages
        GGPage *src = m_project->editModel()->getPage(conn->sourceId());
        GGPage *dest = m_project->editModel()->getPage(conn->destinationId());
        if (!src || !dest) return false;
        resolveConnectionPages(conn, src, dest);

        // Register
        if (!m_project->editModel()->registerConnectionWithId(conn)) {
            return false;
        }

        // Resolve to slot
        if (!m_connSlots.contains(conn->id())) {
            return false;
        }
        GGConnectionSlot slot = m_connSlots[conn->id()].second;
        GGConnection *oldConn = NULL;
        if (!slot.connect(m_connSlots[conn->id()].first, conn, &oldConn)) {
            return false;
        }
        if (oldConn) {
            Q_ASSERT(false);
            return false;
        }
    }

    m_project->editModel()->synchronizeNextIds();
    m_project->mediaManager()->synchronizeNextMediaId();

    return true;
}

bool GGBasicProjectUnserializer::unserializeVariable(QVariant data, GGVariable &var)
{
    if (!m_processor->processVariable(data)) return false;
    if (!data.canConvert<QVariantMap>()) return false;

    QVariantMap map = data.value<QVariantMap>();
    if (!map.contains("name") || !map.contains("type")) {
        return false;
    }

    bool ok;
    var = GGVariable(map["name"].toString(), map["initial"].toString(), static_cast<GGVariable::Type>(map["type"].toUInt(&ok)));
    return ok;
}

bool GGBasicProjectUnserializer::unserializeMedia(QVariant data)
{
    if (!m_processor->processMedia(data)) return false;
    if (!data.canConvert<QVariantMap>()) return false;

    QVariantMap map = data.value<QVariantMap>();
    if (!map.contains("id") || !map.contains("path")) {
        return false;
    }

    return injectMedia(m_project->mediaManager(), map["id"].toString(), map["path"].toString());
}

bool GGBasicProjectUnserializer::unserializeCondition(QVariant data, GGCondition &cond)
{
    if (!m_processor->processCondition(data)) return false;
    if (!data.isValid()) return false;
    QVariantMap map;
    data >> map;
    if (map.isEmpty()) return true;

    if (!map.contains("variable") || !map.contains("value") || !map.contains("operator")) return false;
    cond.setVariableName(map["variable"].toString());
    cond.setValue(map["value"].toString());
    bool ok;
    GGCondition::Type type = static_cast<GGCondition::Type> (map["operator"].toUInt(&ok));
    if (!ok) return false;
    cond.setType(type);
    return true;
}

bool GGBasicProjectUnserializer::unserializeAction(QVariant data, GGAction &act)
{
    if (!m_processor->processAction(data)) return false;
    if (!data.isValid()) return false;
    QVariantMap map;
    data >> map;
    if (!map.contains("variable") || !map.contains("value") || !map.contains("operator")) return false;
    act.setVariableName(map["variable"].toString());
    act.setValue(map["value"].toString());
    bool ok;
    GGAction::Type type = static_cast<GGAction::Type> (map["operator"].toUInt(&ok));
    if (!ok) return false;
    act.setType(type);
    return true;
}

bool GGBasicProjectUnserializer::unserializeContent(QVariant data, GGContentElement *&content)
{
    content = NULL;
    if (!data.isValid()) return true;
    if (!m_processor->processContentElement(data)) return false;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;
    if (map.isEmpty()) return true;

    if (!map.contains("type")) return false;
    QString type;
    map["type"] >> type;

    if (type == "image") {
        QString media;
        if (!m_processor->processMediaRef(map["media"])) return false;
        map["file"] >> media;
        content = new GGImageContent;
        static_cast<GGImageContent*> (content)->setImageFilePath(media);
    } else if (type == "text") {
        QString text;
        map["text"] >> text;
        content = new GGTextContent;
        static_cast<GGTextContent*> (content)->setTextContent(text);
    } else {
        return false;
    }
    return true;
}

bool GGBasicProjectUnserializer::unserializeBasicStyle(QVariant data, GGBasicStyle &bs)
{
    if (!data.isValid()) return true;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;

    QString font;
    quint32 size;
    QColor fore;
    QColor back;
    map["font"] >> font;
    map["size"] >> size;
    map["foreground"] >> fore;
    map["background"] >> back;

    bs.setFont(font);
    bs.setPointSize((quint8) size);
    bs.setForeground(fore);
    bs.setBackground(back);

    return true;
}

bool GGBasicProjectUnserializer::unserializeStyle(QVariant data, GGStyle &style)
{
    if (!data.isValid()) return true;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;

    if (!map.contains("name")) return false;

    QString name;
    QColor fore;
    map["name"] >> name;
    map["foreground"] >> fore;
    style.setName(name);
    style.setForeground(fore);

    return true;
}

bool GGBasicProjectUnserializer::unserializeMappedLink(QVariant data, GGMappedLink &link, GGPage *page, int &idx)
{
    if (!data.isValid()) return false;
    if (!m_processor->processMappedLink(data)) return false;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;
    //if (map.isEmpty()) return true;

    if (!map.contains("type") || !map.contains("index")) return false;

    bool ok;
    GGMappedLink::Type t = static_cast<GGMappedLink::Type> (map["type"].toUInt(&ok));
    if (!ok) return false;

    QRect r;
    switch (t) {
    case GGMappedLink::Rectangle:
        r = map["rect"].value<QRect>();
        link = GGMappedLink::rectangle(r);
        break;
    case GGMappedLink::Ellipse:
        r = map["ellipse"].value<QRect>();
        link = GGMappedLink::ellipse(r);
        break;
    default:
        return false;
    }

    idx = map["index"].toInt(&ok);
    if (!ok) return false;

    GGLink l;
    ok = this->unserializeLink(map["link"], l, page, GGConnectionSlotData(GGConnectionSlotData::MappedConnection, idx));
    if (!ok) return false;

    link.setLink(l);
    return true;
}

bool GGBasicProjectUnserializer::unserializeDecisionLink(QVariant data, GGLink &link, GGPage *page, int &idx)
{
    if (!data.isValid()) return false;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;
    //if (map.isEmpty()) return true;

    bool ok;
    idx = map["index"].toInt(&ok);
    if (!ok) return false;

    GGLink l;
    ok = this->unserializeLink(map, link, page, GGConnectionSlotData(GGConnectionSlotData::DecisionConnection, idx));
    if (!ok) return false;

    return true;
}

bool GGBasicProjectUnserializer::unserializeLink(QVariant data, GGLink &link, GGPage *page, GGConnectionSlotData slot)
{
    if (!data.isValid()) return false;
    if (!m_processor->processLink(data)) return false;
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    data >> map;

    bool ok = true;
    link.setName(map["name"].toString());
    GGAction act;
    ok &= unserializeAction(map["action"], act);
    link.setAction(act);
    ok &= storeConnection(map, "connection", page, slot);

    return ok;
}

bool GGBasicProjectUnserializer::storeConnection(QVariantMap map, const QString &key, GGPage *page, GGConnectionSlotData slot)
{
    if (!map.contains(key)) return true;
    return storeConnection(map[key], page, slot);
}

bool GGBasicProjectUnserializer::storeConnection(QVariant data, GGPage *page, GGConnectionSlotData slot)
{
    if (!data.isValid()) return false;
    if (!m_processor->processConnectionRef(data)) return false;

    bool ok;
    GG::ConnectionID id = static_cast<GG::ConnectionID> (data.toInt(&ok));
    if (!ok) return false;

    m_connSlots[id] = qMakePair(page, slot);
    return true;
}
