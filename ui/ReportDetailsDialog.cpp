#include "ReportDetailsDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "QtBridge.h"

using namespace std;

namespace {

QString formatDate(const QDate &value) {
    if (!value.isValid()) {
        return QObject::tr("Khong ro");
    }
    return value.toString(Qt::ISODate);
}

QString formatDateTime(const QDateTime &value) {
    if (!value.isValid()) {
        return QObject::tr("Khong ro");
    }
    return value.toString(QStringLiteral("yyyy-MM-dd hh:mm"));
}

QLabel *makeCaption(const QString &text, QWidget *parent) {
    auto *label = new QLabel(text, parent);
    label->setMinimumWidth(120);
    return label;
}

QLabel *makeValueLabel(const QString &text, QWidget *parent) {
    auto *label = new QLabel(text, parent);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    return label;
}

}  // namespace

namespace pbl2::ui {

ReportDetailsDialog::ReportDetailsDialog(const model::ReportRequest &report,
                                         const QString &statusText,
                                         QWidget *parent)
    : QDialog(parent) {
    setWindowTitle(tr("Chi tiet bao cao"));
    setModal(true);
    setMinimumSize(640, 560);
    setWindowIcon(QIcon(":/ui/resources/icons/report.png"));
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

    const QString requestId = bridge::toQString(report.getRequestId());
    const QString staff = bridge::toQString(report.getStaffUsername());
    const QString notes = bridge::toQString(report.getNotes()).trimmed();

    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    form->addRow(makeCaption(tr("Ma yeu cau"), this), makeValueLabel(displayOrFallback(requestId, tr("Khong ro")), this));
    form->addRow(makeCaption(tr("Nhan vien"), this), makeValueLabel(displayOrFallback(staff, tr("Khong ro")), this));
    form->addRow(makeCaption(tr("Tu ngay"), this), makeValueLabel(formatDate(bridge::toQDate(report.getFromDate())), this));
    form->addRow(makeCaption(tr("Den ngay"), this), makeValueLabel(formatDate(bridge::toQDate(report.getToDate())), this));
    form->addRow(makeCaption(tr("Trang thai"), this), makeValueLabel(displayOrFallback(statusText, tr("Khong ro")), this));
    form->addRow(makeCaption(tr("Ngay tao"), this), makeValueLabel(formatDateTime(bridge::toQDateTime(report.getCreatedAt())), this));
    form->addRow(makeCaption(tr("So phieu xu ly"), this), makeValueLabel(QString::number(report.getHandledLoans()), this));
    form->addRow(makeCaption(tr("Sach mat/hu"), this), makeValueLabel(QString::number(report.getLostOrDamaged()), this));
    form->addRow(makeCaption(tr("Doc gia qua han"), this), makeValueLabel(QString::number(report.getOverdueReaders()), this));

    auto *notesLabel = new QLabel(tr("Ghi chu"), this);
    auto *notesViewer = new QPlainTextEdit(this);
    notesViewer->setPlainText(notes.isEmpty() ? tr("(Khong co ghi chu)") : notes);
    notesViewer->setReadOnly(true);
    notesViewer->setMinimumHeight(140);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ReportDetailsDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addLayout(form);
    layout->addWidget(notesLabel);
    layout->addWidget(notesViewer);
    layout->addWidget(buttonBox, 0, Qt::AlignRight);
}

QString ReportDetailsDialog::displayOrFallback(const QString &value, const QString &fallback) {
    return value.trimmed().isEmpty() ? fallback : value;
}

}  // namespace pbl2::ui
