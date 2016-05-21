#ifndef GGSTYLEDIALOG_H
#define GGSTYLEDIALOG_H

#include <QDialog>
#include <QList>
#include <style/ggstyle.h>
#include <command/ggcommandstack.h>

class QListWidgetItem;
class QLabel;
class GGEditModel;

namespace Ui {
class GGStyleDialog;
}

Q_DECLARE_METATYPE(GGStyle)

class GGStyleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGStyleDialog(QWidget *parent = 0);
    ~GGStyleDialog();

    void setModel(GGEditModel *model);
    GGCommandStack *getExecutedCommands();

private:
    GGBasicStyle getBasicStyle() const;
    void setStyles();
    void setBasicStyle();

private slots:
    void basicStyleChanged();
    void addStyle();
    void removeStyle();
    void showStyle();
    void currentStyleRenamed();
    void currentStyleChanged();
    void clearCurrentStyle();

    void on_btnChangeFore_clicked();
    void on_btnChangeBack_clicked();
    void on_btnChangeStyleFore_clicked();

private:
    bool getColor(QColor &color);
    void setLabelColor(QLabel *lbl, QColor c);
    QListWidgetItem *addItemForStyle(const GGStyle &s);
    bool checkStyleName(const QString &name);
    GGStyle getCurrentStyle();

private:
    Ui::GGStyleDialog *ui;
    GGCommandStack *m_stack;
    GGEditModel *m_model;
};

#endif // GGSTYLEDIALOG_H
