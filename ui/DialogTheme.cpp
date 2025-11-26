#include "DialogTheme.h"

#include <QWidget>

namespace pbl2::ui {

namespace {
QString buildDialogStylesheet() {
    QString s;

     // Màu nền cho QDialog (login)
     s += "QDialog { background-color: #00D2FF; }";

     // Base button style – primary action
    s += "QPushButton {"
         "  background-color: #1976D2;"   // primary blue
         "  color: white;"
         "  border: none;"
         "  border-radius: 4px;"
         "  padding: 6px 14px;"
         "  font-weight: 500;"
         "}"
         "QPushButton:hover {"
         "  background-color: #1565C0;"
         "}"
         "QPushButton:pressed {"
         "  background-color: #0D47A1;"
         "}"
         "QPushButton:disabled {"
         "  background-color: #B0BEC5;"
         "  color: #ECEFF1;"
         "}";

    // Secondary / subtle actions (filters, refresh, etc.)
    s += "QPushButton#refreshButton,"
         "QPushButton#bookFilterButton,"
         "QPushButton#readerFilterButton,"
         "QPushButton#loanFilterButton,"
         "QPushButton#reportApplyButton,"
         "QPushButton#reportClearButton,"
         "QPushButton#staffFilterButton,"
         "QPushButton#applyFilterButton {"
         "  background-color: #546E7A;"
         "}"
         "QPushButton#refreshButton:hover,"
         "QPushButton#bookFilterButton:hover,"
         "QPushButton#readerFilterButton:hover,"
         "QPushButton#loanFilterButton:hover,"
         "QPushButton#reportApplyButton:hover,"
         "QPushButton#reportClearButton:hover,"
         "QPushButton#staffFilterButton:hover,"
         "QPushButton#applyFilterButton:hover {"
         "  background-color: #455A64;"
         "}";

    // Positive / success actions (approve, return book, etc.)
    s += "QPushButton#approveReportButton,"
         "QPushButton#returnLoanButton,"
         "QPushButton#loginButton,"
         "QPushButton#saveConfigButton,"
         "QPushButton#addBookButton,"
         "QPushButton#addReaderButton,"
         "QPushButton#addStaffButton,"
         "QPushButton#addAccountButton {"
         "  background-color: #2E7D32;"
         "}"
         "QPushButton#approveReportButton:hover,"
         "QPushButton#returnLoanButton:hover,"
         "QPushButton#loginButton:hover,"
         "QPushButton#saveConfigButton:hover,"
         "QPushButton#addBookButton:hover,"
         "QPushButton#addReaderButton:hover,"
         "QPushButton#addStaffButton:hover,"
         "QPushButton#addAccountButton:hover {"
         "  background-color: #1B5E20;"
         "}";

    // Warning actions (lost / damaged)
    s += "QPushButton#lostLoanButton,"
         "QPushButton#damageLoanButton {"
         "  background-color: #FB8C00;"
         "}"
         "QPushButton#lostLoanButton:hover,"
         "QPushButton#damageLoanButton:hover {"
         "  background-color: #EF6C00;"
         "}";

    // Destructive / danger actions (delete, reject, logout, exit)
    s += "QPushButton#deleteBookButton,"
         "QPushButton#deleteReaderButton,"
         "QPushButton#deleteLoanButton,"
         "QPushButton#deleteStaffButton,"
         "QPushButton#deleteAccountButton,"
         "QPushButton#rejectReportButton,"
         "QPushButton#logoutButton,"
         "QPushButton#exitButton {"
         "  background-color: #E53935;"
         "}"
         "QPushButton#deleteBookButton:hover,"
         "QPushButton#deleteReaderButton:hover,"
         "QPushButton#deleteLoanButton:hover,"
         "QPushButton#deleteStaffButton:hover,"
         "QPushButton#deleteAccountButton:hover,"
         "QPushButton#rejectReportButton:hover,"
         "QPushButton#logoutButton:hover,"
         "QPushButton#exitButton:hover {"
         "  background-color: #C62828;"
         "}";

    return s;
}
}  // namespace

QString dialogStylesheet() {
    static const QString stylesheet = buildDialogStylesheet();
    return stylesheet;
}

void applyDialogPalette(const QWidget *widget) {
    Q_UNUSED(widget);
}

void applyDialogTheme(QWidget *widget) {
    if (!widget) return;
    widget->setStyleSheet(dialogStylesheet());
    applyDialogPalette(widget);
}

}  // namespace pbl2::ui
