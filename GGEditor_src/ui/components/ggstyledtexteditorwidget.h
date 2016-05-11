#ifndef GGSTYLEDTEXTEDITORWIDGET_H
#define GGSTYLEDTEXTEDITORWIDGET_H

#include <QWidget>

class GGAbstractStyler;

namespace Ui {
class GGStyledTextEditorWidget;
}

class GGStyledTextEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGStyledTextEditorWidget(QWidget *parent = 0);
    ~GGStyledTextEditorWidget();

    void setStyler(GGAbstractStyler *styler);

public slots:
    void setFormattedText(const QString &text);

signals:
    void textChanged(const QString &text);

private slots:
    void on_btnEdit_clicked();

private:
    Ui::GGStyledTextEditorWidget *ui;
    GGAbstractStyler *m_styler;
    QString m_text;
};

#endif // GGSTYLEDTEXTEDITORWIDGET_H
