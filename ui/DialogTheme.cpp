#include "DialogTheme.h"

#include <QWidget>

namespace pbl2::ui {

namespace {
QString buildDialogStylesheet() {
    return QString::fromLatin1(
        "QDialog {"
        "  background-color: #f8fafc;"
        "  color: #0f172a;"
        "}"
        "QLabel {"
        "  color: #0f172a;"
        "  font-size: 14px;"
        "}"
        "QGroupBox {"
        "  background-color: #ffffff;"
        "  border: 1px solid #d8e3f2;"
        "  border-radius: 12px;"
        "  margin-top: 12px;"
        "  padding-top: 12px;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  left: 16px;"
        "  padding: 0 8px;"
        "  font-weight: 600;"
        "  color: #1f2937;"
        "}"
        "QCheckBox {"
        "  color: #0f172a;"
        "  font-size: 14px;"
        "  spacing: 8px;"
        "}"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit, QDateTimeEdit, QPlainTextEdit, QTextEdit {"
        "  border: 1px solid #cbd5f5;"
        "  border-radius: 8px;"
        "  padding: 8px 14px;"
        "  min-height: 36px;"
        "  background-color: #ffffff;"
        "  color: #0f172a;"
        "  selection-background-color: #bfdbfe;"
        "  selection-color: #0f172a;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus, QDateEdit:focus, QDateTimeEdit:focus, QPlainTextEdit:focus, QTextEdit:focus {"
        "  border: 1px solid #1d4ed8;"
        "  background-color: #f8fafc;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #ffffff;"
        "  border: 1px solid #d8e3f2;"
        "  selection-background-color: rgba(37, 99, 235, 0.18);"
        "  selection-color: #0f172a;"
        "}"
        "QCheckBox::indicator {"
        "  width: 18px;"
        "  height: 18px;"
        "  border: 1px solid #cbd5f5;"
        "  border-radius: 4px;"
        "  background: #ffffff;"
        "}"
    "QCheckBox::indicator:checked {"
    "  background-color: #2563eb;"
    "  border-color: #1d4ed8;"
    "  image: url(:/qt-project.org/styles/commonstyle/images/check.png);"
    "  background-position: center;"
    "  background-repeat: no-repeat;"
        "}"
        "QCheckBox::indicator:unchecked:hover {"
        "  border-color: #94a3f5;"
        "}" 
        "QCheckBox::indicator:disabled {"
        "  background: #e2e8f0;"
        "  border-color: #cbd5f5;"
        "}"
    /* Default button style (secondary/neutral actions) */
    "QPushButton {"
    "  background-color: #374151;" /* slate-700 */
    "  color: #ffffff;"
    "  border-radius: 8px;"
    "  padding: 8px 16px;"
    "  font-weight: 600;"
    "  border: none;"
    "}"
    "QPushButton:hover {"
    "  background-color: #4b5563;"
    "}"
    "QPushButton:pressed {"
    "  background-color: #111827;"
    "}"

    /* Primary dialog actions (OK/Save) use a blue gradient for emphasis */
    "QDialogButtonBox QPushButton {"
    "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3B82F6, stop:1 #2563EB);"
    "  color: #ffffff;"
    "  border-radius: 8px;"
    "  padding: 8px 20px;"
    "  font-weight: 700;"
    "  border: none;"
    "}"
    "QDialogButtonBox QPushButton:hover {"
    "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #60A5FA, stop:1 #3B82F6);"
    "}"
    "QDialogButtonBox QPushButton:pressed {"
    "  background: #1E40AF;"
    "}"
        "QDialogButtonBox QPushButton:disabled {"
        "  background-color: #e2e8f0;"
        "  color: #94a3b8;"
        "}"
        "QDialogButtonBox QPushButton::menu-indicator {"
        "  image: none;"
        "}"
        "QPlainTextEdit, QTextEdit {"
        "  min-height: 120px;"
        "}");
}
}  // namespace

QString dialogStylesheet() {
    static const QString stylesheet = buildDialogStylesheet();
    return stylesheet;
}

void applyDialogPalette(QWidget *widget) {
    if (!widget) return;
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Window, QColor(0xF8, 0xFA, 0xFC));
    pal.setColor(QPalette::Base, QColor(0xFF, 0xFF, 0xFF));
    pal.setColor(QPalette::AlternateBase, QColor(0xF1, 0xF5, 0xF9));
    pal.setColor(QPalette::WindowText, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::Text, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::Button, QColor(0xE2, 0xE8, 0xF0));
    pal.setColor(QPalette::ButtonText, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::Highlight, QColor(0xDB, 0xEA, 0xFE));
    pal.setColor(QPalette::HighlightedText, QColor(0x0F, 0x17, 0x2A));
    widget->setPalette(pal);
}

void applyDialogTheme(QWidget *widget) {
    if (!widget) return;
    widget->setStyleSheet(dialogStylesheet());
    applyDialogPalette(widget);
}

}  // namespace pbl2::ui
