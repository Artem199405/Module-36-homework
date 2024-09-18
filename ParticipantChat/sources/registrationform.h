#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>
#include <QMessageBox>
#include "ui_registrationform.h"
#include "chat.h"

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget *parent = nullptr);

    ~RegistrationForm();

    void setChat(std::shared_ptr<Chat> chatPtr);

signals:
    void loginRequested();

    void accepted();

    void rejected();

private slots:
    void on_loginPushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::RegistrationForm *ui;
    std::shared_ptr<Chat> m_chatPtr;
};

#endif // REGISTRATIONFORM_H
