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
        LoginDialog->resize(597, 604);
        LoginDialog->setStyleSheet(QString::fromUtf8("QDialog{\n"
"background-color: rgb(70, 70, 70);\n"
"    padding: 6px 12px;\n"
"    font-weight: bold;\n"
"}"));
        titleLabel = new QLabel(LoginDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(80, 80, 451, 91));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(24);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        errorLabel = new QLabel(LoginDialog);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setGeometry(QRect(120, 440, 371, 41));
        errorLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginButton = new QPushButton(LoginDialog);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(350, 500, 151, 51));
        loginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4CAF50;   /* M\303\240u n\341\273\201n n\303\272t */\n"
"    color: white;                /* M\303\240u ch\341\273\257 */\n"
"    border-radius: 10px;         /* Bo g\303\263c */\n"
"    padding: 6px 12px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color:E74C3C ;   /* Khi r\303\252 chu\341\273\231t */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #2e7031;   /* Khi nh\341\272\245n gi\341\273\257 */\n"
"}"));
        exitButton = new QPushButton(LoginDialog);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(50, 500, 161, 51));
        exitButton->setAutoFillBackground(false);
        exitButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #E74C3C;   /* M\303\240u n\341\273\201n n\303\272t */\n"
"    color: white;                /* M\303\240u ch\341\273\257 */\n"
"    border-radius: 10px;         /* Bo g\303\263c */\n"
"    padding: 6px 12px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color:E74C3C ;   /* Khi r\303\252 chu\341\273\231t */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #2e7031;   /* Khi nh\341\272\245n gi\341\273\257 */\n"
"}"));
        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(130, 310, 351, 41));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        passwordLabel = new QLabel(LoginDialog);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setGeometry(QRect(30, 330, 59, 21));
        usernameEdit = new QLineEdit(LoginDialog);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(130, 230, 351, 41));
        usernameEdit->setTabletTracking(true);
        usernameLabel = new QLabel(LoginDialog);
        usernameLabel->setObjectName("usernameLabel");
        usernameLabel->setGeometry(QRect(30, 230, 93, 28));
        roleComboBox = new QComboBox(LoginDialog);
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->setObjectName("roleComboBox");
        roleComboBox->setGeometry(QRect(130, 390, 171, 31));

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
