#include "StaffDialog.h"

#include "QtBridge.h"
#include "service/StaffService.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace std;  // project-wide request

namespace pbl2::ui {

StaffDialog::StaffDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thông tin nhân viên"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/staff.png"));
    const QFont font("Segoe UI", 11);
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

    idEdit = new QLineEdit(this);
        idEdit->setValidator(new QIntValidator(0, 2147483647, this)); // Chỉ cho phép nhập số
    fullNameEdit = new QLineEdit(this);
    genderCombo = new QComboBox(this);
    genderCombo->setEditable(true);
    genderCombo->addItems({tr("Chọn"), tr("Nam"), tr("Nữ"), tr("Khác")});
    genderCombo->setCurrentIndex(-1);
    genderCombo->setEditText(QString());
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
        phoneEdit->setValidator(new QIntValidator(0, 2147483647, this)); // Chỉ cho phép nhập số
    emailEdit = new QLineEdit(this);
    positionCombo = new QComboBox(this);
    positionCombo->setEditable(true);
    positionCombo->addItems({tr("Chọn"), tr("Thủ thư"), tr("Quản lý"), tr("Nhân viên"), tr("Trợ lý"), tr("Khác")});
    positionCombo->setCurrentIndex(-1);
    positionCombo->setEditText(QString());
    notesEdit = new QLineEdit(this);
    dobEdit = new QDateEdit(this);
    dobEdit->setCalendarPopup(true);
    dobEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    dobEdit->setDate(QDate::currentDate());
    hireDateEdit = new QDateEdit(this);
    hireDateEdit->setCalendarPopup(true);
    hireDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    hireDateEdit->setDate(QDate::currentDate());

    activeCheck = new QCheckBox(tr("Đang hoạt động"), this);
    activeCheck->setChecked(true);
    connect(activeCheck, &QCheckBox::toggled, this, [this](bool checked) {
        activeFlag = checked;
    });

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);
    

    auto *formGroup = new QGroupBox(tr("Thông tin nhân viên"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Mã nhân viên"), idEdit);
    form->addRow(tr("Họ tên"), fullNameEdit);
    form->addRow(tr("Giới tính"), genderCombo);
    form->addRow(tr("Địa chỉ"), addressEdit);
    form->addRow(tr("Số điện thoại"), phoneEdit);
    form->addRow(tr("Email"), emailEdit);
    form->addRow(tr("Chức vụ"), positionCombo);
    form->addRow(tr("Ghi chú"), notesEdit);
    form->addRow(tr("Ngày sinh"), dobEdit);
    form->addRow(tr("Ngày vào làm"), hireDateEdit);
    form->addRow(tr("Trạng thái"), activeCheck);
    formGroup->setLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &StaffDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &StaffDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addWidget(formGroup);
    layout->addWidget(errorLabel);
    layout->addWidget(buttonBox);
    setMinimumSize(640, 600);
}

void StaffDialog::setStaff(const model::Staff &staff, bool editing) {
    editingMode = editing;
    idEdit->setText(bridge::toQString(staff.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    idEdit->setEnabled(!editing && !forceIdReadOnly);
    fullNameEdit->setText(bridge::toQString(staff.getFullName()));
    if (const QString genderText = bridge::toQString(staff.getGender()).trimmed(); !genderText.isEmpty()) {
        genderCombo->setCurrentText(genderText);
    } else {
        genderCombo->setCurrentIndex(-1);
        genderCombo->setEditText(QString());
    }
    addressEdit->setText(bridge::toQString(staff.getAddress()));
    phoneEdit->setText(bridge::toQString(staff.getPhone()));
    emailEdit->setText(bridge::toQString(staff.getEmail()));
    if (const QString positionText = bridge::toQString(staff.getPosition()).trimmed(); !positionText.isEmpty()) {
        if (positionCombo->findText(positionText, Qt::MatchFixedString | Qt::MatchCaseSensitive) == -1) {
            positionCombo->addItem(positionText);
        }
        positionCombo->setCurrentText(positionText);
    } else {
        positionCombo->setCurrentIndex(-1);
        positionCombo->setEditText(QString());
    }
    notesEdit->setText(bridge::toQString(staff.getNotes()));
    if (staff.getDob().isValid()) {
        dobEdit->setDate(bridge::toQDate(staff.getDob()));
    } else {
        dobEdit->setDate(QDate::currentDate());
    }
    if (staff.getHireDate().isValid()) {
        hireDateEdit->setDate(bridge::toQDate(staff.getHireDate()));
    } else {
        hireDateEdit->setDate(QDate::currentDate());
    }
    activeFlag = staff.isActive();
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

void StaffDialog::presetId(const QString &id, bool lockField) {
    forceIdReadOnly = lockField;
    idEdit->setText(id.trimmed());
    idEdit->setReadOnly(editingMode || forceIdReadOnly);
}

model::Staff StaffDialog::staff() const {
    model::Staff s;
    s.setId(bridge::toCustomString(idEdit->text().trimmed()));
    s.setFullName(bridge::toCustomString(fullNameEdit->text().trimmed()));
    s.setGender(bridge::toCustomString(genderCombo->currentText().trimmed()));
    s.setAddress(bridge::toCustomString(addressEdit->text().trimmed()));
    s.setPhone(bridge::toCustomString(phoneEdit->text().trimmed()));
    s.setEmail(bridge::toCustomString(emailEdit->text().trimmed()));
    s.setPosition(bridge::toCustomString(positionCombo->currentText().trimmed()));
    s.setNotes(bridge::toCustomString(notesEdit->text().trimmed()));
    s.setDob(bridge::toCoreDate(dobEdit->date()));
    s.setHireDate(bridge::toCoreDate(hireDateEdit->date()));
    s.setActive(activeFlag);
    return s;
}

bool StaffDialog::validateInputs() const {
    if (idEdit->text().trimmed().isEmpty()) {
        showError(tr("Mã nhân viên không được để trống"));
        return false;
    }
    if (fullNameEdit->text().trimmed().isEmpty()) {
        showError(tr("Họ tên không được để trống."));
        return false;
    }
    return true;
}

void StaffDialog::showError(const QString &message) const {
    errorLabel->setText(message);
    errorLabel->setVisible(true);
}

void StaffDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) return;

    // Kiểm tra trùng lặp số điện thoại
    extern pbl2::service::StaffService *staffService;
    const auto phone = phoneEdit->text().trimmed();
    if (staffService && staffService->isDuplicatePhone(bridge::toCustomString(phone))) {
        showError(tr("Số điện thoại đã tồn tại trong hệ thống."));
        return;
    }
    QDialog::accept();
}

}  // namespace ui
