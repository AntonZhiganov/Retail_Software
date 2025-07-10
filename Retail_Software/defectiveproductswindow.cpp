#include "defectiveproductswindow.h"
#include "ui_defectiveproductswindow.h"
#include "mainwindow.h"
#include "removedproductswindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DefectiveProductsWindow::DefectiveProductsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DefectiveProductsWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

DefectiveProductsWindow::~DefectiveProductsWindow()
{
    delete ui;
}

void DefectiveProductsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}

void DefectiveProductsWindow::on_confirmPushButton_clicked()
{

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QString productName = ui->productNameLineEdit->text().trimmed();
    double quantityToRemove = ui->productQuantityLineEdit->text().toDouble();

    if (productName.isEmpty() || quantityToRemove <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid product name and quantity.");
        return;
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT quantity FROM products WHERE name = ?");
    checkQuery.addBindValue(productName);

    if (!checkQuery.exec()) {
        qDebug() << "Error checking product:" << checkQuery.lastError().text();
        return;
    }

    if (checkQuery.next()) {
        int currentQuantity = checkQuery.value(0).toInt();

        if (currentQuantity < quantityToRemove) {
            QMessageBox::warning(this, "Not Enough Stock", "There is not enough product to remove.");
            return;
        }

        int newQuantity = currentQuantity - quantityToRemove;

        QSqlQuery updateQuery(db);
        if (newQuantity == 0) {
            updateQuery.prepare("DELETE FROM products WHERE name = ?");
            updateQuery.addBindValue(productName);
        }

        else {
            updateQuery.prepare("UPDATE products SET quantity = ? WHERE name = ?");
            updateQuery.addBindValue(newQuantity);
            updateQuery.addBindValue(productName);
        }

        if (!updateQuery.exec()) {
            qDebug() << "Failed to update/remove product:" << updateQuery.lastError().text();
        }

        QString comment = ui->resonLineEdit->text().trimmed();
        QString date = ui->dateEdit->date().toString("yyyy-MM-dd");

        QSqlQuery insertRemoved(db);
        insertRemoved.prepare(R"(
            INSERT INTO RemovedProducts (name, quantity, comment, date)
            VALUES (?, ?, ?, ?)
        )");
        insertRemoved.addBindValue(productName);
        insertRemoved.addBindValue(quantityToRemove);
        insertRemoved.addBindValue(comment);
        insertRemoved.addBindValue(date);

        if (!insertRemoved.exec()) {
            qDebug() << "Failed to insert into RemovedProducts:" << insertRemoved.lastError().text();
        }

        else {

            RemovedProductsWindow *removedproductwindow = new RemovedProductsWindow();
            removedproductwindow ->show();
            this -> hide();
        }

    } else {
        QMessageBox::warning(this, "Product Not Found", "No product with that name was found.");
    }
}
