#ifndef GGEDITORSCENE_H
#define GGEDITORSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QRect>
#include <QList>
#include <QPair>

class GGUIController;
class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGPageItem;
class GGConnectionItem;
class GGSelectionItem;

class GGEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGEditorScene(QObject *parent = 0);
    void itemMoved(GGPageItem *item);
    void connectToController(GGUIController *ctrl);

    GGPageItem *itemForPage(GGViewPage *page);

    GGViewModel *model() { return m_model; }

    qreal pageBorderWidth() const { return 3; }
    QColor pageColor() const { return Qt::black; }
    QColor pageFillColor() const { return Qt::white; }
    QColor pageHighlightColor() const { return Qt::blue; }
    qreal connectionWidth() const { return 5; }
    QColor connectionColor() const { return Qt::black; }
    QColor connectionHighlightColor() const { return Qt::blue; }

public:
    static const int PAGE_ZVALUE = 1;
    static const int SELECTION_ZVALUE = 5;
    static const int CONNECTION_ZVALUE = 0;

public slots:
    void resetModel(GGViewModel *model);
    void refresh();

signals:
    void pageMoved(GGViewPage *page, QRect r);
    void multiplePagesMoved(QList<QPair<GGViewPage*, QRect> > movements);
    void multipleObjectsDeleted(QList<GGViewPage *> pages, QList<GGViewConnection *> connections);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void pageReg(GGViewPage *p);
    void pageUnreg(GGViewPage *p);
    void connReg(GGViewConnection *c);
    void connUnreg(GGViewConnection *c);
    void pageUpd(GGViewPage *p);
    void pageViewUpd(GGViewPage *p);

    void updateSelectionItem();

private:
    void initSelItem();

private:
    GGViewModel *m_model;
    GGSelectionItem *m_selItem;

    QMap<GGViewPage *, GGPageItem *> m_pageMap;
    QMap<GGViewConnection *, GGConnectionItem *> m_connMap;
};

#endif // GGEDITORSCENE_H