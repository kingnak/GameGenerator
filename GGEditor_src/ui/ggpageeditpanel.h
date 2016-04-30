#ifndef GGPAGEEDITPANEL_H
#define GGPAGEEDITPANEL_H

#include <QWidget>
#include <model/ggabstractmodel.h>

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

    GGPage *displayedPage() const;

public slots:
    void displayPage(GGPage *page, GGAbstractModel::PageSections updateSections = GGAbstractModel::AllSections);

private slots:
    void pageUpdated(GGPage *page, GGAbstractModel::PageSections sections);
    void modelReset();
    void on_txtName_editingFinished();
//    void on_txtScene_editingFinished();

private:
    void setCommon(GGAbstractModel::PageSections updateSections);
    void setStart(GGAbstractModel::PageSections updateSections);
    void setEnd(GGAbstractModel::PageSections updateSections);
    void setCondition(GGAbstractModel::PageSections updateSections);
    void setAction(GGAbstractModel::PageSections updateSections);
    void setDecision(GGAbstractModel::PageSections updateSections);

    void setContent(GGAbstractModel::PageSections updateSections);
    void setMappedContent(GGAbstractModel::PageSections updateSections);

private:
    Ui::GGPageEditPanel *ui;
    GGUIController *m_ctrl;
    GGPage *m_page;
};

#endif // GGPAGEEDITPANEL_H
