#include "ReportRequestDialog.h"

#include <QAbstractSpinBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPlainTextEdit>
#include <QSet>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>

#include "QtBridge.h"
#include "../core/DynamicArray.h"

using namespace std;

namespace {

QString generateRequestId(const QString &staff) {
    return QStringLiteral("REQ-%1-%2")
        .arg(staff.toUpper(), QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmss")));
}

}

namespace pbl2::ui {

    ReportRequestDialog::ReportRequestDialog(const QString &staffUsername,
                                             const core::DynamicArray<core::CustomString> &knownBookIds,
                                             QWidget *parent)
        : QDialog(parent), staffUsername(staffUsername), knownBookIds(knownBookIds) {
        for (auto &id : this->knownBookIds) {
            const QString upper = bridge::toQString(id).trimmed().toUpper();
            id = bridge::toCustomString(upper);
        }
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
        connect(fromDateEdit, &QDateEdit::dateChanged, this, [this]() { refreshAutoFields(); });

        toDateEdit = new QDateEdit(this);
        toDateEdit->setCalendarPopup(true);
        toDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        toDateEdit->setDate(QDate::currentDate());
        connect(toDateEdit, &QDateEdit::dateChanged, this, [this]() { refreshAutoFields(); });

        handledSpin = new QSpinBox(this);
        handledSpin->setRange(0, 100000);

        lostSpin = new QSpinBox(this);
        lostSpin->setRange(0, 100000);
        lostSpin->setReadOnly(true);
        lostSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

        overdueSpin = new QSpinBox(this);
        overdueSpin->setRange(0, 100000);

        affectedBooksEdit = new QPlainTextEdit(this);
        affectedBooksEdit->setPlaceholderText(tr("Nhập sách mất/hỏng dạng <Mã sách>:<Số lượng>, ví dụ: BK001:1; BK010:2"));
        affectedBooksEdit->setMinimumHeight(80);
        connect(affectedBooksEdit, &QPlainTextEdit::textChanged, this, [this]() {
            userEditedAffected = true;
            refreshAffectedCountPreview();
        });

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
        form->addRow(tr("Sách mất/hỏng"), affectedBooksEdit);
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

        refreshAffectedCountPreview();
    }

    core::DynamicArray<QString> ReportRequestDialog::splitTokens(const QString &normalized) const {
        core::DynamicArray<QString> tokens;
        QString current;
        for (const QChar ch : normalized) {
            if (ch == QChar(';')) {
                const QString trimmed = current.trimmed();
                if (!trimmed.isEmpty()) tokens.append(trimmed);
                current.clear();
            } else {
                current.append(ch);
            }
        }
        const QString trimmed = current.trimmed();
        if (!trimmed.isEmpty()) tokens.append(trimmed);
        return tokens;
    }

    bool ReportRequestDialog::isKnownBookId(const QString &idUpper) const {
        if (knownBookIds.isEmpty()) return true;
        for (const auto &id : knownBookIds) {
            if (bridge::toQString(id).compare(idUpper, Qt::CaseInsensitive) == 0) {
                return true;
            }
        }
        return false;
    }

    core::DynamicArray<ReportRequestDialog::AffectedBookEntry> ReportRequestDialog::parseAffectedBooks(core::DynamicArray<QString> *errors) const {
        core::DynamicArray<AffectedBookEntry> aggregated;
        const QString raw = affectedBooksEdit ? affectedBooksEdit->toPlainText() : QString();
        QString normalized = raw;
        normalized.replace('\n', ';');
        normalized.replace(',', ';');

        const auto tokens = splitTokens(normalized);
        for (const auto &token : tokens) {
            const int sep = token.indexOf(':');
            const QString id = (sep >= 0 ? token.left(sep) : token).trimmed();
            const QString countStr = sep >= 0 ? token.mid(sep + 1).trimmed() : QStringLiteral("1");

            if (id.isEmpty()) {
                if (errors) errors->append(tr("Thiếu mã sách trong dòng \"%1\".").arg(token));
                continue;
            }

            bool ok = true;
            const int count = countStr.isEmpty() ? 1 : countStr.toInt(&ok);
            if (!ok) {
                if (errors) errors->append(tr("Số lượng không hợp lệ ở dòng \"%1\".").arg(token));
                continue;
            }

            const QString canonicalId = id.toUpper();
            if (!isKnownBookId(canonicalId)) {
                if (errors) errors->append(tr("Mã sách \"%1\" không tồn tại.").arg(id));
                continue;
            }

            int existingIndex = -1;
            for (int i = 0; i < aggregated.size(); ++i) {
                if (aggregated[i].id.compare(canonicalId, Qt::CaseInsensitive) == 0) {
                    existingIndex = i;
                    break;
                }
            }
            if (existingIndex >= 0) {
                aggregated[existingIndex].count += count;
            } else {
                AffectedBookEntry entry;
                entry.id = canonicalId;
                entry.count = count;
                aggregated.append(entry);
            }
        }
        return aggregated;
    }

    void ReportRequestDialog::refreshAffectedCountPreview() const {
        int total = 0;
        const auto parsed = parseAffectedBooks(nullptr);
        for (int i = 0; i < parsed.size(); ++i) {
            total += parsed[i].count;
        }
        if (lostSpin) lostSpin->setValue(total);
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
        core::DynamicArray<QString> errors;
        const auto affected = parseAffectedBooks(&errors);
        if (!errors.isEmpty()) {
            QString merged;
            for (int i = 0; i < errors.size(); ++i) {
                if (!merged.isEmpty()) merged.append('\n');
                merged.append(errors[i]);
            }
            showError(merged);
            return false;
        }
        if (affected.isEmpty()) {
            showError(tr("Vui lòng nhập danh sách sách bị mất/hỏng (dạng <Mã sách>:<Số lượng>)."));
            return false;
        }
        int total = 0;
        for (int i = 0; i < affected.size(); ++i) {
            total += affected[i].count;
        }
        if (lostSpin) lostSpin->setValue(total);

        return true;
    }

    void ReportRequestDialog::showError(const QString &message) const {
        errorLabel->setText(message);
        errorLabel->setVisible(true);
    }

    model::ReportRequest ReportRequestDialog::reportRequest() const {
        const auto affected = parseAffectedBooks(nullptr);
        QString serialized;
        int totalLost = 0;
        for (int i = 0; i < affected.size(); ++i) {
            const auto &entry = affected[i];
            if (!serialized.isEmpty()) serialized.append(';');
            serialized.append(QStringLiteral("%1:%2").arg(entry.id, QString::number(entry.count)));
            totalLost += entry.count;
        }

        model::ReportRequest req;
        req.setRequestId(bridge::toCustomString(requestIdEdit->text().trimmed()));
        req.setStaffUsername(bridge::toCustomString(staffUsername));
        req.setFromDate(bridge::toCoreDate(fromDateEdit->date()));
        req.setToDate(bridge::toCoreDate(toDateEdit->date()));
        req.setHandledLoans(handledSpin->value());
        req.setLostOrDamaged(totalLost);
        req.setOverdueReaders(overdueSpin->value());
        req.setAffectedBooks(bridge::toCustomString(serialized));
        req.setNotes(bridge::toCustomString(notesEdit->toPlainText().trimmed()));
        req.setStatus(core::CustomStringLiteral("PENDING"));
        req.setCreatedAt(core::DateTime::nowUtc());
        return req;
    }

    void ReportRequestDialog::setLoansData(core::DynamicArray<model::Loan> data) {
        loansData = std::move(data);
        userEditedAffected = false;
        refreshAutoFields();
    }

    void ReportRequestDialog::refreshAutoFields() {
        if (!fromDateEdit || !toDateEdit || loansData.isEmpty()) return;

        const QDate from = fromDateEdit->date();
        const QDate to = toDateEdit->date();
        const QDate today = QDate::currentDate();

        auto normalize = [](const core::CustomString &text) {
            return bridge::toQString(text).trimmed().toUpper();
        };

        int handledCount = 0;
        QSet<QString> overdueReaders;
        QMap<QString, int> lostByBook;

        for (const auto &loan : loansData) {
            const QDate borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()) : QDate();
            if (!borrowDate.isValid() || borrowDate < from || borrowDate > to) continue;

            const QString status = normalize(loan.getStatus());
            const bool isBorrowed = status == QStringLiteral("BORROWED");
            const bool isLostOrDamaged = status == QStringLiteral("LOST") || status == QStringLiteral("DAMAGED");
            if (!isBorrowed && !isLostOrDamaged) continue;

            if (isBorrowed) {
                ++handledCount;
            }

            if (isLostOrDamaged) {
                const QString bookId = bridge::toQString(loan.getBookId()).trimmed().toUpper();
                if (!bookId.isEmpty()) {
                    lostByBook[bookId] += 1;
                }
            }

            const QDate dueDate = loan.getDueDate().isValid() ? bridge::toQDate(loan.getDueDate()) : QDate();
            const bool hasReturn = loan.getReturnDate().isValid();
            if (isBorrowed && !hasReturn && dueDate.isValid() && today > dueDate) {
                const QString readerId = bridge::toQString(loan.getReaderId()).trimmed().toUpper();
                if (!readerId.isEmpty()) overdueReaders.insert(readerId);
            }
        }

        if (handledSpin) handledSpin->setValue(handledCount);
        if (overdueSpin) overdueSpin->setValue(overdueReaders.size());

        // Only auto-fill the sách mất/hỏng field if user hasn't edited it manually.
        if (!userEditedAffected && affectedBooksEdit) {
            QStringList entries;
            int totalLost = 0;
            for (auto it = lostByBook.cbegin(); it != lostByBook.cend(); ++it) {
                entries.append(QStringLiteral("%1:%2").arg(it.key(), QString::number(it.value())));
                totalLost += it.value();
            }
            const QSignalBlocker blocker(affectedBooksEdit);
            affectedBooksEdit->setPlainText(entries.join("; "));
            if (lostSpin) lostSpin->setValue(totalLost);
            // Do not set userEditedAffected here; allow further auto-updates unless user types.
        }
    }

    void ReportRequestDialog::accept() {
        errorLabel->setVisible(false);
        if (!validateInputs()) return;
        QDialog::accept();
    }

}
