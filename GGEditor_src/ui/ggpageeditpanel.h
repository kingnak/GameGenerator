#ifndef GGPAGEEDITPANEL_H
#define GGPAGEEDITPANEL_H

#include <QWidget>

class GGPage;
class GGUIController;

namespace Ui {
class GGPageEditPanel;
}

class GGPageEditPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GGPageEditPanel(QWidget *parent = 0);
    void setController(GGUIController *ctrl);
    ~GGPageEditPanel();

public slots:
    void displayPage(GGPage *page);

private slots:
    void pageUpdated(GGPage *page);
    void modelReset();
    void on_txtName_editingFinished();
    void on_txtScene_editingFinished();

private:
    void setCommon();
    void setStart();
    void setEnd();
    void setCondition();
    void setAction();
    void setDecision();

    void setContent();
    void setMappedContent();

private:
    Ui::GGPageEditPanel *ui;
    GGUIController *m_ctrl;
    GGPage *m_page;
};

#endif // GGPAGEEDITPANEL_H
