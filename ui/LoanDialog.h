#pragma once

#include <QDialog>
#include <QString>
#include "../core/DynamicArray.h"

#include "model/Book.h"
#include "model/Loan.h"
#include "model/Reader.h"

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;

namespace pbl2::ui{

class LoanDialog final : public QDialog {
    Q_OBJECT

public:
    LoanDialog(const core::DynamicArray<model::Reader> &readers,
               const core::DynamicArray<model::Book> &books,
               int maxBorrowDays,
               const QString &staffUsername,
               QWidget *parent = nullptr);

    void presetLoanId(const QString &loanId, bool lockField = true);
    [[nodiscard]] model::Loan loan() const;

protected:
    void accept() override;

private:
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    core::DynamicArray<model::Reader> readers;
    core::DynamicArray<model::Book> books;
    QLineEdit *loanIdEdit{nullptr};
    QComboBox *readerCombo{nullptr};
    QComboBox *bookCombo{nullptr};
    QLineEdit *staffEdit{nullptr};
    QDateEdit *borrowDateEdit{nullptr};
    QDateEdit *dueDateEdit{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
    bool editingMode{false};
    bool forceIdReadOnly{false};
};

}  // namespace ui
