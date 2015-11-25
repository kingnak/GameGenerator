#ifndef GGCHOSECONNECTIONSLOTDLG_H
#define GGCHOSECONNECTIONSLOTDLG_H

#include <QDialog>

class GGPage;
class GGConnectionSlot;

namespace Ui {
class GGChoseConnectionSlotDlg;
}

class GGChoseConnectionSlotDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GGChoseConnectionSlotDlg(QWidget *parent = 0);
    ~GGChoseConnectionSlotDlg();

    GGPage *page();
    GGConnectionSlot selectedSlot();

public slots:
    void setConnectionSlots(GGPage *source);

private slots:
    void toggledOne();

private:
    Ui::GGChoseConnectionSlotDlg *ui;
    GGPage *m_page;
    int m_curSelected;
};

#endif // GGCHOSECONNECTIONSLOTDLG_H
