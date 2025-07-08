#include "addproductwindow.h"
#include "ui_addproductwindow.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AddProductWindow::AddProductWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddProductWindow)
{
    ui->setupUi(this);
    ui->orderDateEdit->setDate(QDate::currentDate());

    scrollLayout = new QVBoxLayout;
    ui->productsWidget->setLayout(scrollLayout);
}

AddProductWindow::~AddProductWindow()
{
    delete ui;
}

void AddProductWindow::on_newProductTypePushButton_clicked()
{
    int index = nameEdits.size() + 1;

    QLabel* nameName = new QLabel(QString("Product name %1").arg(index), this);
    QLineEdit* nameLineEdit = new QLineEdit(this);

    QLabel* priceName = new QLabel(QString("Product price %1 (for everyone)").arg(index), this);
    QLineEdit* priceLineEdit = new QLineEdit(this);

    QLabel* quantityName = new QLabel(QString("Product quantity %1").arg(index), this);
    QSpinBox* quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(1000);

    ui->verticalLayout->insertWidget(0, quantitySpinBox);
    ui->verticalLayout->insertWidget(0, quantityName);
    ui->verticalLayout->insertWidget(0, priceLineEdit);
    ui->verticalLayout->insertWidget(0, priceName);
    ui->verticalLayout->insertWidget(0, nameLineEdit);
    ui->verticalLayout->insertWidget(0, nameName);

    nameEdits.append(nameLineEdit);
    priceEdits.append(priceLineEdit);
    quantitySpinBoxes.append(quantitySpinBox);
}

void AddProductWindow::on_confirmOrderPushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    double totalGoodsSpent = 0;
    QString commentString;
    QString date = ui->orderDateEdit->date().toString("yyyy-MM-dd");

    for (int pos = 0; pos < nameEdits.size(); ++pos) {
        QString name = nameEdits[pos]->text().trimmed();
        bool priceOk;
        double price = priceEdits[pos]->text().toDouble(&priceOk);
        if (!priceOk) {
            qDebug() << "Invalid price input at index" << pos;
            continue;
        }

        int quantity = quantitySpinBoxes[pos]->value();
        if (quantity == 0) {
            continue;
        }

        totalGoodsSpent += price * quantity;
        commentString += name + " x" + QString::number(quantity) + ", ";

        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT quantity FROM products WHERE name = ?");
        checkQuery.addBindValue(name);

        if (checkQuery.exec() && checkQuery.next()) {
            int existingQuantity = checkQuery.value(0).toInt();
            int newQuantity = existingQuantity + quantity;

            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE products SET quantity = ?, price = ?, date = ? WHERE name = ?");
            updateQuery.addBindValue(newQuantity);
            updateQuery.addBindValue(price);
            updateQuery.addBindValue(date);
            updateQuery.addBindValue(name);

            if (!updateQuery.exec()) {
                qDebug() << "Failed to update product:" << updateQuery.lastError().text();
            } else {
                qDebug() << "Product updated successfully:" << name;
            }
        } else {
            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO products (name, quantity, price, date) VALUES (?, ?, ?, ?)");
            insertQuery.addBindValue(name);
            insertQuery.addBindValue(quantity);
            insertQuery.addBindValue(price);
            insertQuery.addBindValue(date);

            if (!insertQuery.exec()) {
                qDebug() << "Failed to insert product:" << insertQuery.lastError().text();
            } else {
                qDebug() << "Product inserted successfully:" << name;
            }
        }
    }

    if (totalGoodsSpent > 0) {
        double lastIncome = 0;
        double lastTotalSpent = 0;
        double spentOnGoods = totalGoodsSpent;

        QSqlQuery lastQuery(db);
        if (lastQuery.exec("SELECT Income, Total_spent, spent_on_goods FROM incomeAndExpenses ORDER BY id DESC LIMIT 1") && lastQuery.next()) {
            lastIncome = lastQuery.value(0).toDouble();
            lastTotalSpent = lastQuery.value(1).toDouble();
        }


        double spentOnAdvertising = 0;
        double newSpentOnGoods = spentOnGoods;
        double newTotalSpent = lastTotalSpent + totalGoodsSpent;
        double totalEarned = 0;
        double income = lastIncome - totalGoodsSpent;

        if (commentString.endsWith(", ")) {
            commentString.chop(2);
        }

        QSqlQuery insertExpenseQuery(db);
        insertExpenseQuery.prepare(R"(
            INSERT INTO incomeAndExpenses
            (Total_spent, Total_earned, spent_on_advertising, spent_on_goods, Income, Comment, date)
            VALUES (?, ?, ?, ?, ?, ?, ?)
        )");

        insertExpenseQuery.addBindValue(newTotalSpent);
        insertExpenseQuery.addBindValue(totalEarned);
        insertExpenseQuery.addBindValue(spentOnAdvertising);
        insertExpenseQuery.addBindValue(newSpentOnGoods);
        insertExpenseQuery.addBindValue(income);
        insertExpenseQuery.addBindValue(commentString);
        insertExpenseQuery.addBindValue(date);

        if (!insertExpenseQuery.exec()) {
            qDebug() << "Failed to insert total expense:" << insertExpenseQuery.lastError().text();
        } else {
            qDebug() << "Total expense for order recorded: -" << totalGoodsSpent;
            qDebug() << "New Total_spent: " << newTotalSpent;
            qDebug() << "New spent_on_goods: " << newSpentOnGoods;
            qDebug() << "New Income: " << income;
        }
    }

    WarehoseWindow *warehosewindow = new WarehoseWindow();
    warehosewindow->show();
    this -> hide();
}


void AddProductWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
