#ifndef GGEDITCONTENTELEMENTWIDGET_H
#define GGEDITCONTENTELEMENTWIDGET_H

#include <QWidget>

class GGContentElement;
class GGAbstractMediaResolver;
class GGSceneMediaManager;
class GGScene;

namespace Ui {
class GGEditContentElementWidget;
}

class GGEditContentElementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGEditContentElementWidget(QWidget *parent = 0);
    ~GGEditContentElementWidget();

    void setManager(GGSceneMediaManager *manager);
    GGContentElement *getContentElement();

public slots:
    void setContentElement(GGContentElement *elem, GGScene *scene);

private slots:
    void changePage();
    void on_btnBrowseImage_clicked();

    void loadPreviewImage(const QString &media, GGAbstractMediaResolver *resolver);

private:
    Ui::GGEditContentElementWidget *ui;
    GGSceneMediaManager *m_manager;
    GGScene *m_curScene;
};

#endif // GGEDITCONTENTELEMENTWIDGET_H
