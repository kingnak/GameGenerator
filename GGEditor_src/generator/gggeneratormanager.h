#ifndef GGGENERATORMANAGER_H
#define GGGENERATORMANAGER_H

#include <QObject>
#include <QDir>
#include <QList>
#include "gggeneratorinterface.h"

class QPluginLoader;

class GGGeneratorManager : public QObject
{
    Q_OBJECT

public:
    GGGeneratorManager(QObject *parent = 0);
    ~GGGeneratorManager();

    bool loadGenerators(QDir pluginDir);
    void unloadGenerators();

    QList<GGGeneratorInterface *> getGenerators();

private:
    QList<GGGeneratorInterface *> m_generators;
    QList<QPluginLoader *> m_plugins;
};

#endif // GGGENERATORMANAGER_H
