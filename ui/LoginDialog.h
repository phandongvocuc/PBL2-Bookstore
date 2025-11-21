#pragma once

#include <QDialog>
#include <QString>

#include <memory>
#include <QComboBox>

#include "model/Account.h"
#include "service/AccountService.h"

class QLabel;
class QLineEdit;
class QPushButton;

namespace Ui {
class LoginDialog;
}

namespace pbl2::ui {


class LoginDialog final : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(service::AccountService &accountService, QWidget *parent = nullptr);
    ~LoginDialog() override;

    [[nodiscard]] model::Account account() const { return authenticatedAccount; }
    [[nodiscard]] bool exitRequested() const { return exitRequestedFlag; }

private slots:
    void attemptLogin();
    void onExitClicked();

protected:
    void reject() override; // override default reject to avoid treating window-close as explicit Exit

private:
    void showError(const QString &message) const;

    service::AccountService &accountService;
    std::unique_ptr<Ui::LoginDialog> ui;
    QLineEdit *usernameEdit{nullptr};
    QLineEdit *passwordEdit{nullptr};
    QLabel *errorLabel{nullptr};
    QPushButton *loginButton{nullptr};
    QPushButton *exitButton{nullptr};
    QComboBox *roleComboBox{nullptr};
    model::Account authenticatedAccount;
    bool exitRequestedFlag{false};
};

}  // namespace pbl2::ui


