#include "advertisinginformationwindow.h"
#include "ui_advertisinginformationwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QTableWidgetItem>

AdvertisingInformationWindow::AdvertisingInformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdvertisingInformationWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS advertising");
    QString createAdvertisingTableQuery = R"(
        CREATE TABLE IF NOT EXISTS advertising (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            seller TEXT,
            price Float,
            impressions INTEGER,
            clients INTEGER,
            date TEXT
        )
    )";

    if (!query.exec(createAdvertisingTableQuery)) {
        qDebug() << "Error creating advertising table:" << query.lastError().text();
    } else {
        qDebug() << "Advertising table created or already exists";
    }

    loadData();
}

AdvertisingInformationWindow::~AdvertisingInformationWindow()
{
    delete ui;
}

void AdvertisingInformationWindow::loadData()
{
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT seller, price, impressions, clients, date FROM advertising")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->advertisingTableWidget->clear();
    ui->advertisingTableWidget->setColumnCount(5);
    ui->advertisingTableWidget->setHorizontalHeaderLabels(QStringList() << "Seller" << "Price" << "Impressions" << "Clients" << "Date");
    ui->advertisingTableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->advertisingTableWidget->insertRow(row);
        ui->advertisingTableWidget->setItem(row, 0, new QTableWidgetItem(query.value("seller").toString()));
        ui->advertisingTableWidget->setItem(row, 1, new QTableWidgetItem(query.value("price").toString()));
        ui->advertisingTableWidget->setItem(row, 2, new QTableWidgetItem(query.value("impressions").toString()));
        ui->advertisingTableWidget->setItem(row, 3, new QTableWidgetItem(query.value("clients").toString()));
        ui->advertisingTableWidget->setItem(row, 4, new QTableWidgetItem(query.value("date").toString()));
        ++row;
    }
}
