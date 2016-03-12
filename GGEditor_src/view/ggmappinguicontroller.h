#ifndef GGMAPPINGUICONTROLLER_H
#define GGMAPPINGUICONTROLLER_H

#include <QObject>
#include <QRect>

class GGEditModel;
class GGMappedContentPage;
class GGCommandStack;
class GGAbstractModelCommandFactory;
class GGLink;

class GGMappingUIController : public QObject
{
    Q_OBJECT
public:
    GGMappingUIController(GGEditModel *model, QObject *parent = 0);
    ~GGMappingUIController();

    GGCommandStack *getCommandStack();

public slots:
    void changeLink(GGMappedContentPage *page, int idx, const GGLink &link);
    void createLink(GGMappedContentPage *page, QRect rect);
    void moveLink(GGMappedContentPage *page, int idx, QRect rect);
    void deleteLink(GGMappedContentPage *page, int idx);

    void undo();
    void redo();

    void undoAll();

private:
    GGEditModel *m_model;
    GGCommandStack *m_stack;
    GGAbstractModelCommandFactory *m_fac;
};

#endif // GGMAPPINGUICONTROLLER_H
