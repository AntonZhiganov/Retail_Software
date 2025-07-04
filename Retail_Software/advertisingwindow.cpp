#include "advertisingwindow.h"
#include "ui_advertisingwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

AdvertisingWindow::AdvertisingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdvertisingWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    query.exec("DROP TABLE IF EXISTS advertising");
    QString createExpensesTableQuery = R"(
        CREATE TABLE IF NOT EXISTS advertising (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            seller TEXT,
            price Float,
            impressions INTEGER,
            clients INTEGER,
            date TEXT
        )
    )";

    if (!query.exec(createExpensesTableQuery)) {
        qDebug() << "Error creating advertising table:" << query.lastError().text();
    } else {
        qDebug() << "Advertising table created or already exists";
    }

}

AdvertisingWindow::~AdvertisingWindow()
{
    delete ui;
}
