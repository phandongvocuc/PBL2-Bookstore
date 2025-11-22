#pragma once

#include <QDialog>
#include <QString>

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
    explicit ReportRequestDialog(const QString &staffUsername, QWidget *parent = nullptr);

    [[nodiscard]] model::ReportRequest reportRequest() const;

protected:
    void accept() override;

private:
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    QString staffUsername;
    QLineEdit *requestIdEdit{nullptr};
    QDateEdit *fromDateEdit{nullptr};
    QDateEdit *toDateEdit{nullptr};
    QSpinBox *handledSpin{nullptr};
    QSpinBox *lostSpin{nullptr};
    QSpinBox *overdueSpin{nullptr};
    QPlainTextEdit *notesEdit{nullptr};
    QLabel *errorLabel{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
};

}  // namespace pbl2::ui

