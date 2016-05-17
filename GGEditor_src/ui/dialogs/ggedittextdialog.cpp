#include "ggedittextdialog.h"
#include "ui_ggedittextdialog.h"
#include <style/ggabstractstyler.h>
#include <QMimeData>

Q_DECLARE_METATYPE(QTextCharFormat)

GGEditTextDialog::GGEditTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGEditTextDialog),
    m_styler(NULL),
    m_updatingStyle(false)
{
    ui->setupUi(this);
}

GGEditTextDialog::~GGEditTextDialog()
{
    delete ui;
}

void GGEditTextDialog::setStyler(const GGAbstractStyler *styler)
{
    m_styler = styler;
    ui->txtText->clear();
    ui->txtText->document()->setModified(false);

    ui->btnItalic->setChecked(false);
    ui->cmbStyles->clear();

    m_updatingStyle = true;
    if (m_styler) {
        ui->cmbStyles->addItem("Default", QVariant::fromValue<QTextCharFormat> (m_styler->formatForDefaultStyle().toCharFormat()));
        ui->btnItalic->setVisible(styler->supplementalOptions().testFlag(GGAbstractStyler::Italic));

        foreach (GGStyle s, m_styler->styles()) {
            // Convert style to char format
            QTextCharFormat f = m_styler->formatForStyle(s).toCharFormat();
            ui->cmbStyles->addItem(s.name(), QVariant::fromValue<QTextCharFormat>(f));
        }
    } else {
        ui->cmbStyles->addItem("Default", QTextCharFormat());
    }
    m_updatingStyle = false;
}

void GGEditTextDialog::setFormattedDocument(const QString &doc)
{
    if (!m_styler) return;
    ui->txtText->setDocument(m_styler->styleString(doc));
    QTextCursor c = ui->txtText->textCursor();
    c.movePosition(QTextCursor::Start);
    ui->txtText->setTextCursor(c);
    ui->txtText->document()->setModified(false);
}

QString GGEditTextDialog::getFormattedDocument() const
{
    if (!m_styler) return QString::null;
    return m_styler->parseDocument(ui->txtText->document());
}

QTextDocument *GGEditTextDialog::getDocument() const
{
    return ui->txtText->document();
}

bool GGEditTextDialog::hasModification() const
{
    return ui->txtText->document()->isModified();
}

void GGEditTextDialog::applyStyle()
{
    if (m_updatingStyle) return;

    QTextCharFormat style = ui->cmbStyles->currentData().value<QTextCharFormat>();
    doApplyStyle(style);
}

void GGEditTextDialog::toggleItalic()
{
    if (m_updatingStyle) return;

    QTextCharFormat style;
    if (ui->btnItalic->isChecked()) {
        style.setFontItalic(true);
    } else {
        style.setFontItalic(false);
    }
    doApplyStyle(style);
}

void GGEditTextDialog::updateStyle(QTextCharFormat f)
{
    m_updatingStyle = true;

    if (f.fontItalic()) {
        ui->btnItalic->setChecked(true);
    } else {
        ui->btnItalic->setChecked(false);
    }

    QString name = m_styler ? m_styler->styleNameForFormat(f) : QString::null;
    if (name.isNull()) {
        ui->cmbStyles->setCurrentIndex(0);
    } else {
        ui->cmbStyles->setCurrentText(name);
    }

    m_updatingStyle = false;
}

void GGEditTextDialog::doApplyStyle(QTextCharFormat f)
{
    QTextCursor c = ui->txtText->textCursor();
    if (!c.hasSelection()) {
        c.movePosition(QTextCursor::StartOfWord);
        c.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    }
    c.mergeCharFormat(f);
}

void PlainPasteTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasText() && canInsertFromMimeData(source)) {
        insertPlainText(source->text());
    }
}
