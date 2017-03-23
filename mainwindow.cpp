#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"
#include "placeholder.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>

#include <memory>

MainWindow::MainWindow(const QString& filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearList()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addTodoClicked()));
    connect(ui->todoList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteClicked()));
//    connect(ui->upButton, SIGNAL(clicked(bool)), this, SLOT(upClicked()));
//    connect(ui->downButton, SIGNAL(clicked(bool)), this, SLOT(downClicked()));
    connect(ui->actionOpenFromHere, SIGNAL(triggered(bool)), this, SLOT(openFromHereClicked()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openClicked()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveClicked()));
    connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveAsClicked()));

    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(selectedItemChanged()));

    auto file = openFile(filename);
    if (file)
    {
        parseFile(*file);
    }

    selectedItemChanged();
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
    if (ui->todoList->count() != 0)
    {
        auto buttonState = QMessageBox::question(this, "Question",
                                                 "Current list contains some items, do you wish to discard it?",
                                                 QMessageBox::StandardButton::Yes| QMessageBox::StandardButton::No,
                                                 QMessageBox::StandardButton::No);
        if (buttonState == QMessageBox::StandardButton::Yes)
        {
            ui->todoList->clear();
        }
    }

    QTextStream stream(&file);
    QString buffer;

    while (!stream.atEnd())
    {
        buffer = stream.readLine(256);
        ui->todoList->addItem(buffer);
    }
}

void MainWindow::selectedItemChanged()
{
    ui->deleteButton->setDisabled(false);
//    ui->doneButton->setDisabled(false);
//    ui->upButton->setDisabled(false);
//    ui->downButton->setDisabled(false);

    if (ui->todoList->currentItem() == nullptr)
    {
        ui->deleteButton->setDisabled(true);
//        ui->doneButton->setDisabled(true);
//        ui->upButton->setDisabled(true);
//        ui->downButton->setDisabled(true);
    }

//    int currentRow = ui->todoList->currentRow();

//    if (currentRow == 0)
//    {
//        ui->upButton->setDisabled(true);
//    }

//    if (currentRow == ui->todoList->count() - 1)
//    {
//        ui->downButton->setDisabled(true);
//    }
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

void MainWindow::openFromHereClicked()
{
    auto file = openFile("todolist.todo");
    if (file)
    {
        parseFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft Error",
                             "Couldn't open the file from current directory. Does the directory have strict permissions?");
}

void MainWindow::openClicked()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open", {}, "*.todo");
    if (filename == "")
    {
        return;
    }

    auto file = openFile(filename);
    if (file)
    {
        parseFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft Error",
                             "Couldn't open the file chosen. Does the directory/folder have strict permissions?");
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
    file->open(QIODevice::ReadWrite);
    if (file->isOpen())
    {
        return file;
    }

    QMessageBox::information(this, "Soft error", "Couldn't open the specified file. Is the path correct?");
    return nullptr;
}

void MainWindow::saveClicked()
{
    auto file = openFile("todolist.todo");
    if (file)
    {
        writeIntoFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft error", "Couldn't create/open file \"todolist.todo\" in current directory. "
                                   "Does the directory/file has strict permissions?");
}

void MainWindow::saveAsClicked()
{
//    QString filename;
//    auto fileDialog = new QFileDialog(this, "Save as", "", "*.todo");
//    fileDialog->setFileMode(QFileDialog::FileMode::AnyFile);
//    Placeholder pholder;
//    connect(fileDialog, SIGNAL(fileSelected(QString)), &pholder, SLOT(marshalFilename(QString)));
//    fileDialog->exec();

    auto filename = QFileDialog::getSaveFileName(this, "Save as", "", "*.todo");

    if (filename == "")
    {
        return;
    }

    auto file = openFile(filename);
    if (file)
    {
        writeIntoFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft error", "Couldn't create/open file chosen. "
                                   "Does the directory/file have strict permissions?");
}

void MainWindow::writeIntoFile(QFile &file)
{
    QTextStream stream(&file);

    int listLength = ui->todoList->count();
    for (int i = 0; i < listLength; ++i)
    {
        stream << ui->todoList->item(i)->text() << '\n';
    }

    QMessageBox::information(this, "Notification", "Saved the list successfully");
}

MainWindow::~MainWindow()
{
    delete ui;
}
