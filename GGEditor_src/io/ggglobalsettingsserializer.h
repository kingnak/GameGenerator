#ifndef GGGLOBALSETTINGSSERIALIZER_H
#define GGGLOBALSETTINGSSERIALIZER_H

#include <QString>

class QIODevice;
class GGGlobalUserInfo;

class GGGlobalSettingsSerializer
{
public:
    GGGlobalSettingsSerializer();

    static QString defaultSettingsFilePath();

    static bool saveDefaultFile(const GGGlobalUserInfo *info);
    static bool loadDefaultFile(GGGlobalUserInfo *info);

    bool save(QIODevice *dev, const GGGlobalUserInfo *info);
    bool load(QIODevice *dev, GGGlobalUserInfo *info);
};

#endif // GGGLOBALSETTINGSSERIALIZER_H
