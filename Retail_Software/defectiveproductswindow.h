#ifndef DEFECTIVEPRODUCTSWINDOW_H
#define DEFECTIVEPRODUCTSWINDOW_H

#include <QWidget>

namespace Ui {
class DefectiveProductsWindow;
}

class DefectiveProductsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DefectiveProductsWindow(QWidget *parent = nullptr);
    ~DefectiveProductsWindow();

private:
    Ui::DefectiveProductsWindow *ui;

private slots:
    void on_menuPushButton_clicked();
};

#endif // DEFECTIVEPRODUCTSWINDOW_H
