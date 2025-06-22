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

    openDB();
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
    db = QSqlDatabase::addDatabase("QSQLITE", "WarehoseConnection");
    db.setDatabaseName("warehose.db");

    if (!db.open()) {
        qDebug() << "Could not open the database:" << db.lastError().text();
        return;
    }
    qDebug() << "Database opened successfully";

    QSqlQuery query(db);
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            quantity INTEGER,
            purchase INTEGER,
            sale INTEGER,
            date TEXT
        )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table created or already exists";
    }
}
