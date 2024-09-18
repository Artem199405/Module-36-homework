#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "startscreen.h"
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

    static MainWindow* createClient(std::shared_ptr<Chat> chatPtr = nullptr);

private slots:
    void on_actionOpenAnotherClient_triggered();

    void on_actionCloseClient_triggered();

    void on_sendPushButton_clicked();

    void updateChats();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // MAINWINDOW_H
