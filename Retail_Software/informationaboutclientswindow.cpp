#include "informationaboutclientswindow.h"
#include "ui_informationaboutclientswindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QDate>

InformationAboutClientsWindow::InformationAboutClientsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationAboutClientsWindow)
{
    ui->setupUi(this);
    ui->clientsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->clientsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS clients");
    QString createClientsTableQuery = R"(
        CREATE TABLE IF NOT EXISTS clients (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            total_spent DOUBLE,
            purchases TEXT,
            comunication TEXT
        )
    )";

    if (!query.exec(createClientsTableQuery)) {
        qDebug() << "Error creating clients table:" << query.lastError().text();
    } else {
        qDebug() << "clients table created or already exists";
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM clients");

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count == 0) {
            QSqlQuery insertQuery(db);
            insertQuery.prepare(R"(
            INSERT INTO clients
            (name,  total_spent, purchases, comunication)
            VALUES (?, ?, ?, ?)
        )");

            insertQuery.addBindValue("-");
            insertQuery.addBindValue(0);
            insertQuery.addBindValue("-");
            insertQuery.addBindValue("-");

            if (!insertQuery.exec()) {
                qDebug() << "Failed to insert initial row:" << insertQuery.lastError().text();
            } else {
                qDebug() << "Initial zero row inserted into clients";
            }
        }
    }

    loadData();

}

void InformationAboutClientsWindow::loadData() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not open!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT name,  total_spent, purchases, comunication FROM clients ORDER BY id DESC")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->clientsTableWidget->setRowCount(0);
    QStringList headers = {"Client name", "Total spent", "Purchases", "Comunication"};
    ui->clientsTableWidget->setColumnCount(headers.size());
    ui->clientsTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->clientsTableWidget->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->clientsTableWidget->setItem(row, col, item);
        }
        ++row;
    }
}

InformationAboutClientsWindow::~InformationAboutClientsWindow()
{
    delete ui;
}

void InformationAboutClientsWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();

}
