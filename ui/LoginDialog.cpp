#include "LoginDialog.h"

#include <QVBoxLayout>

#include "QtBridge.h"
#include "ui_LoginDialog.h"

using namespace std;

namespace {

}

namespace pbl2::ui {

LoginDialog::LoginDialog(service::AccountService &accountService, QWidget *parent)
    : QDialog(parent), accountService(accountService), ui(std::make_unique<Ui::LoginDialog>()) {
    setModal(true);
    setMinimumSize(400, 280);

    // Modern font and light background
    QFont font("Arial", 30);
    setFont(font);

   ui->setupUi(this);
    // setStyleSheet đã bị xoá để dùng font-size cho tiêu đề trực tiếp trong LoginDialog.ui
    usernameEdit = ui->usernameEdit;
    passwordEdit = ui->passwordEdit;
    errorLabel = ui->errorLabel;
    loginButton = ui->loginButton;
    exitButton = ui->exitButton;
    roleComboBox = ui->roleComboBox;

    if (usernameEdit) usernameEdit->setMinimumHeight(32);
    if (passwordEdit) passwordEdit->setMinimumHeight(32);
    if (errorLabel) errorLabel->setVisible(false);

    if (exitButton) {
        exitButton->setMinimumHeight(32);
        connect(exitButton, &QPushButton::clicked, this, &LoginDialog::onExitClicked);
    }
    if (loginButton) {
        loginButton->setMinimumHeight(32);
        loginButton->setDefault(true);
        loginButton->setAutoDefault(true);
        connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    }
    if (roleComboBox) {
        roleComboBox->setMinimumHeight(32);
    }

    setAutoFillBackground(true);
}

LoginDialog::~LoginDialog() = default;

void LoginDialog::reject() {
    // Do not mark exitRequestedFlag on generic reject (e.g. window close)
    QDialog::reject();
}

void LoginDialog::onExitClicked() {
    // Explicit Exit button was clicked: mark exit requested then close
    exitRequestedFlag = true;
    QDialog::reject();
}

void LoginDialog::attemptLogin() {
    if (!usernameEdit || !passwordEdit || !roleComboBox) {
        showError(tr("Khong khoi tao duoc giao dien dang nhap."));
        return;
    }
    const QString username = usernameEdit->text().trimmed();
    const QString password = passwordEdit->text();
    const QString selectedRole = roleComboBox->currentText().trimmed();

    if (username.isEmpty() || password.isEmpty() || selectedRole.isEmpty()) {
        showError(tr("Vui long nhap du thong tin."));
        return;
    }

    const auto customUsername = pbl2::bridge::toCustomString(username);
    const auto accountOpt = accountService.findByUsername(customUsername);
    if (!accountOpt.has_value()) {
        showError(tr("Khong tim thay tai khoan."));
        return;
    }

    if (!accountOpt.value().isActive()) {
        showError(tr("Tai khoan dang bi khoa."));
        return;
    }

    const auto authenticated = accountService.authenticate(pbl2::bridge::toCustomString(username), pbl2::bridge::toCustomString(password));
    if (!authenticated.has_value()) {
        showError(tr("Mat khau khong dung."));
        return;
    }

    // Kiểm tra vai trò được chọn
    const QString userRole = pbl2::bridge::toQString(authenticated.value().getRole()).trimmed();
    if (userRole.compare(selectedRole, Qt::CaseInsensitive) != 0) {
        showError(tr("Vai trò không đúng. Vui lòng chọn đúng vai trò."));
        return;
    }

    authenticatedAccount = *authenticated;
    exitRequestedFlag = false;
    accept();
}

void LoginDialog::showError (const QString &message) const {
    if (!errorLabel) return;
    errorLabel->setText(message);
    errorLabel->setVisible(true);
}

}  // namespace pbl2::ui
