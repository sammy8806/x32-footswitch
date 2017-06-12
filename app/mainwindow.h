#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>

#include <x32Types/x32status.h>
#include <x32Types/x32configuserctrl.h>

#include <osc/composer/OscMessageComposer.h>

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

    QList<UserctrlButton> *btnData;

public slots:
    void updateStatus(X32Status status);
    void updateUserctrl(X32ConfigUserctrl config);

private slots:
    void on_btn5_clicked();

signals:
    void mute(qint8 channel, bool state);
};

#endif // MAINWINDOW_H
