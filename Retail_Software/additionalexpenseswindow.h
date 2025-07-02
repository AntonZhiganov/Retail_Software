#ifndef ADDITIONALEXPENSESWINDOW_H
#define ADDITIONALEXPENSESWINDOW_H

#include <QWidget>

namespace Ui {
class AdditionalExpensesWindow;
}

class AdditionalExpensesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdditionalExpensesWindow(QWidget *parent = nullptr);
    ~AdditionalExpensesWindow();

private:
    Ui::AdditionalExpensesWindow *ui;

private slots:
    void on_defectiveProductButton_clicked();
    void on_advertisingButton_clicked();
};

#endif // ADDITIONALEXPENSESWINDOW_H
