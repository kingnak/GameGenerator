#ifndef GGEDITORSCENE_H
#define GGEDITORSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QRect>
#include <QList>
#include <QPair>

class GGUIController;
class GGScene;
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
    GGEditorScene(GGUIController *ctrl, QObject *parent = 0);
    void connectToController(GGUIController *ctrl);

    GGPageItem *itemForPage(GGViewPage *page);

    GGViewModel *model() { return m_model; }
    GGScene *modelScene();

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
    void setSelection(QSet<GGViewPage *> pages, QSet<GGViewConnection *> conns);
    void deleteCurrentSelection();

signals:
    void pageMoved(GGViewPage *page, QRect r);
    void multiplePagesMoved(QList<QPair<GGViewPage*, QRect> > movements);
    void multipleObjectsDeleted(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections);

    void itemsSelected(QSet<GGViewPage*> pages, QSet<GGViewConnection*> conns);
    void clickedEmptySpace(QPointF pos);

    void connectPages(GGViewPage *source, GGViewPage *destination);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

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
    GGPageItem *getItemAtPos(QPointF pos);
    void selectionToSets(QSet<GGViewPage*> &pages, QSet<GGViewConnection*> &conns);

private:
    GGViewModel *m_model;
    GGSelectionItem *m_selItem;
    GGUIController *m_ctrl;
    QGraphicsLineItem *m_connectorLine;
    GGPageItem *m_connectSource;

    QMap<GGViewPage *, GGPageItem *> m_pageMap;
    QMap<GGViewConnection *, GGConnectionItem *> m_connMap;
    
    bool m_inUpdateSelection;
};

#endif // GGEDITORSCENE_H
