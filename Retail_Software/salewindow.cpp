#include "salewindow.h"
#include "ui_salewindow.h"
#include "mainwindow.h"
#include "informationaboutclientswindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>
#include <QInputDialog>
#include <QMessageBox>

SaleWindow::SaleWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SaleWindow)
{
    ui->setupUi(this);

    QStringList headers = {"Product", "Price", "In Stock", "Quantity to Sell", "Select"};
    ui->saleTableWidget->setColumnCount(headers.size());
    ui->saleTableWidget->setHorizontalHeaderLabels(headers);
    ui->saleTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    loadProducts();
}


SaleWindow::~SaleWindow()
{
    delete ui;
}

void SaleWindow::loadProducts() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "DB not open!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT name, selling_price, quantity FROM products")) {
        qDebug() << "Query error:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->saleTableWidget->insertRow(row);

        QString name = query.value(0).toString();
        double price = query.value(1).toDouble();
        int stock = query.value(2).toInt();

        ui->saleTableWidget->setItem(row, 0, new QTableWidgetItem(name));
        ui->saleTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(price, 'f', 1)));
        ui->saleTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(stock)));

        QSpinBox *quantitySpin = new QSpinBox();
        quantitySpin->setRange(0, stock);
        ui->saleTableWidget->setCellWidget(row, 3, quantitySpin);

        QCheckBox *checkBox = new QCheckBox();
        ui->saleTableWidget->setCellWidget(row, 4, checkBox);

        ++row;
    }
}

void SaleWindow::on_confirmPushButton_clicked() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) return;

    QString clientName = QInputDialog::getText(this, "Client Name", "Enter client name:");
    double totalEarned = 0;
    QString comment;

    if (clientName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Client name cannot be empty.");
        return;
    }

    for (int row = 0; row < ui->saleTableWidget->rowCount(); ++row) {
        QCheckBox *cb = qobject_cast<QCheckBox *>(ui->saleTableWidget->cellWidget(row, 4));
        if (cb && cb->isChecked()) {
            QString name = ui->saleTableWidget->item(row, 0)->text();
            double sellingPrice = ui->saleTableWidget->item(row, 1)->text().toDouble();
            int inStock = ui->saleTableWidget->item(row, 2)->text().toInt();
            QSpinBox *spin = qobject_cast<QSpinBox *>(ui->saleTableWidget->cellWidget(row, 3));
            int toSell = spin ? spin->value() : 0;

            if (toSell <= 0 || toSell > inStock) continue;

            totalEarned += sellingPrice * toSell;
            comment += name + " x" + QString::number(toSell) + ", ";

            int newQuantity = inStock - toSell;
            QSqlQuery query(db);
            if (newQuantity == 0) {
                query.prepare("DELETE FROM products WHERE name = ?");
                query.addBindValue(name);
            } else {
                query.prepare("UPDATE products SET quantity = ? WHERE name = ?");
                query.addBindValue(newQuantity);
                query.addBindValue(name);
            }

            if (!query.exec()) {
                qDebug() << "Failed to update stock:" << query.lastError().text();
            } else {
                qDebug() << "Sold" << toSell << "of" << name;
            }
        }
    }

    if (comment.endsWith(", ")) comment.chop(2);
    comment = "Client: " + clientName + " — " + comment;

    if (totalEarned > 0) {
        double prevIncome = 0, prevSpent = 0, prevTotalEarned = 0;
        QSqlQuery lastQuery(db);
        if (lastQuery.exec("SELECT income, total_spent, total_earned FROM incomeAndExpenses ORDER BY id DESC LIMIT 1") && lastQuery.next()) {
            prevIncome = lastQuery.value(0).toDouble();
            prevSpent = lastQuery.value(1).toDouble();
            prevTotalEarned = lastQuery.value(2).toDouble();
        }

        double newIncome = prevIncome + totalEarned;
        double newTotalEarned = prevTotalEarned + totalEarned;
        QString date = QDate::currentDate().toString("yyyy-MM-dd");
        if (comment.endsWith(", ")) comment.chop(2);

        QSqlQuery insertQuery(db);
        insertQuery.prepare(R"(
            INSERT INTO incomeAndExpenses
            (total_spent, total_earned, spent_or_earned_for_this_order, income, comment, date)
            VALUES (?, ?, ?, ?, ?, ?)
        )");
        insertQuery.addBindValue(QString::number(prevSpent, 'f', 2));
        insertQuery.addBindValue(QString::number(newTotalEarned, 'f', 2));
        insertQuery.addBindValue("+" + QString::number(totalEarned, 'f', 2));
        insertQuery.addBindValue(QString::number(newIncome, 'f', 2));
        insertQuery.addBindValue(comment);
        insertQuery.addBindValue(date);

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting into incomeAndExpenses:" << insertQuery.lastError().text();
        } else {
            qDebug() << "Sale recorded";
        }
    }

    if (comment.endsWith(", ")) comment.chop(2);
    QString purchasesStr = comment.section(" — ", 1);
    QString communication = "Enter information";

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT id, total_spent, purchases FROM clients WHERE name = ?");
    checkQuery.addBindValue(clientName);

    if (checkQuery.exec() && checkQuery.next()) {
        int clientId = checkQuery.value("id").toInt();
        double oldSpent = checkQuery.value("total_spent").toDouble();
        QString oldPurchases = checkQuery.value("purchases").toString();

        double newSpent = oldSpent + totalEarned;
        QString newPurchases = oldPurchases;
        if (!newPurchases.trimmed().isEmpty() && !purchasesStr.trimmed().isEmpty()) {
            newPurchases += ", " + purchasesStr;
        } else if (!purchasesStr.trimmed().isEmpty()) {
            newPurchases = purchasesStr;
        }

        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE clients SET total_spent = ?, purchases = ? WHERE id = ?");
        updateQuery.addBindValue(newSpent);
        updateQuery.addBindValue(newPurchases);
        updateQuery.addBindValue(clientId);

        if (!updateQuery.exec()) {
            qDebug() << "Failed to update client:" << updateQuery.lastError().text();
        } else {
            qDebug() << "Client updated";
        }

    } else {
        QSqlQuery insertClientQuery(db);
        insertClientQuery.prepare(R"(
        INSERT INTO clients (name, total_spent, purchases, communication)
        VALUES (?, ?, ?, ?)
    )");
        insertClientQuery.addBindValue(clientName);
        insertClientQuery.addBindValue(totalEarned);
        insertClientQuery.addBindValue(purchasesStr);
        insertClientQuery.addBindValue(communication);

        if (!insertClientQuery.exec()) {
            qDebug() << "Failed to insert new client:" << insertClientQuery.lastError().text();
        } else {
            qDebug() << "New client added";
        }
    }

    QSqlQuery insertOrderQuery(db);
    insertOrderQuery.prepare(R"(
    INSERT INTO orders (client, purchases, spent, date)
    VALUES (?, ?, ?, ?)
    )");
    insertOrderQuery.addBindValue(clientName);
    insertOrderQuery.addBindValue(purchasesStr);
    insertOrderQuery.addBindValue(totalEarned);
    insertOrderQuery.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (!insertOrderQuery.exec()) {
        qDebug() << "Failed to insert order:" << insertOrderQuery.lastError().text();
    } else {
        qDebug() << "Order added to orders table.";
    }

    ui->saleTableWidget->setRowCount(0);
    loadProducts();

    InformationAboutClientsWindow *informationaboutclientswindow = new InformationAboutClientsWindow();
    informationaboutclientswindow  ->show();
    this -> hide();
}


void SaleWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();

}
