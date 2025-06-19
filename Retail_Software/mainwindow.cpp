#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "warehosewindow.h"
#include "addproductwindow.h"

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
 // buttons
void MainWindow::on_purchaseButton_clicked()
{
    AddProductWindow *addproductwindow = new AddProductWindow();
    addproductwindow->show();
    this -> hide();
}

void MainWindow::on_additionalExpensesButton_clicked()
{
    QMessageBox::information(this, "Additional expenses", "'Additional expenses' pressed");
}

void MainWindow::on_saleButton_clicked()
{
    QMessageBox::information(this, "Sale", "'Sale' pressed");
}

void MainWindow::on_checkButton_clicked()
{

    WarehoseWindow *warehosewindow = new WarehoseWindow();
    warehosewindow->show();
    this -> hide();                                                                              //Hides the main window

}

void MainWindow::on_incomeAndExpensesButton_clicked()
{
    QMessageBox::information(this, "Income and expenses", "'Information about income and expenses' pressed");
}

void MainWindow::on_clientsButton_clicked()
{
    QMessageBox::information(this, "Information about clients", "'Information about clients' pressed");
}

void MainWindow::on_ordersButton_clicked()
{
    QMessageBox::information(this, "Orders", "'Orders' pressed");
}
