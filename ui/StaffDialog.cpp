#include "StaffDialog.h"

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
#include <QVBoxLayout>

using namespace std;  // project-wide request

namespace pbl2::ui {

StaffDialog::StaffDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thong tin nhan vien"));
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
    fullNameEdit = new QLineEdit(this);
    genderCombo = new QComboBox(this);
    genderCombo->setEditable(true);
    genderCombo->addItems({tr("chon"), tr("Nam"), tr("Nu"), tr("Khac")});
    genderCombo->setCurrentIndex(-1);
    genderCombo->setEditText(QString());
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    positionCombo = new QComboBox(this);
    positionCombo->setEditable(true);
    positionCombo->addItems({tr("chon"),tr("Thu thu"), tr("Quan ly"), tr("Nhan vien"), tr("Tro ly"), tr("Khac")});
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

    activeCheck = new QCheckBox(tr("Dang hoat dong"), this);
    activeCheck->setChecked(true);
    connect(activeCheck, &QCheckBox::toggled, this, [this](bool checked) {
        activeFlag = checked;
    });

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);
    

    auto *formGroup = new QGroupBox(tr("Thong tin nhan vien"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Ma nhan vien"), idEdit);
    form->addRow(tr("Ho ten"), fullNameEdit);
    form->addRow(tr("Gioi tinh"), genderCombo);
    form->addRow(tr("Dia chi"), addressEdit);
    form->addRow(tr("So dien thoai"), phoneEdit);
    form->addRow(tr("Email"), emailEdit);
    form->addRow(tr("Chuc vu"), positionCombo);
    form->addRow(tr("Ghi chu"), notesEdit);
    form->addRow(tr("Ngay sinh"), dobEdit);
    form->addRow(tr("Ngay vao lam"), hireDateEdit);
    form->addRow(tr("Trang thai"), activeCheck);
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
    idEdit->setText(pbl2::bridge::toQString(staff.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    fullNameEdit->setText(pbl2::bridge::toQString(staff.getFullName()));
    const QString genderText = pbl2::bridge::toQString(staff.getGender()).trimmed();
    if (!genderText.isEmpty()) {
        genderCombo->setCurrentText(genderText);
    } else {
        genderCombo->setCurrentIndex(-1);
        genderCombo->setEditText(QString());
    }
    addressEdit->setText(pbl2::bridge::toQString(staff.getAddress()));
    phoneEdit->setText(pbl2::bridge::toQString(staff.getPhone()));
    emailEdit->setText(pbl2::bridge::toQString(staff.getEmail()));
    const QString positionText = pbl2::bridge::toQString(staff.getPosition()).trimmed();
    if (!positionText.isEmpty()) {
        if (positionCombo->findText(positionText, Qt::MatchFixedString | Qt::MatchCaseSensitive) == -1) {
            positionCombo->addItem(positionText);
        }
        positionCombo->setCurrentText(positionText);
    } else {
        positionCombo->setCurrentIndex(-1);
        positionCombo->setEditText(QString());
    }
    notesEdit->setText(pbl2::bridge::toQString(staff.getNotes()));
    if (staff.getDob().isValid()) {
        dobEdit->setDate(pbl2::bridge::toQDate(staff.getDob()));
    } else {
        dobEdit->setDate(QDate::currentDate());
    }
    if (staff.getHireDate().isValid()) {
        hireDateEdit->setDate(pbl2::bridge::toQDate(staff.getHireDate()));
    } else {
        hireDateEdit->setDate(QDate::currentDate());
    }
    activeFlag = staff.isActive();
    activeCheck->setChecked(activeFlag);
}

void StaffDialog::presetId(const QString &id, bool lockField) {
    forceIdReadOnly = lockField;
    idEdit->setText(id.trimmed());
    idEdit->setReadOnly(editingMode || forceIdReadOnly);
}

model::Staff StaffDialog::staff() const {
    model::Staff s;
    s.setId(pbl2::bridge::toCustomString(idEdit->text().trimmed()));
    s.setFullName(pbl2::bridge::toCustomString(fullNameEdit->text().trimmed()));
    s.setGender(pbl2::bridge::toCustomString(genderCombo->currentText().trimmed()));
    s.setAddress(pbl2::bridge::toCustomString(addressEdit->text().trimmed()));
    s.setPhone(pbl2::bridge::toCustomString(phoneEdit->text().trimmed()));
    s.setEmail(pbl2::bridge::toCustomString(emailEdit->text().trimmed()));
    s.setPosition(pbl2::bridge::toCustomString(positionCombo->currentText().trimmed()));
    s.setNotes(pbl2::bridge::toCustomString(notesEdit->text().trimmed()));
    s.setDob(pbl2::bridge::toCoreDate(dobEdit->date()));
    s.setHireDate(pbl2::bridge::toCoreDate(hireDateEdit->date()));
    s.setActive(activeFlag);
    return s;
}

bool StaffDialog::validateInputs() const {
    if (idEdit->text().trimmed().isEmpty()) {
        showError(tr("Ma nhan vien khong duoc de trong."));
        return false;
    }
    if (fullNameEdit->text().trimmed().isEmpty()) {
        showError(tr("Ho ten khong duoc de trong."));
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
    QDialog::accept();
}

}  // namespace ui
