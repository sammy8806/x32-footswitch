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
#include <QKeyEvent>

#include <x32Types/x32status.h>
#include <x32Types/channel.h>
#include <x32Types/mutegroup.h>

#include <x32console.h>

#include <osc/composer/OscMessageComposer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setConsole(X32Console *console);
    ~MainWindow();

    void processBtnClick(qint8 btn);

    void keyPressEvent(QKeyEvent* event) override;

    void switchBank(int bank);
    
private:
    Ui::MainWindow *ui;

    QMap<qint8, UserctrlButton*> *btnData;
    X32Console* console;

    QList<QPushButton*> btn;

public slots:
    void updateStatus(X32Status status);
    void updateUserctrl(UserctrlBank *bank, qint8 btnNr);
    void updateUserctrl(UserctrlBank *bank);
    void updateChannel(Channel *channel);

private slots:
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn8_clicked();
    void on_btn7_clicked();
    void on_btn9_clicked();
    void on_btn10_clicked();

signals:
    void mute(qint8 channel);
    void recall(QString target);
};

#endif // MAINWINDOW_H
