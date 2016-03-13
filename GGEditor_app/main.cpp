#include "ggmainwindow.h"
#include <QApplication>

#ifdef USE_VLD
#include <vld.h>
#endif

#include <view/gguicontroller.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <model/gglink.h>
#include <model/ggpage.h>

void massTest(GGMainWindow *w)
{
    const int COLS = 10;
    const int ROWS = 10;
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
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);
    GGMainWindow w;
    w.show();

    massTest(&w);

    //return a.exec();
}
