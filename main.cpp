#include "mainwindow.h"
#include "addnewdialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filename;
    if (argc == 2)
    {
        filename = argv[1];
    }

    MainWindow w(filename);
    w.show();

    return a.exec();
}
