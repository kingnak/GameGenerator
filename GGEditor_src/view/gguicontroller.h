#ifndef GGUICONTROLLER_H
#define GGUICONTROLLER_H

#include <QObject>
#include <QRect>

class GGViewModel;
class GGViewCommandFactory;
class GGCommandStack;
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

signals:
    void commandError(QString err);

private:
    GGViewModel *m_model;
    GGCommandStack *m_stack;
    GGViewCommandFactory *m_cmdFactory;

};

#endif // GGUICONTROLLER_H
