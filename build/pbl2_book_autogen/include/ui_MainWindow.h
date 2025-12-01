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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QLabel *homeTitleLabel;
    QLabel *homeHintLabel;
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
    QPushButton *deleteReportButton;
    QSpacerItem *reportsActionsSpacer;
    QWidget *statsTab;
    QVBoxLayout *statsTabOuterLayout;
    QVBoxLayout *statsTabLayout;
    QGroupBox *statsFilterGroup;
    QHBoxLayout *statsFilterLayout;
    QLabel *timePeriodLabel;
    QComboBox *timePeriodCombo;
    QPushButton *applyFilterButton;
    QSpacerItem *spacerItem;
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
        MainWindow->resize(1333, 929);
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
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(navigationList->sizePolicy().hasHeightForWidth());
        navigationList->setSizePolicy(sizePolicy1);
        navigationList->setMinimumSize(QSize(220, 0));
        navigationList->setMaximumSize(QSize(400, 16777215));
        navigationList->setFrameShape(QFrame::Shape::NoFrame);
        navigationList->setFrameShadow(QFrame::Shadow::Plain);
        navigationList->setLineWidth(0);
        navigationList->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        navigationList->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        navigationList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        navigationList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        navigationList->setIconSize(QSize(22, 22));
        navigationList->setTextElideMode(Qt::TextElideMode::ElideNone);
        navigationList->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        navigationList->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        navigationList->setMovement(QListView::Movement::Static);
        navigationList->setFlow(QListView::Flow::TopToBottom);
        navigationList->setProperty("isWrapping", QVariant(false));
        navigationList->setResizeMode(QListView::ResizeMode::Adjust);
        navigationList->setLayoutMode(QListView::LayoutMode::Batched);
        navigationList->setSpacing(8);
        navigationList->setBatchSize(1000);
        navigationList->setWordWrap(true);
        navigationList->setItemAlignment(Qt::AlignmentFlag::AlignVCenter);

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
        homeTab->setStyleSheet(QString::fromUtf8("QWidget#homeTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
        homeTitleLabel = new QLabel(homeTab);
        homeTitleLabel->setObjectName("homeTitleLabel");
        homeTitleLabel->setGeometry(QRect(150, 30, 571, 131));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Academy Engraved LET")});
        font1.setPointSize(18);
        font1.setBold(false);
        homeTitleLabel->setFont(font1);
        homeHintLabel = new QLabel(homeTab);
        homeHintLabel->setObjectName("homeHintLabel");
        homeHintLabel->setGeometry(QRect(34, 483, 931, 181));
        homeHintLabel->setWordWrap(true);
        tabs->addTab(homeTab, QString());
        booksTab = new QWidget();
        booksTab->setObjectName("booksTab");
        booksTab->setStyleSheet(QString::fromUtf8("QWidget#booksTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        readersTab->setStyleSheet(QString::fromUtf8("QWidget#readersTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        loansTab->setStyleSheet(QString::fromUtf8("QWidget#loansTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        reportsTab->setStyleSheet(QString::fromUtf8("QWidget#reportsTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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

        deleteReportButton = new QPushButton(reportsActionsGroup);
        deleteReportButton->setObjectName("deleteReportButton");

        reportsActionsLayout->addWidget(deleteReportButton);

        reportsActionsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        reportsActionsLayout->addItem(reportsActionsSpacer);


        reportsTabLayout->addWidget(reportsActionsGroup);

        tabs->addTab(reportsTab, QString());
        statsTab = new QWidget();
        statsTab->setObjectName("statsTab");
        statsTab->setStyleSheet(QString::fromUtf8("QWidget#statsTab {\n"
"    background-color: #c9cdd5ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
        statsTabOuterLayout = new QVBoxLayout(statsTab);
        statsTabOuterLayout->setSpacing(0);
        statsTabOuterLayout->setObjectName("statsTabOuterLayout");
        statsTabOuterLayout->setContentsMargins(0, 0, 0, 0);
        statsTabLayout = new QVBoxLayout();
        statsTabLayout->setSpacing(10);
        statsTabLayout->setObjectName("statsTabLayout");
        statsTabLayout->setContentsMargins(12, 12, 12, 12);
        statsFilterGroup = new QGroupBox(statsTab);
        statsFilterGroup->setObjectName("statsFilterGroup");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(statsFilterGroup->sizePolicy().hasHeightForWidth());
        statsFilterGroup->setSizePolicy(sizePolicy3);
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


        statsTabOuterLayout->addLayout(statsTabLayout);

        tabs->addTab(statsTab, QString());
        staffsTab = new QWidget();
        staffsTab->setObjectName("staffsTab");
        staffsTab->setStyleSheet(QString::fromUtf8("QWidget#staffsTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        accountsTab->setStyleSheet(QString::fromUtf8("QWidget#accountsTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        settingsTab->setStyleSheet(QString::fromUtf8("QWidget#settingsTab {\n"
"    background-color: #090e18ff;\n"
"    border-image: url(:/icons/logobook.png) 0 0 0 0 stretch stretch;\n"
"    background-image: url(:/icons/logobook.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"}"));
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
        menubar->setGeometry(QRect(0, 0, 1333, 37));
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "H\341\273\206 TH\341\273\220NG CHO THU\303\212 TRUY\341\273\206N V\303\200 S\303\201CH PBL2", nullptr));
        refreshAction->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        logoutAction->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
        navigationTitle->setText(QCoreApplication::translate("MainWindow", "\304\220i\341\273\201u h\306\260\341\273\233ng", nullptr));
        refreshButton->setText(QCoreApplication::translate("MainWindow", "T\341\272\243i l\341\272\241i", nullptr));
        logoutButton->setText(QCoreApplication::translate("MainWindow", "\304\220\304\203ng xu\341\272\245t", nullptr));
        homeTitleLabel->setText(QCoreApplication::translate("MainWindow", "H\341\273\206 TH\341\273\220NG QU\341\272\242N L\303\235 CHO THU\303\212 S\303\201CH V\303\200 TRUY\341\273\206N", nullptr));
        homeHintLabel->setText(QCoreApplication::translate("MainWindow", "S\341\273\255 d\341\273\245ng c\303\241c tab b\303\252n d\306\260\341\273\233i ho\341\272\267c menu b\303\252n tr\303\241i \304\221\341\273\203 di chuy\341\273\203n gi\341\273\257a c\303\241c khu v\341\273\261c. T\341\272\245t c\341\272\243 th\303\264ng tin m\341\273\233i nh\341\272\245t v\341\273\201 s\303\241ch, b\341\272\241n \304\221\341\273\215c v\303\240 phi\341\272\277u m\306\260\341\273\243n s\341\272\275 \304\221\306\260\341\273\243c c\341\272\255p nh\341\272\255t sau m\341\273\227i l\341\272\247n t\341\272\243i l\341\272\241i.", nullptr));
        tabs->setTabText(tabs->indexOf(homeTab), QCoreApplication::translate("MainWindow", "Trang ch\341\273\247", nullptr));
        booksFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        bookSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\254m theo ti\303\252u \304\221\341\273\201, t\303\241c gi\341\272\243, th\341\273\203 lo\341\272\241i ho\341\272\267c nh\303\240 xu\341\272\245t b\341\272\243n", nullptr));
        bookFilterButton->setText(QCoreApplication::translate("MainWindow", "L\341\273\215c", nullptr));
        booksActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        addBookButton->setText(QCoreApplication::translate("MainWindow", "Th\303\252m s\303\241ch", nullptr));
        editBookButton->setText(QCoreApplication::translate("MainWindow", "C\341\272\255p nh\341\272\255t", nullptr));
        deleteBookButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a s\303\241ch", nullptr));
        restockBookButton->setText(QCoreApplication::translate("MainWindow", "Nh\341\272\255p h\303\240ng", nullptr));
        tabs->setTabText(tabs->indexOf(booksTab), QCoreApplication::translate("MainWindow", "S\303\241ch", nullptr));
        readersFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        readerSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\254m \304\221\341\273\231c gi\341\272\243 theo t\303\252n, m\303\243 \304\221\341\273\231c gi\341\272\243, email ho\341\272\267c s\341\273\221 \304\221i\341\273\207n tho\341\272\241i", nullptr));
        readerFilterButton->setText(QCoreApplication::translate("MainWindow", "L\341\273\215c", nullptr));
        readersActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        addReaderButton->setText(QCoreApplication::translate("MainWindow", "Th\303\252m \304\221\341\273\231c gi\341\272\243", nullptr));
        editReaderButton->setText(QCoreApplication::translate("MainWindow", "C\341\272\255p nh\341\272\255t", nullptr));
        toggleReaderStatusButton->setText(QCoreApplication::translate("MainWindow", "T\341\272\241m kh\303\263a / M\341\273\237 kh\303\263a", nullptr));
        deleteReaderButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a", nullptr));
        tabs->setTabText(tabs->indexOf(readersTab), QCoreApplication::translate("MainWindow", "\304\220\341\273\231c gi\341\272\243", nullptr));
        loansFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        loanSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\254m theo m\303\243 m\306\260\341\273\243n, m\303\243 \304\221\341\273\231c gi\341\272\243 ho\341\272\267c m\303\243 s\303\241ch", nullptr));
        loanFilterButton->setText(QCoreApplication::translate("MainWindow", "L\341\273\215c", nullptr));
        loansActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        newLoanButton->setText(QCoreApplication::translate("MainWindow", "L\341\272\255p phi\341\272\277u m\306\260\341\273\243n", nullptr));
        returnLoanButton->setText(QCoreApplication::translate("MainWindow", "\304\220\303\241nh d\341\272\245u \304\221\303\243 tr\341\272\243", nullptr));
        extendLoanButton->setText(QCoreApplication::translate("MainWindow", "Gia h\341\272\241n", nullptr));
        lostLoanButton->setText(QCoreApplication::translate("MainWindow", "M\341\272\245t s\303\241ch", nullptr));
        damageLoanButton->setText(QCoreApplication::translate("MainWindow", "H\306\260 h\341\273\217ng", nullptr));
        deleteLoanButton->setText(QCoreApplication::translate("MainWindow", "H\341\273\247y phi\341\272\277u", nullptr));
        tabs->setTabText(tabs->indexOf(loansTab), QCoreApplication::translate("MainWindow", "M\306\260\341\273\243n tr\341\272\243", nullptr));
        reportsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        reportStaffFilter->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\254m theo nh\303\242n vi\303\252n x\341\273\255 l\303\275", nullptr));
        reportApplyButton->setText(QCoreApplication::translate("MainWindow", "\303\201p d\341\273\245ng", nullptr));
        reportClearButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a loc", nullptr));
        reportsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        submitReportButton->setText(QCoreApplication::translate("MainWindow", "L\341\272\255p y\303\252u c\341\272\247u", nullptr));
        approveReportButton->setText(QCoreApplication::translate("MainWindow", "Duy\341\273\207t", nullptr));
        rejectReportButton->setText(QCoreApplication::translate("MainWindow", "T\341\273\253 ch\341\273\221i", nullptr));
        deleteReportButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a b\303\241o c\303\241o", nullptr));
        tabs->setTabText(tabs->indexOf(reportsTab), QCoreApplication::translate("MainWindow", "B\303\241o c\303\241o", nullptr));
        statsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        timePeriodLabel->setText(QCoreApplication::translate("MainWindow", "Th\341\273\235i gian:", nullptr));
        timePeriodCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Tu\341\272\247n n\303\240y", nullptr));
        timePeriodCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Th\303\241ng n\303\240y", nullptr));
        timePeriodCombo->setItemText(2, QCoreApplication::translate("MainWindow", "N\304\203m nay", nullptr));
        timePeriodCombo->setItemText(3, QCoreApplication::translate("MainWindow", "T\341\272\245t c\341\272\243", nullptr));

        applyFilterButton->setText(QCoreApplication::translate("MainWindow", "\303\201p d\341\273\245ng", nullptr));
        tabs->setTabText(tabs->indexOf(statsTab), QCoreApplication::translate("MainWindow", "Th\341\273\221ng k\303\252", nullptr));
        staffsFilterGroup->setTitle(QCoreApplication::translate("MainWindow", "B\341\273\231 l\341\273\215c", nullptr));
        staffSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\254m nh\303\242n vi\303\252n theo t\303\252n, m\303\243 nh\303\242n vi\303\252n ho\341\272\267c ch\341\273\251c v\341\273\245", nullptr));
        staffFilterButton->setText(QCoreApplication::translate("MainWindow", "L\341\273\215c", nullptr));
        staffsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        addStaffButton->setText(QCoreApplication::translate("MainWindow", "Th\303\252m nh\303\242n vi\303\252n", nullptr));
        editStaffButton->setText(QCoreApplication::translate("MainWindow", "C\341\272\255p nh\341\272\255t", nullptr));
        deleteStaffButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a", nullptr));
        toggleStaffStatusButton->setText(QCoreApplication::translate("MainWindow", "Chuy\341\273\203n tr\341\272\241ng th\303\241i", nullptr));
        tabs->setTabText(tabs->indexOf(staffsTab), QCoreApplication::translate("MainWindow", "Nh\303\242n vi\303\252n", nullptr));
        accountsActionsGroup->setTitle(QCoreApplication::translate("MainWindow", "T\303\241c v\341\273\245", nullptr));
        addAccountButton->setText(QCoreApplication::translate("MainWindow", "Th\303\252m t\303\240i kho\341\272\243n", nullptr));
        resetPasswordButton->setText(QCoreApplication::translate("MainWindow", "\304\220\341\272\267t l\341\272\241i m\341\272\255t kh\341\272\251u", nullptr));
        toggleAccountButton->setText(QCoreApplication::translate("MainWindow", "K\303\255ch ho\341\272\241t / V\303\264 hi\341\273\207u", nullptr));
        deleteAccountButton->setText(QCoreApplication::translate("MainWindow", "X\303\263a tai khoan", nullptr));
        tabs->setTabText(tabs->indexOf(accountsTab), QCoreApplication::translate("MainWindow", "T\303\240i kho\341\272\243n", nullptr));
        settingsGroup->setTitle(QCoreApplication::translate("MainWindow", "Thi\341\272\277t l\341\272\255p h\341\273\207 th\341\273\221ng", nullptr));
        maxBorrowDaysLabel->setText(QCoreApplication::translate("MainWindow", "S\341\273\221 ng\303\240y m\306\260\341\273\243n t\341\273\221i \304\221a", nullptr));
        finePerDayLabel->setText(QCoreApplication::translate("MainWindow", "Ti\341\273\201n ph\341\272\241t m\341\273\227i ng\303\240y", nullptr));
        maxBooksPerReaderLabel->setText(QCoreApplication::translate("MainWindow", "S\341\273\221 s\303\241ch t\341\273\221i \304\221a m\341\273\227i \304\221\341\273\231c gi\341\272\243", nullptr));
        saveConfigButton->setText(QCoreApplication::translate("MainWindow", "L\306\260u c\341\272\245u h\303\254nh", nullptr));
        tabs->setTabText(tabs->indexOf(settingsTab), QCoreApplication::translate("MainWindow", "C\303\240i \304\221\341\272\267t", nullptr));
        menuSystem->setTitle(QCoreApplication::translate("MainWindow", "H\341\273\207 th\341\273\221ng", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
