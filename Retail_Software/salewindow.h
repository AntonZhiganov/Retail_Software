#ifndef SALEWINDOW_H
#define SALEWINDOW_H

#include <QWidget>

namespace Ui {
class SaleWindow;
}

class SaleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SaleWindow(QWidget *parent = nullptr);
    ~SaleWindow();

private:
    Ui::SaleWindow *ui;
    void loadProducts();

private slots:
    void on_menuPushButton_clicked();
    void on_confirmPushButton_clicked();
};

#endif // SALEWINDOW_H
