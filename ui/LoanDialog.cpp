#include "LoanDialog.h"

#include "QtBridge.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVariant>
#include <QVBoxLayout>

using namespace std;  // project-wide request

namespace {

using pbl2::model::Book;
using pbl2::model::Loan;
using pbl2::model::Reader;

QString displayReader(const Reader &reader) {
    const QString id = pbl2::bridge::toQString(reader.getId());
    const QString name = pbl2::bridge::toQString(reader.getFullName());
    return QStringLiteral("%1 - %2").arg(id, name);
}

QString displayBook(const Book &book) {
    const QString bookId = pbl2::bridge::toQString(book.getId());
    const QString title = pbl2::bridge::toQString(book.getTitle());
    return QStringLiteral("%1 - %2").arg(bookId, title);
}

}

namespace pbl2 {
namespace ui {

LoanDialog::LoanDialog(const custom::Vector<model::Reader> &readers, const custom::Vector<model::Book> &books, int maxBorrowDays, QWidget *parent)
        : QDialog(parent),
            readers(readers),
            books(books) {
    setWindowTitle(tr("Phieu muon tra"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/loan.png"));
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
/* Form labels: darker for better contrast */
QLabel { font-size: 11pt; color: #6b7280; }
QLabel[error="true"] { color: #dc2626; font-size: 10.5pt; padding: 6px; font-weight: 600; }
)");

    loanIdEdit = new QLineEdit(this);
    readerCombo = new QComboBox(this);
    bookCombo = new QComboBox(this);

    for (const auto &reader : readers) {
        readerCombo->addItem(displayReader(reader), QVariant(pbl2::bridge::toQString(reader.getId())));
    }
    for (const auto &book : books) {
        const QString bookId = pbl2::bridge::toQString(book.getId());
        bookCombo->addItem(displayBook(book), QVariant(bookId));
    }

    borrowDateEdit = new QDateEdit(this);
    borrowDateEdit->setCalendarPopup(true);
    borrowDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    borrowDateEdit->setDate(QDate::currentDate());

    dueDateEdit = new QDateEdit(this);
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    dueDateEdit->setDate(QDate::currentDate().addDays(maxBorrowDays > 0 ? maxBorrowDays : 14));

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Chi tiet phieu muon"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Ma phieu"), loanIdEdit);
    form->addRow(tr("Ban doc"), readerCombo);
    form->addRow(tr("Sach"), bookCombo);
    form->addRow(tr("Ngay muon"), borrowDateEdit);
    form->addRow(tr("Ngay tra han"), dueDateEdit);
    formGroup->setLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LoanDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LoanDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addWidget(formGroup);
    layout->addWidget(errorLabel);
    layout->addWidget(buttonBox);
    setMinimumSize(600, 500);
}

void LoanDialog::presetLoanId(const QString &loanId, bool lockField) {
    forceIdReadOnly = lockField;
    loanIdEdit->setText(loanId.trimmed());
    loanIdEdit->setReadOnly(editingMode || forceIdReadOnly);
}

model::Loan LoanDialog::loan() const {
    model::Loan loan;
    loan.setLoanId(pbl2::bridge::toCustomString(loanIdEdit->text().trimmed()));
    loan.setReaderId(pbl2::bridge::toCustomString(readerCombo->currentData().toString()));
    loan.setBookId(pbl2::bridge::toCustomString(bookCombo->currentData().toString()));
    loan.setBorrowDate(pbl2::bridge::toCoreDate(borrowDateEdit->date()));
    loan.setDueDate(pbl2::bridge::toCoreDate(dueDateEdit->date()));
    loan.setStatus(pbl2::bridge::toCustomString(QStringLiteral("BORROWED")));
    loan.setFine(0);
    return loan;
}

bool LoanDialog::validateInputs() const {
    if (loanIdEdit->text().trimmed().isEmpty()) {
        return false;
    }
    if (readerCombo->currentIndex() < 0 || bookCombo->currentIndex() < 0) {
        return false;
    }
    if (dueDateEdit->date() < borrowDateEdit->date()) {
        return false;
    }
    return true;
}

void LoanDialog::showError(const QString &message) const {
    errorLabel->setText(message);
    errorLabel->setVisible(true);
}

void LoanDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) {
        showError(tr("Vui long kiem tra lai cac truong bat buoc."));
        return;
    }
    QDialog::accept();
}

}  // namespace ui
}  // namespace pbl2
