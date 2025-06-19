#include "addproductwindow.h"
#include "ui_addproductwindow.h"

AddProductWindow::AddProductWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddProductWindow)
{
    ui->setupUi(this);
}

AddProductWindow::~AddProductWindow()
{
    delete ui;
}
