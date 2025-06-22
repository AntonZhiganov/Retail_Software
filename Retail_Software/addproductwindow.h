#ifndef ADDPRODUCTWINDOW_H
#define ADDPRODUCTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

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
    QVBoxLayout* scrollLayout;

private:
    QVBoxLayout* layout;

private slots:
    void onNewProductTypeButtonClicked();
};

#endif // ADDPRODUCTWINDOW_H
