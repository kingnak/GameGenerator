#ifndef GGUICONTROLLER_H
#define GGUICONTROLLER_H

#include <QObject>
#include <QRect>
#include <QList>
#include <QPair>

class GGViewModel;
class GGViewCommandFactory;
class GGCommandStack;
class GGAbstractCommand;
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

    enum CreationMode {
        CreateNone, CreateStartPage, CreateEndPage, CreateConditionPage, CreateActionPage, CreateDecisionPage, CreateConnection
    };
    Q_ENUM(CreationMode)
    void setCreationMode(CreationMode mode);
    CreationMode creationMode() const { return m_createMode; }

public slots:
    void saveCheckpoint();

    void undo();
    void redo();

    void changePageGeometry(GGViewPage *page, QRect rect);
    void changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> > changes);
    void deleteMultipleObjects(QSet<GGViewPage*> pages, QSet<GGViewConnection*> connections);

    void connnectPagesDialog(GGViewPage *src, GGViewPage *dest);

    void setSelection(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections);

    void handleSceneClick(QPointF pos);

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

private:
    bool doExecCmd(GGAbstractCommand *cmd);
    bool checkSaveCheckpoint();

private:
    GGViewModel *m_model;
    GGCommandStack *m_stack;
    GGViewCommandFactory *m_cmdFactory;
    CreationMode m_createMode;
    GGAbstractCommand *m_saveCommand;
};

#endif // GGUICONTROLLER_H
