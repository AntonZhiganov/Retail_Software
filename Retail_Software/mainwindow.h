#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "warehosewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    WarehoseWindow *warehosewindow = nullptr;

private slots:
    void on_purchaseButton_clicked();
    void on_additionalExpensesButton_clicked();
    void on_saleButton_clicked();
    void on_checkButton_clicked();
    void on_incomeAndExpensesButton_clicked();
    void on_clientsButton_clicked();
    void on_ordersButton_clicked();
};


#endif // MAINWINDOW_H
