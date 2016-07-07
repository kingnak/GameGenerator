#include "gggeneratormanager.h"
#include <QPluginLoader>
#include <QDebug>

GGGeneratorManager::GGGeneratorManager(QObject *parent)
    : QObject(parent)
{

}

GGGeneratorManager::~GGGeneratorManager()
{
    unloadGenerators();
}

bool GGGeneratorManager::loadGenerators(QDir pluginDir)
{
    unloadGenerators();

    QString path = pluginDir.absolutePath();
#ifdef Q_OS_WIN
    QStringList lst = pluginDir.entryList(QStringList() << "*.dll", QDir::Files);
#else
    QStringList lst = pluginDir.entryList(QStringList() << "*.so", QDir::Files);
#endif
    foreach (QString p, lst) {
        QString file = pluginDir.absoluteFilePath(p);
        QPluginLoader *loader = new QPluginLoader(file);
        if (loader->load()) {
            QObject *pInst = loader->instance();
            GGGeneratorInterface *gen = qobject_cast<GGGeneratorInterface*> (pInst);
            if (gen) {
                m_generators << gen;
                m_plugins << loader;
            } else {
                loader->unload();
                delete loader;
            }
        } else {
            qDebug() << loader->errorString();
            delete loader;
        }
    }

    return true;
}

void GGGeneratorManager::unloadGenerators()
{
    foreach (QPluginLoader *p, m_plugins) {
        p->unload();
        delete p;
    }

    m_plugins.clear();
    m_generators.clear();
}

QList<GGGeneratorInterface *> GGGeneratorManager::getGenerators()
{
    return m_generators;
}

