#pragma once

#include <QDialog>
#include <QString>
#include "core/custom/QtContainers.h"
#include "core/custom/CustomString.h"
#include "model/Staff.h"

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;

namespace pbl2 :: ui {

class AccountDialog final : public QDialog {
    Q_OBJECT

public:
    explicit AccountDialog(QWidget *parent = nullptr);

    [[nodiscard]] custom::CustomString username() const;
    [[nodiscard]] custom::CustomString password() const;
    [[nodiscard]] custom::CustomString role() const;
    [[nodiscard]] bool isActive() const;
    // Staff linkage accessors
    void setStaffList(const custom::Vector<model::Staff> &staffs) const;
    [[nodiscard]] custom::CustomString staffId() const;

protected:
    void accept() override;

private:
    [[nodiscard]] bool validateInputs() const;
    void showError(const custom::CustomString &message) const;

    QLineEdit *usernameEdit{nullptr};
    QLineEdit *passwordEdit{nullptr};
    QLineEdit *confirmEdit{nullptr};
    QComboBox *roleCombo{nullptr};
    QComboBox *staffCombo{nullptr};
    QCheckBox *activeCheck{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
    bool editingMode{false};
};

}  // namespace ui
