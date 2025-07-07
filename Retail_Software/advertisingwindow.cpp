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

void AdvertisingWindow::on_confirmAdvertisingPushButton_clicked()
{
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

    if (!okPrice || !okImpressions || !okClients || seller.isEmpty()) {
        QMessageBox::warning(this, "Input error", "Please fill in all fields correctly.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO advertising (seller, price, impressions, clients, date) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(seller);
    query.addBindValue(price);
    query.addBindValue(impressions);
    query.addBindValue(clients);
    query.addBindValue(date);

    if (!query.exec()) {
        qDebug() << "Failed to insert advertising row:" << query.lastError().text();
        return;
    } else {
        qDebug() << "Advertising record added!";
    }

    double lastIncome = 0;
    double lastTotalSpent = 0;
    double lastSpentOnAdvertising = 0;

    QSqlQuery lastQuery(db);
    if (lastQuery.exec("SELECT Income, Total_spent, spent_on_advertising FROM incomeAndExpenses ORDER BY id DESC LIMIT 1") && lastQuery.next()) {
        lastIncome = lastQuery.value(0).toDouble();
        lastTotalSpent = lastQuery.value(1).toDouble();
        lastSpentOnAdvertising = lastQuery.value(2).toDouble();
    }

    double newSpentOnAdvertising = lastSpentOnAdvertising + price;
    double newTotalSpent = lastTotalSpent + price;
    double income = lastIncome - price;
    double totalEarned = 0;
    double spentOnGoods = 0;
    QString comment = QString("Advertising: seller %1").arg(seller);

    QSqlQuery insertExpenseQuery(db);
    insertExpenseQuery.prepare(R"(
        INSERT INTO incomeAndExpenses
        (Total_spent, Total_earned, spent_on_advertising, spent_on_goods, Income, Comment, date)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

    insertExpenseQuery.addBindValue(newTotalSpent);
    insertExpenseQuery.addBindValue(totalEarned);
    insertExpenseQuery.addBindValue(newSpentOnAdvertising);
    insertExpenseQuery.addBindValue(spentOnGoods);
    insertExpenseQuery.addBindValue(income);
    insertExpenseQuery.addBindValue(comment);
    insertExpenseQuery.addBindValue(date);

    if (!insertExpenseQuery.exec()) {
        qDebug() << "Failed to insert expense record for advertising:" << insertExpenseQuery.lastError().text();
    } else {
        qDebug() << "Advertising expense recorded in incomeAndExpenses.";
        qDebug() << "New Total_spent: " << newTotalSpent;
        qDebug() << "New spent_on_advertising: " << newSpentOnAdvertising;
        qDebug() << "New Income: " << income;
    }
}

void AdvertisingWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();

}
