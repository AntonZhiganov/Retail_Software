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
    connect(ui->clientsTableWidget, &QTableWidget::itemChanged, this, &InformationAboutClientsWindow::onClientTableItemChanged);
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
            communication TEXT
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
            (name,  total_spent, purchases, communication)
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
    if (!query.exec("SELECT name,  total_spent, purchases, communication FROM clients ORDER BY id DESC")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->clientsTableWidget->setRowCount(0);
    QStringList headers = {"Client name", "Total spent", "Purchases", "Communication"};
    ui->clientsTableWidget->setColumnCount(headers.size());
    ui->clientsTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->clientsTableWidget->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (col == 3) {
                item->setFlags(item->flags() | Qt::ItemIsEditable);
            } else {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
            ui->clientsTableWidget->setItem(row, col, item);
        }
        ++row;
    }
}

void InformationAboutClientsWindow::addClientPurchase(const QString &clientName, const QString &purchaseDetails, double amountSpent, const QString &comment)
{
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare(R"(
        INSERT INTO clients (name, total_spent, purchases, communication)
        VALUES (?, ?, ?, ?)
    )");

    insertQuery.addBindValue(clientName);
    insertQuery.addBindValue(amountSpent);
    insertQuery.addBindValue(purchaseDetails);
    insertQuery.addBindValue(comment);

    if (!insertQuery.exec()) {
        qDebug() << "Failed to insert client data:" << insertQuery.lastError().text();
    } else {
        qDebug() << "Client data inserted successfully";
        loadData();
    }
}

void InformationAboutClientsWindow::onClientTableItemChanged(QTableWidgetItem *item)
{
    if (!item) return;

    int col = item->column();
    if (col != 3) return;

    int row = item->row();
    QString newCommunication = item->text();
    QString clientName = ui->clientsTableWidget->item(row, 0)->text();

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("UPDATE clients SET communication = ? WHERE name = ?");
    query.addBindValue(newCommunication);
    query.addBindValue(clientName);

    if (!query.exec()) {
        qDebug() << "Failed to update communication:" << query.lastError().text();
    } else {
        qDebug() << "Communication updated for client" << clientName;
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
    this -> close();

}
