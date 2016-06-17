#include "ggtemplategenerator.h"
#include <QFile>
#include <QTextStream>
#include <ggutilities.h>
#include <model/ggproject.h>
#include <model/ggabstractmodel.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggcontentelement.h>
#include <model/ggmediaresolver.h>
#include <style/ggabstractstyler.h>

GGTemplateGenerator::GGTemplateGenerator()
    : m_ctDecisions(3), m_startupError(false)
{

}

GGTemplateGenerator &GGTemplateGenerator::setPageFile(const QString &file)
{
    parseFile(file, m_tmplPage);
    return *this;
}

GGTemplateGenerator &GGTemplateGenerator::setConditionFile(const QString &file)
{
    parseFile(file, m_tmplCond);
    return *this;
}

GGTemplateGenerator &GGTemplateGenerator::setJSFile(const QString &file)
{
    parseFile(file, m_tmplJS);
    return *this;
}

GGTemplateGenerator &GGTemplateGenerator::setCSSFile(const QString &file)
{
    parseFile(file, m_tmplCSS);
    return *this;
}

GGTemplateGenerator &GGTemplateGenerator::setDicisionCount(quint8 ct)
{
    m_ctDecisions = ct;
    return *this;
}

GGTemplateGenerator &GGTemplateGenerator::setOutputDirectory(const QString &dir)
{
    m_outDir = dir;
    return *this;
}

bool GGTemplateGenerator::generate(const GGProject *project)
{
    if (m_startupError) {
        return false;
    }
    if (m_outDir.isEmpty()) {
        return setError("Output directory not set");
    }
    m_project = project;

    bool ok = true;
    foreach (GGPage *p, m_project->model()->getPages()) {
        ok &= generatePage(p);
    }

    QString js = m_tmplJS;
    js.replace("###FUNCTIONS###", "");
    writeFile("gggame.js", js);

    QString css = m_tmplCSS;
    // TODO: Styles
    css.replace("###STYLES###", "");
    writeFile("gggame.css", css);

    return ok;
}

bool GGTemplateGenerator::generatePage(const GGPage *page)
{
    if (const GGConditionPage *cp = GG::as<const GGConditionPage>(page)) {
        return generateConditionPage(cp);
    }

    const GGContentPage *cp = GG::as<const GGContentPage>(page);
    Q_ASSERT(cp);
    QMap<QString, QString> markerArray;
    bool ok = true;

    QString tmpl = m_tmplPage;

    if (const GGDecisionPage *dp = GG::as<const GGDecisionPage>(page)) {
        ok &= substituteDecisions(dp->getDecisionLinks(), tmpl);
    } else {
        ok &= substituteDecisions(QList<GGLink>(), tmpl);
    }
    QString mapName;
    if (const GGMappedContentPage *mcp = GG::as<const GGMappedContentPage>(page)) {
        ok &= substituteMapping(mcp->getLinkMap(), tmpl, mapName);
    } else {
        ok &= substituteMapping(QList<GGMappedLink>(), tmpl, mapName);
    }

    markerArray["###MAPPING_NAME###"] = mapName;
    if (!mapName.isEmpty()) {
        markerArray["###MAP_REF###"] = " useMap=\"#"+mapName+"\"";
    } else {
        markerArray["###MAP_REF###"] = "";
    }

    ok &= fillGlobalMarkers(markerArray);
    ok &= fillPageMarkers(cp, markerArray);
    ok &= fillContentMarkerArray(cp, markerArray);

    ok &= substituteMarkerArray(markerArray, tmpl);

    ok &= writeFile(page, tmpl);

    return ok;
}

bool GGTemplateGenerator::generateConditionPage(const GGConditionPage *page)
{
    QString tmpl = m_tmplCond;
    bool ok = true;

    QMap<QString, QString> markerArray;
    ok &= fillGlobalMarkers(markerArray);
    markerArray["###TRUE_LINK###"] = formatLink(page->trueConnection());
    markerArray["###FALSE_LINK###"] = formatLink(page->falseConnection());
    markerArray["###CONDITION###"] = formatCondition(page->getCondition());

    ok &= substituteMarkerArray(markerArray, tmpl);
    ok &= writeFile(page, tmpl);
    return ok;
}

QString GGTemplateGenerator::generatePageBaseName(const GGPage *page)
{
    QString name = QString("%1_%2").arg(page->scene()->name(), page->name());
    return GGUtilities::sanatizeFileName(name);
}

bool GGTemplateGenerator::substituteMapping(QList<GGMappedLink> links, QString &tmpl, QString &mappingName)
{
    if (links.isEmpty()) {
        mappingName = "";
        tmpl = substituteSubpart("###MAPPINGS###", "", tmpl);
        return true;
    }

    mappingName = "contentMap";
    QString mappings = getSubpart("###MAPPINGS###", tmpl);
    const QString areas = getSubpart("###AREAS###", mappings);
    QString areasSubs;

    foreach (GGMappedLink l, links) {
        QMap<QString, QString> markers;
        //switch (l.type()) {} TODO: LATER
        markers["###SHAPE###"] = "rect";
        QRect r = l.rectangle();
        markers["###COORDS###"] = QString("%1,%2,%3,%4").arg(r.left()).arg(r.top()).arg(r.right()).arg(r.bottom());
        markers["###LINK###"] = formatLink(l.link().connection());
        markers["###ACTION_LINK###"] = formatAction(l.link().action());
        markers["###TITLE###"] = l.link().name();

        QString line = areas;
        substituteMarkerArray(markers, line);
        areasSubs += line;
    }
    mappings = substituteSubpart("###AREAS###", areasSubs, mappings);
    tmpl = substituteSubpart("###MAPPINGS###", mappings, tmpl);
    return true;
}

bool GGTemplateGenerator::substituteDecisions(QList<GGLink> links, QString &tmpl)
{
    if (links.isEmpty()) {
        tmpl = substituteSubpart("###DECISIONS###", "", tmpl);
        return true;
    }

    QString decisions = getSubpart("###DECISIONS###", tmpl);
    QString delim = getSubpart("###ROW_DELIM###", decisions);
    QString emptyCell = getSubpart("###EMPTY_CELL###", decisions);
    const QString decs = getSubpart("###DECISION###", decisions);

    QString decsSubs;

    for (int i = 0; i < links.size(); ++i) {
        GGLink l = links[i];
        if ((i % m_ctDecisions) == 0 && i > 0) {
            decsSubs += delim;
        }
        QMap<QString, QString> markers;
        markers["###LINK###"] = formatLink(l.connection());
        markers["###ACTION_LINK###"] = formatAction(l.action());
        markers["###TITLE###"] = l.name();

        QString line = decs;
        substituteMarkerArray(markers, line);
        decsSubs += line;
    }
    int offset = m_ctDecisions - (links.size() % m_ctDecisions);
    if (offset != m_ctDecisions) {
        for (int i = 0; i < offset; ++i) {
            decsSubs += emptyCell;
        }
    }

    decisions = substituteSubpart("###DECISION###", decsSubs, decisions);
    decisions = substituteSubpart("###ROW_DELIM###", "", decisions);
    decisions = substituteSubpart("###EMPTY_CELL###", "", decisions);
    tmpl = substituteSubpart("###DECISIONS###", decisions, tmpl);
    return true;
}

bool GGTemplateGenerator::fillGlobalMarkers(QMap<QString, QString> &marker)
{
    marker["###GAME_NAME###"] = m_project->title();
    marker["###SCRIPT_INCLUDE###"] = QString("<script type=\"text/javascript\" src=\"%1\"></script>")
            .arg("gggame.js");
    marker["###STYLE_INCLUDE###"] = QString("<link rel=\"stylesheet\" type=\"text/css\" href=\"%1\"></script>")
            .arg("gggame.css");
    return true;
}

bool GGTemplateGenerator::fillPageMarkers(const GGContentPage *cp, QMap<QString, QString> &marker)
{
    marker["###CAPTION###"] = m_project->model()->getStyler()->htmlText(cp->caption());
    return true;
}

bool GGTemplateGenerator::fillContentMarkerArray(const GGContentPage *cp, QMap<QString, QString> &marker)
{
    bool ok = true;
    if (const GGImageContent *im = dynamic_cast<const GGImageContent*>(cp->content())) {

        QString imgPath;
        ok &= copyMedia(cp, im->imageFilePath(), imgPath);
        marker["###CONTENT###"] = QString("<img src=\"%1\"###MAP_REF###>").arg(imgPath);
        return ok;
    }
    return setError("Unsupported media type");
}

bool GGTemplateGenerator::substituteMarkerArray(QMap<QString, QString> marker, QString &tmpl)
{
    foreach (QString k, marker.keys()) {
        tmpl.replace(k, marker[k]);
    }
    return true;
}

QString GGTemplateGenerator::substituteSubpart(const QString &part, const QString &content, const QString &tmpl)
{
    int start = tmpl.indexOf(part);
    if (start < 0) return content;
    int startAM = start + part.length();
    int stop = tmpl.indexOf(part, startAM);
    if (stop < 0) return content;
    int stopAM = stop + part.length();

    QString before = tmpl.left(start);
    QString after = tmpl.mid(stopAM);
    //QString between = conten.mid(startAM, stop-startAM);

    QRegExp rx("^(.*)<!--[^>]*$");
    if (rx.indexIn(before) >= 0) before = rx.cap(1);

    QRegExp rx1("^[^<]*-->(.*)$");
    if (rx1.indexIn(after) >= 0) after = rx1.cap(1);

    return before + content + after;
}

QString GGTemplateGenerator::formatAction(GGAction act)
{
    QString op;
    switch (act.type()) {
    case GGAction::None: return "";
    case GGAction::Add: op = "ggVarPlus('%1', %2);"; break;
    case GGAction::Sub: op = "ggVarMinus('%1', %2);"; break;
    //case GGAction::Multiply: op = "ggVarAdd('%1', %2);"; break;
    //case GGAction::Divide: op = "ggVarAdd('%1', %2);"; break;
    case GGAction::Set: op = "ggSetVar('%1', %2);"; break;
    case GGAction::Unset: op = "ggDeleteVar('%1');"; break;
    }
    op = op.arg(act.variableName(), act.value());
    return QString("onclick=\"%1\"").arg(op);
}

QString GGTemplateGenerator::formatCondition(GGCondition cond)
{
    QString op;
    switch (cond.type()) {
    case GGCondition::None: return "true";
    case GGCondition::Equals: op = "=="; break;
    case GGCondition::NotEquals: op = "!="; break;
    case GGCondition::LessThen: op = "<"; break;
    case GGCondition::LessEqual: op = "<="; break;
    case GGCondition::GreaterThen: op = ">"; break;
    case GGCondition::GreaterEquals: op = ">="; break;
    }
    QString var = QString("ggReadVar(\"%1\")").arg(cond.variableName());
    QString val = cond.value();
    return QString("%1 %2 \"%3\"").arg(var, op, val);
}

QString GGTemplateGenerator::formatLink(GGConnection *conn)
{
    if (conn) {
        return generatePageBaseName(conn->destination()) + ".html";
    } else {
        setError("Empty connection");
        return "";
    }
}

QString GGTemplateGenerator::getSubpart(const QString &part, const QString &tmpl)
{
    int start = tmpl.indexOf(part);
    if (start < 0) return "";
    start += part.length();
    int stop = tmpl.indexOf(part, start);
    if (stop < 0) return "";
    QString cont = tmpl.mid(start, stop-start);

    QRegExp rx1("^([^<]*-->)(.*)(<!--[^>]*)$");
    if (rx1.indexIn(cont) >= 0) return rx1.cap(2);

    QRegExp rx2("(.*)(<!--[^>]*)$");
    if (rx2.indexIn(cont) >= 0) return rx2.cap(1);

    QRegExp rx3("^([^<]*-->)(.*)$");
    if (rx3.indexIn(cont) >= 0) return rx3.cap(2);

    return cont;
}

bool GGTemplateGenerator::copyMedia(const GGPage *page, const QString &media, QString &fOut, int ct)
{
    QString fOrig = m_project->resolver()->resolveFile(media);
    if (fOrig.isEmpty()) {
        // Could be a internal resolver, not based on real files.
        // Must use a QIODevice for reading/writing data...
        // TODO: There are no such resolvers yet...
        return setError("Cannot load media from internal files");
    }

    QString name = generatePageBaseName(page);
    if (ct > 0) {
        name += QString::number(ct);
    }
    name += "." + m_project->resolver()->resolveTypeHint(media);
    fOut = name;
    return QFile::copy(fOrig, QDir(m_outDir).absoluteFilePath(name));
}

bool GGTemplateGenerator::writeFile(const GGPage *page, const QString &content)
{
    QString fname = generatePageBaseName(page) + ".html";
    return writeFile(fname, content);
}

bool GGTemplateGenerator::writeFile(const QString &name, const QString &content)
{
    QString fname = QDir(m_outDir).absoluteFilePath(name);
    QFile fOut(fname);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return setError("Cannot open " + fname + " for writing");
    }
    {
        QTextStream ts(&fOut);
        ts << content;
    }
    fOut.close();
    if (fOut.error() != QFile::NoError) {
        return setError("Error writing to " + fname);
    }
    return true;
}

bool GGTemplateGenerator::setError(const QString &err)
{
    if (m_error.isEmpty()) m_error = err;
    return false;
}

bool GGTemplateGenerator::setStartupError(const QString &err)
{
    m_startupError = true;
    m_error = err;
    return false;
}

bool GGTemplateGenerator::parseFile(const QString &file, QString &out)
{
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        return setStartupError("Cannot open file " + file);
    }
    QTextStream ts(&f);
    out = ts.readAll();
    if (f.error() != QFile::NoError) {
        return setStartupError("Error reading file " + file);
    }
    return true;
}
