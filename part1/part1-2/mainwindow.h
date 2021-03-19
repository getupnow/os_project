#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFrame>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFrame frame;
    QPushButton buttonClickMe {&frame};

};

#endif // MAINWINDOW_H
