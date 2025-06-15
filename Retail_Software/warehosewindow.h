#ifndef WAREHOSEWINDOW_H
#define WAREHOSEWINDOW_H

#include <QWidget>

namespace Ui {
class WarehoseWindow;
}

class WarehoseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WarehoseWindow(QWidget *parent = nullptr);
    ~WarehoseWindow();

private:
    Ui::WarehoseWindow *ui;
};

#endif // WAREHOSEWINDOW_H
