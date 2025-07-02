#ifndef ADVERTISINGWINDOW_H
#define ADVERTISINGWINDOW_H

#include <QWidget>

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
};

#endif // ADVERTISINGWINDOW_H
