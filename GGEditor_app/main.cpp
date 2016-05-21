#include "ggmainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <utils/ggglobaluserinfo.h>
#include <utils/ggtrasher.h>
#include <io/ggglobalsettingsserializer.h>

#ifdef Q_OS_WIN
#include <utils/ggwintrasher.h>
#endif

#ifdef USE_VLD
#include <vld.h>
#endif

//#define MASS_TEST
#ifdef MASS_TEST
#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <model/gglink.h>
#include <model/ggpage.h>

#include <model/ggsearch.h>

#include <QTime>

void massTest(GGMainWindow *w)
{
    QTime timer;
    timer.start();
    const int COLS = 50;
    const int ROWS = 50;
    GGPage *prev = NULL;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            w->controller()->setCreationMode(GGUIController::CreateDecisionPage);
            w->controller()->handleSceneClick(QPointF(75*j,100*i));
            GGPage *cur = w->currentModel()->editModel()->getPage(i*COLS+j);
            w->controller()->changePageName(cur, QString("abba%1").arg(i*COLS+j));
            w->controller()->changePageScene(cur, QString("abba%1").arg(i));

            GGLink l;
            l.setName("Right");
            w->controller()->addDecisionLink(GG::as<GGDecisionPage> (cur));
            w->controller()->changeLink(cur, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0), l);
            l.setName("Down");
            w->controller()->addDecisionLink(GG::as<GGDecisionPage> (cur));
            w->controller()->changeLink(cur, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 1), l);


            if (j > 0) {
                w->controller()->connectPageDirect(prev, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0));
                w->controller()->setDirectConnectionPage(cur);
            }
            if (i > 0) {
                GGPage *above = w->currentModel()->editModel()->getPage((i-1)*COLS+j);
                w->controller()->connectPageDirect(above, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 1));
                w->controller()->setDirectConnectionPage(cur);
            }

            prev = cur;
        }
    }
    qDebug(qPrintable(QString("Create %1 items: %2 ms").arg(ROWS*COLS).arg(timer.elapsed())));


    GGSearchRequest req("ri");
    timer.restart();
    GGSearchResult res = w->controller()->model()->editModel()->search(req);
    qDebug(qPrintable(QString("Search %1 items: %2 ms").arg(res.count()).arg(timer.elapsed())));

}
#endif

//#define MEDIA_TEST
#ifdef MEDIA_TEST
#include <model/ggmediamanager.h>
void mediaTest()
{
    QDir base("D:/tmp/mediatest");
    GGMediaManager m(base);
    QString s = m.checkIn("D:\\tmp\\mediatest\\media\\images\\Chrysanthemum.jpg");
    m.synchronize();
    //QString s2 = m.checkIn("D:\\tmp\\mediatest\\media\\images\\Chrysanthemum.jpg");
    QIODevice *dev = m.resolver()->resolve(s);
    delete dev;
    QString n = m.resolver()->resolveName(s);
    QString t = m.resolver()->resolveTypeHint(s);
}
#endif

int main(int argc, char *argv[])
{
#if defined(USE_VLD) && defined(VLD_PER_PROJECT)
    VLDGlobalDisable();
    VLDMarkAllLeaksAsReported();
#endif

    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);

    QApplication::setApplicationName("GameGenerator");

    GGDefaultTrasher defTrash;
    GGTrasher::setTrasher(&defTrash);

#ifdef Q_OS_WIN
    GGWinTrasher winTrash;
    GGTrasher::setTrasher(&winTrash);
#endif

    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    dataDir.mkpath(".");
    QString globUserData = dataDir.absoluteFilePath("userData.xml");

    {
        QFile f(globUserData);
        if (f.open(QIODevice::ReadOnly)) {
            GGGlobalSettingsSerializer ser;
            ser.load(&f, &GGGlobalUserInfo::instance());
            /*
            QDataStream s(&f);
            QVariant v;
            s >> v;
            v >> GGGlobalUserInfo::instance();
            */
        }
        f.close();
    }

    GGMainWindow w;
    w.show();

#ifdef MASS_TEST
    massTest(&w);
#endif

#ifdef MEDIA_TEST
    mediaTest();
#endif

    int ret = a.exec();

    {
        QFile f(globUserData);
        if (f.open(QIODevice::WriteOnly)) {
            GGGlobalSettingsSerializer ser;
            ser.save(&f, &GGGlobalUserInfo::instance());
            /*
            QDataStream s(&f);
            QVariant v;
            v << GGGlobalUserInfo::instance();
            s << v;
            */
        }
    }

    return ret;
}
