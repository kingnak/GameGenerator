#ifndef GGSCENE_H
#define GGSCENE_H

#include <gg_definitions.h>
#include <gg_core_global.h>
#include <QSet>

class GGPage;
class GGAbstractModel;

class GG_CORESHARED_EXPORT GGScene
{
public:
    GGScene();
    ~GGScene();

    GG::SceneID id() const { return m_id; }

    GGAbstractModel *model() const { return m_model; }

    QString name() const { return m_name; }
    void setName(const QString &name);

    QString mediaDir() const { return m_mediaDir; }
    void setMediaDir(const QString &dir);

    QSet<GGPage *> pages() const;
    void addPage(GGPage *page);
    void removePage(GGPage *page);

private:
    GG::SceneID m_id;
    GGAbstractModel *m_model;
    QString m_name;
    QString m_mediaDir;
    QSet<GGPage *> m_pages;

    friend class GGAbstractModel;
};

#endif // GGSCENE_H
