#include "orderswindow.h"
#include "ui_orderswindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QDate>

OrdersWindow::OrdersWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrdersWindow)
{
    ui->setupUi(this);
    ui->ordersTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ordersTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS orders");
    QString createOrdersTableQuery = R"(
        CREATE TABLE IF NOT EXISTS orders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            client TEXT,
            purchases TEXT,
            spent DOUBLE,
            date TEXT
        )
    )";

    if (!query.exec(createOrdersTableQuery)) {
        qDebug() << "Error creating orders table:" << query.lastError().text();
    } else {
        qDebug() << "Orders table created or already exists";
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM orders");

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count == 0) {
            QSqlQuery insertQuery(db);
            insertQuery.prepare(R"(
            INSERT INTO orders
            (client, purchases, spent, date)
            VALUES (?, ?, ?, ?)
        )");

            insertQuery.addBindValue("-");
            insertQuery.addBindValue("-");
            insertQuery.addBindValue(0);
            insertQuery.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

            if (!insertQuery.exec()) {
                qDebug() << "Failed to insert initial row:" << insertQuery.lastError().text();
            } else {
                qDebug() << "Initial zero row inserted into orders";
            }
        }
    }

    loadData();
}

void OrdersWindow::loadData() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not open!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT client, purchases, spent, date FROM orders ORDER BY id DESC")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->ordersTableWidget->setRowCount(0);
    QStringList headers = {"Client name", "Purchases", "Spent", "Date"};
    ui->ordersTableWidget->setColumnCount(headers.size());
    ui->ordersTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->ordersTableWidget->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->ordersTableWidget->setItem(row, col, item);
        }
        ++row;
    }
}

OrdersWindow::~OrdersWindow()
{
    delete ui;
}

void OrdersWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
