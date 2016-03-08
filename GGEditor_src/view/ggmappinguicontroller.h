#ifndef GGMAPPINGUICONTROLLER_H
#define GGMAPPINGUICONTROLLER_H

#include <QObject>
#include <QRect>

class GGEditModel;
class GGMappedContentPage;
class GGCommandStack;
class GGAbstractModelCommandFactory;

class GGMappingUIController : public QObject
{
    Q_OBJECT
public:
    GGMappingUIController(GGEditModel *model, QObject *parent = 0);
    ~GGMappingUIController();

    GGCommandStack *getCommandStack();

public slots:
    void createLink(GGMappedContentPage *page, QRect rect);
    void moveLink(GGMappedContentPage *page, int idx, QRect rect);

    void undo();
    void redo();

    void undoAll();

private:
    GGEditModel *m_model;
    GGCommandStack *m_stack;
    GGAbstractModelCommandFactory *m_fac;
};

#endif // GGMAPPINGUICONTROLLER_H
