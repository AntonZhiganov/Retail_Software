#include "defectiveproductswindow.h"
#include "ui_defectiveproductswindow.h"
#include "mainwindow.h"

DefectiveProductsWindow::DefectiveProductsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DefectiveProductsWindow)
{
    ui->setupUi(this);
}

DefectiveProductsWindow::~DefectiveProductsWindow()
{
    delete ui;
}

void DefectiveProductsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
