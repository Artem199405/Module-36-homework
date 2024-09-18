#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chat.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<Chat> chatPtr = nullptr,
                        QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_databasePushButton_clicked();

    void on_serverPushButton_clicked();

    void on_disablePushButton_clicked();

    void on_banPushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Chat> m_chatPtr;
    std::thread t1;
};

#endif // MAINWINDOW_H
