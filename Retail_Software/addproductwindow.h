#ifndef ADDPRODUCTWINDOW_H
#define ADDPRODUCTWINDOW_H

#include <QWidget>

namespace Ui {
class AddProductWindow;
}

class AddProductWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddProductWindow(QWidget *parent = nullptr);
    ~AddProductWindow();

private:
    Ui::AddProductWindow *ui;
};

#endif // ADDPRODUCTWINDOW_H
