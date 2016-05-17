#include "ggstyledtexteditorwidget.h"
#include "ui_ggstyledtexteditorwidget.h"
#include <ui/dialogs/ggedittextdialog.h>
#include <style/ggabstractstyler.h>

GGStyledTextEditorWidget::GGStyledTextEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGStyledTextEditorWidget),
    m_styler(NULL)
{
    ui->setupUi(this);
}

GGStyledTextEditorWidget::~GGStyledTextEditorWidget()
{
    delete ui;
}

void GGStyledTextEditorWidget::setStyler(GGAbstractStyler *styler)
{
    m_styler = styler;
}

void GGStyledTextEditorWidget::setFormattedText(const QString &text)
{
    m_text = text;
    ui->txtText->setText(m_styler->previewText(text));
}

void GGStyledTextEditorWidget::editText()
{
    GGEditTextDialog dlg(this);
    dlg.setStyler(m_styler);
    dlg.setFormattedDocument(m_text);
    if (dlg.exec() == QDialog::Accepted) {
        emit textChanged(dlg.getFormattedDocument());
    }
}

void GGStyledTextEditorWidget::on_btnEdit_clicked()
{
    editText();
}
