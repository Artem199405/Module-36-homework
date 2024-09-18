#include "registrationform.h"

RegistrationForm::RegistrationForm(QWidget *parent): QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::setChat(std::shared_ptr<Chat> chatPtr)
{
    m_chatPtr = chatPtr;
}

void RegistrationForm::on_loginPushButton_clicked()
{
    emit loginRequested();
}

void RegistrationForm::on_buttonBox_accepted()
{
    if (ui->passwordLineEdit->text() != ui->confirmPasswordLineEdit->text())
    {
        QMessageBox::critical(this, "Error", "Passwords don't match");
        return;
    }

    int error = m_chatPtr->registration(ui->loginLineEdit->text().toStdString(),
                                     ui->passwordLineEdit->text().toStdString(),
                                     ui->nameLineEdit->text().toStdString());

    switch(error)
    {
    case(-1):
        QMessageBox::critical(this, "Error",
                              "A user with this login already exists. Come up with a new login.");
        break;
    case(-2):
        QMessageBox::critical(this, "Error",
                              "A user with this username already exists. Come up with a new username.");
        break;
    default:
        m_chatPtr->authorization(ui->loginLineEdit->text().toStdString(),
                              ui->passwordLineEdit->text().toStdString());
        emit accepted();
    }

    return;
}

void RegistrationForm::on_buttonBox_rejected()
{
    emit rejected();
}

