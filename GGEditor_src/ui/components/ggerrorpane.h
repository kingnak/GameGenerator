#ifndef GGERRORPANE_H
#define GGERRORPANE_H

#include <QWidget>
#include <model/ggmodelverifier.h>

class QListWidgetItem;

namespace Ui {
class GGErrorPane;
}

class GGErrorPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGErrorPane(QWidget *parent = 0);
    ~GGErrorPane();

public slots:
    void clear();
    void enableVerify(bool enable);
    void setErrors(GGModelErrorList list);

signals:
    void reverify();
    void showPage(const GGPage *p, GGConnectionSlotData slot);

private slots:
    void on_btnRefresh_clicked();
    void showError(QListWidgetItem *item);

private:
    Ui::GGErrorPane *ui;
};

#endif // GGERRORPANE_H
