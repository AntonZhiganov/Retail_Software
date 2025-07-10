#ifndef REMOVEDPRODUCTSWINDOW_H
#define REMOVEDPRODUCTSWINDOW_H

#include <QWidget>

namespace Ui {
class RemovedProductsWindow;
}

class RemovedProductsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RemovedProductsWindow(QWidget *parent = nullptr);
    ~RemovedProductsWindow();

private:
    Ui::RemovedProductsWindow *ui;

private slots:
    void on_menuPushButton_clicked();
    void loadData();
};

#endif // REMOVEDPRODUCTSWINDOW_H
