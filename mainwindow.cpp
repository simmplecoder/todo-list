#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <memory>

MainWindow::MainWindow(const QString& filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->clearButton, SIGNAL(released()), this, SLOT(clearList()));
    connect(ui->addButton, SIGNAL(released()), this, SLOT(addTodoClicked()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveClicked()));

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
