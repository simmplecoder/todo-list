#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& filename = "", QWidget* parent = 0);
    ~MainWindow();

public slots:
    void clearList();
    void addTodoClicked();
    void saveClicked();

private:
    Ui::MainWindow *ui;

private:
    void parseFile(QFile& file);
    std::unique_ptr<QFile> openFile(const QString& filename);
};

#endif // MAINWINDOW_H
