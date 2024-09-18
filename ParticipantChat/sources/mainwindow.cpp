#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(std::shared_ptr<Chat> chatPtr,
    QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chatPtr = chatPtr;

    auto timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);

    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::createClient(std::shared_ptr<Chat> chatPtr)
{
    StartScreen s(chatPtr);

    auto result = s.exec();

    if (result == QDialog::Rejected)
        return nullptr;

    auto w = new MainWindow(chatPtr);
    w->setAttribute(Qt::WA_DeleteOnClose);

    return w;
}

void MainWindow::on_actionOpenAnotherClient_triggered()
{
    std::shared_ptr<Chat> chatAnother = std::make_shared<Chat>();

    chatAnother->connectServer();

    auto w = createClient(chatAnother);

    if (w)
        w->show();
    else
        return;
}

void MainWindow::on_actionCloseClient_triggered()
{
    this->close();
}

void MainWindow::on_sendPushButton_clicked()
{
    int error = m_chatPtr->sendMessage(ui->nameRecipientLineEdit->text().toStdString(),
                                       ui->messageLineEdit->text().toStdString());

    switch(error)
    {
    case(-1):
        QMessageBox::critical(this, "Error",
                              "A user with that name was not found. Try again.");
        break;
    case(-2):
        QMessageBox::critical(this, "Error",
                              "Sorry! You was banned.");
        break;
    default:
        QMessageBox::critical(this, "Note",
                              "The message has been sent.");
    }

    return;
}

void MainWindow::updateChats()
{
    auto chatCommonMessages = m_chatPtr->getChatCommonMessages();
    QString chat;

    for (const auto &msg : chatCommonMessages)
    {
        chat.append(QString::fromStdString(msg) + "\n");
    }
    if (ui->commonChatBrowser->toPlainText() != chat)
        ui->commonChatBrowser->setText(chat);

    chat.clear();

    auto chatPrivateMessages = m_chatPtr->getChatPrivateMessage();

    for (const auto &msg : chatPrivateMessages)
    {
        chat.append(QString::fromStdString(msg) + "\n");
    }
    if (ui->privateChatBrowser->toPlainText() != chat)
        ui->privateChatBrowser->setText(chat);
}
