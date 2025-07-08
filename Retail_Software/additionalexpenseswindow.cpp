#include "additionalexpenseswindow.h"
#include "ui_additionalexpenseswindow.h"
#include "advertisingwindow.h"
#include "mainwindow.h"
#include "defectiveproductswindow.h"
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

AdditionalExpensesWindow::AdditionalExpensesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdditionalExpensesWindow)
{
    ui->setupUi(this);
}

AdditionalExpensesWindow::~AdditionalExpensesWindow()
{
    delete ui;
}

void AdditionalExpensesWindow::on_defectiveProductPushButton_clicked()
{
    DefectiveProductsWindow * defectiveproductswindow = new DefectiveProductsWindow();
    defectiveproductswindow->show();
    this -> hide();
}

void AdditionalExpensesWindow::on_advertisingPushButton_clicked()
{
    AdvertisingWindow * advertisingwindow = new AdvertisingWindow();
    advertisingwindow->show();
    this -> hide();
}

void AdditionalExpensesWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
