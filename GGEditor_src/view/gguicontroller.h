#ifndef GGUICONTROLLER_H
#define GGUICONTROLLER_H

#include <QObject>
#include <QRect>
#include <QList>
#include <QPair>

#include <model/ggconnectionslot.h>

class GGViewModel;
class GGViewCommandFactory;
class GGCommandStack;
class GGAbstractCommand;
class GGPage;
class GGContentPage;
class GGDecisionPage;
class GGContentElement;
class GGLink;
class GGViewPage;
class GGViewConnection;

class GGUIController : public QObject
{
    Q_OBJECT
public:
    GGUIController(QObject *parent = 0);
    ~GGUIController();

    GGViewModel *model() { return m_model; }

    void setModel(GGViewModel *model);

    void applySubcommandsAsGroup(GGCommandStack *stack);
    void applySubcommandsAsSingle(GGCommandStack *stack);

    enum CreationMode {
        CreateNone, CreateStartPage, CreateEndPage, CreateConditionPage, CreateActionPage, CreateDecisionPage, CreateConnection, CreateConnectionDirect
    };
    Q_ENUMS(CreationMode)
    void setCreationMode(CreationMode mode);
    CreationMode creationMode() const { return m_createMode; }

public slots:
    void saveCheckpoint();

    void undo();
    void redo();

    void changePageGeometry(GGViewPage *page, QRect rect);
    void changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> > changes);
    void deleteConnection(GGConnection *c);
    void deleteMultipleObjects(QSet<GGViewPage*> pages, QSet<GGViewConnection*> connections);

    void changePageName(GGPage *page, QString name);
    void changePageScene(GGPage *page, QString scene);
    void changeContentPageCaption(GGContentPage *page, QString cap);
    void changeContentElement(GGContentPage *page, GGContentElement *elem);
    void changeLink(GGPage *page, GGConnectionSlot slot, const GGLink &link);
    void removeDecisionLink(GGDecisionPage *page, int idx);
    void addDecisionLink(GGDecisionPage *page);

    void connnectPagesDialog(GGViewPage *src, GGViewPage *dest);

    void setSelection(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections);

    void handleSceneClick(QPointF pos);

    void connectPageDirect(GGPage *src, GGConnectionSlot slot);
    void setDirectConnectionPage(GGPage *dest);
    void abortDirectConnection();

signals:
    void objectsSelected(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections);
    void singlePageSelected(GGViewPage *page);
    void singleConnectionSelected(GGViewConnection *con);
    void selectionCleared();
    void commandError(QString err);
    void modelReset(GGViewModel *model);

    void modelDirty(bool dirty);
    void undoAvailable(bool avail);
    void redoAvailable(bool avail);

    void creationModeChanged(CreationMode mode);
    void connectingDirect(GGPage *page, GGConnectionSlot slot);

private:
    bool doExecCmd(GGAbstractCommand *cmd);
    bool checkSaveCheckpoint();

private:
    GGViewModel *m_model;
    GGCommandStack *m_stack;
    GGViewCommandFactory *m_cmdFactory;
    CreationMode m_createMode;
    GGAbstractCommand *m_saveCommand;

    GGPage *m_directConnSource;
    GGConnectionSlot m_directConnSlot;
};

#endif // GGUICONTROLLER_H
