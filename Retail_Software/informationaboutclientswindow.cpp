#include "informationaboutclientswindow.h"
#include "ui_informationaboutclientswindow.h"
#include "mainwindow.h"

InformationAboutClientsWindow::InformationAboutClientsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationAboutClientsWindow)
{
    ui->setupUi(this);
}

InformationAboutClientsWindow::~InformationAboutClientsWindow()
{
    delete ui;
}

void InformationAboutClientsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();

}
