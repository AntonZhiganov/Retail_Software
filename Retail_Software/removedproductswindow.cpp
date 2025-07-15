#include "removedproductswindow.h"
#include "ui_removedproductswindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QDate>

RemovedProductsWindow::RemovedProductsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RemovedProductsWindow)
{
    ui->setupUi(this);
    ui->removedProductsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->removedProductsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS RemovedProducts");
    QString createRemovedProductsQuery = R"(
        CREATE TABLE IF NOT EXISTS RemovedProducts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            quantity INTEGER,
            comment TEXT,
            date TEXT
        )
    )";

    if (!query.exec(createRemovedProductsQuery)) {
        qDebug() << "Error creating RemovedProducts table:" << query.lastError().text();
    } else {
        qDebug() << "RemovedProducts table created or already exists";
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM RemovedProducts");

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count == 0) {
            QSqlQuery insertQuery(db);
            insertQuery.prepare(R"(
            INSERT INTO RemovedProducts (name, quantity, comment, date)
            VALUES (?, ?, ?, ?)
        )");

            insertQuery.addBindValue("Nothing yet");
            insertQuery.addBindValue(0);
            insertQuery.addBindValue("-");
            insertQuery.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

            if (!insertQuery.exec()) {
                qDebug() << "Failed to insert initial row:" << insertQuery.lastError().text();
            } else {
                qDebug() << "Initial zero row inserted into RemovedProducts";
            }
        }
    }

    loadData();

}


void RemovedProductsWindow::loadData() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not open!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT name, quantity, comment, date FROM RemovedProducts")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->removedProductsTableWidget->setRowCount(0);
    QStringList headers = {"Name", "Quantity", "Comment", "Date"};
    ui->removedProductsTableWidget->setColumnCount(headers.size());
    ui->removedProductsTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->removedProductsTableWidget->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->removedProductsTableWidget->setItem(row, col, item);
        }
        ++row;
    }
}


RemovedProductsWindow::~RemovedProductsWindow()
{
    delete ui;
}

void RemovedProductsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> close();
}
