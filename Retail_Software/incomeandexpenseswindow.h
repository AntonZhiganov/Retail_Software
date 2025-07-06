#ifndef INCOMEANDEXPENSESWINDOW_H
#define INCOMEANDEXPENSESWINDOW_H

#include <QWidget>

namespace Ui {
class IncomeAndExpensesWindow;
}

class IncomeAndExpensesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit IncomeAndExpensesWindow(QWidget *parent = nullptr);
    ~IncomeAndExpensesWindow();

private:
    Ui::IncomeAndExpensesWindow *ui;

private slots:
    void on_menuPushButton_clicked();
    void loadData();
};

#endif // INCOMEANDEXPENSESWINDOW_H
