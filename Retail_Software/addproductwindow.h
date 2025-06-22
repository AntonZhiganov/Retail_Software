#ifndef ADDPRODUCTWINDOW_H
#define ADDPRODUCTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include <QLineEdit>
#include <QSpinBox>

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
    QList<QLineEdit*> nameEdits;
    QList<QLineEdit*> priceEdits;
    QList<QSpinBox*> quantitySpinBoxes;

private:
    QVBoxLayout* layout;

private slots:
    void onNewProductTypeButtonClicked();
    void onConfirmOrderButtonClicked();
};

#endif // ADDPRODUCTWINDOW_H
