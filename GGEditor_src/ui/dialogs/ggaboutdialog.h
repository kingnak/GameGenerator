#ifndef GGABOUTDIALOG_H
#define GGABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class GGAboutDialog;
}

class GGAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGAboutDialog(QWidget *parent = 0);
    ~GGAboutDialog();

private:
    Ui::GGAboutDialog *ui;
};

#endif // GGABOUTDIALOG_H
