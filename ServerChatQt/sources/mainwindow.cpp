#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(std::shared_ptr<Chat> chatPtr,
                       QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chatPtr = chatPtr;
}

MainWindow::~MainWindow()
{
    t1.join();

    delete ui;
}

void MainWindow::on_databasePushButton_clicked()
{
    m_chatPtr->connectToDatabase();
}

void MainWindow::on_serverPushButton_clicked()
{
    t1 = std::thread(&Chat::runServer, m_chatPtr);
}

void MainWindow::on_disablePushButton_clicked()
{

}

void MainWindow::on_banPushButton_clicked()
{

}

