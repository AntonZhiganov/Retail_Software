#ifndef INFORMATIONABOUTCLIENTSWINDOW_H
#define INFORMATIONABOUTCLIENTSWINDOW_H

#include <QWidget>

namespace Ui {
class InformationAboutClientsWindow;
}

class InformationAboutClientsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InformationAboutClientsWindow(QWidget *parent = nullptr);
    ~InformationAboutClientsWindow();

private:
    Ui::InformationAboutClientsWindow *ui;

private slots:
    void on_menuPushButton_clicked();
};

#endif // INFORMATIONABOUTCLIENTSWINDOW_H
