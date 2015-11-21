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

class GGUIController : public QObject
{
    Q_OBJECT
public:
    GGUIController(QObject *parent = 0);
    ~GGUIController();

    GGViewModel *model() { return m_model; }

public slots:
    void undo();
    void redo();

    void changePageGeometry(GGViewPage *page, QRect rect);
    void changeMultiplePagesGeometry(QList<QPair<GGViewPage*,QRect> > changes);

signals:
    void commandError(QString err);

private:
    void doExecCmd(GGAbstractCommand *cmd);

private:
    GGViewModel *m_model;
    GGCommandStack *m_stack;
    GGViewCommandFactory *m_cmdFactory;

};

#endif // GGUICONTROLLER_H
