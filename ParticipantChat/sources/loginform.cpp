#include "loginform.h"

LoginForm::LoginForm(QWidget *parent) : QWidget(parent), ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setChat(std::shared_ptr<Chat> chatPtr)
{
    m_chatPtr = chatPtr;
}

void LoginForm::on_registrationPushButton_clicked()
{
    emit registrationRequested();
}

void LoginForm::on_buttonBox_accepted()
{
    int error = m_chatPtr->authorization(ui->loginLineEdit->text().toStdString(),
                                      ui->passwordLineEdit->text().toStdString());

    switch(error)
    {
    case(-1):
        QMessageBox::critical(this, "Error",
                              "You have entered an incorrect login or password. Try again.");
        break;
    case(-2):
        QMessageBox::critical(this, "Error",
                              "Sorry! You have been disconnected from the chat.");
        break;
    default:
        emit accepted();
    }

    return;
}

void LoginForm::on_buttonBox_rejected()
{
    emit rejected();
}

