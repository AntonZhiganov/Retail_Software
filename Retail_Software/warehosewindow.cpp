#include "warehosewindow.h"
#include "ui_warehosewindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

WarehoseWindow::WarehoseWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WarehoseWindow)
{
    ui->setupUi(this);

    QFont font = ui->productTableWidget->font();
    font.setPointSize(10);
    ui->productTableWidget->setFont(font);

    ui->productTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->productTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    openDB();
    loadProducts();
}

WarehoseWindow::~WarehoseWindow()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

void WarehoseWindow::openDB()
{
    db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isValid()) {
        qDebug() << "Database connection not found!";
        return;
    }

    if (!db.open()) {
        qDebug() << "Could not open the database:" << db.lastError().text();
        return;
    }
    qDebug() << "Database opened successfully";

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS products");
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            quantity INTEGER,
            price INTEGER,
            date TEXT
        )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table created or already exists";
    }
}

void WarehoseWindow::loadProducts()
{
    QSqlQuery query(db);
    if (!query.exec("SELECT id, name, quantity, price, date FROM products")) {
        qDebug() << "Failed to fetch products:" << query.lastError().text();
        return;
    }

    ui->productTableWidget->clear();
    ui->productTableWidget->setColumnCount(5);
    ui->productTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Quantity", "Price", "Date"});
    ui->productTableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->productTableWidget->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            ui->productTableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}
