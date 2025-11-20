/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "StatsChart.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *refreshAction;
    QAction *logoutAction;
    QWidget *centralwidget;
    QHBoxLayout *centralLayout;
    QFrame *navigationPanel;
    QVBoxLayout *navigationLayout;
    QLabel *navigationTitle;
    QListWidget *navigationList;
    QPushButton *refreshButton;
    QPushButton *logoutButton;
    QTabWidget *tabs;
    QWidget *homeTab;
    QVBoxLayout *homeTabLayout;
    QLabel *homeTitleLabel;
    QLabel *homeHintLabel;
    QSpacerItem *homeSpacer;
    QWidget *booksTab;
    QVBoxLayout *booksTabLayout;
    QGroupBox *booksFilterGroup;
    QVBoxLayout *booksFilterLayout;
    QHBoxLayout *booksFilterRow;
    QLineEdit *bookSearchEdit;
    QComboBox *bookStatusFilter;
    QPushButton *bookFilterButton;
    QListWidget *booksList;
    QGroupBox *booksActionsGroup;
    QHBoxLayout *booksActionsLayout;
    QPushButton *addBookButton;
    QPushButton *editBookButton;
    QPushButton *deleteBookButton;
    QPushButton *restockBookButton;
    QSpacerItem *booksActionsSpacer;
    QWidget *readersTab;
    QVBoxLayout *readersTabLayout;
    QGroupBox *readersFilterGroup;
    QHBoxLayout *readersFilterLayout;
    QLineEdit *readerSearchEdit;
    QComboBox *readerStatusFilter;
    QPushButton *readerFilterButton;
    QListWidget *readersList;
    QGroupBox *readersActionsGroup;
    QHBoxLayout *readersActionsLayout;
    QPushButton *addReaderButton;
    QPushButton *editReaderButton;
    QPushButton *toggleReaderStatusButton;
    QPushButton *deleteReaderButton;
    QSpacerItem *readersActionsSpacer;
    QWidget *loansTab;
    QVBoxLayout *loansTabLayout;
    QGroupBox *loansFilterGroup;
    QGridLayout *loansFilterLayout;
    QLineEdit *loanSearchEdit;
    QComboBox *loanStatusFilter;
    QPushButton *loanFilterButton;
    QListWidget *loansList;
    QGroupBox *loansActionsGroup;
    QHBoxLayout *loansActionsLayout;
    QPushButton *newLoanButton;
    QPushButton *returnLoanButton;
    QPushButton *extendLoanButton;
    QPushButton *lostLoanButton;
    QPushButton *damageLoanButton;
    QPushButton *deleteLoanButton;
    QSpacerItem *loansActionsSpacer;
    QWidget *reportsTab;
    QVBoxLayout *reportsTabLayout;
    QGroupBox *reportsFilterGroup;
    QGridLayout *reportsFilterLayout;
    QLineEdit *reportStaffFilter;
    QDateEdit *reportFromFilter;
    QDateEdit *reportToFilter;
    QPushButton *reportApplyButton;
    QPushButton *reportClearButton;
    QListWidget *reportsList;
    QGroupBox *reportsActionsGroup;
    QHBoxLayout *reportsActionsLayout;
    QPushButton *submitReportButton;
    QPushButton *approveReportButton;
    QPushButton *rejectReportButton;
    QSpacerItem *reportsActionsSpacer;
    QWidget *statsTab;
    QVBoxLayout *statsTabOuterLayout;
    QScrollArea *statsScrollArea;
    QWidget *statsScrollContent;
    QVBoxLayout *statsTabLayout;
    QGroupBox *statsFilterGroup;
    QHBoxLayout *statsFilterLayout;
    QLabel *timePeriodLabel;
    QComboBox *timePeriodCombo;
    QPushButton *applyFilterButton;
    QSpacerItem *spacerItem;
    QHBoxLayout *statsCardsLayout;
    QGroupBox *totalBooksCard;
    QLabel *totalBooksValue;
    QGroupBox *totalReadersCard;
    QLabel *totalReadersValue;
    QGroupBox *totalLoansCard;
    QLabel *totalLoansValue;
    QGroupBox *totalFinesCard;
    QLabel *totalFinesValue;
    QHBoxLayout *chartsRowLayout;
    QFrame *topBooksFrame;
    QVBoxLayout *topBooksFrameLayout;
    QLabel *topBooksTitle;
    StatsChart *topBooksChart;
    QFrame *topBooksListFrame;
    QVBoxLayout *topBooksListLayout;
    QLabel *topBooksListTitle;
    QListWidget *topBooksList;
    QHBoxLayout *bottomRowLayout;
    QVBoxLayout *warningColumn;
    QFrame *overdueWarningFrame;
    QVBoxLayout *vboxLayout;
    QLabel *overdueWarningTitle;
    QLabel *overdueCount;
    QFrame *monthlyFinesFrame;
    QVBoxLayout *vboxLayout1;
    QLabel *monthlyFinesTitle;
    QLabel *monthlyFinesValue;
    QFrame *revenueChartFrame;
    QVBoxLayout *vboxLayout2;
    QLabel *revenueChartTitle;
    StatsChart *revenueChart;
    QFrame *activeReadersFrame;
    QVBoxLayout *vboxLayout3;
    QLabel *activeReadersTitle;
    QListWidget *activeReadersList;
    QWidget *staffsTab;
    QVBoxLayout *staffsTabLayout;
    QGroupBox *staffsFilterGroup;
    QHBoxLayout *staffsFilterLayout;
    QLineEdit *staffSearchEdit;
    QComboBox *staffStatusFilter;
    QPushButton *staffFilterButton;
    QListWidget *staffsList;
    QGroupBox *staffsActionsGroup;
    QHBoxLayout *staffsActionsLayout;
    QPushButton *addStaffButton;
    QPushButton *editStaffButton;
    QPushButton *deleteStaffButton;
    QPushButton *toggleStaffStatusButton;
    QSpacerItem *staffsActionsSpacer;
    QWidget *accountsTab;
    QVBoxLayout *accountsTabLayout;
    QListWidget *accountsList;
    QGroupBox *accountsActionsGroup;
    QHBoxLayout *accountsActionsLayout;
    QPushButton *addAccountButton;
    QPushButton *resetPasswordButton;
    QPushButton *toggleAccountButton;
    QPushButton *deleteAccountButton;
    QSpacerItem *accountsActionsSpacer;
    QWidget *settingsTab;
    QVBoxLayout *settingsTabLayout;
    QGroupBox *settingsGroup;
    QGridLayout *settingsLayout;
    QLabel *maxBorrowDaysLabel;
    QSpinBox *maxBorrowDaysSpin;
    QLabel *finePerDayLabel;
    QSpinBox *finePerDaySpin;
    QLabel *maxBooksPerReaderLabel;
    QSpinBox *maxBooksPerReaderSpin;
    QPushButton *saveConfigButton;
    QMenuBar *menubar;
    QMenu *menuSystem;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1313, 866);
        MainWindow->setMinimumSize(QSize(800, 600));
        refreshAction = new QAction(MainWindow);
        refreshAction->setObjectName("refreshAction");
        logoutAction = new QAction(MainWindow);
        logoutAction->setObjectName("logoutAction");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QHBoxLayout(centralwidget);
        centralLayout->setSpacing(12);
        centralLayout->setObjectName("centralLayout");
        centralLayout->setContentsMargins(12, 12, 12, 12);
        navigationPanel = new QFrame(centralwidget);
        navigationPanel->setObjectName("navigationPanel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(navigationPanel->sizePolicy().hasHeightForWidth());
        navigationPanel->setSizePolicy(sizePolicy);
        navigationPanel->setFrameShape(QFrame::Shape::NoFrame);
        navigationPanel->setFrameShadow(QFrame::Shadow::Plain);
        navigationLayout = new QVBoxLayout(navigationPanel);
        navigationLayout->setSpacing(10);
        navigationLayout->setObjectName("navigationLayout");
        navigationLayout->setContentsMargins(12, 12, 12, 12);
        navigationTitle = new QLabel(navigationPanel);
        navigationTitle->setObjectName("navigationTitle");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        navigationTitle->setFont(font);
        navigationTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        navigationLayout->addWidget(navigationTitle);

        navigationList = new QListWidget(navigationPanel);
        navigationList->setObjectName("navigationList");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(navigationList->sizePolicy().hasHeightForWidth());
        navigationList->setSizePolicy(sizePolicy1);

        navigationLayout->addWidget(navigationList);

        refreshButton = new QPushButton(navigationPanel);
        refreshButton->setObjectName("refreshButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(refreshButton->sizePolicy().hasHeightForWidth());
        refreshButton->setSizePolicy(sizePolicy2);

        navigationLayout->addWidget(refreshButton);

        logoutButton = new QPushButton(navigationPanel);
        logoutButton->setObjectName("logoutButton");
        sizePolicy2.setHeightForWidth(logoutButton->sizePolicy().hasHeightForWidth());
        logoutButton->setSizePolicy(sizePolicy2);

        navigationLayout->addWidget(logoutButton);


        centralLayout->addWidget(navigationPanel);

        tabs = new QTabWidget(centralwidget);
        tabs->setObjectName("tabs");
        homeTab = new QWidget();
        homeTab->setObjectName("homeTab");
        homeTabLayout = new QVBoxLayout(homeTab);
        homeTabLayout->setSpacing(12);
        homeTabLayout->setObjectName("homeTabLayout");
        homeTabLayout->setContentsMargins(18, 18, 18, 18);
        homeTitleLabel = new QLabel(homeTab);
        homeTitleLabel->setObjectName("homeTitleLabel");
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        homeTitleLabel->setFont(font1);

        homeTabLayout->addWidget(homeTitleLabel);

        homeHintLabel = new QLabel(homeTab);
        homeHintLabel->setObjectName("homeHintLabel");
        homeHintLabel->setWordWrap(true);

        homeTabLayout->addWidget(homeHintLabel);

        homeSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        homeTabLayout->addItem(homeSpacer);

        tabs->addTab(homeTab, QString());
        booksTab = new QWidget();
        booksTab->setObjectName("booksTab");
        booksTabLayout = new QVBoxLayout(booksTab);
        booksTabLayout->setSpacing(12);
        booksTabLayout->setObjectName("booksTabLayout");
        booksTabLayout->setContentsMargins(18, 18, 18, 18);
        booksFilterGroup = new QGroupBox(booksTab);
        booksFilterGroup->setObjectName("booksFilterGroup");
        booksFilterLayout = new QVBoxLayout(booksFilterGroup);
        booksFilterLayout->setSpacing(10);
        booksFilterLayout->setObjectName("booksFilterLayout");
        booksFilterLayout->setContentsMargins(18, 18, 18, 18);
        booksFilterRow = new QHBoxLayout();
        booksFilterRow->setSpacing(10);
        booksFilterRow->setObjectName("booksFilterRow");
        bookSearchEdit = new QLineEdit(booksFilterGroup);
        bookSearchEdit->setObjectName("bookSearchEdit");

        booksFilterRow->addWidget(bookSearchEdit);

        bookStatusFilter = new QComboBox(booksFilterGroup);
        bookStatusFilter->setObjectName("bookStatusFilter");

        booksFilterRow->addWidget(bookStatusFilter);

        bookFilterButton = new QPushButton(booksFilterGroup);
        bookFilterButton->setObjectName("bookFilterButton");

        booksFilterRow->addWidget(bookFilterButton);


        booksFilterLayout->addLayout(booksFilterRow);


        booksTabLayout->addWidget(booksFilterGroup);

        booksList = new QListWidget(booksTab);
        booksList->setObjectName("booksList");

        booksTabLayout->addWidget(booksList);

        booksActionsGroup = new QGroupBox(booksTab);
        booksActionsGroup->setObjectName("booksActionsGroup");
        booksActionsLayout = new QHBoxLayout(booksActionsGroup);
        booksActionsLayout->setSpacing(12);
        booksActionsLayout->setObjectName("booksActionsLayout");
        booksActionsLayout->setContentsMargins(18, 18, 18, 18);
        addBookButton = new QPushButton(booksActionsGroup);
        addBookButton->setObjectName("addBookButton");

        booksActionsLayout->addWidget(addBookButton);

        editBookButton = new QPushButton(booksActionsGroup);
        editBookButton->setObjectName("editBookButton");

        booksActionsLayout->addWidget(editBookButton);

        deleteBookButton = new QPushButton(booksActionsGroup);
        deleteBookButton->setObjectName("deleteBookButton");

        booksActionsLayout->addWidget(deleteBookButton);

        restockBookButton = new QPushButton(booksActionsGroup);
        restockBookButton->setObjectName("restockBookButton");

        booksActionsLayout->addWidget(restockBookButton);

        booksActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        booksActionsLayout->addItem(booksActionsSpacer);


        booksTabLayout->addWidget(booksActionsGroup);

        tabs->addTab(booksTab, QString());
        readersTab = new QWidget();
        readersTab->setObjectName("readersTab");
        readersTabLayout = new QVBoxLayout(readersTab);
        readersTabLayout->setSpacing(12);
        readersTabLayout->setObjectName("readersTabLayout");
        readersTabLayout->setContentsMargins(18, 18, 18, 18);
        readersFilterGroup = new QGroupBox(readersTab);
        readersFilterGroup->setObjectName("readersFilterGroup");
        readersFilterLayout = new QHBoxLayout(readersFilterGroup);
        readersFilterLayout->setSpacing(10);
        readersFilterLayout->setObjectName("readersFilterLayout");
        readersFilterLayout->setContentsMargins(18, 18, 18, 18);
        readerSearchEdit = new QLineEdit(readersFilterGroup);
        readerSearchEdit->setObjectName("readerSearchEdit");

        readersFilterLayout->addWidget(readerSearchEdit);

        readerStatusFilter = new QComboBox(readersFilterGroup);
        readerStatusFilter->setObjectName("readerStatusFilter");

        readersFilterLayout->addWidget(readerStatusFilter);

        readerFilterButton = new QPushButton(readersFilterGroup);
        readerFilterButton->setObjectName("readerFilterButton");

        readersFilterLayout->addWidget(readerFilterButton);


        readersTabLayout->addWidget(readersFilterGroup);

        readersList = new QListWidget(readersTab);
        readersList->setObjectName("readersList");

        readersTabLayout->addWidget(readersList);

        readersActionsGroup = new QGroupBox(readersTab);
        readersActionsGroup->setObjectName("readersActionsGroup");
        readersActionsLayout = new QHBoxLayout(readersActionsGroup);
        readersActionsLayout->setSpacing(12);
        readersActionsLayout->setObjectName("readersActionsLayout");
        readersActionsLayout->setContentsMargins(18, 18, 18, 18);
        addReaderButton = new QPushButton(readersActionsGroup);
        addReaderButton->setObjectName("addReaderButton");

        readersActionsLayout->addWidget(addReaderButton);

        editReaderButton = new QPushButton(readersActionsGroup);
        editReaderButton->setObjectName("editReaderButton");

        readersActionsLayout->addWidget(editReaderButton);

        toggleReaderStatusButton = new QPushButton(readersActionsGroup);
        toggleReaderStatusButton->setObjectName("toggleReaderStatusButton");

        readersActionsLayout->addWidget(toggleReaderStatusButton);

        deleteReaderButton = new QPushButton(readersActionsGroup);
        deleteReaderButton->setObjectName("deleteReaderButton");

        readersActionsLayout->addWidget(deleteReaderButton);

        readersActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        readersActionsLayout->addItem(readersActionsSpacer);


        readersTabLayout->addWidget(readersActionsGroup);

        tabs->addTab(readersTab, QString());
        loansTab = new QWidget();
        loansTab->setObjectName("loansTab");
        loansTabLayout = new QVBoxLayout(loansTab);
        loansTabLayout->setSpacing(12);
        loansTabLayout->setObjectName("loansTabLayout");
        loansTabLayout->setContentsMargins(18, 18, 18, 18);
        loansFilterGroup = new QGroupBox(loansTab);
        loansFilterGroup->setObjectName("loansFilterGroup");
        loansFilterLayout = new QGridLayout(loansFilterGroup);
        loansFilterLayout->setSpacing(10);
        loansFilterLayout->setObjectName("loansFilterLayout");
        loansFilterLayout->setContentsMargins(18, 18, 18, 18);
        loanSearchEdit = new QLineEdit(loansFilterGroup);
        loanSearchEdit->setObjectName("loanSearchEdit");

        loansFilterLayout->addWidget(loanSearchEdit, 0, 0, 1, 1);

        loanStatusFilter = new QComboBox(loansFilterGroup);
        loanStatusFilter->setObjectName("loanStatusFilter");

        loansFilterLayout->addWidget(loanStatusFilter, 0, 1, 1, 1);

        loanFilterButton = new QPushButton(loansFilterGroup);
        loanFilterButton->setObjectName("loanFilterButton");

        loansFilterLayout->addWidget(loanFilterButton, 0, 2, 1, 1);


        loansTabLayout->addWidget(loansFilterGroup);

        loansList = new QListWidget(loansTab);
        loansList->setObjectName("loansList");

        loansTabLayout->addWidget(loansList);

        loansActionsGroup = new QGroupBox(loansTab);
        loansActionsGroup->setObjectName("loansActionsGroup");
        loansActionsLayout = new QHBoxLayout(loansActionsGroup);
        loansActionsLayout->setSpacing(12);
        loansActionsLayout->setObjectName("loansActionsLayout");
        loansActionsLayout->setContentsMargins(18, 18, 18, 18);
        newLoanButton = new QPushButton(loansActionsGroup);
        newLoanButton->setObjectName("newLoanButton");

        loansActionsLayout->addWidget(newLoanButton);

        returnLoanButton = new QPushButton(loansActionsGroup);
        returnLoanButton->setObjectName("returnLoanButton");

        loansActionsLayout->addWidget(returnLoanButton);

        extendLoanButton = new QPushButton(loansActionsGroup);
        extendLoanButton->setObjectName("extendLoanButton");

        loansActionsLayout->addWidget(extendLoanButton);

        lostLoanButton = new QPushButton(loansActionsGroup);
        lostLoanButton->setObjectName("lostLoanButton");

        loansActionsLayout->addWidget(lostLoanButton);

        damageLoanButton = new QPushButton(loansActionsGroup);
        damageLoanButton->setObjectName("damageLoanButton");

        loansActionsLayout->addWidget(damageLoanButton);

        deleteLoanButton = new QPushButton(loansActionsGroup);
        deleteLoanButton->setObjectName("deleteLoanButton");

        loansActionsLayout->addWidget(deleteLoanButton);

        loansActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loansActionsLayout->addItem(loansActionsSpacer);


        loansTabLayout->addWidget(loansActionsGroup);

        tabs->addTab(loansTab, QString());
        reportsTab = new QWidget();
        reportsTab->setObjectName("reportsTab");
        reportsTabLayout = new QVBoxLayout(reportsTab);
        reportsTabLayout->setSpacing(12);
        reportsTabLayout->setObjectName("reportsTabLayout");
        reportsTabLayout->setContentsMargins(18, 18, 18, 18);
        reportsFilterGroup = new QGroupBox(reportsTab);
        reportsFilterGroup->setObjectName("reportsFilterGroup");
        reportsFilterLayout = new QGridLayout(reportsFilterGroup);
        reportsFilterLayout->setSpacing(12);
        reportsFilterLayout->setObjectName("reportsFilterLayout");
        reportsFilterLayout->setContentsMargins(18, 18, 18, 18);
        reportStaffFilter = new QLineEdit(reportsFilterGroup);
        reportStaffFilter->setObjectName("reportStaffFilter");

        reportsFilterLayout->addWidget(reportStaffFilter, 0, 0, 1, 1);

        reportFromFilter = new QDateEdit(reportsFilterGroup);
        reportFromFilter->setObjectName("reportFromFilter");
        reportFromFilter->setCalendarPopup(true);

        reportsFilterLayout->addWidget(reportFromFilter, 0, 1, 1, 1);

        reportToFilter = new QDateEdit(reportsFilterGroup);
        reportToFilter->setObjectName("reportToFilter");
        reportToFilter->setCalendarPopup(true);

        reportsFilterLayout->addWidget(reportToFilter, 0, 2, 1, 1);

        reportApplyButton = new QPushButton(reportsFilterGroup);
        reportApplyButton->setObjectName("reportApplyButton");

        reportsFilterLayout->addWidget(reportApplyButton, 0, 3, 1, 1);

        reportClearButton = new QPushButton(reportsFilterGroup);
        reportClearButton->setObjectName("reportClearButton");

        reportsFilterLayout->addWidget(reportClearButton, 0, 4, 1, 1);


        reportsTabLayout->addWidget(reportsFilterGroup);

        reportsList = new QListWidget(reportsTab);
        reportsList->setObjectName("reportsList");

        reportsTabLayout->addWidget(reportsList);

        reportsActionsGroup = new QGroupBox(reportsTab);
        reportsActionsGroup->setObjectName("reportsActionsGroup");
        reportsActionsLayout = new QHBoxLayout(reportsActionsGroup);
        reportsActionsLayout->setSpacing(12);
        reportsActionsLayout->setObjectName("reportsActionsLayout");
        reportsActionsLayout->setContentsMargins(18, 18, 18, 18);
        submitReportButton = new QPushButton(reportsActionsGroup);
        submitReportButton->setObjectName("submitReportButton");

        reportsActionsLayout->addWidget(submitReportButton);

        approveReportButton = new QPushButton(reportsActionsGroup);
        approveReportButton->setObjectName("approveReportButton");

        reportsActionsLayout->addWidget(approveReportButton);

        rejectReportButton = new QPushButton(reportsActionsGroup);
        rejectReportButton->setObjectName("rejectReportButton");

        reportsActionsLayout->addWidget(rejectReportButton);

        reportsActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        reportsActionsLayout->addItem(reportsActionsSpacer);


        reportsTabLayout->addWidget(reportsActionsGroup);

        tabs->addTab(reportsTab, QString());
        statsTab = new QWidget();
        statsTab->setObjectName("statsTab");
        statsTabOuterLayout = new QVBoxLayout(statsTab);
        statsTabOuterLayout->setSpacing(0);
        statsTabOuterLayout->setObjectName("statsTabOuterLayout");
        statsTabOuterLayout->setContentsMargins(0, 0, 0, 0);
        statsScrollArea = new QScrollArea(statsTab);
        statsScrollArea->setObjectName("statsScrollArea");
        statsScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        statsScrollArea->setWidgetResizable(true);
        statsScrollContent = new QWidget();
        statsScrollContent->setObjectName("statsScrollContent");
        statsScrollContent->setGeometry(QRect(0, 0, 991, 822));
        statsTabLayout = new QVBoxLayout(statsScrollContent);
        statsTabLayout->setSpacing(10);
        statsTabLayout->setObjectName("statsTabLayout");
        statsTabLayout->setContentsMargins(12, 12, 12, 12);
        statsFilterGroup = new QGroupBox(statsScrollContent);
        statsFilterGroup->setObjectName("statsFilterGroup");
        statsFilterLayout = new QHBoxLayout(statsFilterGroup);
        statsFilterLayout->setSpacing(12);
        statsFilterLayout->setObjectName("statsFilterLayout");
        timePeriodLabel = new QLabel(statsFilterGroup);
        timePeriodLabel->setObjectName("timePeriodLabel");
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        timePeriodLabel->setFont(font2);

        statsFilterLayout->addWidget(timePeriodLabel);

        timePeriodCombo = new QComboBox(statsFilterGroup);
        timePeriodCombo->addItem(QString());
        timePeriodCombo->addItem(QString());
        timePeriodCombo->addItem(QString());
        timePeriodCombo->addItem(QString());
        timePeriodCombo->setObjectName("timePeriodCombo");
        timePeriodCombo->setMinimumSize(QSize(150, 0));

        statsFilterLayout->addWidget(timePeriodCombo);

        applyFilterButton = new QPushButton(statsFilterGroup);
        applyFilterButton->setObjectName("applyFilterButton");

        statsFilterLayout->addWidget(applyFilterButton);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statsFilterLayout->addItem(spacerItem);


        statsTabLayout->addWidget(statsFilterGroup);

        statsCardsLayout = new QHBoxLayout();
        statsCardsLayout->setSpacing(12);
        statsCardsLayout->setObjectName("statsCardsLayout");
        totalBooksCard = new QGroupBox(statsScrollContent);
        totalBooksCard->setObjectName("totalBooksCard");
        totalBooksCard->setMinimumSize(QSize(150, 90));
        totalBooksCard->setMaximumSize(QSize(16777215, 90));
        totalBooksValue = new QLabel(totalBooksCard);
        totalBooksValue->setObjectName("totalBooksValue");
        totalBooksValue->setGeometry(QRect(10, 30, 130, 40));
        QFont font3;
        font3.setPointSize(20);
        font3.setBold(true);
        totalBooksValue->setFont(font3);
        totalBooksValue->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardsLayout->addWidget(totalBooksCard);

        totalReadersCard = new QGroupBox(statsScrollContent);
        totalReadersCard->setObjectName("totalReadersCard");
        totalReadersCard->setMinimumSize(QSize(150, 90));
        totalReadersCard->setMaximumSize(QSize(16777215, 90));
        totalReadersValue = new QLabel(totalReadersCard);
        totalReadersValue->setObjectName("totalReadersValue");
        totalReadersValue->setGeometry(QRect(10, 30, 130, 40));
        totalReadersValue->setFont(font3);
        totalReadersValue->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardsLayout->addWidget(totalReadersCard);

        totalLoansCard = new QGroupBox(statsScrollContent);
        totalLoansCard->setObjectName("totalLoansCard");
        totalLoansCard->setMinimumSize(QSize(150, 90));
        totalLoansCard->setMaximumSize(QSize(16777215, 90));
        totalLoansValue = new QLabel(totalLoansCard);
        totalLoansValue->setObjectName("totalLoansValue");
        totalLoansValue->setGeometry(QRect(10, 30, 130, 40));
        totalLoansValue->setFont(font3);
        totalLoansValue->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardsLayout->addWidget(totalLoansCard);

        totalFinesCard = new QGroupBox(statsScrollContent);
        totalFinesCard->setObjectName("totalFinesCard");
        totalFinesCard->setMinimumSize(QSize(150, 90));
        totalFinesCard->setMaximumSize(QSize(16777215, 90));
        totalFinesValue = new QLabel(totalFinesCard);
        totalFinesValue->setObjectName("totalFinesValue");
        totalFinesValue->setGeometry(QRect(10, 30, 130, 40));
        totalFinesValue->setFont(font3);
        totalFinesValue->setAlignment(Qt::AlignmentFlag::AlignCenter);

        statsCardsLayout->addWidget(totalFinesCard);


        statsTabLayout->addLayout(statsCardsLayout);

        chartsRowLayout = new QHBoxLayout();
        chartsRowLayout->setSpacing(12);
        chartsRowLayout->setObjectName("chartsRowLayout");
        topBooksFrame = new QFrame(statsScrollContent);
        topBooksFrame->setObjectName("topBooksFrame");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(topBooksFrame->sizePolicy().hasHeightForWidth());
        topBooksFrame->setSizePolicy(sizePolicy3);
        topBooksFrame->setFrameShape(QFrame::Shape::StyledPanel);
        topBooksFrameLayout = new QVBoxLayout(topBooksFrame);
        topBooksFrameLayout->setObjectName("topBooksFrameLayout");
        topBooksTitle = new QLabel(topBooksFrame);
        topBooksTitle->setObjectName("topBooksTitle");
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(true);
        topBooksTitle->setFont(font4);

        topBooksFrameLayout->addWidget(topBooksTitle);

        topBooksChart = new StatsChart(topBooksFrame);
        topBooksChart->setObjectName("topBooksChart");
        topBooksChart->setMinimumSize(QSize(200, 160));
        topBooksChart->setMaximumSize(QSize(16777215, 160));

        topBooksFrameLayout->addWidget(topBooksChart);


        chartsRowLayout->addWidget(topBooksFrame);

        topBooksListFrame = new QFrame(statsScrollContent);
        topBooksListFrame->setObjectName("topBooksListFrame");
        sizePolicy3.setHeightForWidth(topBooksListFrame->sizePolicy().hasHeightForWidth());
        topBooksListFrame->setSizePolicy(sizePolicy3);
        topBooksListFrame->setFrameShape(QFrame::Shape::StyledPanel);
        topBooksListLayout = new QVBoxLayout(topBooksListFrame);
        topBooksListLayout->setObjectName("topBooksListLayout");
        topBooksListTitle = new QLabel(topBooksListFrame);
        topBooksListTitle->setObjectName("topBooksListTitle");
        topBooksListTitle->setFont(font4);

        topBooksListLayout->addWidget(topBooksListTitle);

        topBooksList = new QListWidget(topBooksListFrame);
        topBooksList->setObjectName("topBooksList");
        topBooksList->setMinimumSize(QSize(200, 160));
        topBooksList->setMaximumSize(QSize(16777215, 160));

        topBooksListLayout->addWidget(topBooksList);


        chartsRowLayout->addWidget(topBooksListFrame);


        statsTabLayout->addLayout(chartsRowLayout);

        bottomRowLayout = new QHBoxLayout();
        bottomRowLayout->setSpacing(12);
        bottomRowLayout->setObjectName("bottomRowLayout");
        warningColumn = new QVBoxLayout();
        warningColumn->setSpacing(10);
        warningColumn->setObjectName("warningColumn");
        overdueWarningFrame = new QFrame(statsScrollContent);
        overdueWarningFrame->setObjectName("overdueWarningFrame");
        sizePolicy3.setHeightForWidth(overdueWarningFrame->sizePolicy().hasHeightForWidth());
        overdueWarningFrame->setSizePolicy(sizePolicy3);
        overdueWarningFrame->setFrameShape(QFrame::Shape::StyledPanel);
        vboxLayout = new QVBoxLayout(overdueWarningFrame);
        vboxLayout->setObjectName("vboxLayout");
        overdueWarningTitle = new QLabel(overdueWarningFrame);
        overdueWarningTitle->setObjectName("overdueWarningTitle");
        overdueWarningTitle->setFont(font4);

        vboxLayout->addWidget(overdueWarningTitle);

        overdueCount = new QLabel(overdueWarningFrame);
        overdueCount->setObjectName("overdueCount");
        overdueCount->setFont(font);

        vboxLayout->addWidget(overdueCount);


        warningColumn->addWidget(overdueWarningFrame);

        monthlyFinesFrame = new QFrame(statsScrollContent);
        monthlyFinesFrame->setObjectName("monthlyFinesFrame");
        sizePolicy3.setHeightForWidth(monthlyFinesFrame->sizePolicy().hasHeightForWidth());
        monthlyFinesFrame->setSizePolicy(sizePolicy3);
        monthlyFinesFrame->setFrameShape(QFrame::Shape::StyledPanel);
        vboxLayout1 = new QVBoxLayout(monthlyFinesFrame);
        vboxLayout1->setObjectName("vboxLayout1");
        monthlyFinesTitle = new QLabel(monthlyFinesFrame);
        monthlyFinesTitle->setObjectName("monthlyFinesTitle");
        monthlyFinesTitle->setFont(font4);

        vboxLayout1->addWidget(monthlyFinesTitle);

        monthlyFinesValue = new QLabel(monthlyFinesFrame);
        monthlyFinesValue->setObjectName("monthlyFinesValue");
        monthlyFinesValue->setFont(font);

        vboxLayout1->addWidget(monthlyFinesValue);


        warningColumn->addWidget(monthlyFinesFrame);


        bottomRowLayout->addLayout(warningColumn);

        revenueChartFrame = new QFrame(statsScrollContent);
        revenueChartFrame->setObjectName("revenueChartFrame");
        sizePolicy3.setHeightForWidth(revenueChartFrame->sizePolicy().hasHeightForWidth());
        revenueChartFrame->setSizePolicy(sizePolicy3);
        revenueChartFrame->setMaximumSize(QSize(16777215, 200));
        revenueChartFrame->setFrameShape(QFrame::Shape::StyledPanel);
        vboxLayout2 = new QVBoxLayout(revenueChartFrame);
        vboxLayout2->setObjectName("vboxLayout2");
        revenueChartTitle = new QLabel(revenueChartFrame);
        revenueChartTitle->setObjectName("revenueChartTitle");
        revenueChartTitle->setFont(font4);

        vboxLayout2->addWidget(revenueChartTitle);

        revenueChart = new StatsChart(revenueChartFrame);
        revenueChart->setObjectName("revenueChart");
        revenueChart->setMinimumSize(QSize(100, 140));
        revenueChart->setMaximumSize(QSize(16777215, 140));

        vboxLayout2->addWidget(revenueChart);


        bottomRowLayout->addWidget(revenueChartFrame);

        activeReadersFrame = new QFrame(statsScrollContent);
        activeReadersFrame->setObjectName("activeReadersFrame");
        sizePolicy3.setHeightForWidth(activeReadersFrame->sizePolicy().hasHeightForWidth());
        activeReadersFrame->setSizePolicy(sizePolicy3);
        activeReadersFrame->setMaximumSize(QSize(16777215, 200));
        activeReadersFrame->setFrameShape(QFrame::Shape::StyledPanel);
        vboxLayout3 = new QVBoxLayout(activeReadersFrame);
        vboxLayout3->setObjectName("vboxLayout3");
        activeReadersTitle = new QLabel(activeReadersFrame);
        activeReadersTitle->setObjectName("activeReadersTitle");
        activeReadersTitle->setFont(font4);

        vboxLayout3->addWidget(activeReadersTitle);

        activeReadersList = new QListWidget(activeReadersFrame);
        activeReadersList->setObjectName("activeReadersList");
        activeReadersList->setMinimumSize(QSize(100, 140));
        activeReadersList->setMaximumSize(QSize(16777215, 140));

        vboxLayout3->addWidget(activeReadersList);


        bottomRowLayout->addWidget(activeReadersFrame);


        statsTabLayout->addLayout(bottomRowLayout);

        statsScrollArea->setWidget(statsScrollContent);

        statsTabOuterLayout->addWidget(statsScrollArea);

        tabs->addTab(statsTab, QString());
        staffsTab = new QWidget();
        staffsTab->setObjectName("staffsTab");
        staffsTabLayout = new QVBoxLayout(staffsTab);
        staffsTabLayout->setSpacing(12);
        staffsTabLayout->setObjectName("staffsTabLayout");
        staffsTabLayout->setContentsMargins(18, 18, 18, 18);
        staffsFilterGroup = new QGroupBox(staffsTab);
        staffsFilterGroup->setObjectName("staffsFilterGroup");
        staffsFilterLayout = new QHBoxLayout(staffsFilterGroup);
        staffsFilterLayout->setSpacing(10);
        staffsFilterLayout->setObjectName("staffsFilterLayout");
        staffsFilterLayout->setContentsMargins(18, 18, 18, 18);
        staffSearchEdit = new QLineEdit(staffsFilterGroup);
        staffSearchEdit->setObjectName("staffSearchEdit");

        staffsFilterLayout->addWidget(staffSearchEdit);

        staffStatusFilter = new QComboBox(staffsFilterGroup);
        staffStatusFilter->setObjectName("staffStatusFilter");

        staffsFilterLayout->addWidget(staffStatusFilter);

        staffFilterButton = new QPushButton(staffsFilterGroup);
        staffFilterButton->setObjectName("staffFilterButton");

        staffsFilterLayout->addWidget(staffFilterButton);


        staffsTabLayout->addWidget(staffsFilterGroup);

        staffsList = new QListWidget(staffsTab);
        staffsList->setObjectName("staffsList");

        staffsTabLayout->addWidget(staffsList);

        staffsActionsGroup = new QGroupBox(staffsTab);
        staffsActionsGroup->setObjectName("staffsActionsGroup");
        staffsActionsLayout = new QHBoxLayout(staffsActionsGroup);
        staffsActionsLayout->setSpacing(12);
        staffsActionsLayout->setObjectName("staffsActionsLayout");
        staffsActionsLayout->setContentsMargins(18, 18, 18, 18);
        addStaffButton = new QPushButton(staffsActionsGroup);
        addStaffButton->setObjectName("addStaffButton");

        staffsActionsLayout->addWidget(addStaffButton);

        editStaffButton = new QPushButton(staffsActionsGroup);
        editStaffButton->setObjectName("editStaffButton");

        staffsActionsLayout->addWidget(editStaffButton);

        deleteStaffButton = new QPushButton(staffsActionsGroup);
        deleteStaffButton->setObjectName("deleteStaffButton");

        staffsActionsLayout->addWidget(deleteStaffButton);

        toggleStaffStatusButton = new QPushButton(staffsActionsGroup);
        toggleStaffStatusButton->setObjectName("toggleStaffStatusButton");

        staffsActionsLayout->addWidget(toggleStaffStatusButton);

        staffsActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        staffsActionsLayout->addItem(staffsActionsSpacer);


        staffsTabLayout->addWidget(staffsActionsGroup);

        tabs->addTab(staffsTab, QString());
        accountsTab = new QWidget();
        accountsTab->setObjectName("accountsTab");
        accountsTabLayout = new QVBoxLayout(accountsTab);
        accountsTabLayout->setSpacing(12);
        accountsTabLayout->setObjectName("accountsTabLayout");
        accountsTabLayout->setContentsMargins(18, 18, 18, 18);
        accountsList = new QListWidget(accountsTab);
        accountsList->setObjectName("accountsList");

        accountsTabLayout->addWidget(accountsList);

        accountsActionsGroup = new QGroupBox(accountsTab);
        accountsActionsGroup->setObjectName("accountsActionsGroup");
        accountsActionsLayout = new QHBoxLayout(accountsActionsGroup);
        accountsActionsLayout->setSpacing(12);
        accountsActionsLayout->setObjectName("accountsActionsLayout");
        accountsActionsLayout->setContentsMargins(18, 18, 18, 18);
        addAccountButton = new QPushButton(accountsActionsGroup);
        addAccountButton->setObjectName("addAccountButton");

        accountsActionsLayout->addWidget(addAccountButton);

        resetPasswordButton = new QPushButton(accountsActionsGroup);
        resetPasswordButton->setObjectName("resetPasswordButton");

        accountsActionsLayout->addWidget(resetPasswordButton);

        toggleAccountButton = new QPushButton(accountsActionsGroup);
        toggleAccountButton->setObjectName("toggleAccountButton");

        accountsActionsLayout->addWidget(toggleAccountButton);

        deleteAccountButton = new QPushButton(accountsActionsGroup);
        deleteAccountButton->setObjectName("deleteAccountButton");

        accountsActionsLayout->addWidget(deleteAccountButton);

        accountsActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        accountsActionsLayout->addItem(accountsActionsSpacer);


        accountsTabLayout->addWidget(accountsActionsGroup);

        tabs->addTab(accountsTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName("settingsTab");
        settingsTabLayout = new QVBoxLayout(settingsTab);
        settingsTabLayout->setSpacing(12);
        settingsTabLayout->setObjectName("settingsTabLayout");
        settingsTabLayout->setContentsMargins(18, 18, 18, 18);
        settingsGroup = new QGroupBox(settingsTab);
        settingsGroup->setObjectName("settingsGroup");
        settingsLayout = new QGridLayout(settingsGroup);
        settingsLayout->setSpacing(16);
        settingsLayout->setObjectName("settingsLayout");
        settingsLayout->setContentsMargins(24, 24, 24, 24);
        maxBorrowDaysLabel = new QLabel(settingsGroup);
        maxBorrowDaysLabel->setObjectName("maxBorrowDaysLabel");

        settingsLayout->addWidget(maxBorrowDaysLabel, 0, 0, 1, 1);

        maxBorrowDaysSpin = new QSpinBox(settingsGroup);
        maxBorrowDaysSpin->setObjectName("maxBorrowDaysSpin");

        settingsLayout->addWidget(maxBorrowDaysSpin, 0, 1, 1, 1);

        finePerDayLabel = new QLabel(settingsGroup);
        finePerDayLabel->setObjectName("finePerDayLabel");

        settingsLayout->addWidget(finePerDayLabel, 1, 0, 1, 1);

        finePerDaySpin = new QSpinBox(settingsGroup);
        finePerDaySpin->setObjectName("finePerDaySpin");

        settingsLayout->addWidget(finePerDaySpin, 1, 1, 1, 1);

        maxBooksPerReaderLabel = new QLabel(settingsGroup);
        maxBooksPerReaderLabel->setObjectName("maxBooksPerReaderLabel");

        settingsLayout->addWidget(maxBooksPerReaderLabel, 2, 0, 1, 1);

        maxBooksPerReaderSpin = new QSpinBox(settingsGroup);
        maxBooksPerReaderSpin->setObjectName("maxBooksPerReaderSpin");

        settingsLayout->addWidget(maxBooksPerReaderSpin, 2, 1, 1, 1);

        saveConfigButton = new QPushButton(settingsGroup);
        saveConfigButton->setObjectName("saveConfigButton");

        settingsLayout->addWidget(saveConfigButton, 3, 0, 1, 2);


        settingsTabLayout->addWidget(settingsGroup);

        tabs->addTab(settingsTab, QString());

        centralLayout->addWidget(tabs);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1313, 37));
        menuSystem = new QMenu(menubar);
        menuSystem->setObjectName("menuSystem");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSystem->menuAction());
        menuSystem->addAction(refreshAction);
        menuSystem->addAction(logoutAction);

        retranslateUi(MainWindow);

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "HE THONG CHO THUE TRUYEN VA SACH PBL2", nullptr));
        refreshAction->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        logoutAction->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
        navigationTitle->setText(QCoreApplication::translate("MainWindow", "Dieu huong", nullptr));
        refreshButton->setText(QCoreApplication::translate("MainWindow", "Tai lai", nullptr));
        logoutButton->setText(QCoreApplication::translate("MainWindow", "Dang xuat", nullptr));
        homeTitleLabel->setText(QCoreApplication::translate("MainWindow", "Chao mung tro lai he thong quan ly thu vien", nullptr));
        homeHintLabel->setText(QCoreApplication::translate("MainWindow", "Su dung cac tab ben duoi hoac menu ben trai de di chuyen giua cac khu vuc. Tat ca thong tin moi nhat ve sach, ban doc va phieu muon se duoc cap nhat sau moi lan tai lai.", nullptr));
        tabs->setTabText(tabs->indexOf(homeTab), QCoreApplication::translate("MainWindow", "Trang chu", nullptr));
        booksFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        bookSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tim theo tieu de, tac gia, the loai hoac nha xuat ban", nullptr));
        bookFilterButton->setText(QCoreApplication::translate("MainWindow", "Loc", nullptr));
        booksActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        addBookButton->setText(QCoreApplication::translate("MainWindow", "Them sach", nullptr));
        editBookButton->setText(QCoreApplication::translate("MainWindow", "Cap nhat", nullptr));
        deleteBookButton->setText(QCoreApplication::translate("MainWindow", "Xoa sach", nullptr));
        restockBookButton->setText(QCoreApplication::translate("MainWindow", "Nhap hang", nullptr));
        tabs->setTabText(tabs->indexOf(booksTab), QCoreApplication::translate("MainWindow", "Sach", nullptr));
        readersFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        readerSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tim doc gia theo ten, ma doc gia, email hoac so dien thoai", nullptr));
        readerFilterButton->setText(QCoreApplication::translate("MainWindow", "Loc", nullptr));
        readersActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        addReaderButton->setText(QCoreApplication::translate("MainWindow", "Them doc gia", nullptr));
        editReaderButton->setText(QCoreApplication::translate("MainWindow", "Cap nhat", nullptr));
        toggleReaderStatusButton->setText(QCoreApplication::translate("MainWindow", "Tam khoa / Mo khoa", nullptr));
        deleteReaderButton->setText(QCoreApplication::translate("MainWindow", "Xoa", nullptr));
        tabs->setTabText(tabs->indexOf(readersTab), QCoreApplication::translate("MainWindow", "Doc gia", nullptr));
        loansFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        loanSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tim theo ma muon, ma doc gia hoac ma sach", nullptr));
        loanFilterButton->setText(QCoreApplication::translate("MainWindow", "Loc", nullptr));
        loansActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        newLoanButton->setText(QCoreApplication::translate("MainWindow", "Lap phieu muon", nullptr));
        returnLoanButton->setText(QCoreApplication::translate("MainWindow", "Danh dau da tra", nullptr));
        extendLoanButton->setText(QCoreApplication::translate("MainWindow", "Gia han", nullptr));
        lostLoanButton->setText(QCoreApplication::translate("MainWindow", "Mat sach", nullptr));
        damageLoanButton->setText(QCoreApplication::translate("MainWindow", "Hu hong", nullptr));
        deleteLoanButton->setText(QCoreApplication::translate("MainWindow", "Huy phieu", nullptr));
        tabs->setTabText(tabs->indexOf(loansTab), QCoreApplication::translate("MainWindow", "Muon tra", nullptr));
        reportsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        reportStaffFilter->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tim theo nhan vien xu ly", nullptr));
        reportApplyButton->setText(QCoreApplication::translate("MainWindow", "Ap dung", nullptr));
        reportClearButton->setText(QCoreApplication::translate("MainWindow", "Xoa loc", nullptr));
        reportsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        submitReportButton->setText(QCoreApplication::translate("MainWindow", "Lap yeu cau", nullptr));
        approveReportButton->setText(QCoreApplication::translate("MainWindow", "Duyet", nullptr));
        rejectReportButton->setText(QCoreApplication::translate("MainWindow", "Tu choi", nullptr));
        tabs->setTabText(tabs->indexOf(reportsTab), QCoreApplication::translate("MainWindow", "Bao cao", nullptr));
        statsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        timePeriodLabel->setText(QCoreApplication::translate("MainWindow", "Thoi gian:", nullptr));
        timePeriodCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Tuan nay", nullptr));
        timePeriodCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Thang nay", nullptr));
        timePeriodCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Nam nay", nullptr));
        timePeriodCombo->setItemText(3, QCoreApplication::translate("MainWindow", "Tat ca", nullptr));

        applyFilterButton->setText(QCoreApplication::translate("MainWindow", "Ap dung", nullptr));
        totalBooksCard->setStyleSheet(QCoreApplication::translate("MainWindow", "QGroupBox { background: white; border: 2px solid #3b82f6; border-radius: 8px; margin-top: 10px; font-weight: bold; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }", nullptr));
        totalBooksCard->setTitle(QCoreApplication::translate("MainWindow", "Tong So Sach", nullptr));
        totalBooksValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        totalReadersCard->setStyleSheet(QCoreApplication::translate("MainWindow", "QGroupBox { background: white; border: 2px solid #10b981; border-radius: 8px; margin-top: 10px; font-weight: bold; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }", nullptr));
        totalReadersCard->setTitle(QCoreApplication::translate("MainWindow", "Tong Doc Gia", nullptr));
        totalReadersValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        totalLoansCard->setStyleSheet(QCoreApplication::translate("MainWindow", "QGroupBox { background: white; border: 2px solid #f59e0b; border-radius: 8px; margin-top: 10px; font-weight: bold; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }", nullptr));
        totalLoansCard->setTitle(QCoreApplication::translate("MainWindow", "Tong Phieu Muon", nullptr));
        totalLoansValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        totalFinesCard->setStyleSheet(QCoreApplication::translate("MainWindow", "QGroupBox { background: white; border: 2px solid #ef4444; border-radius: 8px; margin-top: 10px; font-weight: bold; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }", nullptr));
        totalFinesCard->setTitle(QCoreApplication::translate("MainWindow", "Tong Tien Phat", nullptr));
        totalFinesValue->setText(QCoreApplication::translate("MainWindow", "0\304\221", nullptr));
        topBooksFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 8px; padding: 16px; }", nullptr));
        topBooksTitle->setText(QCoreApplication::translate("MainWindow", "Luot thue theo the loai", nullptr));
        topBooksListFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 8px; padding: 16px; }", nullptr));
        topBooksListTitle->setText(QCoreApplication::translate("MainWindow", "The loai muon nhieu nhat", nullptr));
        overdueWarningFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: #fef2f2; border: 1px solid #fecaca; border-radius: 8px; padding: 16px; }", nullptr));
        overdueWarningTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #dc2626;", nullptr));
        overdueWarningTitle->setText(QCoreApplication::translate("MainWindow", "\342\232\240 Tre han", nullptr));
        overdueCount->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #991b1b;", nullptr));
        overdueCount->setText(QCoreApplication::translate("MainWindow", "0 sach dang tre", nullptr));
        monthlyFinesFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 8px; padding: 16px; }", nullptr));
        monthlyFinesTitle->setText(QCoreApplication::translate("MainWindow", "Tien phat ", nullptr));
        monthlyFinesValue->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #111827;", nullptr));
        monthlyFinesValue->setText(QCoreApplication::translate("MainWindow", "0d", nullptr));
        revenueChartFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 8px; padding: 10px; }", nullptr));
        revenueChartTitle->setText(QCoreApplication::translate("MainWindow", "Doanh thu", nullptr));
        activeReadersFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 8px; padding: 10px; }", nullptr));
        activeReadersTitle->setText(QCoreApplication::translate("MainWindow", "Hoat dong ban doc", nullptr));
        tabs->setTabText(tabs->indexOf(statsTab), QCoreApplication::translate("MainWindow", "Thong ke", nullptr));
        staffsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "Bo loc", nullptr));
        staffSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Tim nhan vien theo ten, ma nhan vien hoac chuc vu", nullptr));
        staffFilterButton->setText(QCoreApplication::translate("MainWindow", "Loc", nullptr));
        staffsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        addStaffButton->setText(QCoreApplication::translate("MainWindow", "Them nhan vien", nullptr));
        editStaffButton->setText(QCoreApplication::translate("MainWindow", "Cap nhat", nullptr));
        deleteStaffButton->setText(QCoreApplication::translate("MainWindow", "Xoa", nullptr));
        toggleStaffStatusButton->setText(QCoreApplication::translate("MainWindow", "Chuyen trang thai", nullptr));
        tabs->setTabText(tabs->indexOf(staffsTab), QCoreApplication::translate("MainWindow", "Nhan vien", nullptr));
        accountsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "Tac vu", nullptr));
        addAccountButton->setText(QCoreApplication::translate("MainWindow", "Them tai khoan", nullptr));
        resetPasswordButton->setText(QCoreApplication::translate("MainWindow", "Dat lai mat khau", nullptr));
        toggleAccountButton->setText(QCoreApplication::translate("MainWindow", "Kich hoat / Vo hieu", nullptr));
        deleteAccountButton->setText(QCoreApplication::translate("MainWindow", "Xoa tai khoan", nullptr));
        tabs->setTabText(tabs->indexOf(accountsTab), QCoreApplication::translate("MainWindow", "Tai khoan", nullptr));
        settingsGroup->setTitle(QCoreApplication::translate("MainWindow", "Thiet lap he thong", nullptr));
        maxBorrowDaysLabel->setText(QCoreApplication::translate("MainWindow", "So ngay muon toi da", nullptr));
        finePerDayLabel->setText(QCoreApplication::translate("MainWindow", "Tien phat moi ngay", nullptr));
        maxBooksPerReaderLabel->setText(QCoreApplication::translate("MainWindow", "So sach toi da moi doc gia", nullptr));
        saveConfigButton->setText(QCoreApplication::translate("MainWindow", "Luu cau hinh", nullptr));
        tabs->setTabText(tabs->indexOf(settingsTab), QCoreApplication::translate("MainWindow", "Cai dat", nullptr));
        menuSystem->setTitle(QCoreApplication::translate("MainWindow", "He thong", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
