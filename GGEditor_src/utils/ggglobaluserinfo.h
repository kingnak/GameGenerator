#ifndef GGGLOBALUSERINFO_H
#define GGGLOBALUSERINFO_H

#include <gg_core_global.h>
#include <QMap>
#include <QList>
#include <QRect>
#include <QVariant>

class GGWindowInfo
{
public:
    GGWindowInfo(const QString &name);
    GGWindowInfo();

    bool isValid() const;
    QString name() const;

    QRect position() const;
    void setPosition(const QRect &position);

    QList<qreal> splitters() const;
    void setSplitters(const QList<qreal> &splitters);

    bool isMaximized() const;
    void setMaximized(bool isMaximized);

private:
    QString m_name;
    QRect m_position;
    QList<qreal> m_splitters;
    bool m_maximized;

    friend void operator >> (const QVariant &v, GGWindowInfo &wndInfo);
};

void operator << (QVariant &v, const GGWindowInfo &wndInfo);
void operator >> (const QVariant &v, GGWindowInfo &wndInfo);

class GGGlobalUserInfo
{
private:
    GGGlobalUserInfo();
    Q_DISABLE_COPY(GGGlobalUserInfo)

public:
    static GGGlobalUserInfo &instance();

    void setExternalEditor(const QString &path);
    QString externalEditor() const;

    void setWindowInfo(GGWindowInfo info);
    GGWindowInfo windowInfo(const QString &name) const;

    QString generatorOutputPath() const;
    void setGeneratorOutputPath(const QString &generatorOutputPath);

private:
    QString m_externalEditor;
    QString m_generatorOutputPath;
    QMap<QString, GGWindowInfo> m_wndInfo;

    friend void operator << (QVariant &v, const GGGlobalUserInfo &info);
    friend void operator >> (const QVariant &v, GGGlobalUserInfo &info);
};

void operator << (QVariant &v, const GGGlobalUserInfo &info);
void operator >> (const QVariant &v, GGGlobalUserInfo &info);

//////////////////////////

class QSplitter;
class GGWindowHelper {
public:
    GGWindowHelper(QWidget *w, const QString &name);
    void addSplitter(QSplitter *w);

    void backup();
    void restore();

private:
    QWidget *m_wnd;
    QString m_name;
    QList<QSplitter *> m_splitters;
};

#endif // GGGLOBALUSERINFO_H
