#include "ggglobaluserinfo.h"
#include <io/ggserialization.hpp>
#include <QtWidgets>

GGWindowInfo::GGWindowInfo(const QString &name)
    : m_name(name),
      m_maximized(false)
{

}

GGWindowInfo::GGWindowInfo()
    : m_maximized(false)
{

}

bool GGWindowInfo::isValid() const
{
    return !m_name.isEmpty();
}

QList<qreal> GGWindowInfo::splitters() const
{
    return m_splitters;
}

void GGWindowInfo::setSplitters(const QList<qreal> &splitters)
{
    m_splitters = splitters;
}

bool GGWindowInfo::isMaximized() const
{
    return m_maximized;
}

void GGWindowInfo::setMaximized(bool maximized)
{
    m_maximized = maximized;
}

QRect GGWindowInfo::position() const
{
    return m_position;
}

void GGWindowInfo::setPosition(const QRect &position)
{
    m_position = position;
}


QString GGWindowInfo::name() const
{
    return m_name;
}

void operator <<(QVariant &v, const GGWindowInfo &wndInfo)
{
    QVariantMap map;
    map["name"] << wndInfo.name();
    map["position"] << wndInfo.position();
    map["maximized"] << wndInfo.isMaximized();
    map["splitter"] << wndInfo.splitters();
    v << map;
}

void operator >>(const QVariant &v, GGWindowInfo &wndInfo)
{
    QVariantMap map;
    v >> map;
    map["name"] >> wndInfo.m_name;
    map["position"] >> wndInfo.m_position;
    map["maximized"] >> wndInfo.m_maximized;
    map["splitter"] >> wndInfo.m_splitters;
}

//////////////////////////

GGGlobalUserInfo::GGGlobalUserInfo()
{

}

GGGlobalUserInfo &GGGlobalUserInfo::instance()
{
    static GGGlobalUserInfo inst_;
    return inst_;
}

void GGGlobalUserInfo::setWindowInfo(GGWindowInfo info)
{
    if (info.isValid())
        m_wndInfo[info.name()] = info;
}

GGWindowInfo GGGlobalUserInfo::windowInfo(const QString &name)
{
    return m_wndInfo.value(name);
}

void operator <<(QVariant &v, const GGGlobalUserInfo &info)
{
    QVariantMap map;
    map["window"] << info.m_wndInfo.values();
    v << map;
}

void operator >>(const QVariant &v, GGGlobalUserInfo &info)
{
    info.m_wndInfo.clear();
    QVariantMap m;
    v >> m;
    QVariantList l;
    m["window"] >> l;
    foreach (QVariant v, l) {
        GGWindowInfo w;
        v >> w;
        info.setWindowInfo(w);
    }
}

/////////////////////////////

GGWindowHelper::GGWindowHelper(QWidget *w, const QString &name)
    : m_wnd(w),
      m_name(name)
{

}

void GGWindowHelper::addSplitter(QSplitter *w)
{
    m_splitters.append(w);
}

void GGWindowHelper::backup()
{
    GGWindowInfo info = GGGlobalUserInfo::instance().windowInfo(m_name);
    if (!info.isValid()) {
        info = GGWindowInfo(m_name);
    }
    if (m_wnd->isMaximized()) {
        info.setMaximized(m_wnd->isMaximized());
    } else {
        info.setMaximized(false);
        info.setPosition(QRect(m_wnd->x(), m_wnd->y(), m_wnd->width(), m_wnd->height()));
    }
    QList<qreal> positions;
    foreach (QSplitter *s, m_splitters) {
        quint32 total;
        if (s->orientation() == Qt::Horizontal) {
            total = s->width();
        } else {
            total = s->height();
        }

        foreach (quint32 ref, s->sizes()) {
            positions << qreal(ref) / qreal(total);
        }
    }
    info.setSplitters(positions);
    GGGlobalUserInfo::instance().setWindowInfo(info);
}

void GGWindowHelper::restore()
{
    GGWindowInfo info = GGGlobalUserInfo::instance().windowInfo(m_name);

    bool hide = false;
    if (!m_wnd->isVisible()) {
        hide = true;
        m_wnd->setAttribute(Qt::WA_DontShowOnScreen, true);
        m_wnd->show();
    }

    if (!info.isValid()) {
        // Store initial values
        backup();
    } else {
        int scrn = QApplication::desktop()->primaryScreen();
        QRect avail = QApplication::desktop()->availableGeometry(scrn);

        QRect pos = info.position();
        pos.moveTopLeft(m_wnd->pos());
        pos = pos.intersected(avail);

        //m_wnd->move(pos.x(), pos.y());
        m_wnd->resize(pos.width(), pos.height());
        if (info.isMaximized()) {
            m_wnd->showMaximized();
        }

        for (int i = 0, idx = 0; i < m_splitters.size(); ++i) {
            const int ct = m_splitters[i]->sizes().size();
            int total;
            if (m_splitters[i]->orientation() == Qt::Horizontal) {
                total = m_splitters[i]->width();
            } else {
                total = m_splitters[i]->height();
            }
            QList<int> sizes;
            for (int j = 0; j < ct; ++j) {
                qreal r = info.splitters().value(idx++);
                sizes << r * total;
            }
            m_splitters[i]->setSizes(sizes);
        }
    }

    if (hide) {
        m_wnd->hide();
        m_wnd->setAttribute(Qt::WA_DontShowOnScreen, false);
    }
}
