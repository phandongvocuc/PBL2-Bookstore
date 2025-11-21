#include "AccountDialog.h"

#include "QtBridge.h"
#include "core/custom/CaseSensitivity.h"
#include "core/custom/CustomString.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace std;  // project-wide request

namespace pbl2::ui {

AccountDialog::AccountDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thong tin tai khoan"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/account.png"));
    const QFont font("Segoe UI", 11);
    setFont(font);
    // Dùng chung style đơn giản với Thong tin ban doc
    setStyleSheet("QDialog { background: #f8fafc; border-radius: 12px; } "
                  "QGroupBox { font-weight: bold; } "
                  "QLineEdit, QComboBox, QSpinBox, QDateEdit, QPlainTextEdit { "
                  "  min-height: 32px; font-size: 11pt; } "
                  "QDialogButtonBox QPushButton { "
                  "  min-width: 90px; min-height: 32px; font-size: 11pt; } "
                  "QLabel { font-size: 11pt; } ");

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmEdit = new QLineEdit(this);
    confirmEdit->setEchoMode(QLineEdit::Password);

    roleCombo = new QComboBox(this);
    roleCombo->addItems({QStringLiteral("STAFF"), QStringLiteral("ADMIN")});

    staffCombo = new QComboBox(this);
    staffCombo->setEditable(false);
    staffCombo->addItem(tr("(Chua lien ket)"), QString());

    activeCheck = new QCheckBox(tr("Dang hoat dong"), this);
    activeCheck->setChecked(true);

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thong tin tai khoan"), this);
    auto *form = new QFormLayout();
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Ten dang nhap"), usernameEdit);
    form->addRow(tr("Mat khau"), passwordEdit);
    form->addRow(tr("Nhap lai mat khau"), confirmEdit);
    form->addRow(tr("Quyen"), roleCombo);
    form->addRow(tr("Nhan vien"), staffCombo);
    form->addRow(tr("Trang thai"), activeCheck);
    formGroup->setLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AccountDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AccountDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addWidget(formGroup);
    layout->addWidget(errorLabel);
    layout->addWidget(buttonBox);
    setMinimumSize(640, 520);
}

custom::CustomString AccountDialog::username() const { return pbl2::bridge::toCustomString(usernameEdit->text().trimmed()); }

custom::CustomString AccountDialog::password() const { return pbl2::bridge::toCustomString(passwordEdit->text()); }

custom::CustomString AccountDialog::role() const { return pbl2::bridge::toCustomString(roleCombo->currentText()); }

bool AccountDialog::isActive() const { return activeCheck->isChecked(); }

bool AccountDialog::validateInputs() const {
    if (usernameEdit->text().trimmed().isEmpty()) {
    showError(pbl2::bridge::toCustomString(tr("Ten dang nhap khong duoc de trong.")));
        return false;
    }
    if (!editingMode || !passwordEdit->text().isEmpty() || !confirmEdit->text().isEmpty()) {
        if (passwordEdit->text().length() < 4) {
            showError(pbl2::bridge::toCustomString(tr("Mat khau phai co it nhat 4 ky tu.")));
            return false;
        }
        if (passwordEdit->text() != confirmEdit->text()) {
            showError(pbl2::bridge::toCustomString(tr("Mat khau nhap lai khong trung khop.")));
            return false;
        }
    }
    return true;
}

void AccountDialog::showError(const custom::CustomString &message) const {
    errorLabel->setText(pbl2::bridge::toQString(message));
    errorLabel->setVisible(true);
}

void AccountDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) return;
    QDialog::accept();
}

void AccountDialog::setStaffList(const custom::Vector<model::Staff> &staffs) const {
    // Clear except first placeholder
    const custom::CustomString currentSel = pbl2::bridge::toCustomString(staffCombo->currentData().toString());
    staffCombo->clear();
    staffCombo->addItem(tr("(Chua lien ket)"), QString());
    for (const auto &s : staffs) {
        const QString id = pbl2::bridge::toQString(s.getId());
        const QString name = pbl2::bridge::toQString(s.getFullName());
        const QString display = QStringLiteral("%1 - %2").arg(id, name);
        staffCombo->addItem(display, id);
    }
    // Try reselect
    for (int i = 0; i < staffCombo->count(); ++i) {
        if (pbl2::bridge::toCustomString(staffCombo->itemData(i).toString()).compare(currentSel, custom::CaseSensitivity::Insensitive) == 0) {
            staffCombo->setCurrentIndex(i);
            break;
        }
    }
}

custom::CustomString AccountDialog::staffId() const {
    return pbl2::bridge::toCustomString(staffCombo->currentData().toString());
}

}  // namespace ui
