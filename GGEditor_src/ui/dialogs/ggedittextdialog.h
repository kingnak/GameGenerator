#ifndef GGEDITTEXTDIALOG_H
#define GGEDITTEXTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QTextCharFormat>
#include <QTextEdit>

class GGAbstractStyler;
class QTextDocument;

namespace Ui {
class GGEditTextDialog;
}

class PlainPasteTextEdit : public QTextEdit {
public:
    PlainPasteTextEdit(QWidget *parent = 0) : QTextEdit(parent) {}
protected:
    void insertFromMimeData(const QMimeData *source);
};

class GGEditTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGEditTextDialog(QWidget *parent = 0);
    ~GGEditTextDialog();

    void setStyler(const GGAbstractStyler *styler);
    void setFormattedDocument(const QString &doc);
    QString getFormattedDocument() const;
    QTextDocument *getDocument() const;
    bool hasModification() const;

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void applyStyle();
    void toggleItalic();
    void updateStyle(QTextCharFormat f);

private:
    void doApplyStyle(QTextCharFormat f);

private:
    Ui::GGEditTextDialog *ui;
    const GGAbstractStyler *m_styler;
    bool m_updatingStyle;
};

#endif // GGEDITTEXTDIALOG_H
