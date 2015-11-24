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

public slots:
    void undo();
    void redo();

    void changePageGeometry(GGViewPage *page, QRect rect);
    void changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> > changes);
    void deleteMultipleObjects(QList<GGViewPage*> pages, QList<GGViewConnection*> connections);

    GGViewPage *createStartPage(QRect r);
    GGViewPage *createEndPage(QRect r);
    GGViewPage *createConditionPage(QRect r);
    GGViewPage *createActionPage(QRect r);
    GGViewPage *createDecisionPage(QRect r);

signals:
    void commandError(QString err);
    void modelReset(GGViewModel *model);

private:
    bool doExecCmd(GGAbstractCommand *cmd);

private:
    GGViewModel *m_model;
    GGCommandStack *m_stack;
    GGViewCommandFactory *m_cmdFactory;

};

#endif // GGUICONTROLLER_H
