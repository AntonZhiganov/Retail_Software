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

    for (int pos = 0; pos < nameEdits.size(); ++pos) {
        QString name = nameEdits[pos]->text().trimmed();;
        bool priceOk;
        double price = priceEdits[pos]->text().toDouble(&priceOk);
        if (!priceOk) {
            qDebug() << "Invalid price input at index" << pos;
            continue;
        }

        int quantity = quantitySpinBoxes[pos]->value();
        QString date = ui->orderDateEdit->date().toString("yyyy-MM-dd");

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
}

void AddProductWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
