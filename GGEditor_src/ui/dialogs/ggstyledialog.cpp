#include "ggstyledialog.h"
#include "ui_ggstyledialog.h"
#include <QColorDialog>
#include <model/ggeditmodel.h>
#include <command/ggeditcommandfactory.h>
#include <command/ggmodelgeneralcommands.h>
#include <command/ggcommandgroup.h>
#include <style/ggabstractstyler.h>

#define COLOR_PROPERTY "STYLE_COLOR"

GGStyleDialog::GGStyleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGStyleDialog),
    m_model(NULL)
{
    ui->setupUi(this);
    m_stack = new GGCommandStack;
}

GGStyleDialog::~GGStyleDialog()
{
    delete ui;
    delete m_stack;
}

void GGStyleDialog::setModel(GGEditModel *model)
{
    m_model = model;
    setBasicStyle();
    setStyles();
}

GGCommandStack *GGStyleDialog::getExecutedCommands()
{
    return m_stack;
}

GGBasicStyle GGStyleDialog::getBasicStyle() const
{
    GGBasicStyle bs;
    bs.setFont(ui->cmbFont->currentText());
    bs.setPointSize(ui->spnPointSize->value());
    bs.setForeground(ui->lblFore->property(COLOR_PROPERTY).value<QColor>());
    bs.setBackground(ui->lblBack->property(COLOR_PROPERTY).value<QColor>());
    return bs;
}

void GGStyleDialog::setStyles()
{
    ui->lstStyles->clear();

    foreach (GGStyle s, m_model->getStyler()->styles()) {
        addItemForStyle(s);
    }

    clearCurrentStyle();
}

void GGStyleDialog::setBasicStyle()
{
    GGBasicStyle style = m_model->getStyler()->basicStyle();
    ui->cmbFont->setCurrentText(style.font());
    ui->spnPointSize->setValue(style.pointSize());
    setLabelColor(ui->lblBack, style.background());
    setLabelColor(ui->lblFore, style.foreground());
}

void GGStyleDialog::basicStyleChanged()
{
    GGBasicStyle bs = getBasicStyle();
    if (!m_stack->execute(GGEditCommandFactory(m_model).setBasicStyle(bs))) {

    }
}

void GGStyleDialog::addStyle()
{
    GGStyle s;
    QString name;
    bool ok = false;
    for (int i = 1; !ok; ++i) {
        name = QString("Style %1").arg(i);
        ok = checkStyleName(name);
    }
    s.setName(name);

    if (!m_stack->execute(GGEditCommandFactory(m_model).addStyle(s))) {
        return;
    }

    QListWidgetItem *itm = addItemForStyle(s);
    ui->lstStyles->setCurrentItem(itm);
}

void GGStyleDialog::removeStyle()
{
    QListWidgetItem *itm = ui->lstStyles->currentItem();
    if (!itm) return;
    if (!m_stack->execute(GGEditCommandFactory(m_model).removeStyle(itm->text()))) {
        return;
    }

    delete itm;
    clearCurrentStyle();
}

void GGStyleDialog::showStyle()
{
    if (ui->lstStyles->currentRow() < 0) {
        clearCurrentStyle();
        return;
    }
    QListWidgetItem *itm = ui->lstStyles->currentItem();
    GGStyle s = itm->data(Qt::UserRole).value<GGStyle>();
    ui->txtStyleName->setText(s.name());
    setLabelColor(ui->lblStyleFore, s.foreground());
    ui->btnRemoveStyle->setEnabled(true);

    ui->btnChangeStyleFore->setEnabled(true);
    ui->txtStyleName->setEnabled(true);
}

void GGStyleDialog::currentStyleRenamed()
{
    QListWidgetItem *itm = ui->lstStyles->currentItem();
    if (!itm) return;

    GGStyle s = itm->data(Qt::UserRole).value<GGStyle>();
    if (!checkStyleName(ui->txtStyleName->text())) {
        ui->txtStyleName->setText(s.name());
        return;
    }

    GGEditCommandFactory fac(m_model);

    QString oldName = s.name();
    s.setName(ui->txtStyleName->text());
    GGCommandGroup *grp = new GGCommandGroup;
    grp->addCommand(fac.removeStyle(oldName));
    grp->addCommand(fac.addStyle(s));
    if (!m_stack->execute(grp)) {
        ui->txtStyleName->setText(oldName);
        itm->setText(oldName);
        itm->setData(Qt::UserRole, QVariant::fromValue(s));
    } else {
        itm->setText(s.name());
    }
}

void GGStyleDialog::currentStyleChanged()
{
    QListWidgetItem *itm = ui->lstStyles->currentItem();
    if (!itm) return;

    GGStyle s = itm->data(Qt::UserRole).value<GGStyle>();

    GGEditCommandFactory fac(m_model);

    GGStyle newStyle = getCurrentStyle();
    GGCommandGroup *grp = new GGCommandGroup;
    grp->addCommand(fac.removeStyle(s.name()));
    grp->addCommand(fac.addStyle(newStyle));
    if (!m_stack->execute(grp)) {
        showStyle();
    } else {
        itm->setData(Qt::UserRole, QVariant::fromValue(newStyle));
    }
}

void GGStyleDialog::clearCurrentStyle()
{
    ui->txtStyleName->setText("");
    setLabelColor(ui->lblStyleFore, Qt::transparent);
    ui->txtStyleName->setEnabled(false);
    ui->btnChangeStyleFore->setEnabled(false);
    ui->btnRemoveStyle->setEnabled(false);
}

void GGStyleDialog::on_btnChangeFore_clicked()
{
    QColor c = ui->lblFore->property(COLOR_PROPERTY).value<QColor>();
    if (getColor(c)) {
        setLabelColor(ui->lblFore, c);
        basicStyleChanged();
    }
}

void GGStyleDialog::on_btnChangeBack_clicked()
{
    QColor c = ui->lblBack->property(COLOR_PROPERTY).value<QColor>();
    if (getColor(c)) {
        setLabelColor(ui->lblBack, c);
        basicStyleChanged();
    }
}

void GGStyleDialog::on_btnChangeStyleFore_clicked()
{
    QColor c = ui->lblStyleFore->property(COLOR_PROPERTY).value<QColor>();
    if (getColor(c)) {
        setLabelColor(ui->lblStyleFore, c);
        currentStyleChanged();
    }
}

bool GGStyleDialog::getColor(QColor &color)
{
    QColorDialog dlg;
    dlg.setCurrentColor(color);
    if (dlg.exec() == QDialog::Accepted) {
        color = dlg.currentColor();
        return true;
    }
    return false;
}

void GGStyleDialog::setLabelColor(QLabel *lbl, QColor c)
{
    QString s = QString("background-color: #%1").arg(QString::number((quint32)c.rgb(), 16));
    lbl->setStyleSheet(s);
    lbl->setProperty(COLOR_PROPERTY, c);
}

QListWidgetItem *GGStyleDialog::addItemForStyle(const GGStyle &s)
{
    QListWidgetItem *itm = new QListWidgetItem;
    itm->setText(s.name());
    itm->setData(Qt::UserRole, QVariant::fromValue(s));
    Qt::ItemFlags f = itm->flags();
    f |= Qt::ItemIsEditable;
    itm->setFlags(f);
    ui->lstStyles->addItem(itm);
    return itm;
}

bool GGStyleDialog::checkStyleName(const QString &name)
{
    foreach (GGStyle s, m_model->getStyler()->styles()) {
        if (s.name() == name) {
            return false;
        }
    }
    return true;
}

GGStyle GGStyleDialog::getCurrentStyle()
{
    GGStyle s;
    s.setName(ui->txtStyleName->text());
    s.setForeground(ui->lblStyleFore->property(COLOR_PROPERTY).value<QColor>());
    return s;
}
