#ifndef GGCREATESCENEDLG_H
#define GGCREATESCENEDLG_H

#include <QDialog>

namespace Ui {
class GGCreateSceneDlg;
}

class GGCreateSceneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GGCreateSceneDlg(QWidget *parent = 0);
    ~GGCreateSceneDlg();

private:
    Ui::GGCreateSceneDlg *ui;
};

#endif // GGCREATESCENEDLG_H
