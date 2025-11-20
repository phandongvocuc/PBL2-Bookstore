#pragma once

#include <QMainWindow>
#include <QString>
#include <QMessageBox>

#include <memory>
#include <QDate>

#include "core/custom/Optional.h"
#include "core/custom/CustomString.h"

#include "service/AccountService.h"
#include "service/BookService.h"
#include "service/ConfigService.h"
#include "service/LoanService.h"
#include "service/ReaderService.h"
#include "service/ReportService.h"
#include "service/StaffService.h"
#include "model/Account.h"
#include "model/ReportRequest.h"
#include "model/Book.h"
#include "model/Loan.h"
#include "model/Reader.h"
#include "model/Staff.h"
#include "SystemConfig.h"
#include "StatsWidget.h"
#include "core/custom/QtContainers.h"

namespace Ui {
class MainWindow;
}

class QLabel;
class QTabWidget;
class QListWidget;
class QTableWidget;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QPushButton;
class QSpinBox;
class QCheckBox;
namespace pbl2::ui {

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const QString &dataDir, const model::Account &signedInAccount, QWidget *parent = nullptr);
    ~MainWindow() override;
    enum class EventSeverity { Info, Success, Warning, Error };

signals:
    void logoutRequested();

private:
    void setupUi();

    void configureBooksTab();
    void configureReadersTab();
    void configureStaffsTab();
    void configureLoansTab();
    void configureReportsTab();
    void configureStatsTab();
    void configureAccountsTab();
    void configureSettingsTab();

    void setupNavigationMenu();
    void handleNavigationSelection(int index) const;

    void reloadData();
    void populateBooks();
    void populateReaders();
    void populateStaffs();
    void populateLoans();
    void populateReports();
    void fillReportsList(const custom::Vector<model::ReportRequest> &reports) const;
    void applyReportFilter();
    void clearReportFilter();
    void updateStatisticsSummary();
    void refreshAccounts();
    void refreshConfigInputs() const;

    void applyBookFilter();
    void fillBooksList(const custom::Vector<model::Book> &books) const;
    void applyReaderFilter();
    void fillReadersList(const custom::Vector<model::Reader> &readers) const;
    void applyStaffFilter();
    void fillStaffsList(const custom::Vector<model::Staff> &staffs) const;
    void applyLoanFilter();
    void fillLoansList(const custom::Vector<model::Loan> &loans);
    void fillAccountsList(const custom::Vector<model::Account> &accounts);

    void handleAddBook();
    void handleEditBook();
    void handleDeleteBook();
    void handleRestockBook();

    void handleAddReader();
    void handleEditReader();
    void handleDeleteReader();
    void handleToggleReaderActive();
    void handleAddStaff();
    void handleEditStaff();
    void handleDeleteStaff();
    void handleToggleStaffActive();

    void handleNewLoan();
    void handleMarkReturned();
    void handleExtendLoan();
    void handleMarkLost();
    void handleMarkDamaged();
    void handleDeleteLoan();
    void handleLossOrDamage(const QString &status);

    void handleSubmitReport();
    void handleReportStatusChange(const QString &status);
    void handleViewReportDetails();

    void handleAddAccount();
    void handleResetPassword();
    void handleToggleAccountActive();
    void handleDeleteAccount();

    void handleSaveConfig();
    void handleLogout();

    static custom::Optional<int> currentRow(const QListWidget *list);
    QString nextBookId() const;
    QString nextReaderId() const;
    QString nextStaffId() const;
    QString nextLoanId() const;
    std::unique_ptr<Ui::MainWindow> ui;

    QTabWidget *tabs{nullptr};
    QListWidget *booksList{nullptr};
    QListWidget *readersList{nullptr};
    QListWidget *staffsList{nullptr};
    QListWidget *loansList{nullptr};
    QListWidget *reportsList{nullptr};
    QListWidget *accountsList{nullptr};
    QListWidget *navigationList{nullptr};
    QPushButton *refreshButton{nullptr};
    QLabel *statisticsSummaryLabel{nullptr};
    QWidget *navRail{nullptr};
    QLabel *userInfoLabel{nullptr};
    QPushButton *navToggleButton{nullptr};

    QLabel *totalBooksValue{nullptr};
    QLabel *totalReadersValue{nullptr};
    QLabel *totalLoansValue{nullptr};
    QLabel *overdueLoansValue{nullptr};
    QLabel *totalFinesValue{nullptr};
    QLabel *summaryBorrowedValue{nullptr};
    QLabel *summaryReturnedValue{nullptr};
    QLabel *summaryOverdueValue{nullptr};
    QLabel *summaryFinesValue{nullptr};
    QPushButton *navRailButton{nullptr};
    QLabel *statsLabel{nullptr};
    
    StatsWidget *statsWidget{nullptr};

    custom::CustomString dataDirectory;
    service::BookService bookService;
    service::ReaderService readerService;
    service::StaffService staffService;
    service::LoanService loanService;
    service::AccountService accountService;
    service::ReportService reportService;
    service::ConfigService configService;
    model::Account currentAccount;

        void refreshSimpleStats();
    bool navCollapsed{false};
    bool navPinned{false};
    // Preferred widths (pixels) for collapsed and expanded navigation
    int navCollapsedWidth{56};
    int navExpandedWidth{220};
    // NavMode removed: simple show/hide behavior requested by user
    // NavMode navMode{NavMode::On};

    // Event filter to detect hover enter/leave on navigation list
    bool eventFilter(QObject *watched, QEvent *event) override;
    // Toggle or set collapsed state for the navigation pane
    void setNavigationCollapsed(bool collapsed, bool pinned = false);
    void repositionNavRailButton() const;
    void showNavigationPopupMenu();
    void resizeEvent(QResizeEvent *event) override;

    QLineEdit *reportStaffFilter{nullptr};
    QDateEdit *reportFromFilter{nullptr};
    QDateEdit *reportToFilter{nullptr};
    custom::Vector<model::ReportRequest> reportsCache;
    custom::Vector<model::Book> booksCache;
    custom::Vector<model::Reader> readersCache;
    custom::Vector<model::Staff> staffsCache;
    custom::Vector<model::Loan> loansCache;
    custom::Vector<model::Account> accountsCache;

    QLineEdit *bookSearchEdit{nullptr};
    QComboBox *bookStatusFilter{nullptr};
    QLineEdit *readerSearchEdit{nullptr};
    QComboBox *readerStatusFilter{nullptr};
    QLineEdit *staffSearchEdit{nullptr};
    QComboBox *staffStatusFilter{nullptr};
    QLineEdit *loanSearchEdit{nullptr};
    QComboBox *loanStatusFilter{nullptr};

    QSpinBox *maxBorrowDaysSpin{nullptr};
    QSpinBox *finePerDaySpin{nullptr};
    QSpinBox *maxBooksPerReaderSpin{nullptr};

    QComboBox *timePeriodCombo{nullptr};
    QComboBox *genreFilterCombo{nullptr};
    QString statsSelectedGenre;
    QTableWidget *loanStatsTable{nullptr};
    QLabel *overdueCount{nullptr};
    QLabel *monthlyFinesValue{nullptr};
    QListWidget *activeReadersList{nullptr};
    QListWidget *topBooksList{nullptr};
    StatsChart *topBooksChart{nullptr};
    StatsChart *revenueChart{nullptr};
    StatsChart *genreChart{nullptr};
    QPushButton *applyFilterButton{nullptr};
    
    QDate statsStartDate;
    QDate statsEndDate;

    void applyStatsFilter();
    void updateStatsCards();
    void updateStatsCharts();

    // void exportStatisticsPdf(); // Removed - no longer needed

    bool adminRole{false};
    bool staffRole{false};
    model::SystemConfig currentConfig;

    void notifyEvent(const QString &message, EventSeverity severity = EventSeverity::Info, int durationMs = 2000);
    void showInfoDialog(const QString &title, const QString &message);
    void showWarningDialog(const QString &title, const QString &message);
    void showErrorDialog(const QString &title, const QString &message);
    QMessageBox::StandardButton askEventQuestion(const QString &title, const QString &message, QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};

}  // namespace pbl2::ui
