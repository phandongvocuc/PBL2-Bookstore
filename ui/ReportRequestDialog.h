#pragma once

#include <QDialog>
#include <QString>

#include "../core/CustomString.h"
#include "../core/DynamicArray.h"
#include "model/Loan.h"
#include "model/ReportRequest.h"

class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QSpinBox;

namespace pbl2::ui {

class ReportRequestDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ReportRequestDialog(const QString &staffUsername,
                                 const core::DynamicArray<core::CustomString> &knownBookIds,
                                 QWidget *parent = nullptr);

    [[nodiscard]] model::ReportRequest reportRequest() const;
    void setLoansData(core::DynamicArray<model::Loan> data);

protected:
    void accept() override;

private:
    struct AffectedBookEntry {
        QString id;
        int count{0};
    };

    [[nodiscard]] core::DynamicArray<AffectedBookEntry> parseAffectedBooks(core::DynamicArray<QString> *errors = nullptr) const;
    [[nodiscard]] core::DynamicArray<QString> splitTokens(const QString &normalized) const;
    [[nodiscard]] bool isKnownBookId(const QString &idUpper) const;
    void refreshAffectedCountPreview() const;
    void refreshAutoFields();
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    QString staffUsername;
    core::DynamicArray<core::CustomString> knownBookIds;
    core::DynamicArray<model::Loan> loansData;
    bool userEditedAffected{false};
    QLineEdit *requestIdEdit{nullptr};
    QDateEdit *fromDateEdit{nullptr};
    QDateEdit *toDateEdit{nullptr};
    QSpinBox *handledSpin{nullptr};
    QSpinBox *lostSpin{nullptr};
    QSpinBox *overdueSpin{nullptr};
    QPlainTextEdit *affectedBooksEdit{nullptr};
    QPlainTextEdit *notesEdit{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
};

}  // namespace pbl2::ui
