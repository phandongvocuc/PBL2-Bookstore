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

    custom::CustomString username() const;
    custom::CustomString password() const;
    custom::CustomString role() const;
    bool isActive() const;
    // Staff linkage accessors
    void setStaffList(const custom::Vector<model::Staff> &staffs) const;
    custom::CustomString staffId() const;

protected:
    void accept() override;

private:
    bool validateInputs() const;
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
