#include "ggvariableeditdialog.h"
#include "ui_ggvariableeditdialog.h"
#include <QPushButton>
#include <model/ggeditmodel.h>
#include <command/ggcommandstack.h>
#include <command/ggeditcommandfactory.h>
#include <command/ggmodelgeneralcommands.h>

GGVariableEditModel::GGVariableEditModel(GGEditModel *model, QWidget *parent)
    : QAbstractListModel(parent),
      m_model(model),
      m_renameOk(false),
      m_deleteOk(false)
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
    QString oldName = v.name();
    bool isRename = false;

    switch (index.column()) {
    case 0: if (role == Qt::EditRole) { v.setName(value.toString()); isRename = true; } break;
    case 1: if (role == Qt::EditRole) v.setInitValue(value.toString()); break;
    case 2: if (role == Qt::CheckStateRole) v.setType(value.toInt() == Qt::Checked ? GGVariable::Persistent : GGVariable::Transient); break;
    }

    if (isRename) {
        m_renameOk = false;
        emit checkRenameVar(oldName, v.name());
        if (!m_renameOk) {
            return false;
        }
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
    GGVariable v = m_sortedList[idx];
    m_deleteOk = false;
    emit checkDeleteVar(v.name());
    if (m_deleteOk) {
        m_stack->execute(GGEditCommandFactory(m_model).removeVariable(v.name()));
    }
}

void GGVariableEditModel::renameIsOk()
{
    m_renameOk = true;
}

void GGVariableEditModel::deleteIsOk()
{
    m_deleteOk = true;
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
    GGVariableEditModel *vm = new GGVariableEditModel(model, this);
    ui->lstVariables->setModel(vm);
    connect(vm, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(setButtons()));
    connect(vm, SIGNAL(modelReset()), this, SLOT(setButtons()));
    connect(vm, SIGNAL(checkDeleteVar(QString)), this, SLOT(checkDelete(QString)));
    connect(vm, SIGNAL(checkRenameVar(QString,QString)), this, SLOT(checkRename(QString,QString)));
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

void GGVariableEditDialog::checkRename(const QString &oldName, const QString &newName)
{
    static_cast<GGVariableEditModel *> (ui->lstVariables->model())->renameIsOk();
}

void GGVariableEditDialog::checkDelete(const QString &name)
{
    static_cast<GGVariableEditModel *> (ui->lstVariables->model())->deleteIsOk();
}

