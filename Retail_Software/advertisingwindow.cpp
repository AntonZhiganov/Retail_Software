#include "advertisingwindow.h"
#include "ui_advertisingwindow.h"
#include "advertisinginformationwindow.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

AdvertisingWindow::AdvertisingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdvertisingWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

AdvertisingWindow::~AdvertisingWindow()
{
    delete ui;
}

void AdvertisingWindow::on_viewDataPushButton_clicked(){

    AdvertisingInformationWindow *advertisinginformationwindow = new AdvertisingInformationWindow;
    advertisinginformationwindow->show();
    (this) -> hide();
}

void AdvertisingWindow::on_confirmAdvertisingPushButton_clicked(){

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QString seller = ui->advertisingSellerLineEdit->text().trimmed();
    bool okPrice;
    double price = ui->priceLineEdit->text().toDouble(&okPrice);
    bool okImpressions;
    int impressions = ui->totalImpressionLineEdit->text().toInt(&okImpressions);
    bool okClients;
    int clients = ui->newClientsLineEdit->text().toInt(&okClients);
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");

    QSqlQuery query(db);
    query.prepare("INSERT INTO advertising (seller, price, impressions, clients, date) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(seller);
    query.addBindValue(price);
    query.addBindValue(impressions);
    query.addBindValue(clients);
    query.addBindValue(date);

    if (!query.exec()) {
        qDebug() << "Failed to insert advertising row:" << query.lastError().text();
    } else {
        qDebug() << "Advertising record added!";
    }


}

void AdvertisingWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();

}
