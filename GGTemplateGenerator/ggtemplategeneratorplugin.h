#ifndef GGTEMPLATEGENERATORPLUGIN_H
#define GGTEMPLATEGENERATORPLUGIN_H

#include <QObject>
#include <gggeneratorinterface.h>

class SettingsUI;

class GGTemplateGeneratorPlugin : public QObject, public GGGeneratorInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID GGGenerator_iid)
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(GGGeneratorInterface)

public:
    GGTemplateGeneratorPlugin(QObject *parent = 0);

    virtual ~GGTemplateGeneratorPlugin();

    virtual void setBaseDir(const QDir &dir);
    virtual bool activate();
    virtual bool deactivate();
    virtual bool saveSettings();
    virtual bool loadSettings();

    virtual QString name() const;
    virtual QString description() const;
    virtual QWidget *ui();

    virtual bool generate(const GGAbstractModel *model, const QDir &output);

private:
    bool writeFile(QString dest, QString src);

private:
    QDir m_baseDir;
    SettingsUI *m_ui;
};

#endif // GGTEMPLATEGENERATORPLUGIN_H
