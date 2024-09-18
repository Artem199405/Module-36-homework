#include "ui_startscreen.h"
#include "startscreen.h"

StartScreen::StartScreen(std::shared_ptr<Chat> chatPtr, QWidget *parent): QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    m_chatPtr = chatPtr;

    ui->loginWidget->setChat(m_chatPtr);
    ui->registrationWidget->setChat(m_chatPtr);

    connect(ui->loginWidget, &LoginForm::registrationRequested, this, &StartScreen::setRegistrationForm);
    connect(ui->loginWidget, &LoginForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->loginWidget, &LoginForm::rejected, this, &StartScreen::onRejectRequested);
    connect(ui->registrationWidget, &RegistrationForm::loginRequested, this, &StartScreen::setLoginForm);
    connect(ui->registrationWidget, &RegistrationForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->registrationWidget, &RegistrationForm::rejected, this, &StartScreen::onRejectRequested);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::onLoggedIn()
{
    accept();
}

void StartScreen::onRejectRequested()
{
    reject();
}
