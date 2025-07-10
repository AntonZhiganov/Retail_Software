#include "removedproductswindow.h"
#include "ui_removedproductswindow.h"
#include "mainwindow.h"

RemovedProductsWindow::RemovedProductsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RemovedProductsWindow)
{
    ui->setupUi(this);
}

RemovedProductsWindow::~RemovedProductsWindow()
{
    delete ui;
}

void RemovedProductsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
