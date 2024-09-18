#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QMessageBox>
#include "ui_loginform.h"
#include "chat.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);

    ~LoginForm();

    void setChat(std::shared_ptr<Chat> chatPtr);

signals:
    void registrationRequested();

    void accepted();

    void rejected();

private slots:
    void on_registrationPushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LoginForm *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // LOGINFORM_H
