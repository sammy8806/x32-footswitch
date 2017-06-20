#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QListView>
#include <QTreeWidget>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

#include <x32Types/x32status.h>
#include <x32Types/channel.h>
#include <x32Types/mutegroup.h>

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

    // QMap<qint8, UserctrlButton*> *btnData;

public slots:
    void updateStatus(X32Status status);
    // void updateUserctrl(X32ConfigUserctrl config);
    void updateChannel(Channel *channel);

private slots:
    void on_btn5_clicked();

signals:
    void mute(qint8 channel, bool state);
};

#endif // MAINWINDOW_H
