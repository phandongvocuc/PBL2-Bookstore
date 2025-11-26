#pragma once

#include <QDialog>
#include <QString>

#include "model/Reader.h"
#include "service/ReaderService.h"

class QCheckBox;
class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QSpinBox;

namespace pbl2::ui {
// Biến toàn cục cho service
extern pbl2::service::ReaderService *readerService;

class ReaderDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ReaderDialog(QWidget *parent = nullptr);

    void setReader(const model::Reader &reader, bool editing);
    void presetId(const QString &id, bool lockField = true);
    [[nodiscard]] model::Reader reader() const;

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
    QLineEdit *identityCardEdit{nullptr};
    QLineEdit *notesEdit{nullptr};
    QDateEdit *dobEdit{nullptr};
    QDateEdit *createdDateEdit{nullptr};
    QDateEdit *expiryDateEdit{nullptr};
    QSpinBox *totalBorrowedSpin{nullptr};
    QCheckBox *activeCheck{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
    bool editingMode{false};
    bool forceIdReadOnly{false};
    bool activeFlag{true};
};

}
