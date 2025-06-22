#ifndef WAREHOSEWINDOW_H
#define WAREHOSEWINDOW_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

namespace Ui {
class WarehoseWindow;
}

class WarehoseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WarehoseWindow(QWidget *parent = nullptr);
    ~WarehoseWindow();

private:
    Ui::WarehoseWindow *ui;
    QSqlDatabase db;

    void openDB();
    void loadProducts();
};

#endif // WAREHOSEWINDOW_H
