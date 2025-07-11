#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "warehosewindow.h"
#include "addproductwindow.h"
#include "incomeandexpenseswindow.h"
#include "additionalexpenseswindow.h"
#include "salewindow.h"
#include "removedproductswindow.h"
#include "informationaboutclientswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_purchaseButton_clicked()
{
    AddProductWindow *addproductwindow = new AddProductWindow();
    addproductwindow->show();
    this -> hide();
}

void MainWindow::on_additionalExpensesButton_clicked()
{
    AdditionalExpensesWindow * additionalexpenseswindow = new AdditionalExpensesWindow();
    additionalexpenseswindow->show();
    this -> hide();
}

void MainWindow::on_saleButton_clicked()
{
    SaleWindow *salewindow = new SaleWindow();
    salewindow->show();
    this -> hide();
}

void MainWindow::on_checkButton_clicked()
{

    WarehoseWindow *warehosewindow = new WarehoseWindow();
    warehosewindow->show();
    this -> hide();

}

void MainWindow::on_incomeAndExpensesButton_clicked()
{
    IncomeAndExpensesWindow *incomeandexpenseswindow = new IncomeAndExpensesWindow();
    incomeandexpenseswindow ->show();
    this -> hide();
}

void MainWindow::on_clientsButton_clicked()
{
    InformationAboutClientsWindow *informationaboutclientsWindow = new InformationAboutClientsWindow();
    informationaboutclientsWindow ->show();
    this -> hide();
}

void MainWindow::on_ordersButton_clicked()
{
    RemovedProductsWindow *removedproductwindow = new RemovedProductsWindow();
    removedproductwindow ->show();
    this -> hide();
}
