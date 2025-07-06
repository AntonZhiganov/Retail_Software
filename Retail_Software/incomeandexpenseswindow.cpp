#include "incomeandexpenseswindow.h"
#include "ui_incomeandexpenseswindow.h"
#include "mainwindow.h"

IncomeAndExpensesWindow::IncomeAndExpensesWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IncomeAndExpensesWindow)
{
    ui->setupUi(this);
}

IncomeAndExpensesWindow::~IncomeAndExpensesWindow()
{
    delete ui;
}

void IncomeAndExpensesWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
