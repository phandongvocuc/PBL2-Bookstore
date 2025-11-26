#pragma once

#include <QDialog>
#include <QString>

#include "model/Staff.h"
#include "service/StaffService.h"

class QCheckBox;
class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
namespace pbl2::ui {
// Biến toàn cục cho service
extern pbl2::service::StaffService *staffService;

class StaffDialog final : public QDialog {
    Q_OBJECT

public:
    explicit StaffDialog(QWidget *parent = nullptr);

    void setStaff(const model::Staff &staff, bool editing);
    void presetId(const QString &id, bool lockField = true);
    [[nodiscard]] model::Staff staff() const;

protected:
    void accept() override;

private:
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    QLineEdit *idEdit{nullptr};
    QLineEdit *fullNameEdit{nullptr};
    QComboBox *genderCombo{nullptr};
    QLineEdit *addressEdit{nullptr};
    QLineEdit *phoneEdit{nullptr};
    QLineEdit *emailEdit{nullptr};
    QComboBox *positionCombo{nullptr};
    QLineEdit *notesEdit{nullptr};
    QDateEdit *dobEdit{nullptr};
    QDateEdit *hireDateEdit{nullptr};
    QCheckBox *activeCheck{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
    bool editingMode{false};
    bool forceIdReadOnly{false};
    bool activeFlag{true};
};

}  // namespace pbl2::ui

