#pragma once

#include <QDialog>
#include <QString>

#include "core/custom/CustomString.h"
#include "core/custom/QtContainers.h"
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
                                 const custom::Vector<custom::CustomString> &knownBookIds,
                                 QWidget *parent = nullptr);

    [[nodiscard]] model::ReportRequest reportRequest() const;

protected:
    void accept() override;

private:
    struct AffectedBookEntry {
        QString id;
        int count{0};
    };

    [[nodiscard]] custom::Vector<AffectedBookEntry> parseAffectedBooks(custom::Vector<QString> *errors = nullptr) const;
    [[nodiscard]] custom::Vector<QString> splitTokens(const QString &normalized) const;
    [[nodiscard]] bool isKnownBookId(const QString &idUpper) const;
    void refreshAffectedCountPreview() const;
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    QString staffUsername;
    custom::Vector<custom::CustomString> knownBookIds;
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
