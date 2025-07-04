#include "additionalexpenseswindow.h"
#include "ui_additionalexpenseswindow.h"
#include "advertisingwindow.h"
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
    QMessageBox::information(this, "Defective Product", "'Defective Product' pressed");
}

void AdditionalExpensesWindow::on_advertisingPushButton_clicked()
{
   AdvertisingWindow * advertisingwindow = new AdvertisingWindow();
    advertisingwindow->show();
    this -> hide();
}
