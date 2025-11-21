#include "ReaderDialog.h"

#include "QtBridge.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace std;

namespace pbl2::ui {

ReaderDialog::ReaderDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thong tin ban doc"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/reader.png"));
    const QFont font("Segoe UI", 11);
    setFont(font);
    // Giữ nguyên style đơn giản cho màn Thông tin độc giả
    setStyleSheet("QDialog { background: #f8fafc; border-radius: 12px; } "
                  "QGroupBox { font-weight: bold; } "
                  "QLineEdit, QComboBox, QSpinBox, QDateEdit, QPlainTextEdit { "
                  "  min-height: 32px; font-size: 11pt; } "
                  "QDialogButtonBox QPushButton { "
                  "  min-width: 90px; min-height: 32px; font-size: 11pt; } "
                  "QLabel { font-size: 11pt; } ");

    idEdit = new QLineEdit(this);
    fullNameEdit = new QLineEdit(this);
    genderCombo = new QComboBox(this);
    genderCombo->setEditable(true);
    genderCombo->addItems({tr("chon"), tr("Nam"), tr("Nu"), tr("Khac")});
    genderCombo->setCurrentIndex(-1);
    genderCombo->setEditText(QString());
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    identityCardEdit = new QLineEdit(this);
    notesEdit = new QLineEdit(this);
    dobEdit = new QDateEdit(this);
    dobEdit->setCalendarPopup(true);
    dobEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    dobEdit->setDate(QDate::currentDate());
    createdDateEdit = new QDateEdit(this);
    createdDateEdit->setCalendarPopup(true);
    createdDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    createdDateEdit->setDate(QDate::currentDate());
    expiryDateEdit = new QDateEdit(this);
    expiryDateEdit->setCalendarPopup(true);
    expiryDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    expiryDateEdit->setDate(QDate::currentDate());
    totalBorrowedSpin = new QSpinBox(this);
    totalBorrowedSpin->setRange(0, 100000);
    activeCheck = new QCheckBox(tr("Dang hoat dong"), this);
    activeCheck->setChecked(true);
    connect(activeCheck, &QCheckBox::toggled, this, [this](bool checked) {
        activeFlag = checked;
    });
    activeFlag = true;

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thong tin ban doc"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Ma ban doc"), idEdit);
    form->addRow(tr("Ho ten"), fullNameEdit);
    form->addRow(tr("Gioi tinh"), genderCombo);
    form->addRow(tr("Dia chi"), addressEdit);
    form->addRow(tr("So dien thoai"), phoneEdit);
    form->addRow(tr("Email"), emailEdit);
    form->addRow(tr("So CCCD"), identityCardEdit);
    form->addRow(tr("Ghi chu"), notesEdit);
    form->addRow(tr("Ngay sinh"), dobEdit);
    form->addRow(tr("Ngay dang ky"), createdDateEdit);
    form->addRow(tr("Ngay het han"), expiryDateEdit);
    form->addRow(tr("So lan muon"), totalBorrowedSpin);
    form->addRow(tr("Trang thai"), activeCheck);
    formGroup->setLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ReaderDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ReaderDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addWidget(formGroup);
    layout->addWidget(errorLabel);
    layout->addWidget(buttonBox);
    setMinimumSize(640, 600);
}

void ReaderDialog::setReader(const model::Reader &reader, bool editing) {
    editingMode = editing;
    idEdit->setText(bridge::toQString(reader.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    fullNameEdit->setText(bridge::toQString(reader.getFullName()));
    const QString genderText = bridge::toQString(reader.getGender()).trimmed();
    if (!genderText.isEmpty()) {
        genderCombo->setCurrentText(genderText);
    } else {
        genderCombo->setCurrentIndex(-1);
        genderCombo->setEditText(QString());
    }
    addressEdit->setText(bridge::toQString(reader.getAddress()));
    phoneEdit->setText(bridge::toQString(reader.getPhone()));
    emailEdit->setText(bridge::toQString(reader.getEmail()));
    identityCardEdit->setText(bridge::toQString(reader.getIdentityCard()));
    notesEdit->setText(bridge::toQString(reader.getNotes()));
    if (reader.getDob().isValid()) {
        dobEdit->setDate(bridge::toQDate(reader.getDob()));
    } else {
        dobEdit->setDate(QDate::currentDate());
    }
    if (reader.getCreatedDate().isValid()) {
        createdDateEdit->setDate(bridge::toQDate(reader.getCreatedDate()));
    } else {
        createdDateEdit->setDate(QDate::currentDate());
    }
    if (reader.getExpiryDate().isValid()) {
        expiryDateEdit->setDate(bridge::toQDate(reader.getExpiryDate()));
    } else {
        expiryDateEdit->setDate(QDate::currentDate());
    }
    totalBorrowedSpin->setValue(reader.getTotalBorrowed());
    activeFlag = reader.isActive();
    activeCheck->setChecked(activeFlag);
}

void ReaderDialog::presetId(const QString &id, bool lockField) {
    forceIdReadOnly = lockField;
    idEdit->setText(id.trimmed());
    idEdit->setReadOnly(editingMode || forceIdReadOnly);
}

model::Reader ReaderDialog::reader() const {
    model::Reader r;
    r.setId(bridge::toCustomString(idEdit->text().trimmed()));
    r.setFullName(bridge::toCustomString(fullNameEdit->text().trimmed()));
    r.setGender(bridge::toCustomString(genderCombo->currentText().trimmed()));
    r.setAddress(bridge::toCustomString(addressEdit->text().trimmed()));
    r.setPhone(bridge::toCustomString(phoneEdit->text().trimmed()));
    r.setEmail(bridge::toCustomString(emailEdit->text().trimmed()));
    r.setIdentityCard(bridge::toCustomString(identityCardEdit->text().trimmed()));
    r.setNotes(bridge::toCustomString(notesEdit->text().trimmed()));
    r.setDob(bridge::toCoreDate(dobEdit->date()));
    r.setCreatedDate(bridge::toCoreDate(createdDateEdit->date()));
    r.setExpiryDate(bridge::toCoreDate(expiryDateEdit->date()));
    r.setTotalBorrowed(totalBorrowedSpin->value());
    r.setActive(activeFlag);
    return r;
}

bool ReaderDialog::validateInputs() const {
    if (idEdit->text().trimmed().isEmpty()) {
        showError(tr("Ma ban doc khong duoc de trong."));
        return false;
    }
    if (fullNameEdit->text().trimmed().isEmpty()) {
        showError(tr("Ho ten khong duoc de trong."));
        return false;
    }
    return true;
}

void ReaderDialog::showError(const QString &message) const {
    errorLabel->setText(message);
    errorLabel->setVisible(true);
}

void ReaderDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) return;
    QDialog::accept();
}

}  // namespace ui
