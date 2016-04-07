#include "ggvariableeditdialog.h"
#include "ui_ggvariableeditdialog.h"
#include <QPushButton>
#include <model/ggeditmodel.h>
#include <command/ggcommandstack.h>
#include <command/ggeditcommandfactory.h>
#include <command/ggmodelgeneralcommands.h>

GGVariableEditModel::GGVariableEditModel(GGEditModel *model, QWidget *parent)
    : QAbstractListModel(parent),
      m_model(model)
{
    m_stack = new GGCommandStack;
    connect(m_model, SIGNAL(variablesUpdated()), this, SLOT(reloadData()));
    reloadData();
}

int GGVariableEditModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_model->variables().size();
}

int GGVariableEditModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant GGVariableEditModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() < 0 || index.row() >= m_sortedList.size()) return QVariant();
    //if (role != Qt::DisplayRole) return QVariant();

    GGVariable v = m_model->variableByName(m_sortedList[index.row()]);
    switch (index.column()) {
    case 0:
        return (role == Qt::DisplayRole || role == Qt::EditRole) ? QVariant(v.name()) : QVariant();
    case 1:
        return (role == Qt::DisplayRole || role == Qt::EditRole) ? v.initValue() : QVariant();
    case 2:
        if (role == Qt::CheckStateRole)
            return v.type() == GGVariable::Persistent ? QVariant(Qt::Checked) : QVariant(Qt::Unchecked);
        else
            return QVariant();
    default:
        return QVariant();
    }
}

Qt::ItemFlags GGVariableEditModel::flags(const QModelIndex &index) const
{
    if (index.column() == 2)
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

QVariant GGVariableEditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case 0: return "Name";
    case 1: return "Initial";
    case 2: return "Persistent";
    default: return QVariant();
    }
}

bool GGVariableEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;
    if (index.row() < 0 || index.row() >= m_sortedList.size()) return false;
    //if (role != Qt::EditRole) return false;

    GGVariable v = m_model->variableByName(m_sortedList[index.row()]);
    switch (index.column()) {
    case 0: if (role == Qt::EditRole) v.setName(value.toString()); break;
    case 1: if (role == Qt::EditRole) v.setInitValue(value.toString()); break;
    case 2: if (role == Qt::CheckStateRole) v.setType(value.toInt() == Qt::Checked ? GGVariable::Persistent : GGVariable::Transient); break;
    }

    return m_stack->execute(GGEditCommandFactory(m_model).updateVariable(m_sortedList[index.row()], v));
}

void GGVariableEditModel::addVariable()
{
    QString name = tr("var_%1").arg(m_model->variables().size()+1);
    m_stack->execute(GGEditCommandFactory(m_model).addVariable(name));
}

void GGVariableEditModel::removeVariable(int idx)
{
    if (idx < 0 || idx >= m_sortedList.size()) return;
    m_stack->execute(GGEditCommandFactory(m_model).removeVariable(m_sortedList[idx]));
}

void GGVariableEditModel::reloadData()
{
    this->beginResetModel();
    m_sortedList = QStringList(m_model->variableNames());
    qSort(m_sortedList);
    this->endResetModel();
}

////////////////////////////

GGVariableEditDialog::GGVariableEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGVariableEditDialog)
{
    ui->setupUi(this);
}

GGVariableEditDialog::~GGVariableEditDialog()
{
    delete ui;
}

void GGVariableEditDialog::setModel(GGEditModel *model)
{
    ui->lstVariables->setModel(new GGVariableEditModel(model, this));
    connect(ui->lstVariables->model(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(setButtons()));
    connect(ui->lstVariables->model(), SIGNAL(modelReset()), this, SLOT(setButtons()));
    setButtons();
}

GGCommandStack *GGVariableEditDialog::getExecutedCommands()
{
    return static_cast<GGVariableEditModel*> (ui->lstVariables->model())->getCommandStack();
}

void GGVariableEditDialog::on_btnAdd_clicked()
{
    static_cast<GGVariableEditModel *> (ui->lstVariables->model())->addVariable();
}

void GGVariableEditDialog::on_btnRemove_clicked()
{
    static_cast<GGVariableEditModel *> (ui->lstVariables->model())->removeVariable(ui->lstVariables->currentIndex().row());
}

void GGVariableEditDialog::setButtons()
{
    for (int i = 0; i < ui->lstVariables->model()->rowCount(); ++i) {
        QPushButton *btn = new QPushButton("Show Usages");
        btn->setProperty("INDEX", i);
        ui->lstVariables->setIndexWidget(ui->lstVariables->model()->index(i, 3), btn);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(showVarUsage()));
    }
}

void GGVariableEditDialog::showVarUsage()
{
    int idx = sender()->property("INDEX").toInt();
    QString varName = ui->lstVariables->model()->data(ui->lstVariables->model()->index(idx, 0)).toString();

    GGSearchRequest req(varName, GGSearchRequest::CaseSensitive | GGSearchRequest::Exact, GGSearchRequest::Variable);
    emit showUsages(req);
}

