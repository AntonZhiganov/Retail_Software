#ifndef ORDERSWINDOW_H
#define ORDERSWINDOW_H

#include <QWidget>

namespace Ui {
class OrdersWindow;
}

class OrdersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OrdersWindow(QWidget *parent = nullptr);
    ~OrdersWindow();

private:
    Ui::OrdersWindow *ui;
    void loadData();
private slots:
    void on_menuPushButton_clicked();
};

#endif // ORDERSWINDOW_H
