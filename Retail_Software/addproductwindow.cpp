#include "addproductwindow.h"
#include "ui_addproductwindow.h"
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>

AddProductWindow::AddProductWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddProductWindow)
{
    ui->setupUi(this);

    scrollLayout = new QVBoxLayout;
    ui->productsWidget->setLayout(scrollLayout);


    connect(ui->newProductTypeButton, &QPushButton::clicked, this, &AddProductWindow::onNewProductTypeButtonClicked);

}

AddProductWindow::~AddProductWindow()
{
    delete ui;
}

void AddProductWindow::onNewProductTypeButtonClicked()
{
    QLabel* nameName = new QLabel("Product name", this);
    QLineEdit* nameLineEdit = new QLineEdit(this);

    QLabel* priceName = new QLabel("Product price(for everyone)", this);
    QLineEdit* priceLineEdit = new QLineEdit(this);

    QLabel* quantityName = new QLabel("Product quantity", this);
    QSpinBox* quantitySpinBox = new QSpinBox(this);

    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(100);

    ui->verticalLayout->insertWidget(0, quantitySpinBox);
    ui->verticalLayout->insertWidget(0, quantityName);
    ui->verticalLayout->insertWidget(0, priceLineEdit);
    ui->verticalLayout->insertWidget(0, priceName);
    ui->verticalLayout->insertWidget(0, nameLineEdit);
    ui->verticalLayout->insertWidget(0, nameName);
}
