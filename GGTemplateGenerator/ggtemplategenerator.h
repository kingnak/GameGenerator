#ifndef GGTEMPLATEGENERATOR_H
#define GGTEMPLATEGENERATOR_H

#include <QString>
#include <QMap>
#include <model/ggmappedlink.h>
#include <model/ggcondition.h>

class GGProject;
class GGPage;
class GGConditionPage;
class GGContentPage;
class GGContentElement;
class GGStartPage;
class GGEndPage;
class QDir;

class GGTemplateGenerator
{
public:
    GGTemplateGenerator();

    GGTemplateGenerator &setPageFile(const QString &file);
    GGTemplateGenerator &setConditionFile(const QString &file);
    GGTemplateGenerator &setJSFile(const QString &file);
    GGTemplateGenerator &setCSSFile(const QString &file);
    GGTemplateGenerator &setDicisionCount(quint8 ct);
    GGTemplateGenerator &setOutputDirectory(const QString &dir);

    bool generate(const GGProject *project);

    QString error() const { return m_error; }

private:
    bool generatePage(const GGPage *page);
    bool generateConditionPage(const GGConditionPage *page);

    QString generatePageBaseName(const GGPage *page);

    bool substituteMapping(QList<GGMappedLink> links, QString &tmpl, QString &mappingName);
    bool substituteDecisions(QList<GGLink> links, QString &tmpl);
    bool fillGlobalMarkers(QMap<QString, QString> &marker, QString includeRelPath);
    bool fillPageMarkers(const GGContentPage *cp, QMap<QString, QString> &marker);
    bool fillContentMarkerArray(const GGContentPage *cp, QMap<QString, QString> &marker, const QString &contentLink, QString includeRelPath);

    bool generateStartupPages(const GGStartPage *sp, QString &entryLink);

    QString getRestartLink() const;

    bool substituteMarkerArray(QMap<QString, QString> marker, QString &tmpl);
    QString substituteSubpart(const QString &part, const QString &content, const QString &tmpl);

    QString formatStyles();

    QString formatAction(GGAction act);
    QString formatCondition(GGCondition cond);
    QString formatLink(GGConnection *conn);

    QString getSubpart(const QString &part, const QString &tmpl);

    bool copyMedia(const GGPage *page, const QString &media, QString &fOut, int ct = 0);

    bool writeFile(const GGPage *page, const QString &content);
    bool writeFile(const QString &name, const QString &content, bool inSubDir = true);

    bool setError(const QString &err);
    bool setStartupError(const QString &err);

    bool parseFile(const QString &file, QString &out);

    QDir gameDir(bool create = false) const;
    QString gameDirName() const;

private:
    QString m_error;
    bool m_startupError;

    QString m_tmplPage;
    QString m_tmplCond;
    QString m_tmplCSS;
    QString m_tmplJS;
    QString m_outDir;
    quint8 m_ctDecisions;
    const GGProject *m_project;

    QMap<QString, QString> m_mediaMap;
};

#endif // GGTEMPLATEGENERATOR_H
