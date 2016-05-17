#ifndef GGGLOBALSETTINGSSERIALIZER_H
#define GGGLOBALSETTINGSSERIALIZER_H

class QIODevice;
class GGGlobalUserInfo;

class GGGlobalSettingsSerializer
{
public:
    GGGlobalSettingsSerializer();

    bool save(QIODevice *dev, const GGGlobalUserInfo *info);
    bool load(QIODevice *dev, GGGlobalUserInfo *info);
};

#endif // GGGLOBALSETTINGSSERIALIZER_H
