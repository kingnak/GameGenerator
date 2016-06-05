#ifndef GGGENERATORINTERFACE_H
#define GGGENERATORINTERFACE_H

#include <QtPlugin>
#include <QDir>

class GGAbstractModel;

class GGGeneratorInterface
{
public:
    GGGeneratorInterface() {}
    virtual ~GGGeneratorInterface() {}

    virtual void setBaseDir(const QDir &dir) = 0;
    virtual bool activate() = 0;
    virtual bool deactivate() = 0;
    virtual bool saveSettings() = 0;
    virtual bool loadSettings() = 0;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QWidget *ui() = 0;

    virtual bool generate(const GGAbstractModel *model, const QDir &output) = 0;
};

#define GGGenerator_iid "com.gamegenerator.GeneratorInterface"
Q_DECLARE_INTERFACE(GGGeneratorInterface, GGGenerator_iid)

#endif // GGGENERATORINTERFACE_H

