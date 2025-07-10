#include "salewindow.h"
#include "ui_salewindow.h"
#include "mainwindow.h"

SaleWindow::SaleWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SaleWindow)
{
    ui->setupUi(this);
}

SaleWindow::~SaleWindow()
{
    delete ui;
}

void SaleWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
