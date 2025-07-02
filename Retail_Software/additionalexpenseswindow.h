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
};

#endif // ADDITIONALEXPENSESWINDOW_H
