#include "LoginDialog.h"

#include <QVBoxLayout>

#include "QtBridge.h"
#include "DialogTheme.h"
#include "ui_LoginDialog.h"

using namespace std;


namespace pbl2::ui {

LoginDialog::LoginDialog(service::AccountService &accountService, QWidget *parent)
    : QDialog(parent), accountService(accountService), ui(std::make_unique<Ui::LoginDialog>()) {
    setModal(true);
    setMinimumSize(400, 280);

    // Modern font and light background
    const QFont font("Times New Roman", 18);
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
    // Áp dụng theme chung cho dialog
    pbl2::ui::applyDialogTheme(this);
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
        showError(tr("Không khởi tạo được giao diện đăng nhập"));
        return;
    }
    const QString username = usernameEdit->text().trimmed();
    const QString password = passwordEdit->text();
    const QString selectedRole = roleComboBox->currentText().trimmed();

    if (username.isEmpty() || password.isEmpty() || selectedRole.isEmpty()) {
        showError(tr("Vui lòng nhập đủ thông tin."));
        return;
    }

    const auto customUsername = bridge::toCustomString(username);
    const auto accountOpt = accountService.findByUsername(customUsername);
    if (!accountOpt.has_value()) {
        showError(tr("Không tìm thấy tài khoản."));
        return;
    }

    if (!accountOpt.value().isActive()) {
        showError(tr("Tài khoản đã bị khóa."));
        return;
    }

    const auto authenticated = accountService.authenticate(bridge::toCustomString(username), bridge::toCustomString(password));
    if (!authenticated.has_value()) {
        showError(tr("Mật khẩu không đúng."));
        return;
    }

    // Kiểm tra vai trò được chọn
    if (const QString userRole = bridge::toQString(authenticated.value().getRole()).trimmed(); userRole.compare(selectedRole, Qt::CaseInsensitive) != 0) {
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
