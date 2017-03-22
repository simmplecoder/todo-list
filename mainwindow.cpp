#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

#include <memory>

MainWindow::MainWindow(const QString& filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearList()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addTodoClicked()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveClicked()));
    connect(ui->todoList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteClicked()));
    connect(ui->upButton, SIGNAL(clicked(bool)), this, SLOT(upClicked()));
    connect(ui->downButton, SIGNAL(clicked(bool)), this, SLOT(downClicked()));

    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->upButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->downButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));

    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);

    ui->todoList->clearSelection();
    selectedItemChanged();

    auto file = openFile(filename);
    if (file)
    {
        parseFile(*file);
    }
}

void MainWindow::clearList()
{
    ui->todoList->clear();
}

void MainWindow::addTodoClicked()
{
    QString todoDesc;
    auto dialog = std::make_unique<AddNewDialog>(todoDesc);
    dialog->show();
    dialog->exec();
    ui->todoList->addItem(todoDesc);
    //selectedItemChanged();
}

void MainWindow::parseFile(QFile& file)
{
    QTextStream stream(&file);
    QString buffer;

    while (!stream.atEnd())
    {
        buffer = stream.readLine(256);
        ui->todoList->addItem(buffer);
    }

}

void MainWindow::saveClicked()
{
    QFile file("todolist.todo");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);

    int listLength = ui->todoList->count();
    for (int i = 0; i < listLength; ++i)
    {
        stream << ui->todoList->item(i)->text() << '\n';
    }

    QMessageBox::information(this, "Notification", "Saved the list successfully");
}

void MainWindow::selectedItemChanged()
{
    ui->deleteButton->setDisabled(false);
    ui->doneButton->setDisabled(false);
    ui->upButton->setDisabled(false);
    ui->downButton->setDisabled(false);

    if (ui->todoList->currentItem() == nullptr)
    {
        ui->deleteButton->setDisabled(true);
        ui->doneButton->setDisabled(true);
        ui->upButton->setDisabled(true);
        ui->downButton->setDisabled(true);
    }

    int currentRow = ui->todoList->currentRow();

    if (currentRow == 0)
    {
        ui->upButton->setDisabled(true);
    }

    if (currentRow == ui->todoList->count() - 1)
    {
        ui->downButton->setDisabled(true);
    }
}

void MainWindow::deleteClicked()
{
    if (ui->todoList->currentItem() == nullptr)
    {
        qDebug() << "Tried to delete with no item selected, bug!";
        return;
    }

    auto takenItem = ui->todoList->takeItem(ui->todoList->currentRow());
    delete takenItem;
}

void MainWindow::upClicked()
{}

void MainWindow::downClicked()
{}

std::unique_ptr<QFile> MainWindow::openFile(const QString &filename)
{
    if (filename.isEmpty())
    {
        return nullptr;
    }

    if (!filename.endsWith(".todo", Qt::CaseInsensitive))
    {
        QMessageBox::information(this, "Soft error", "The specified file doesn't end with \".todo\", refusing to open.");
        return {};
    }

    auto file = std::make_unique<QFile>(filename);
    file->open(QIODevice::ReadOnly);
    if (file->isOpen())
    {
        return file;
    }

    QMessageBox::information(this, "Soft error", "Couldn't open the specified file. Is the path correct?");
    return nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}
