#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>

#include <x32Types/x32status.h>
#include <x32Types/x32configuserctrl.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void updateStatus(X32Status status);
    void updateUserctrl(X32ConfigUserctrl config);
private slots:
};

#endif // MAINWINDOW_H
