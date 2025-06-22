#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "WarehoseConnection");
    db.setDatabaseName("warehose.db");
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return -1;
    }
    qDebug() << "Database opened successfully";

    MainWindow w;
    w.show();
    return a.exec();
}
