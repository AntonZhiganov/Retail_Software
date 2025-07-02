#include "additionalexpenseswindow.h"
#include "ui_additionalexpenseswindow.h"

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
