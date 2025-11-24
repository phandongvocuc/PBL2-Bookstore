#include "AccountDialog.h"

#include "QtBridge.h"
#include "../core/CaseSensitivity.h"
#include "../core/CustomString.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace std;

namespace pbl2::ui {

AccountDialog::AccountDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thông tin tài khoản"));
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
    staffCombo->addItem(tr("(Chưa liên kết)"), QString());

    activeCheck = new QCheckBox(tr("Đang hoạt động"), this);
    activeCheck->setChecked(true);

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thông tin tài khoản"), this);
    auto *form = new QFormLayout();
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Tên đăng nhập"), usernameEdit);
    form->addRow(tr("Mật khẩu"), passwordEdit);
    form->addRow(tr("Nhập lại mật khẩu"), confirmEdit);
    form->addRow(tr("Quyền"), roleCombo);
    form->addRow(tr("Nhân viên"), staffCombo);
    form->addRow(tr("Trạng thái"), activeCheck);
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

core::CustomString AccountDialog::username() const { return bridge::toCustomString(usernameEdit->text().trimmed()); }

core::CustomString AccountDialog::password() const { return bridge::toCustomString(passwordEdit->text()); }

core::CustomString AccountDialog::role() const { return bridge::toCustomString(roleCombo->currentText()); }

bool AccountDialog::isActive() const { return activeCheck->isChecked(); }

bool AccountDialog::validateInputs() const {
    if (usernameEdit->text().trimmed().isEmpty()) {
    showError(bridge::toCustomString(tr("Tên đăng nhập không được để trống.")));
        return false;
    }
    if (!editingMode || !passwordEdit->text().isEmpty() || !confirmEdit->text().isEmpty()) {
        if (passwordEdit->text().length() < 4) {
            showError(bridge::toCustomString(tr("Mật khẩu phải có ít nhất bốn ký tự.")));
            return false;
        }
        if (passwordEdit->text() != confirmEdit->text()) {
            showError(bridge::toCustomString(tr("Mật khẩu nhập lại không trùng khớp.")));
            return false;
        }
    }
    return true;
}

void AccountDialog::showError(const core::CustomString &message) const {
    errorLabel->setText(bridge::toQString(message));
    errorLabel->setVisible(true);
}

void AccountDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) return;
    QDialog::accept();
}

void AccountDialog::setStaffList(const core::DynamicArray<model::Staff> &staffs) const {
    // Clear except first placeholder
    const core::CustomString currentSel = bridge::toCustomString(staffCombo->currentData().toString());
    staffCombo->clear();
    staffCombo->addItem(tr("(Chưa liên kết)"), QString());
    for (const auto &s : staffs) {
        const QString id = bridge::toQString(s.getId());
        const QString name = bridge::toQString(s.getFullName());
        const QString display = QStringLiteral("%1 - %2").arg(id, name);
        staffCombo->addItem(display, id);
    }
    // Try reselect
    for (int i = 0; i < staffCombo->count(); ++i) {
        if (bridge::toCustomString(staffCombo->itemData(i).toString()).compare(currentSel, core::CaseSensitivity::Insensitive) == 0) {
            staffCombo->setCurrentIndex(i);
            break;
        }
    }
}

core::CustomString AccountDialog::staffId() const {
    return bridge::toCustomString(staffCombo->currentData().toString());
}

}  // namespace ui
