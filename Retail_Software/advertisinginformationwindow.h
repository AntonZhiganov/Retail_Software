#ifndef ADVERTISINGINFORMATIONWINDOW_H
#define ADVERTISINGINFORMATIONWINDOW_H

#include <QWidget>

namespace Ui {
class AdvertisingInformationWindow;
}

class AdvertisingInformationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdvertisingInformationWindow(QWidget *parent = nullptr);
    ~AdvertisingInformationWindow();

private:
    Ui::AdvertisingInformationWindow *ui;

private slots:
    void loadData();
};

#endif // ADVERTISINGINFORMATIONWINDOW_H
