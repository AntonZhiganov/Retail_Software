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

    QLabel* sellingPriceName = new QLabel(QString("selling price %1 (for everyone)").arg(index), this);
    QLineEdit* sellingPriceLineEdit = new QLineEdit(this);

    QLabel* nameName = new QLabel(QString("Product name %1").arg(index), this);
    QLineEdit* nameLineEdit = new QLineEdit(this);

    QLabel* priceName = new QLabel(QString("Product price %1 (for everyone)").arg(index), this);
    QLineEdit* priceLineEdit = new QLineEdit(this);

    QLabel* quantityName = new QLabel(QString("Product quantity %1").arg(index), this);
    QSpinBox* quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(1000);

    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("background-color: black; height: 3px; margin: 10px 0; border: none;");

    ui->verticalLayout->insertWidget(0, separator);
    ui->verticalLayout->insertWidget(0, sellingPriceLineEdit);
    ui->verticalLayout->insertWidget(0, sellingPriceName);
    ui->verticalLayout->insertWidget(0, quantitySpinBox);
    ui->verticalLayout->insertWidget(0, quantityName);
    ui->verticalLayout->insertWidget(0, priceLineEdit);
    ui->verticalLayout->insertWidget(0, priceName);
    ui->verticalLayout->insertWidget(0, nameLineEdit);
    ui->verticalLayout->insertWidget(0, nameName);

    nameEdits.append(nameLineEdit);
    priceEdits.append(priceLineEdit);
    sellingPriceEdits.append(sellingPriceLineEdit);
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
        bool sellingPriceOk;
        double sellingPrice = sellingPriceEdits[pos]->text().toDouble(&sellingPriceOk);
        if (!sellingPriceOk) {
            qDebug() << "Invalid selling price input at index" << pos;
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
            updateQuery.prepare("UPDATE products SET quantity = ?, price = ?, selling_price = ?, date = ? WHERE name = ?");
            updateQuery.addBindValue(newQuantity);
            updateQuery.addBindValue(price);
            updateQuery.addBindValue(sellingPrice);
            updateQuery.addBindValue(date);
            updateQuery.addBindValue(name);

            if (!updateQuery.exec()) {
                qDebug() << "Failed to update product:" << updateQuery.lastError().text();
            } else {
                qDebug() << "Product updated successfully:" << name;
            }
        } else {
            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO products (name, quantity, price, selling_price, date) VALUES (?, ?, ?, ?, ?)");
            insertQuery.addBindValue(name);
            insertQuery.addBindValue(quantity);
            insertQuery.addBindValue(price);
            insertQuery.addBindValue(sellingPrice);
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
        double lastTotalEarned = 0;

        QSqlQuery lastQuery(db);
        if (lastQuery.exec("SELECT income, total_spent, total_earned FROM incomeAndExpenses ORDER BY id DESC LIMIT 1") && lastQuery.next()) {
            lastIncome = lastQuery.value(0).toDouble();
            lastTotalSpent = lastQuery.value(1).toDouble();
            lastTotalEarned = lastQuery.value(2).toDouble();
        }

        double newTotalSpent = lastTotalSpent + totalGoodsSpent;
        double totalEarned = lastTotalEarned;
        double income = lastIncome - totalGoodsSpent;

        if (commentString.endsWith(", ")) {
            commentString.chop(2);
        }

        QSqlQuery insertExpenseQuery(db);
        insertExpenseQuery.prepare(R"(
            INSERT INTO incomeAndExpenses
            (total_spent, total_earned, spent_or_earned_for_this_order, income, comment, date)
            VALUES (?, ?, ?, ?, ?, ?)
        )");

        QString spentThisOrder = "-" + QString::number(totalGoodsSpent, 'f', 2);

        insertExpenseQuery.addBindValue(QString::number(newTotalSpent, 'f', 2));
        insertExpenseQuery.addBindValue(QString::number(totalEarned, 'f', 2));
        insertExpenseQuery.addBindValue(spentThisOrder);
        insertExpenseQuery.addBindValue(QString::number(income, 'f', 2));
        insertExpenseQuery.addBindValue(commentString);
        insertExpenseQuery.addBindValue(date);

        if (!insertExpenseQuery.exec()) {
            qDebug() << "Failed to insert total expense:" << insertExpenseQuery.lastError().text();
        } else {
            qDebug() << "Total expense for order recorded: -" << totalGoodsSpent;
            qDebug() << "New total_spent: " << newTotalSpent;
            qDebug() << "New income: " << income;
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
