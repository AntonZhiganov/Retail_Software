#include "orderswindow.h"
#include "ui_orderswindow.h"
#include "mainwindow.h"

OrdersWindow::OrdersWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrdersWindow)
{
    ui->setupUi(this);
}

OrdersWindow::~OrdersWindow()
{
    delete ui;
}

void OrdersWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
