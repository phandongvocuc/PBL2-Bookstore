#include "ReportRequestDialog.h"

#include <QDateEdit>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

#include "QtBridge.h"

using namespace std;

namespace {

QString generateRequestId(const QString &staff) {
    return QStringLiteral("REQ-%1-%2")
        .arg(staff.toUpper(), QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmss")));
}

}

namespace pbl2::ui {

    ReportRequestDialog::ReportRequestDialog(const QString &staffUsername, QWidget *parent)
        : QDialog(parent), staffUsername(staffUsername) {
        setWindowTitle(tr("Lập báo cáo tổng hợp"));
        setModal(true);
        setWindowIcon(QIcon(":/ui/resources/icons/report.png"));
        setStyleSheet(R"(
         QFont font("Segoe UI", 11);
         setFont(font);
         setStyleSheet(R"(
QDialog { background: #f8fafc; border-radius: 12px; }
QGroupBox { font-weight: bold; border-radius: 8px; }
QLineEdit, QComboBox, QSpinBox, QDateEdit, QPlainTextEdit {
    min-height: 32px; font-size: 11pt; border-radius: 10px; background: #fff;
    border: 1.5px solid #e3e8f0; padding-left: 10px;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDateEdit:focus, QPlainTextEdit:focus {
    border: 2px solid #2f6ad0; background: #f0f6ff;
}
QDialogButtonBox QPushButton, QPushButton {
    min-width: 100px; min-height: 36px; font-size: 11pt; border-radius: 10px;
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2f6ad0, stop:1 #6c63ff);
    color: #fff; font-weight: 500; border: none;
}
QDialogButtonBox QPushButton:hover, QPushButton:hover {
    background: #466ee6;
}
QDialogButtonBox QPushButton:disabled, QPushButton:disabled {
    background: #bfc9db; color: #fff;
}
QLabel { font-size: 11pt; }
QLabel[error="true"] { color: #dc2626; font-size: 10.5pt; padding: 6px; }
)");
        // Modern font and light background
        setStyleSheet("QDialog { background: #f8fafc; border-radius: 12px; } QGroupBox { font-weight: bold; } QLineEdit, QComboBox, QSpinBox, QDateEdit, QPlainTextEdit { min-height: 32px; font-size: 11pt; } QDialogButtonBox QPushButton { min-width: 90px; min-height: 32px; font-size: 11pt; } QLabel { font-size: 11pt; } ");

        requestIdEdit = new QLineEdit(this);
        requestIdEdit->setText(generateRequestId(staffUsername));

        fromDateEdit = new QDateEdit(this);
        fromDateEdit->setCalendarPopup(true);
        fromDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        fromDateEdit->setDate(QDate::currentDate().addDays(-7));

        toDateEdit = new QDateEdit(this);
        toDateEdit->setCalendarPopup(true);
        toDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        toDateEdit->setDate(QDate::currentDate());

        handledSpin = new QSpinBox(this);
        handledSpin->setRange(0, 100000);

        lostSpin = new QSpinBox(this);
        lostSpin->setRange(0, 100000);

        overdueSpin = new QSpinBox(this);
        overdueSpin->setRange(0, 100000);

        notesEdit = new QPlainTextEdit(this);
        notesEdit->setPlaceholderText(tr("Mô tả các vụ việc mất/hỏng sách, yêu cầu xóa sách..."));

        errorLabel = new QLabel(this);
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setVisible(false);

        auto *formGroup = new QGroupBox(tr("Thông tin báo cáo"), this);
        auto *form = new QFormLayout;
        form->setContentsMargins(12, 12, 12, 12);
        form->setHorizontalSpacing(12);
        form->setVerticalSpacing(10);
        form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        form->addRow(tr("Mã yêu cầu"), requestIdEdit);
        form->addRow(tr("Từ ngày"), fromDateEdit);
        form->addRow(tr("Đến ngày"), toDateEdit);
        form->addRow(tr("Số phiếu xử lý"), handledSpin);
        form->addRow(tr("Số sách mất/hỏng"), lostSpin);
        form->addRow(tr("Độc giả quá hạn"), overdueSpin);
        form->addRow(tr("Ghi chú"), notesEdit);
        formGroup->setLayout(form);

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &ReportRequestDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &ReportRequestDialog::reject);

        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(16, 16, 16, 16);
        layout->setSpacing(12);
        layout->addWidget(formGroup);
        layout->addWidget(errorLabel);
        layout->addWidget(buttonBox);
        setMinimumSize(640, 560);
    }

    bool ReportRequestDialog::validateInputs() const {
        if (requestIdEdit->text().trimmed().isEmpty()) {
            showError(tr("Mã yêu cầu không được để trống."));
            return false;
        }
        if (toDateEdit->date() < fromDateEdit->date()) {
            showError(tr("Ngày kết thúc phải lớn hơn ngày bắt đầu."));
            return false;
        }
        return true;
    }

    void ReportRequestDialog::showError(const QString &message) const {
        errorLabel->setText(message);
        errorLabel->setVisible(true);
    }

    model::ReportRequest ReportRequestDialog::reportRequest() const {
        model::ReportRequest req;
        req.setRequestId(bridge::toCustomString(requestIdEdit->text().trimmed()));
        req.setStaffUsername(bridge::toCustomString(staffUsername));
        req.setFromDate(bridge::toCoreDate(fromDateEdit->date()));
        req.setToDate(bridge::toCoreDate(toDateEdit->date()));
        req.setHandledLoans(handledSpin->value());
        req.setLostOrDamaged(lostSpin->value());
        req.setOverdueReaders(overdueSpin->value());
        req.setNotes(bridge::toCustomString(notesEdit->toPlainText().trimmed()));
        req.setStatus(custom::CustomStringLiteral("PENDING"));
        req.setCreatedAt(core::DateTime::nowUtc());
        return req;
    }

    void ReportRequestDialog::accept() {
        errorLabel->setVisible(false);
        if (!validateInputs()) return;
        QDialog::accept();
    }

}
