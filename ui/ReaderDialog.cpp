#include "ReaderDialog.h"

#include "QtBridge.h"
#include "service/ReaderService.h"

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
    setWindowTitle(tr("Thôn tin bạn đọc"));
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
    genderCombo->addItems({tr("chọn"), tr("Nam"), tr("Nữ"), tr("Khác")});
    genderCombo->setCurrentIndex(-1);
    genderCombo->setEditText(QString());
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
        phoneEdit->setValidator(new QIntValidator(0, 2147483647, this)); // Chỉ cho phép nhập số
    emailEdit = new QLineEdit(this);
    identityCardEdit = new QLineEdit(this);
        identityCardEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{12}$"), this)); // Chỉ cho phép nhập đúng 12 số
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
    activeCheck = new QCheckBox(tr("Đang hoạt động"), this);
    activeCheck->setChecked(true);
    connect(activeCheck, &QCheckBox::toggled, this, [this](const bool checked) {
        activeFlag = checked;
    });
    activeFlag = true;

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thông tin bạn đọc"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Mã bạn đọc"), idEdit);
    form->addRow(tr("Họ tên"), fullNameEdit);
    form->addRow(tr("Giới tính"), genderCombo);
    form->addRow(tr("Địa chỉ"), addressEdit);
    form->addRow(tr("Số điện thoại"), phoneEdit);
    form->addRow(tr("Email"), emailEdit);
    form->addRow(tr("Số CCCD"), identityCardEdit);
    form->addRow(tr("Ghi chú"), notesEdit);
    form->addRow(tr("Ngày sinh"), dobEdit);
    form->addRow(tr("Ngày đăng ký"), createdDateEdit);
    form->addRow(tr("Ngày hết hạn"), expiryDateEdit);
    form->addRow(tr("Số lần mượn"), totalBorrowedSpin);
    form->addRow(tr("Trạng thái"), activeCheck);
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

void ReaderDialog::setReader(const model::Reader &reader, const bool editing) {
    editingMode = editing;
    idEdit->setText(bridge::toQString(reader.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    idEdit->setEnabled(!editing && !forceIdReadOnly);
    fullNameEdit->setText(bridge::toQString(reader.getFullName()));
    if (const QString genderText = bridge::toQString(reader.getGender()).trimmed(); !genderText.isEmpty()) {
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

    if (editing) {
        idEdit->setReadOnly(true);
        idEdit->setEnabled(false);
        fullNameEdit->setReadOnly(true);
    } else {
        idEdit->setEnabled(!forceIdReadOnly);
        fullNameEdit->setReadOnly(false);
    }
}

void ReaderDialog::presetId(const QString &id, const bool lockField) {
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
        showError(tr("Mã bạn đọc không được để trống."));
        return false;
    }
    if (fullNameEdit->text().trimmed().isEmpty()) {
        showError(tr("Họ tên không được để trống."));
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

    // Kiểm tra trùng lặp số điện thoại và căn cước công dân
    // Giả sử có thể truy cập service qua biến readerService (cần chỉnh lại nếu tên khác)
    extern pbl2::service::ReaderService *readerService;
    const auto phone = phoneEdit->text().trimmed();
    const auto identityCard = identityCardEdit->text().trimmed();
    if (readerService && readerService->isDuplicatePhoneOrIdentityCard(bridge::toCustomString(phone), bridge::toCustomString(identityCard))) {
        showError(tr("Số điện thoại hoặc số CCCD đã tồn tại trong hệ thống."));
        return;
    }
    QDialog::accept();
}

}  // namespace ui
