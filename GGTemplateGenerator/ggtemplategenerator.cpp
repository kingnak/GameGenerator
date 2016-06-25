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
    gameDir(true);

    m_project = project;

    bool ok = true;
    foreach (GGPage *p, m_project->model()->getPages()) {
        ok &= generatePage(p);
    }

    QString js = m_tmplJS;
    js.replace("###FUNCTIONS###", "");
    writeFile("ggGame.js", js);

    QString css = m_tmplCSS;
    css.replace("###STYLES###", formatStyles());
    writeFile("ggGame.css", css);

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

    QString contentLink;
    QString overrideFileName;
    QString relPath;
    if (const GGStartPage *sp = GG::as<const GGStartPage>(page)) {
        ok &= generateStartupPages(sp, contentLink);
        overrideFileName = "start.html";
        relPath = gameDirName();
    } else if (/*const GGEndPage *ep =*/ GG::as<const GGEndPage>(page)) {
        contentLink = getRestartLink();
    }

    ok &= fillGlobalMarkers(markerArray, relPath);
    ok &= fillPageMarkers(cp, markerArray);
    ok &= fillContentMarkerArray(cp, markerArray, contentLink, relPath);

    ok &= substituteMarkerArray(markerArray, tmpl);

    if (overrideFileName.isEmpty()) {
        ok &= writeFile(page, tmpl);
    } else {
        ok &= writeFile(overrideFileName, tmpl, false);
    }

    return ok;
}

bool GGTemplateGenerator::generateConditionPage(const GGConditionPage *page)
{
    QString tmpl = m_tmplCond;
    bool ok = true;

    QMap<QString, QString> markerArray;
    ok &= fillGlobalMarkers(markerArray, "");
    markerArray["###TRUE_LINK###"] = formatLink(page->trueConnection());
    markerArray["###FALSE_LINK###"] = formatLink(page->falseConnection());
    markerArray["###CONDITION###"] = formatCondition(page->getCondition());
    QString tmplFunc =
            "function ggGetConditionLink() {\n"
            "\tif(###CONDITION###) return \"###TRUE_LINK###\"; else return \"###FALSE_LINK###\";\n"
            "}";

    ok &= substituteMarkerArray(markerArray, tmplFunc);
    markerArray["###CONDITION_FUNCTION###"] = tmplFunc;

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

    QStringList formattedDecs;
    for (int i = 0; i < links.size(); ++i) {
        GGLink l = links[i];
        QMap<QString, QString> markers;
        markers["###LINK###"] = formatLink(l.connection());
        markers["###ACTION_LINK###"] = formatAction(l.action());
        markers["###TITLE###"] = l.name();

        QString line = decs;
        substituteMarkerArray(markers, line);
        formattedDecs << line;
    }


    QString decsSubs;
    int fullRows = links.size() / m_ctDecisions;
    for (int i = 0; i < fullRows; ++i) {
        if (i > 0) {
            decsSubs += delim;
        }
        for (int j = 0; j < m_ctDecisions; ++j) {
            decsSubs += formattedDecs[i*m_ctDecisions+j];
        }
    }

    int remain = links.size() % m_ctDecisions;
    if (remain > 0) {
        decsSubs += delim;
        int off = links.size() - remain;
        int diff = m_ctDecisions - remain;
        bool lastWasLink = false;

        while (diff > 0 || remain > 0) {
            if ((diff > remain) || (diff == remain && lastWasLink)) {
                decsSubs += emptyCell;
                lastWasLink = false;
                diff--;
            } else {
                decsSubs += formattedDecs[off++];
                lastWasLink = true;
                remain--;
            }
        }
    }

    /*
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
    */

    decisions = substituteSubpart("###DECISION###", decsSubs, decisions);
    decisions = substituteSubpart("###ROW_DELIM###", "", decisions);
    decisions = substituteSubpart("###EMPTY_CELL###", "", decisions);
    tmpl = substituteSubpart("###DECISIONS###", decisions, tmpl);
    return true;
}

bool GGTemplateGenerator::fillGlobalMarkers(QMap<QString, QString> &marker, QString includeRelPath)
{
    if (!includeRelPath.isEmpty()) includeRelPath += "/";

    marker["###GAME_NAME###"] = m_project->title();
    marker["###SCRIPT_INCLUDE###"] = QString("<script type=\"text/javascript\" src=\"%1\"></script>")
            .arg(includeRelPath+"ggGame.js");
    marker["###STYLE_INCLUDE###"] = QString("<link rel=\"stylesheet\" type=\"text/css\" href=\"%1\"></script>")
            .arg(includeRelPath+"ggGame.css");
    return true;
}

bool GGTemplateGenerator::fillPageMarkers(const GGContentPage *cp, QMap<QString, QString> &marker)
{
    marker["###CAPTION###"] = m_project->model()->getStyler()->htmlText(cp->caption());
    return true;
}

bool GGTemplateGenerator::fillContentMarkerArray(const GGContentPage *cp, QMap<QString, QString> &marker, const QString &contentLink, QString includeRelPath)
{
    if (!includeRelPath.isEmpty()) includeRelPath += "/";
    bool ok = true;
    if (const GGImageContent *im = dynamic_cast<const GGImageContent*>(cp->content())) {

        QString imgPath;
        ok &= copyMedia(cp, im->imageFilePath(), imgPath);

        QString cont = QString("<img src=\"%1\"###MAP_REF###>").arg(includeRelPath + imgPath);
        if (!contentLink.isEmpty()) {
            cont = QString("<a href=\"%1\">%2</a>").arg(contentLink, cont);
        }

        marker["###CONTENT###"] = cont;
        return ok;
    }
    return setError("Unsupported media type");
}

bool GGTemplateGenerator::generateStartupPages(const GGStartPage *sp, QString &entryLink)
{
    QList<GGVariable> vars = m_project->model()->variables().toList();
    QString init;
    QString reset;

    foreach (GGVariable v, vars) {
        QString varSetter = QString("ggSetVar('%1', '%2');").arg(v.name(), v.value());
        if (v.type() == GGVariable::Persistent) {
            init += varSetter + "\n";
        } else {
            reset += varSetter + "\n";
        }
    }

    QString tmpl =
            "<html>\n"
            "<head>\n"
            "<script type=\"text/javascript\" src=\"%4\"></script>\n"
            "<script type=\"text/javascript\">\n"
            "%1\n"
            "%2\n"
            "window.location.href = \"%3\"\n"
            "</script>\n"
            "</head>\n"
            "</html>\n";

    QString realStartLink = formatLink(sp->startConnection());
    QString contInit = tmpl.arg(init, "ggInitGame();", getRestartLink(), "ggGame.js");
    QString contReset = tmpl.arg(reset, "ggResetGame();", realStartLink, "ggGame.js");

    bool ok = true;
    ok &= writeFile("ggInit.html", contInit);
    ok &= writeFile(getRestartLink(), contReset);

    entryLink = gameDirName() + "/ggInit.html";
    return ok;
}

QString GGTemplateGenerator::getRestartLink() const
{
    return "ggRestart.html";
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
    if (start < 0) return tmpl;
    int startAM = start + part.length();
    int stop = tmpl.indexOf(part, startAM);
    if (stop < 0) return tmpl;
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

QString GGTemplateGenerator::formatStyles()
{
    QString styles;
    styles += m_project->model()->getStyler()->basicStyle().toCSS("body");
    foreach (GGStyle s, m_project->model()->getStyler()->styles()) {
        styles += s.toCSS();
    }
    return styles;
}

QString GGTemplateGenerator::formatAction(GGAction act)
{
    QString op;
    switch (act.type()) {
    case GGAction::None: return "";
    case GGAction::Add: op = "ggVarPlus('%1', %2);"; break;
    case GGAction::Sub: op = "ggVarMinus('%1', %2);"; break;
    //case GGAction::Multiply: op = "ggVarMul('%1', %2);"; break;
    //case GGAction::Divide: op = "ggVarDiv('%1', %2);"; break;
    case GGAction::Set: op = "ggSetVar('%1', '%2');"; break;
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
    // Reuse already created file
    if (m_mediaMap.contains(media)) {
        fOut = m_mediaMap[media];
        return true;
    }

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
    if (QFile::copy(fOrig, gameDir().absoluteFilePath(name))) {
        // Cache name
        m_mediaMap[media] = name;
        return true;
    }
    return false;
}

bool GGTemplateGenerator::writeFile(const GGPage *page, const QString &content)
{
    QString fname = generatePageBaseName(page) + ".html";
    return writeFile(fname, content);
}

bool GGTemplateGenerator::writeFile(const QString &name, const QString &content, bool inSubDir)
{
    QDir d(m_outDir);
    if (inSubDir) d = gameDir();

    QString fname = d.absoluteFilePath(name);
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

QDir GGTemplateGenerator::gameDir(bool create) const
{
    QDir d = QDir(m_outDir);
    if (create) {
        d.mkdir(gameDirName());
    }
    return QDir(d.absoluteFilePath(gameDirName()));
}

QString GGTemplateGenerator::gameDirName() const
{
    return "game";
}
