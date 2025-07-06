#ifndef ADVERTISINGWINDOW_H
#define ADVERTISINGWINDOW_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

namespace Ui {
class AdvertisingWindow;
}

class AdvertisingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdvertisingWindow(QWidget *parent = nullptr);
    ~AdvertisingWindow();

private:
    Ui::AdvertisingWindow *ui;

    void openDB();

private slots:
    void on_viewDataPushButton_clicked();
    void on_confirmAdvertisingPushButton_clicked();
    void on_menuPushButton_clicked();
};

#endif // ADVERTISINGWINDOW_H
