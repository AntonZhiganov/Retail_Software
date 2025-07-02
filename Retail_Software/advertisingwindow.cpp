#include "advertisingwindow.h"
#include "ui_advertisingwindow.h"

AdvertisingWindow::AdvertisingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdvertisingWindow)
{
    ui->setupUi(this);
}

AdvertisingWindow::~AdvertisingWindow()
{
    delete ui;
}
