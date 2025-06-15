#include "warehosewindow.h"
#include "ui_warehosewindow.h"

WarehoseWindow::WarehoseWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WarehoseWindow)
{
    ui->setupUi(this);
}

WarehoseWindow::~WarehoseWindow()
{
    delete ui;
}
