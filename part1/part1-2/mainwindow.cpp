#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buttonClickMe.setText("Click me");
    buttonClickMe.move(10, 10);
    connect(&buttonClickMe, &QPushButton::clicked, [&] {
        QMessageBox(QMessageBox::Icon::NoIcon, "", "Hello, World!").exec();
       });

      setCentralWidget(&frame);
      setWindowTitle("this is a title");
      resize(300, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
}
