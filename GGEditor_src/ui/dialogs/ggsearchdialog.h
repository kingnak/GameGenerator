#ifndef GGSEARCHDIALOG_H
#define GGSEARCHDIALOG_H

#include <QDialog>
#include <model/ggsearch.h>

namespace Ui {
class GGSearchDialog;
}

class GGSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGSearchDialog(QWidget *parent = 0);
    ~GGSearchDialog();

public slots:
    void accept();
    void resetSearch();

signals:
    void executeSearch(const GGSearchRequest &request);

private:
    bool updateSearch();

private:
    Ui::GGSearchDialog *ui;
    GGSearchRequest m_lastSearch;
};

#endif // GGSEARCHDIALOG_H
