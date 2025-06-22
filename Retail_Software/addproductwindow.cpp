#include "addproductwindow.h"
#include "ui_addproductwindow.h"
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

    scrollLayout = new QVBoxLayout;
    ui->productsWidget->setLayout(scrollLayout);


    connect(ui->newProductTypeButton, &QPushButton::clicked, this, &AddProductWindow::onNewProductTypeButtonClicked);

     connect(ui->confirmOrderButton, &QPushButton::clicked, this, &AddProductWindow::onConfirmOrderButtonClicked);
}

AddProductWindow::~AddProductWindow()
{
    delete ui;
}

void AddProductWindow::onNewProductTypeButtonClicked()
{
    int index = nameEdits.size() + 1;

    QLabel* nameName = new QLabel(QString("Product name %1").arg(index), this);
    QLineEdit* nameLineEdit = new QLineEdit(this);

    QLabel* priceName = new QLabel(QString("Product price %1 (for everyone)").arg(index), this);
    QLineEdit* priceLineEdit = new QLineEdit(this);

    QLabel* quantityName = new QLabel(QString("Product quantity %1").arg(index), this);
    QSpinBox* quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(100);

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

void AddProductWindow::onConfirmOrderButtonClicked()
{
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
}

    QSqlQuery query(db);

    for (int pos = 0; pos < nameEdits.size(); ++pos) {
        QString name = nameEdits[pos]->text();
        bool priceOk;
        int purchase = priceEdits[pos]->text().toInt(&priceOk);
        if (!priceOk) {
            qDebug() << "Invalid price input at index" << pos;
            continue;
        }

        int quantity = quantitySpinBoxes[pos]->value();

        QString date = QDate::currentDate().toString("yyyy-MM-dd");

        query.prepare("INSERT INTO products (name, quantity, purchase, sale, date) VALUES (?, ?, ?, 0, ?)");
        query.addBindValue(name);
        query.addBindValue(quantity);
        query.addBindValue(purchase);
        query.addBindValue(date);

        if (!query.exec()) {
            qDebug() << "Failed to insert product:" << query.lastError().text();
        } else {
            qDebug() << "Product inserted successfully:" << name;
        }

    }

}
