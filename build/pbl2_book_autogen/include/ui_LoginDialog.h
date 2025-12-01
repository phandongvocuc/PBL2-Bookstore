/********************************************************************************
** Form generated from reading UI file 'LoginDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QLabel *titleLabel;
    QLabel *errorLabel;
    QPushButton *loginButton;
    QPushButton *exitButton;
    QLineEdit *passwordEdit;
    QLabel *passwordLabel;
    QLineEdit *usernameEdit;
    QLabel *usernameLabel;
    QComboBox *roleComboBox;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(600, 702);
        LoginDialog->setStyleSheet(QString::fromUtf8(""));
        titleLabel = new QLabel(LoginDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(30, 80, 521, 91));
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(18);
        font.setBold(true);
        font.setItalic(false);
        titleLabel->setFont(font);
        titleLabel->setStyleSheet(QString::fromUtf8("font: 700 18pt \"Times New Roman\";"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        errorLabel = new QLabel(LoginDialog);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setGeometry(QRect(120, 440, 371, 41));
        errorLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginButton = new QPushButton(LoginDialog);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(340, 570, 151, 51));
        QFont font1;
        font1.setPointSize(10);
        loginButton->setFont(font1);
        exitButton = new QPushButton(LoginDialog);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(90, 570, 161, 51));
        exitButton->setFont(font1);
        exitButton->setAutoFillBackground(false);
        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(140, 350, 371, 51));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        passwordLabel = new QLabel(LoginDialog);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setGeometry(QRect(30, 360, 81, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Sans Serif")});
        passwordLabel->setFont(font2);
        usernameEdit = new QLineEdit(LoginDialog);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(140, 240, 371, 51));
        usernameEdit->setTabletTracking(true);
        usernameLabel = new QLabel(LoginDialog);
        usernameLabel->setObjectName("usernameLabel");
        usernameLabel->setGeometry(QRect(30, 250, 101, 31));
        usernameLabel->setFont(font2);
        roleComboBox = new QComboBox(LoginDialog);
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->setObjectName("roleComboBox");
        roleComboBox->setGeometry(QRect(140, 420, 171, 31));
        roleComboBox->setFont(font2);

        retranslateUi(LoginDialog);

        loginButton->setDefault(true);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "\304\220\304\203ng nh\341\272\255p", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginDialog", "H\341\273\207 Th\341\273\221ng Qu\341\272\243n L\303\275 Cho Thu\303\252 Truy\341\273\207n V\303\240 S\303\241ch", nullptr));
        errorLabel->setText(QString());
        loginButton->setText(QCoreApplication::translate("LoginDialog", "\304\220\304\203ng nh\341\272\255p", nullptr));
        exitButton->setText(QCoreApplication::translate("LoginDialog", "Tho\303\241t", nullptr));
        passwordLabel->setText(QCoreApplication::translate("LoginDialog", "M\341\272\255t kh\341\272\251u:", nullptr));
        usernameLabel->setText(QCoreApplication::translate("LoginDialog", "T\303\252n \304\221\304\203ng nh\341\272\255p:", nullptr));
        roleComboBox->setItemText(0, QCoreApplication::translate("LoginDialog", "Admin", nullptr));
        roleComboBox->setItemText(1, QCoreApplication::translate("LoginDialog", "Staff", nullptr));

    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
