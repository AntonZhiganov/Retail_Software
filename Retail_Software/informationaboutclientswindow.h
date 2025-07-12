#ifndef INFORMATIONABOUTCLIENTSWINDOW_H
#define INFORMATIONABOUTCLIENTSWINDOW_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class InformationAboutClientsWindow;
}

class InformationAboutClientsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InformationAboutClientsWindow(QWidget *parent = nullptr);
    ~InformationAboutClientsWindow();

    void addClientPurchase(const QString &clientName, const QString &purchaseDetails, double amountSpent, const QString &comment);

private:
    Ui::InformationAboutClientsWindow *ui;

private slots:
    void on_menuPushButton_clicked();
    void loadData();
    void onClientTableItemChanged(QTableWidgetItem *item);
};

#endif // INFORMATIONABOUTCLIENTSWINDOW_H
