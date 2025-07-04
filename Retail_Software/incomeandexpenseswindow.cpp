#include "incomeandexpenseswindow.h"
#include "ui_incomeandexpenseswindow.h"

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

