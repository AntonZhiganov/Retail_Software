#include "incomeandexpenseswindow.h"
#include "ui_incomeandexpenseswindow.h"
#include "mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QDate>

IncomeAndExpensesWindow::IncomeAndExpensesWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IncomeAndExpensesWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not found!";
        return;
    }

    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS incomeAndExpenses");
    QString createIncomeAndExpensesTableQuery = R"(
        CREATE TABLE IF NOT EXISTS incomeAndExpenses (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            Total_spent FLOAT,
            Total_earned FLOAT,
            spent_on_advertising FLOAT,
            spent_on_goods FLOAT,
            Income FLOAT,
            date TEXT
        )
    )";

    if (!query.exec(createIncomeAndExpensesTableQuery)) {
        qDebug() << "Error creating incomeAndExpenses table:" << query.lastError().text();
    } else {
        qDebug() << "incomeAndExpenses table created or already exists";
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM incomeAndExpenses");

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count == 0) {
            QSqlQuery insertQuery(db);
            insertQuery.prepare(R"(
            INSERT INTO incomeAndExpenses
            (Total_spent, Total_earned, spent_on_advertising, spent_on_goods, Income, date)
            VALUES (?, ?, ?, ?, ?, ?)
        )");

            insertQuery.addBindValue(0);
            insertQuery.addBindValue(0);
            insertQuery.addBindValue(0);
            insertQuery.addBindValue(0);
            insertQuery.addBindValue(0);
            insertQuery.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

            if (!insertQuery.exec()) {
                qDebug() << "Failed to insert initial row:" << insertQuery.lastError().text();
            } else {
                qDebug() << "Initial zero row inserted into incomeAndExpenses";
            }
        }
    }

    loadData();

}

void IncomeAndExpensesWindow::loadData() {
    QSqlDatabase db = QSqlDatabase::database("WarehoseConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection not open!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT Total_spent, Total_earned, spent_on_advertising, spent_on_goods, Income, date FROM incomeAndExpenses")) {
        qDebug() << "Failed to select data:" << query.lastError().text();
        return;
    }

    ui->IncomeAndExpensesTableWidget->clear();
    QStringList headers = {"Total_spent", "Total_earned", "spent_on_advertising", "spent_on_goods", "Income", "date"};
    ui->IncomeAndExpensesTableWidget->setColumnCount(headers.size());
    ui->IncomeAndExpensesTableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->IncomeAndExpensesTableWidget->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->IncomeAndExpensesTableWidget->setItem(row, col, item);
        }
        ++row;
    }
}

IncomeAndExpensesWindow::~IncomeAndExpensesWindow()
{
    delete ui;
}

void IncomeAndExpensesWindow::on_menuPushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow  ->show();
    this -> hide();
}
