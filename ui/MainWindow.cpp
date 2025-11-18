#include <algorithm>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyle>
#include <QMessageBox>
#include <QInputDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QIcon>
#include <QFrame>
#include <QSize>
#include <QTabBar>
#include <QSignalBlocker>
#include <QTimer>
#include <QLocale>
#include <QFileDialog>
#include <QTextDocument>
#include <QDir>
#include <QResizeEvent>
#include <QTableWidgetItem>
#include "core/IdGenerator.h"
#include "core/custom/DynamicArray.h"
#include "QtBridge.h"
#include "BookStatus.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QToolButton>
#include "AccountDialog.h"
#include "BookDialog.h"
#include "LoanDialog.h"
#include "ReaderDialog.h"
#include "StaffDialog.h"
#include "ReportRequestDialog.h"
#include "ReportDetailsDialog.h"
#include "StatsWidget.h"

using namespace std;
using namespace pbl2;
using namespace pbl2::bridge;


namespace {
constexpr int kCardRoleId = Qt::UserRole + 1;
constexpr int kCardRoleHeader = Qt::UserRole + 2;
constexpr int kCardRoleMeta = Qt::UserRole + 3;
constexpr int kCardRoleDetail = Qt::UserRole + 4;
constexpr int kCardRoleBadgeText = Qt::UserRole + 5;
constexpr int kCardRoleBadgeColor = Qt::UserRole + 6;
constexpr int kCardRolePillText = Qt::UserRole + 7;
constexpr int kCardRolePillColor = Qt::UserRole + 8;
constexpr int kCardRoleSecondaryDetail = Qt::UserRole + 9;

QString normalizedStatus(const QString &text);

QColor statusBadgeColor(const QString &statusCode) {
    const QString normalized = normalizedStatus(statusCode);
    if (normalized == QStringLiteral("CON")) return QColor(0x28, 0xA7, 0x45);
    if (normalized == QStringLiteral("HET")) return QColor(0x96, 0x9D, 0xA6);
    if (normalized == QStringLiteral("MAT")) return QColor(0xD9, 0x3F, 0x3F);
    if (normalized == QStringLiteral("HONG")) return QColor(0xF0, 0x73, 0x24);
    if (normalized == QStringLiteral("BORROWED")) return QColor(0x2F, 0x6A, 0xD0);
    if (normalized == QStringLiteral("RETURNED")) return QColor(0x28, 0xA7, 0x45);
    if (normalized == QStringLiteral("OVERDUE")) return QColor(0xF5, 0x8B, 0x0A);
    if (normalized == QStringLiteral("LOST")) return QColor(0xD9, 0x3F, 0x3F);
    if (normalized == QStringLiteral("DAMAGED")) return QColor(0xF0, 0x73, 0x24);
    if (normalized == QStringLiteral("PENDING")) return QColor(0xFF, 0xA0, 0x2F);
    if (normalized == QStringLiteral("APPROVED")) return QColor(0x28, 0xA7, 0x45);
    if (normalized == QStringLiteral("REJECTED")) return QColor(0xD9, 0x3F, 0x3F);
    if (normalized == QStringLiteral("ACTIVE")) return QColor(0x28, 0xA7, 0x45);
    if (normalized == QStringLiteral("INACTIVE")) return QColor(0x96, 0x9D, 0xA6);
    return QColor(0x2F, 0x6A, 0xD0);
}

class CardListDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (!painter || !index.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

    const bool selected = option.state & QStyle::State_Selected;
    // Reduce outer margins slightly to make cards more compact
    const QRect outer = option.rect.adjusted(4, 2, -4, -2);
    const QColor background = selected ? QColor(0xDB, 0xEA, 0xFE) : QColor(0xFF, 0xFF, 0xFF);
    QColor border = selected ? QColor(0x93, 0xC5, 0xFD) : QColor(0xD8, 0xE3, 0xF2);

        painter->setPen(border);
        painter->setBrush(background);
    painter->drawRoundedRect(outer, 6, 6);

    // Reduce inner padding so text area is larger relative to the card
    const QRect inner = outer.adjusted(10, 10, -10, -10);
    const QString header = index.data(kCardRoleHeader).toString();
    const QString meta = index.data(kCardRoleMeta).toString();
    const QString detail = index.data(kCardRoleDetail).toString();
    const QString extraDetail = index.data(kCardRoleSecondaryDetail).toString();
        const QString badgeText = index.data(kCardRoleBadgeText).toString();
        QColor badgeColor = index.data(kCardRoleBadgeColor).value<QColor>();
        if (!badgeColor.isValid()) {
            badgeColor = statusBadgeColor(badgeText);
        }
        const QString pillText = index.data(kCardRolePillText).toString();
        QColor pillColor = index.data(kCardRolePillColor).value<QColor>();
        if (!pillColor.isValid()) {
            pillColor = option.palette.alternateBase().color();
        }

        // Make fonts slightly smaller for a denser layout
        QFont titleFont = option.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(std::max(9.0, titleFont.pointSizeF() - 1.0));
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(std::max(9, titleFont.pointSize() - 1));
        } else {
            titleFont.setPointSize(9);
        }
        titleFont.setBold(true);

        QFont bodyFont = option.font;
        QFont secondaryFont = bodyFont;
        if (secondaryFont.pointSizeF() > 0) {
            secondaryFont.setPointSizeF(std::max(8.0, secondaryFont.pointSizeF() - 1.5));
        } else if (secondaryFont.pointSize() > 0) {
            secondaryFont.setPointSize(std::max(8, secondaryFont.pointSize() - 1));
        }

        QFont badgeFont = secondaryFont;
        badgeFont.setBold(true);

        QFontMetrics titleMetrics(titleFont);
        QFontMetrics bodyMetrics(bodyFont);
        QFontMetrics secondaryMetrics(secondaryFont);
        QFontMetrics badgeMetrics(badgeFont);
    // Reduce badge/pill padding so they take less horizontal/vertical space
    const int badgeWidth = badgeMetrics.horizontalAdvance(badgeText) + 12;
    const int badgeHeight = badgeMetrics.height() + 6;
        QRect badgeRect(inner.right() - badgeWidth, inner.top(), badgeWidth, badgeHeight);

        constexpr auto primaryText = QColor(0x0F, 0x17, 0x2A);
    QColor secondaryText = QColor(0x4B, 0x55, 0x6B);
    if (selected) {
        secondaryText = QColor(0x1E, 0x40, 0x8A);
    }

    // Add a small top padding so text doesn't touch the rounded corner.
    int y = inner.top() + 6;

    painter->setFont(titleFont);
    painter->setPen(primaryText);
    const int titleW = std::max(inner.width() - badgeWidth - 12, 80);
    const QRect titleRect(inner.left(), y, titleW, titleMetrics.height());
    // Elide long titles so they don't overflow the card boundaries.
    const QString elidedTitle = titleMetrics.elidedText(header, Qt::ElideRight, titleRect.width());
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignTop, elidedTitle);
    y += titleMetrics.height() + 6;

        // Compose up to 3 lines of body content (meta, detail, extraDetail)
    QStringList parts;
    if (!meta.isEmpty()) parts << meta;
    if (!detail.isEmpty()) parts << detail;
    if (!extraDetail.isEmpty()) parts << extraDetail;
    const QString combinedBody = parts.join('\n');
    painter->setFont(bodyFont);
    painter->setPen(secondaryText);
    // Reserve space for up to 4 wrapped body lines
    constexpr int bodyLines = 4;
    const int bodyAreaHeight = bodyMetrics.height() * bodyLines;

    const int pillWidthReserve = pillText.isEmpty() ? 0 : secondaryMetrics.horizontalAdvance(pillText) + 16;
    const QRect bodyRect(inner.left(), y, std::max(inner.width() - pillWidthReserve - 8, 120), bodyAreaHeight);
    // Align to top so wrapped text starts at the top of the body area and
    // doesn't get vertically centered (which could appear clipped).
    painter->drawText(bodyRect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, combinedBody);
    y += bodyAreaHeight + 4;

        if (!pillText.isEmpty()) {
            painter->setFont(secondaryFont);
            const int pillWidth = secondaryMetrics.horizontalAdvance(pillText) + 12;
            const int pillHeight = secondaryMetrics.height() + 6;
            QRect pillRect(inner.right() - pillWidth, inner.bottom() - pillHeight, pillWidth, pillHeight);

            QColor pillBg = selected ? background.darker(120) : pillColor;
            QColor pillFg = QColor(0x0F, 0x17, 0x2A);
            painter->setPen(Qt::NoPen);
            painter->setBrush(pillBg);
            painter->drawRoundedRect(pillRect, 6, 6);
            painter->setPen(pillFg);
            painter->drawText(pillRect, Qt::AlignCenter, pillText);
        }

        QColor badgeBg = badgeColor.isValid() ? badgeColor : option.palette.highlight().color();
        QColor badgeFg = Qt::white;
        if (badgeBg.lightness() > 200) {
            badgeFg = QColor(0x0F, 0x17, 0x2A);
        }
        if (selected) {
            badgeBg = badgeBg.lighter(140);
            badgeFg = QColor(0x0F, 0x17, 0x2A);
        }

    painter->setFont(badgeFont);
    painter->setPen(Qt::NoPen);
    painter->setBrush(badgeBg);
    const int badgeRadius = std::min(badgeHeight / 2, 8);
    painter->drawRoundedRect(badgeRect, badgeRadius, badgeRadius);
        painter->setPen(badgeFg);
        painter->drawText(badgeRect, Qt::AlignCenter, badgeText);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(index);
        QFont titleFont = option.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        QFont secondary = option.font;
        QFontMetrics titleMetrics(titleFont);
        QFontMetrics bodyMetrics(secondary);
        // Make room for header (1 line) + up to 3 body lines (meta/detail/extra)
        constexpr int bodyLines = 3;
        const int height = 12 + titleMetrics.height() + 6 + bodyMetrics.height() * bodyLines + 12;

        // If the view is in IconMode (grid), return a reasonable fixed width so
        // multiple columns can form. When in ListMode, items should span the
        // full width (option.rect.width()). We try to infer the view and its
        // current mode from option.widget.
        if (option.widget) {
            if (auto *view = qobject_cast<const QListView *>(option.widget)) {
                if (view->viewMode() == QListView::IconMode) {
                    // Target exactly 2 columns: compute per-item width as
                    // (viewport_width - total_spacing) / columns.
                    const int vw = view->viewport() ? view->viewport()->width() : view->width();
                    constexpr int columns = 2;
                    // Estimate total horizontal spacing between items (use
                    // list spacing and a small margin). Use view->spacing()
                    // if available via cast to QListWidget; otherwise assume 8.
                    int spacing = 8;
                    if (auto *lw = qobject_cast<const QListWidget *>(view)) {
                        spacing = lw->spacing();
                    }
                    const int totalGaps = (columns + 1) * spacing;
                    int preferred = (vw - totalGaps) / columns;
                    // Clamp to reasonable visual sizes so cards are not tiny
                    // or huge across different displays.
                    preferred = std::clamp(preferred, 220, 420);
                    // Use a slightly larger height to fit the extra line
                    const int preferredHeight = std::max(height, 180);
                    return QSize(preferred, preferredHeight);
                }
            }
        }
        return QSize(option.rect.width(), std::max(height, 112));
    }
};

class NavigationListDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (!painter || !index.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);

        const QStringList lines = opt.text.split('\n', Qt::KeepEmptyParts);
        const QString title = lines.value(0);
        QString description;
        if (lines.size() > 1) {
            description = lines.mid(1).join(QStringLiteral(" ")).trimmed();
        }

        const QRect baseTextRect = opt.widget ? opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget)
                                              : QApplication::style()->subElementRect(QStyle::SE_ItemViewItemText, &opt, nullptr);

        // If the navigation list is in collapsed mode, render icon-only.
        if (const bool collapsedMode = opt.widget ? opt.widget->property("navCollapsed").toBool() : false) {
            // Draw only the icon centered inside the item rect.
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            const QRect iconRect = option.rect.adjusted(6, 6, -6, -6);
            QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
            if (!icon.isNull()) {
                const QSize isz = option.decorationSize;
                const QPoint center = iconRect.center();
                const QRect drawRect(center.x() - isz.width() / 2, center.y() - isz.height() / 2, isz.width(), isz.height());
                icon.paint(painter, drawRect);
            }
            painter->restore();
            return;
        }

        opt.text.clear();
        if (opt.widget) {
            opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        } else {
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
        }

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        const QRect contentRect = baseTextRect.adjusted(8, 6, -8, -6);
        QFont titleFont = opt.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        QFont descriptionFont = opt.font;
        if (descriptionFont.pointSizeF() > 0) {
            descriptionFont.setPointSizeF(std::max(descriptionFont.pointSizeF() - 0.5, 7.5));
        } else if (descriptionFont.pointSize() > 0) {
            descriptionFont.setPointSize(std::max(descriptionFont.pointSize() - 1, 7));
        }
        descriptionFont.setBold(false);

        const bool selected = option.state & QStyle::State_Selected;
        const bool hovered = option.state & QStyle::State_MouseOver;

        QColor titleColor(0x0F, 0x17, 0x2A);
        QColor descriptionColor(0x64, 0x74, 0x8B);
        if (selected) {
            titleColor = QColor(0x0F, 0x17, 0x2A);
            descriptionColor = QColor(0x1F, 0x36, 0x5B);
        } else if (hovered) {
            titleColor = QColor(0x1D, 0x4E, 0xD8);
            descriptionColor = QColor(0x1D, 0x4E, 0xD8);
        }

        painter->setPen(titleColor);
        painter->setFont(titleFont);
        const QFontMetrics titleMetrics(titleFont);
        const QRect titleRect = titleMetrics.boundingRect(contentRect, Qt::TextSingleLine, title);
        painter->drawText(contentRect, Qt::TextSingleLine, title);

        if (!description.isEmpty()) {
            painter->setPen(descriptionColor);
            painter->setFont(descriptionFont);
            QRect detailRect = contentRect;
            detailRect.setY(detailRect.y() + titleRect.height() + 6);
            painter->drawText(detailRect, Qt::TextWordWrap | Qt::AlignLeft, description);
        }

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);

        const QStringList lines = opt.text.split('\n', Qt::KeepEmptyParts);
        const QString title = lines.value(0);
        QString description;
        if (lines.size() > 1) {
            description = lines.mid(1).join(QStringLiteral(" ")).trimmed();
        }

        QFont titleFont = opt.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        QFont descriptionFont = opt.font;
        if (descriptionFont.pointSizeF() > 0) {
            descriptionFont.setPointSizeF(std::max(descriptionFont.pointSizeF() - 0.5, 7.5));
        } else if (descriptionFont.pointSize() > 0) {
            descriptionFont.setPointSize(std::max(descriptionFont.pointSize() - 1, 7));
        }
        descriptionFont.setBold(false);

    int measuredWidth = option.rect.width();
        if (measuredWidth <= 0 && opt.widget) {
            measuredWidth = opt.widget->width();
        }
        if (measuredWidth <= 0) {
            measuredWidth = 280;
        }
        if (const bool collapsedMode = opt.widget ? opt.widget->property("navCollapsed").toBool() : false) {
            // In collapsed mode, prefer a narrow, icon-only width that fits the
            // decoration size plus a small padding so the list becomes a slim
            // vertical strip of icons.
            const int w = opt.decorationSize.width() + 16;
            const int h = std::max(72, opt.decorationSize.height() + 12);
            return QSize(w, h);
        }
        const int iconWidth = opt.decorationSize.width();
        const int availableWidth = std::max(180, measuredWidth - iconWidth - 60);
        const QFontMetrics titleMetrics(titleFont);
        const QRect titleRect = titleMetrics.boundingRect(0, 0, availableWidth, 0, Qt::TextSingleLine, title);

        int totalHeight = 24 + titleRect.height();
        if (!description.isEmpty()) {
            const QFontMetrics descriptionMetrics(descriptionFont);
            const QRect descriptionRect = descriptionMetrics.boundingRect(0, 0, availableWidth, 0, Qt::TextWordWrap, description);
            totalHeight += 6 + descriptionRect.height();
        }

        return QSize(measuredWidth, std::max(totalHeight, 72));
    }
};

void configureCardListWidget(QListWidget *list) {
    if (!list) return;
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setAlternatingRowColors(true);
    list->setUniformItemSizes(false);
    list->setWordWrap(true);
    list->setSpacing(6);
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    list->setFrameShape(QFrame::NoFrame);
    list->viewport()->setAutoFillBackground(false);

    QFont compactFont = list->font();
    if (compactFont.pointSizeF() > 0) {
        compactFont.setPointSizeF(std::max(9.0, compactFont.pointSizeF()));
    } else if (compactFont.pointSize() > 0) {
        compactFont.setPointSize(std::max(9, compactFont.pointSize()));
    }
    list->setFont(compactFont);
    // Arrange items in a wrapping grid so cards form columns/rows instead of a single column
    list->setFlow(QListView::LeftToRight);
    list->setWrapping(true);
    list->setResizeMode(QListView::Adjust);
    list->setViewMode(QListView::IconMode);

    list->setStyleSheet(
        "QListWidget {"
        "  border: none;"
        "  padding: 4px 0;"
        "  background: transparent;"
        "}"
        "QListWidget::item {"
        "  margin: 6px 4px;"
        "}"
        "QListWidget::item:selected {"
        "  background: transparent;"
        "}");

    if (!dynamic_cast<CardListDelegate *>(list->itemDelegate())) {
        list->setItemDelegate(new CardListDelegate(list));
    }
}

QString normalizedStatus(const QString &text) {
    return text.trimmed().toUpper();
}

QString normalizedStatus(const custom::CustomString &text) {
    return normalizedStatus(bridge::toQString(text));
}

QString bookStatusText(const custom::CustomString &code) {
    const QString value = bridge::toQString(model::canonicalBookStatus(code));
    if (value == QStringLiteral("CON")) return QObject::tr("CON");
    if (value == QStringLiteral("HET")) return QObject::tr("HET");
    if (value == QStringLiteral("MAT")) return QObject::tr("MAT");
    if (value == QStringLiteral("HONG")) return QObject::tr("HONG");
    return value;
}

QString loanStatusText(const QString &code) {
    const QString value = normalizedStatus(code);
    if (value == QStringLiteral("BORROWED")) return QObject::tr("Dang muon");
    if (value == QStringLiteral("RETURNED")) return QObject::tr("Da tra");
    if (value == QStringLiteral("OVERDUE")) return QObject::tr("Qua han");
    if (value == QStringLiteral("LOST")) return QObject::tr("Bao mat");
    if (value == QStringLiteral("DAMAGED")) return QObject::tr("Bao hu");
    return code;
}

QString accountActiveText(bool active) {
    return active ? QObject::tr("Dang hoat dong") : QObject::tr("Da khoa");
}

QString reportStatusText(const QString &status) {
    const QString value = normalizedStatus(status);
    if (value == QStringLiteral("PENDING")) return QObject::tr("Cho duyet");
    if (value == QStringLiteral("APPROVED")) return QObject::tr("Da chap nhan");
    if (value == QStringLiteral("REJECTED")) return QObject::tr("Da tu choi");
    return status;
}

template <typename T>
QVector<T> toQVector(const custom::DynamicArray<T> &values) {
    QVector<T> result;
    result.reserve(static_cast<int>(values.size()));
    for (typename custom::DynamicArray<T>::SizeType i = 0U; i < values.size(); ++i) {
        result.append(values[i]);
    }
    return result;
}

}

namespace pbl2::ui {

MainWindow::MainWindow(const QString &dataDir, const model::Account &signedInAccount, QWidget *parent)
        : QMainWindow(parent),
            ui(std::make_unique<Ui::MainWindow>()),
            dataDirectory(bridge::toCustomString(dataDir)),
            bookService(dataDirectory),
            readerService(dataDirectory),
            staffService(dataDirectory),
            loanService(dataDirectory),
            accountService(dataDirectory),
        reportService(dataDirectory),
        configService(dataDirectory),
        currentAccount(signedInAccount) {
    const QString roleText = normalizedStatus(bridge::toQString(currentAccount.getRole()));
    staffRole = roleText == QStringLiteral("STAFF") || roleText == QStringLiteral("ADMIN");
    adminRole = roleText == QStringLiteral("ADMIN");
    currentConfig = configService.load();
    setupUi();
    reloadData();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    ui->setupUi(this);

    if (ui->centralLayout) {
        ui->centralLayout->setStretch(0, 0);
        ui->centralLayout->setStretch(1, 1);
    }

    tabs = ui->tabs;
    booksList = ui->booksList;
    readersList = ui->readersList;
    staffsList = ui->staffsList;
    loansList = ui->loansList;
    reportsList = ui->reportsList;
    accountsList = ui->accountsList;
    navigationList = ui->navigationList;
    refreshButton = ui->refreshButton;

    auto ensureSummaryVisible = [](QLabel *label, const QString &value) {
        if (!label) return;
        label->setText(value);
        label->setVisible(true);
        label->show();
        label->raise();
    };

    ensureSummaryVisible(summaryBorrowedValue, "0");
    ensureSummaryVisible(summaryReturnedValue, "0");
    ensureSummaryVisible(summaryOverdueValue, "0");
    ensureSummaryVisible(summaryFinesValue, "0 VND");

    if (ui->homeTitleLabel) {
        ui->homeTitleLabel->setStyleSheet(QStringLiteral("color: #0f172a; font-size: 20px; font-weight: 700;"));
        ui->homeTitleLabel->setWordWrap(true);
    }
    if (ui->homeHintLabel) {
        ui->homeHintLabel->setStyleSheet(QStringLiteral("color: #475569; font-size: 14px;"));
        ui->homeHintLabel->setText(tr("Su dung cac tab hoac menu ben trai de truy cap nhanh cac khu vuc quan ly. Du lieu moi nhat ve sach, ban doc va phieu muon se duoc cap nhat ngay sau khi ban tai lai."));
    }

    bookSearchEdit = ui->bookSearchEdit;
    bookStatusFilter = ui->bookStatusFilter;
    readerSearchEdit = ui->readerSearchEdit;
    readerStatusFilter = ui->readerStatusFilter;
    staffSearchEdit = ui->staffSearchEdit;
    staffStatusFilter = ui->staffStatusFilter;
    loanSearchEdit = ui->loanSearchEdit;
    loanStatusFilter = ui->loanStatusFilter;
    reportStaffFilter = ui->reportStaffFilter;
    reportFromFilter = ui->reportFromFilter;
    reportToFilter = ui->reportToFilter;
    maxBorrowDaysSpin = ui->maxBorrowDaysSpin;
    finePerDaySpin = ui->finePerDaySpin;
    maxBooksPerReaderSpin = ui->maxBooksPerReaderSpin;

    QFont baseFont = font();
    if (baseFont.pointSizeF() > 0) {
        baseFont.setPointSizeF(baseFont.pointSizeF() + 1.0);
    } else if (baseFont.pointSize() > 0) {
        baseFont.setPointSize(baseFont.pointSize() + 1);
    }
    setFont(baseFont);

    QFont accentFont = baseFont;
    if (accentFont.pointSizeF() > 0) {
        accentFont.setPointSizeF(accentFont.pointSizeF() + 1.0);
    } else if (accentFont.pointSize() > 0) {
        accentFont.setPointSize(accentFont.pointSize() + 1);
    }

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(0xF8, 0xFA, 0xFC));
    pal.setColor(QPalette::Base, QColor(0xFF, 0xFF, 0xFF));
    pal.setColor(QPalette::AlternateBase, QColor(0xF1, 0xF5, 0xF9));
    pal.setColor(QPalette::Button, QColor(0xE2, 0xE8, 0xF0));
    pal.setColor(QPalette::WindowText, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::Text, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::ButtonText, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::Highlight, QColor(0xDB, 0xEA, 0xFE));
    pal.setColor(QPalette::HighlightedText, QColor(0x0F, 0x17, 0x2A));
    pal.setColor(QPalette::BrightText, QColor(0xB9, 0x1C, 0x1C));
    pal.setColor(QPalette::Link, QColor(0x1D, 0x4E, 0xD8));
    setPalette(pal);
    setAutoFillBackground(true);

    if (tabs) {
        tabs->setDocumentMode(false);
        if (auto *bar = tabs->tabBar()) {
            bar->show();
            bar->setExpanding(false);
            bar->setStyleSheet(
                "QTabBar::tab {"
                "  background: #e2e8f0;"
                "  color: #0f172a;"
                "  padding: 8px 16px;"
                "  border: 1px solid #cbd5f5;"
                "  border-bottom: none;"
                "  border-top-left-radius: 6px;"
                "  border-top-right-radius: 6px;"
                "  margin-right: 4px;"
                "}"
                "QTabBar::tab:selected {"
                "  background: #ffffff;"
                "  color: #1d4ed8;"
                "  border-color: #cbd5f5;"
                "}"
                "QTabBar::tab:hover {"
                "  background: #f8fafc;"
                "}");
        }
        tabs->setStyleSheet(QString());
        connect(tabs, &QTabWidget::currentChanged, [this](int index) {
            if (navigationList && navigationList->currentRow() != index) {
                QSignalBlocker blocker(navigationList);
                navigationList->setCurrentRow(index);
            }
        });
    }

    if (navigationList) {
        navigationList->setSelectionMode(QAbstractItemView::SingleSelection);
        navigationList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        navigationList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    navigationList->setWordWrap(true);
    navigationList->setUniformItemSizes(false);
    navigationList->setSpacing(8);
    navigationList->setTextElideMode(Qt::ElideNone);
    navigationList->setItemDelegate(new NavigationListDelegate(navigationList));
        connect(navigationList, &QListWidget::currentRowChanged, this, &MainWindow::handleNavigationSelection);
    }

    // Create a slim navigation rail widget (icon-only) that overlays the
    // left side when the main navigation is collapsed. This contains icon
    // buttons for each nav entry and action buttons at the bottom.
    if (!navRail) {
        navRail = new QWidget(this);
        navRail->setObjectName("navRail");
        navRail->setFixedWidth(navCollapsedWidth);
        navRail->setVisible(false);
        QVBoxLayout *railLayout = new QVBoxLayout(navRail);
        railLayout->setContentsMargins(6, 8, 6, 8);
        railLayout->setSpacing(6);
        // Placeholder for top icons; we'll populate after setupNavigationMenu()
    }

    if (ui->refreshAction) {
        ui->refreshAction->setText(tr("Reset"));
        connect(ui->refreshAction, &QAction::triggered, this, &MainWindow::reloadData);
    }
    if (ui->logoutAction) {
        ui->logoutAction->setText(tr("Logout"));
        connect(ui->logoutAction, &QAction::triggered, this, &MainWindow::handleLogout);
    }
    if (ui->refreshButton) {
        ui->refreshButton->setText(tr("Tai lai"));
        connect(ui->refreshButton, &QPushButton::clicked, [this]() {
            this->statusBar()->showMessage(tr("Dang tai lai..."));
            this->reloadData();
            this->statusBar()->showMessage(tr("Da tai lai du lieu."), 2000);
        });
    }
    if (ui->logoutButton) {
        ui->logoutButton->setText(tr("Dang xuat"));
        connect(ui->logoutButton, &QPushButton::clicked, [this]() { handleLogout(); });
    }

    userInfoLabel = new QLabel(this);
    const QString signedInLabel = tr("Dang dang nhap %1 (%2)")
                                      .arg(bridge::toQString(currentAccount.getUsername()).trimmed())
                                      .arg(bridge::toQString(currentAccount.getRole().trimmed()));
    userInfoLabel->setText(signedInLabel);
    userInfoLabel->setFont(accentFont);
    statusBar()->addPermanentWidget(userInfoLabel);

    // Navigation toggle/pin button: when clicked, toggles pinned state of the
    // navigation pane. When the pane is collapsed it becomes an icon-only
    // vertical strip; hovering expands it temporarily unless pinned.
    navToggleButton = new QPushButton(this);
    navToggleButton->setText(tr("Ẩn"));
    navToggleButton->setToolTip(tr("Ẩn/Hiện thanh điều hướng"));
    navToggleButton->setFixedHeight(24);
    navToggleButton->setCursor(Qt::PointingHandCursor);
    connect(navToggleButton, &QPushButton::clicked, [this]() {
        if (!navigationList) return;
        if (navCollapsed) {
            setNavigationCollapsed(false, true);
        } else {
            setNavigationCollapsed(true, false);
        }
    });
    statusBar()->addPermanentWidget(navToggleButton);

    // Install hover event filter on the navigation list so we can expand on
    // hover when in collapsed (unpinned) mode.
    if (navigationList) {
        navigationList->setProperty("navCollapsed", false);
        navigationList->installEventFilter(this);
        // Record the current width as the expanded width so collapse/expand
        // toggles keep a reasonable size.
        navExpandedWidth = navigationList->width() > 0 ? navigationList->width() : navExpandedWidth;
    }

    // Create a small rail button inside the navigation area (hidden by
    // default). When the nav is collapsed, this button will be visible and
    // will open a popup menu of navigation entries for quick access.
    if (navigationList) {
        // Parent the rail button to the main window so it remains accessible
        // when the navigationList itself is hidden (Off mode).
        navRailButton = new QPushButton(this);
        navRailButton->setText(QStringLiteral("»"));
        navRailButton->setFixedSize(28, 28);
        navRailButton->setToolTip(tr("Mở menu điều hướng"));
        navRailButton->setCursor(Qt::PointingHandCursor);
        navRailButton->setVisible(false);
        navRailButton->setStyleSheet(
            "QPushButton { border-radius:14px; background: rgba(0,0,0,0.06); }"
            "QPushButton::hover { background: rgba(0,0,0,0.12); }"
        );
        connect(navRailButton, &QPushButton::clicked, [this]() { showNavigationPopupMenu(); });

        setNavigationCollapsed(false, true);
        repositionNavRailButton();
    }

    if (adminRole) {
        statsLabel = new QLabel(this);
        statsLabel->setFont(accentFont);
        statusBar()->addPermanentWidget(statsLabel);
    }

    configureBooksTab();
    configureReadersTab();
    configureLoansTab();
    configureReportsTab();
    configureStatsTab();
    
    // Force reload cache for statistics display
    if (booksCache.isEmpty()) {
        booksCache = toQVector(bookService.fetchAll());
    }
    if (readersCache.isEmpty()) {
        readersCache = toQVector(readerService.fetchAll());
    }
    if (loansCache.isEmpty()) {
        loansCache = toQVector(loanService.fetchAll());
    }
    
    // Force refresh stats display with real data
    refreshSimpleStats();
    
    configureStaffsTab();
    configureAccountsTab();
    configureSettingsTab();

    auto removeTabIfExists = [this](QWidget *tabWidget) {
        if (!tabs || !tabWidget) return;
        const int index = tabs->indexOf(tabWidget);
        if (index != -1) {
            tabs->removeTab(index);
            tabWidget->setParent(nullptr);
        }
    };

    if (!adminRole) {
        removeTabIfExists(ui->statsTab);
        removeTabIfExists(ui->staffsTab);
        removeTabIfExists(ui->accountsTab);
        removeTabIfExists(ui->settingsTab);
    }

    setupNavigationMenu();
    if (navigationList && tabs) {
        QSignalBlocker blocker(navigationList);
        navigationList->setCurrentRow(tabs->currentIndex());
    }

    repositionNavRailButton();
}
void MainWindow::configureBooksTab() {
    configureCardListWidget(booksList);
    // Allow books to display as a grid of cards (IconMode) so items appear
    // in multiple columns. configureCardListWidget already sets IconMode
    // we explicitly ensure wrapping/flow here for clarity.
    if (booksList) {
        booksList->setFlow(QListView::LeftToRight);
        booksList->setWrapping(true);
        booksList->setViewMode(QListView::IconMode);
    }

    if (bookStatusFilter) {
        bookStatusFilter->clear();
        bookStatusFilter->addItem(tr("Tat ca tinh trang"), QStringLiteral("ALL"));
        bookStatusFilter->addItem(tr("CON"), QStringLiteral("CON"));
        bookStatusFilter->addItem(tr("HET"), QStringLiteral("HET"));
    }

    if (bookSearchEdit) {
        connect(bookSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyBookFilter);
    }
    if (bookStatusFilter) {
        connect(bookStatusFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyBookFilter);
    }
    if (ui->bookFilterButton) {
        connect(ui->bookFilterButton, &QPushButton::clicked, [this]() { applyBookFilter(); });
    }

    if (ui->addBookButton) {
        ui->addBookButton->setVisible(adminRole);
        connect(ui->addBookButton, &QPushButton::clicked, [this]() { handleAddBook(); });
    }
    if (ui->editBookButton) {
        ui->editBookButton->setVisible(adminRole);
        connect(ui->editBookButton, &QPushButton::clicked, [this]() { handleEditBook(); });
    }
    if (ui->deleteBookButton) {
        ui->deleteBookButton->setVisible(adminRole);
        connect(ui->deleteBookButton, &QPushButton::clicked, [this]() { handleDeleteBook(); });
    }
    if (ui->booksActionsGroup) {
        ui->booksActionsGroup->setVisible(adminRole);
    }
}

void MainWindow::configureReadersTab() {
    configureCardListWidget(readersList);
    // Use grid/card layout for readers: allow wrapping and IconMode so cards
    // form columns similar to Books/Reports.
    if (readersList) {
        readersList->setFlow(QListView::LeftToRight);
        readersList->setWrapping(true);
        readersList->setViewMode(QListView::IconMode);
    }

    if (readerSearchEdit) {
        connect(readerSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyReaderFilter);
    }
    if (readerStatusFilter) {
        readerStatusFilter->clear();
        readerStatusFilter->addItem(tr("Tat ca trang thai"), QStringLiteral("ALL"));
        readerStatusFilter->addItem(tr("Dang hoat dong"), QStringLiteral("ACTIVE"));
        readerStatusFilter->addItem(tr("Tam khoa"), QStringLiteral("INACTIVE"));
        connect(readerStatusFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyReaderFilter);
    }
    if (ui->readerFilterButton) {
        connect(ui->readerFilterButton, &QPushButton::clicked, [this]() { applyReaderFilter(); });
    }

    if (ui->addReaderButton) {
        ui->addReaderButton->setVisible(staffRole);
        connect(ui->addReaderButton, &QPushButton::clicked, [this]() { handleAddReader(); });
    }
    if (ui->editReaderButton) {
        ui->editReaderButton->setVisible(staffRole);
        connect(ui->editReaderButton, &QPushButton::clicked, [this]() { handleEditReader(); });
    }
    if (ui->toggleReaderStatusButton) {
        ui->toggleReaderStatusButton->setVisible(staffRole);
        connect(ui->toggleReaderStatusButton, &QPushButton::clicked, [this]() { handleToggleReaderActive(); });
    }
    if (ui->deleteReaderButton) {
        ui->deleteReaderButton->setVisible(staffRole);
        connect(ui->deleteReaderButton, &QPushButton::clicked, [this]() { handleDeleteReader(); });
    }
    if (ui->readersActionsGroup) {
        ui->readersActionsGroup->setVisible(staffRole);
    }
}

void MainWindow::configureLoansTab() {
    configureCardListWidget(loansList);

    if (loanStatusFilter) {
        loanStatusFilter->clear();
        loanStatusFilter->addItem(tr("Tat ca tinh trang"), QStringLiteral("ALL"));
        loanStatusFilter->addItem(tr("Dang muon"), QStringLiteral("BORROWED"));
        loanStatusFilter->addItem(tr("Qua han"), QStringLiteral("OVERDUE"));
        loanStatusFilter->addItem(tr("Da tra"), QStringLiteral("RETURNED"));
        loanStatusFilter->addItem(tr("Mat"), QStringLiteral("LOST"));
        loanStatusFilter->addItem(tr("Hu hong"), QStringLiteral("DAMAGED"));
        connect(loanStatusFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyLoanFilter);
    }
    if (loanSearchEdit) {
        connect(loanSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyLoanFilter);
    }
    if (ui->loanFilterButton) {
        connect(ui->loanFilterButton, &QPushButton::clicked, [this]() { applyLoanFilter(); });
    }

    if (ui->newLoanButton) {
        ui->newLoanButton->setVisible(staffRole);
        connect(ui->newLoanButton, &QPushButton::clicked, [this]() { handleNewLoan(); });
    }
    if (ui->returnLoanButton) {
        ui->returnLoanButton->setVisible(staffRole);
        connect(ui->returnLoanButton, &QPushButton::clicked, [this]() { handleMarkReturned(); });
    }
    if (ui->extendLoanButton) {
        ui->extendLoanButton->setVisible(staffRole);
        connect(ui->extendLoanButton, &QPushButton::clicked, [this]() { handleExtendLoan(); });
    }
    if (ui->lostLoanButton) {
        ui->lostLoanButton->setVisible(staffRole);
        connect(ui->lostLoanButton, &QPushButton::clicked, [this]() { handleMarkLost(); });
    }
    if (ui->damageLoanButton) {
        ui->damageLoanButton->setVisible(staffRole);
        connect(ui->damageLoanButton, &QPushButton::clicked, [this]() { handleMarkDamaged(); });
    }
    if (ui->deleteLoanButton) {
        ui->deleteLoanButton->setVisible(staffRole);
        connect(ui->deleteLoanButton, &QPushButton::clicked, [this]() { handleDeleteLoan(); });
    }
    if (ui->loansActionsGroup) {
        ui->loansActionsGroup->setVisible(staffRole);
    }

    // Use grid/card layout for loans as well so each loan appears as a card.
    if (loansList) {
        loansList->setFlow(QListView::LeftToRight);
        loansList->setWrapping(true);
        loansList->setViewMode(QListView::IconMode);
    }
}

void MainWindow::configureReportsTab() {
    configureCardListWidget(reportsList);
    // Use grid/card layout for reports: allow wrapping and IconMode so cards
    // form columns. CardListDelegate sizeHint adapts to IconMode to allow
    // multi-column display.
    if (reportsList) {
        reportsList->setFlow(QListView::LeftToRight);
        reportsList->setWrapping(true);
        reportsList->setViewMode(QListView::IconMode);
    }

    if (reportsList) {
        connect(reportsList, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *) {
            handleViewReportDetails();
        });
        connect(reportsList, &QListWidget::itemActivated, [this](QListWidgetItem *) {
            handleViewReportDetails();
        });
    }

    if (reportFromFilter) {
        reportFromFilter->setCalendarPopup(true);
        reportFromFilter->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        reportFromFilter->setDateRange(QDate(2000, 1, 1), QDate(7999, 12, 31));
        reportFromFilter->setSpecialValueText(tr("Bat ky"));
        reportFromFilter->setDate(reportFromFilter->minimumDate());
        connect(reportFromFilter, &QDateEdit::dateChanged, this, &MainWindow::applyReportFilter);
    }
    if (reportToFilter) {
        reportToFilter->setCalendarPopup(true);
        reportToFilter->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        reportToFilter->setDateRange(QDate(2000, 1, 1), QDate(7999, 12, 31));
        reportToFilter->setSpecialValueText(tr("Bat ky"));
        reportToFilter->setDate(reportToFilter->minimumDate());
        connect(reportToFilter, &QDateEdit::dateChanged, this, &MainWindow::applyReportFilter);
    }

    if (reportStaffFilter) {
        connect(reportStaffFilter, &QLineEdit::textChanged, this, &MainWindow::applyReportFilter);
    }
    if (ui->reportApplyButton) {
        connect(ui->reportApplyButton, &QPushButton::clicked, [this]() { applyReportFilter(); });
    }
    if (ui->reportClearButton) {
        connect(ui->reportClearButton, &QPushButton::clicked, [this]() { clearReportFilter(); });
    }

    if (ui->submitReportButton) {
        connect(ui->submitReportButton, &QPushButton::clicked, [this]() { handleSubmitReport(); });
    }
    if (ui->approveReportButton) {
        ui->approveReportButton->setVisible(adminRole);
        connect(ui->approveReportButton, &QPushButton::clicked, [this]() {
            handleReportStatusChange(QStringLiteral("APPROVED"));
        });
    }
    if (ui->rejectReportButton) {
        ui->rejectReportButton->setVisible(adminRole);
        connect(ui->rejectReportButton, &QPushButton::clicked, [this]() {
            handleReportStatusChange(QStringLiteral("REJECTED"));
        });
    }
}

void MainWindow::configureAccountsTab() {
    configureCardListWidget(accountsList);

    if (accountsList) {
        accountsList->setFlow(QListView::LeftToRight);
        accountsList->setWrapping(true);
        accountsList->setViewMode(QListView::IconMode);
    }

    if (ui->addAccountButton) {
        ui->addAccountButton->setVisible(adminRole);
        connect(ui->addAccountButton, &QPushButton::clicked, this, &MainWindow::handleAddAccount);
    }
    if (ui->resetPasswordButton) {
        ui->resetPasswordButton->setVisible(adminRole);
        connect(ui->resetPasswordButton, &QPushButton::clicked, this, &MainWindow::handleResetPassword);
    }
    if (ui->toggleAccountButton) {
        ui->toggleAccountButton->setVisible(adminRole);
        connect(ui->toggleAccountButton, &QPushButton::clicked, this, &MainWindow::handleToggleAccountActive);
    }
    if (ui->deleteAccountButton) {
        ui->deleteAccountButton->setVisible(adminRole);
        connect(ui->deleteAccountButton, &QPushButton::clicked, this, &MainWindow::handleDeleteAccount);
    }
    if (ui->accountsActionsGroup) {
        ui->accountsActionsGroup->setVisible(adminRole);
    }
}

void MainWindow::configureStatsTab() {
    // Find widgets - try both direct and through scroll content
    const QWidget *searchWidget = ui->statsTab;
    if (const auto scrollContent = ui->statsTab->findChild<QWidget *>(QStringLiteral("statsScrollContent"))) {
        searchWidget = scrollContent;
    }
    
    timePeriodCombo = searchWidget->findChild<QComboBox *>(QStringLiteral("timePeriodCombo"));
    loanStatsTable = searchWidget->findChild<QTableWidget *>(QStringLiteral("loanStatsTable"));
    applyFilterButton = searchWidget->findChild<QPushButton *>(QStringLiteral("applyFilterButton"));
    
    // Initialize date range to "Tuan nay"
    const QDate today = QDate::currentDate();
    statsStartDate = today.addDays(-(today.dayOfWeek() - 1));
    statsEndDate = today;
    
    // Get card labels
    totalBooksValue = searchWidget->findChild<QLabel *>(QStringLiteral("totalBooksValue"));
    totalReadersValue = searchWidget->findChild<QLabel *>(QStringLiteral("totalReadersValue"));
    totalLoansValue = searchWidget->findChild<QLabel *>(QStringLiteral("totalLoansValue"));
    totalFinesValue = searchWidget->findChild<QLabel *>(QStringLiteral("totalFinesValue"));
    overdueCount = searchWidget->findChild<QLabel *>(QStringLiteral("overdueCount"));
    monthlyFinesValue = searchWidget->findChild<QLabel *>(QStringLiteral("monthlyFinesValue"));
    
    // Get charts
    topBooksChart = searchWidget->findChild<StatsChart *>(QStringLiteral("topBooksChart"));
    revenueChart = searchWidget->findChild<StatsChart *>(QStringLiteral("revenueChart"));
    activeReadersList = searchWidget->findChild<QListWidget *>(QStringLiteral("activeReadersList"));
    topBooksList = searchWidget->findChild<QListWidget *>(QStringLiteral("topBooksList"));
    
    // Setup top books list with custom delegate for right-aligned numbers
    if (topBooksList) {
        class BooksDelegate : public QStyledItemDelegate {
        public:
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
                QStyleOptionViewItem opt = option;
                initStyleOption(&opt, index);
                
                painter->save();
                if (opt.state & QStyle::State_Selected) {
                    painter->fillRect(opt.rect, opt.palette.highlight());
                }
                
                QString text = index.data(Qt::DisplayRole).toString();
                QString count = index.data(Qt::UserRole + 1).toString();
                
                QRect textRect = opt.rect.adjusted(5, 2, -50, -2);
                QRect countRect = opt.rect.adjusted(opt.rect.width() - 55, 2, -5, -2);
                
                QFont font = painter->font();
                font.setPointSize(10);
                painter->setFont(font);
                
                painter->setPen(opt.palette.text().color());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
                painter->setPen(QColor(0x6b, 0x72, 0x80));
                painter->drawText(countRect, Qt::AlignRight | Qt::AlignVCenter, count);
                
                painter->restore();
            }
        };
        topBooksList->setItemDelegate(new BooksDelegate());
    }
    
    // Setup active readers list with delegate
    if (activeReadersList) {
        class ReadersDelegate : public QStyledItemDelegate {
        public:
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
                QStyleOptionViewItem opt = option;
                initStyleOption(&opt, index);
                
                painter->save();
                if (opt.state & QStyle::State_Selected) {
                    painter->fillRect(opt.rect, opt.palette.highlight());
                }
                
                QString text = index.data(Qt::DisplayRole).toString();
                QString count = index.data(Qt::UserRole + 1).toString();
                
                // Vẽ text (có thể nhiều dòng)
                QRect textRect = opt.rect.adjusted(5, 2, -50, -2);
                QRect countRect = opt.rect.adjusted(opt.rect.width() - 55, 2, -5, -2);
                
                QFont font = painter->font();
                font.setPointSize(10);
                painter->setFont(font);
                
                painter->setPen(opt.palette.text().color());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
                painter->setPen(QColor(0x6b, 0x72, 0x80));
                painter->drawText(countRect, Qt::AlignRight | Qt::AlignVCenter, count);
                
                painter->restore();
            }
        };
        activeReadersList->setItemDelegate(new ReadersDelegate());
    }
    
    // Setup table
    if (loanStatsTable) {
        loanStatsTable->setColumnWidth(0, 80);   // Ma phieu
        loanStatsTable->setColumnWidth(1, 200);  // Ten sach
        loanStatsTable->setColumnWidth(2, 150);  // Doc gia
        loanStatsTable->setColumnWidth(3, 100);  // Ngay muon
        loanStatsTable->setColumnWidth(4, 100);  // Ngay tra
        loanStatsTable->setColumnWidth(5, 80);   // Tien
        loanStatsTable->setColumnWidth(6, 120);  // Trang thai
    }
    
    // Setup charts
    if (topBooksChart) {
        topBooksChart->setMode(StatsChart::Mode::Bar);
        topBooksChart->setTitle(QString());
    }
    
    // Connect filter button
    if (applyFilterButton) {
        connect(applyFilterButton, &QPushButton::clicked, this, &MainWindow::applyStatsFilter);
    }
}

void MainWindow::configureSettingsTab() {
    if (maxBorrowDaysSpin) {
        maxBorrowDaysSpin->setRange(1, 90);
    }
    if (finePerDaySpin) {
        finePerDaySpin->setRange(0, 100000);
    }
    if (maxBooksPerReaderSpin) {
        maxBooksPerReaderSpin->setRange(1, 50);
    }
    if (ui->saveConfigButton) {
        connect(ui->saveConfigButton, &QPushButton::clicked, this, &MainWindow::handleSaveConfig);
    }
}

void MainWindow::reloadData() {
    booksCache = toQVector(bookService.fetchAll());
    applyBookFilter();

    readersCache = toQVector(readerService.fetchAll());
    applyReaderFilter();

    if (adminRole) {
        staffsCache = toQVector(staffService.fetchAll());
        applyStaffFilter();
    } else {
        staffsCache.clear();
    }

    loansCache = toQVector(loanService.fetchAll());
    applyLoanFilter();

    reportsCache = toQVector(reportService.fetchAll());
    applyReportFilter();
    
    // Update stats tab
    applyStatsFilter();

    if (adminRole) {
        accountsCache = toQVector(accountService.fetchAll());
        refreshAccounts();
    } else {
        accountsCache.clear();
        if (accountsList) accountsList->clear();
    }

    currentConfig = configService.load();
    if (maxBorrowDaysSpin) refreshConfigInputs();

    const int books = booksCache.size();
    const int readers = readersCache.size();
    const int loans = loansCache.size();

    if (statsLabel) {
        statsLabel->setText(tr("Sach: %1 | Ban doc: %2 | Phieu muon: %3 | Gioi han sach/ban doc: %4 | Tien phat/ngay: %5 VND")
                                .arg(books)
                                .arg(readers)
                                .arg(loans)
                                .arg(currentConfig.getMaxBooksPerReader())
                                .arg(currentConfig.getFinePerDay()));
    }
    updateStatisticsSummary();
}

void MainWindow::populateBooks() {
    booksCache = toQVector(bookService.fetchAll());
    applyBookFilter();
}

void MainWindow::fillBooksList(const QVector<model::Book> &books) const {
    if (!booksList) return;

    const QListWidgetItem *currentItem = booksList->currentItem();
    QString selectedId;
    if (currentItem) {
        selectedId = currentItem->data(kCardRoleId).toString();
        if (selectedId.isEmpty()) {
            selectedId = currentItem->data(Qt::UserRole).toString();
        }
    }

    booksList->setUpdatesEnabled(false);
    booksList->clear();

    const QFontMetrics metrics(booksList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < books.size(); ++row) {
        const auto &book = books[row];
        const QString id = bridge::toQString(book.getId());
        const QString title = bridge::toQString(book.getTitle());
        const QString author = bridge::toQString(book.getAuthor());
        const QString genre = bridge::toQString(book.getGenre());
        const QString isbn = bridge::toQString(book.getIsbn());
        const QDate publishDate = bridge::toQDate(book.getPublishDate());
        const QString dateText = publishDate.isValid() ? publishDate.toString(Qt::ISODate) : tr("Khong ro");
        const QString status = bookStatusText(book.getStatus());
        const QString statusCode = bridge::toQString(model::canonicalBookStatus(book.getStatus()));

        const QString headerLine = tr("%1 - %2").arg(id, title);
        const QString metaLine = tr("Tac gia: %1 | The loai: %2").arg(author, genre);
        const QString detailLine = tr("ISBN: %1 | Phat hanh: %2 | Nam: %3")
                                     .arg(isbn.isEmpty() ? tr("Khong ro") : isbn)
                                     .arg(dateText)
                                     .arg(book.getPublishYear());

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, tr("Tinh trang: %1").arg(status));
        item->setData(kCardRoleBadgeText, status);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, tr("So luong: %1").arg(book.getQuantity()));
        item->setData(kCardRolePillColor, QVariant());
        item->setToolTip(QStringList{headerLine, metaLine, detailLine, tr("Tinh trang: %1").arg(status), tr("So luong: %1").arg(book.getQuantity())}.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        booksList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == id) {
            restoreRow = row;
        }
    }

    booksList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        booksList->setCurrentRow(restoreRow);
        booksList->scrollToItem(booksList->item(restoreRow));
    } else {
        // Do not auto-select the first item after refresh — preserve no-selection state
        booksList->clearSelection();
    }
}

void MainWindow::applyBookFilter() {
    QVector<model::Book> filtered;
    filtered.reserve(booksCache.size());
    const QString term = bookSearchEdit ? bookSearchEdit->text().trimmed().toLower() : QString();
    const QString statusFilter = bookStatusFilter ? bookStatusFilter->currentData().toString() : QStringLiteral("ALL");
    for (const auto &book : booksCache) {
        if (!term.isEmpty()) {
            const QString haystack = QStringList{
                                             bridge::toQString(book.getId()),
                                             bridge::toQString(book.getTitle()),
                                             bridge::toQString(book.getAuthor()),
                                             bridge::toQString(book.getGenre()),
                                             bridge::toQString(book.getIsbn())}
                                         .join(' ')
                                         .toLower();
            if (!haystack.contains(term)) continue;
        }
        const QString bookStatus = bridge::toQString(model::canonicalBookStatus(book.getStatus()));
        if (bookStatus == QStringLiteral("MAT") || bookStatus == QStringLiteral("HONG")) {
            continue;
        }
        if (statusFilter != QStringLiteral("ALL") && bookStatus != statusFilter) {
            continue;
        }
        filtered.push_back(book);
    }
    fillBooksList(filtered);
}

void MainWindow::populateReaders() {
    readersCache = toQVector(readerService.fetchAll());
    applyReaderFilter();
}

void MainWindow::fillReadersList(const QVector<model::Reader> &readers) const {
    if (!readersList) return;

    const QListWidgetItem *currentItem = readersList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    readersList->setUpdatesEnabled(false);
    readersList->clear();

    const QFontMetrics metrics(readersList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < readers.size(); ++row) {
    const auto &r = readers[row];
    const QString id = bridge::toQString(r.getId());
    const QString name = bridge::toQString(r.getFullName());
    const QString gender = bridge::toQString(r.getGender());
    const QString address = bridge::toQString(r.getAddress());
    const QString phone = bridge::toQString(r.getPhone());
    const QString email = bridge::toQString(r.getEmail());
    const QString identityCard = bridge::toQString(r.getIdentityCard());
    const QString notes = bridge::toQString(r.getNotes());
    const QString dob = r.getDob().isValid() ? bridge::toQDate(r.getDob()).toString(Qt::ISODate) : tr("Khong ro");
    const QString created = r.getCreatedDate().isValid() ? bridge::toQDate(r.getCreatedDate()).toString(Qt::ISODate) : tr("Khong ro");
    const QString expiry = r.getExpiryDate().isValid() ? bridge::toQDate(r.getExpiryDate()).toString(Qt::ISODate) : tr("Khong ro");
        const int totalBorrowed = r.getTotalBorrowed();
        const bool active = r.isActive();
        const QString statusText = active ? tr("Dang hoat dong") : tr("Tam khoa");
        const QString statusCode = active ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");

        const QString headerLine = tr("%1 - %2").arg(id, name);
        const QString metaLine = tr("SDT: %1 | Email: %2").arg(phone.isEmpty() ? tr("Khong ro") : phone)
                                                          .arg(email.isEmpty() ? tr("Khong ro") : email);
        const QString detailLine = tr("Dia chi: %1 | Gioi tinh: %2").arg(address.isEmpty() ? tr("Khong ro") : address)
                                                                   .arg(gender.isEmpty() ? tr("Khong ro") : gender);
        const QString secondaryLine = tr("Dang ky: %1 | Het han: %2 | Muon: %3 lan")
                                           .arg(created)
                                           .arg(expiry)
                                           .arg(totalBorrowed);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, secondaryLine);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, identityCard.isEmpty() ? QString() : tr("CCCD %1").arg(identityCard));
        item->setData(kCardRolePillColor, QVariant());
        QStringList tooltipLines{headerLine, metaLine, detailLine, secondaryLine, tr("Trang thai: %1").arg(statusText)};
        if (!notes.trimmed().isEmpty()) {
            tooltipLines.append(tr("Ghi chu: %1").arg(notes));
        }
        item->setToolTip(tooltipLines.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        readersList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == id) {
            restoreRow = row;
        }
    }

    readersList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        readersList->setCurrentRow(restoreRow);
        readersList->scrollToItem(readersList->item(restoreRow));
    } else {
        readersList->clearSelection();
    }
}

void MainWindow::applyReaderFilter() {
    QVector<model::Reader> filtered;
    filtered.reserve(readersCache.size());
    const QString term = readerSearchEdit ? readerSearchEdit->text().trimmed().toLower() : QString();
    const QString statusSelection = readerStatusFilter ? readerStatusFilter->currentData().toString() : QStringLiteral("ALL");
    const QString statusFilter = statusSelection.isEmpty() ? QStringLiteral("ALL") : statusSelection;
    for (const auto &reader : readersCache) {
        if (!term.isEmpty()) {
        const QString haystack = QStringList{bridge::toQString(reader.getId()),
                         bridge::toQString(reader.getFullName()),
                         bridge::toQString(reader.getGender()),
                         bridge::toQString(reader.getPhone()),
                         bridge::toQString(reader.getEmail()),
                         bridge::toQString(reader.getAddress()),
                         bridge::toQString(reader.getIdentityCard()),
                         bridge::toQString(reader.getNotes()),
                         reader.getCreatedDate().isValid() ? bridge::toQDate(reader.getCreatedDate()).toString(Qt::ISODate) : QString(),
                         reader.getExpiryDate().isValid() ? bridge::toQDate(reader.getExpiryDate()).toString(Qt::ISODate) : QString(),
                         QString::number(reader.getTotalBorrowed())}
                     .join(' ')
                     .toLower();
            if (!haystack.contains(term)) continue;
        }
        if (statusFilter == QStringLiteral("ACTIVE") && !reader.isActive()) {
            continue;
        }
        if (statusFilter == QStringLiteral("INACTIVE") && reader.isActive()) {
            continue;
        }
        filtered.push_back(reader);
    }
    fillReadersList(filtered);
}

void MainWindow::populateLoans() {
    loansCache = toQVector(loanService.fetchAll());
    applyLoanFilter();
}

void MainWindow::fillLoansList(const QVector<model::Loan> &loans) {
    if (!loansList) return;

    const QListWidgetItem *currentItem = loansList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    loansList->setUpdatesEnabled(false);
    loansList->clear();

    const QFontMetrics metrics(loansList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    auto readerDisplay = [&](const QString &readerId) -> QString {
        QString name;
        for (const auto &reader : readersCache) {
            const QString readerIdText = bridge::toQString(reader.getId());
            if (readerIdText.compare(readerId, Qt::CaseInsensitive) == 0) {
                name = bridge::toQString(reader.getFullName());
                break;
            }
        }
        if (name.isEmpty()) return readerId;
        return tr("%1 (%2)").arg(name, readerId);
    };

    auto bookDisplay = [&](const QString &bookId) -> QString {
        QString title;
        for (const auto &book : booksCache) {
            if (bridge::toQString(book.getId()).compare(bookId, Qt::CaseInsensitive) == 0) {
                title = bridge::toQString(book.getTitle());
                break;
            }
        }
        if (title.isEmpty()) return bookId;
        return tr("%1 (%2)").arg(title, bookId);
    };

    int restoreRow = -1;
    for (int row = 0; row < loans.size(); ++row) {
        const auto &loan = loans[row];
        const QString loanId = bridge::toQString(loan.getLoanId());
        const QString readerId = bridge::toQString(loan.getReaderId());
        const QString bookId = bridge::toQString(loan.getBookId());
        const QString statusCode = normalizedStatus(bridge::toQString(loan.getStatus()));
        const QString statusText = loanStatusText(bridge::toQString(loan.getStatus()));

        const QString borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()).toString(Qt::ISODate) : tr("Khong ro");
        const QString dueDate = loan.getDueDate().isValid() ? bridge::toQDate(loan.getDueDate()).toString(Qt::ISODate) : tr("Khong ro");
        const QString returnDate = loan.getReturnDate().isValid() ? bridge::toQDate(loan.getReturnDate()).toString(Qt::ISODate) : tr("Chua tra");

        const QString headerLine = tr("%1 - %2").arg(loanId, bookDisplay(bookId));
        const QString metaLine = tr("Ban doc: %1").arg(readerDisplay(readerId));
        const QString detailLine = tr("Muon: %1 | Han: %2").arg(borrowDate, dueDate);
        QString extraDetail;
        if (statusCode == QStringLiteral("OVERDUE")) {
            const QDate due = loan.getDueDate().isValid() ? bridge::toQDate(loan.getDueDate()) : QDate();
            if (due.isValid()) {
                const int daysLate = std::max(0, static_cast<int>(due.daysTo(QDate::currentDate())));
                extraDetail = daysLate > 0
                                  ? tr("Qua han: %1 ngay").arg(daysLate)
                                  : tr("Qua han");
            } else {
                extraDetail = tr("Qua han");
            }
        } else {
            extraDetail = tr("Tra: %1").arg(returnDate);
        }

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, loanId);
        item->setData(kCardRoleId, loanId);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, extraDetail);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, tr("Tien phat: %1 VND").arg(loan.getFine()));
        item->setData(kCardRolePillColor, QVariant());
        item->setToolTip(QStringList{headerLine, metaLine, detailLine, extraDetail, tr("Trang thai: %1").arg(statusText)}.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        loansList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == loanId) {
            restoreRow = row;
        }
    }

    loansList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        loansList->setCurrentRow(restoreRow);
        loansList->scrollToItem(loansList->item(restoreRow));
    } else {
        loansList->clearSelection();
    }
}

void MainWindow::applyLoanFilter() {
    QVector<model::Loan> filtered;
    filtered.reserve(loansCache.size());
    const QString term = loanSearchEdit ? loanSearchEdit->text().trimmed().toLower() : QString();
    const QString statusFilter = loanStatusFilter ? loanStatusFilter->currentData().toString() : QStringLiteral("ALL");
    for (const auto &loan : loansCache) {
        const QString loanId = bridge::toQString(loan.getLoanId());
        const QString readerId = bridge::toQString(loan.getReaderId());
        const QString bookId = bridge::toQString(loan.getBookId());
        const QString statusCode = normalizedStatus(bridge::toQString(loan.getStatus()));

        if (!term.isEmpty()) {
            const QString haystack = QStringList{loanId, readerId, bookId}
                                         .join(' ')
                                         .toLower();
            if (!haystack.contains(term)) continue;
        }
        if (statusFilter != QStringLiteral("ALL") && statusCode != statusFilter) {
            continue;
        }
        filtered.push_back(loan);
    }
    fillLoansList(filtered);
}

void MainWindow::populateReports() {
    reportsCache = toQVector(reportService.fetchAll());
    applyReportFilter();
}

void MainWindow::fillReportsList(const QVector<model::ReportRequest> &reports) const {
    if (!reportsList) return;

    const QListWidgetItem *currentItem = reportsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    reportsList->setUpdatesEnabled(false);
    reportsList->clear();

    const QFontMetrics metrics(reportsList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < reports.size(); ++row) {
        const auto &report = reports[row];
        const QString requestId = bridge::toQString(report.getRequestId());
        const QString staff = bridge::toQString(report.getStaffUsername());
        const QString fromDate = report.getFromDate().isValid() ? bridge::toQDate(report.getFromDate()).toString(Qt::ISODate) : tr("Khong ro");
        const QString toDate = report.getToDate().isValid() ? bridge::toQDate(report.getToDate()).toString(Qt::ISODate) : tr("Khong ro");
        const QString createdAt = report.getCreatedAt().isValid() ? bridge::toQDateTime(report.getCreatedAt()).toString(Qt::ISODate) : tr("Khong ro");
        const QString statusCode = normalizedStatus(bridge::toQString(report.getStatus()));
        const QString statusText = reportStatusText(bridge::toQString(report.getStatus()));
        const QString notes = bridge::toQString(report.getNotes()).trimmed();

        const QString headerLine = tr("%1 - %2").arg(requestId, staff.isEmpty() ? tr("Nhan vien khong ro") : staff);
        const QString metaLine = tr("Tu: %1 | Den: %2").arg(fromDate, toDate);
        const QString detailLine = tr("Xu ly: %1 | Mat/Hu: %2 | Qua han: %3")
                                      .arg(report.getHandledLoans())
                                      .arg(report.getLostOrDamaged())
                                      .arg(report.getOverdueReaders());
        const QString extraDetail = tr("Tao: %1").arg(createdAt);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, requestId);
        item->setData(kCardRoleId, requestId);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, extraDetail);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, tr("Ngay tao: %1").arg(createdAt));
        item->setData(kCardRolePillColor, QVariant());
        QStringList tooltipLines{headerLine, metaLine, detailLine, tr("Trang thai: %1").arg(statusText), extraDetail};
        if (!notes.isEmpty()) {
            tooltipLines.append(tr("Ghi chu: %1").arg(notes));
        }
        item->setToolTip(tooltipLines.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        reportsList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == requestId) {
            restoreRow = row;
        }
    }

    reportsList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        reportsList->setCurrentRow(restoreRow);
        reportsList->scrollToItem(reportsList->item(restoreRow));
    } else {
        reportsList->clearSelection();
    }
}

void MainWindow::fillAccountsList(const QVector<model::Account> &accounts) {
    if (!accountsList) return;

    const QListWidgetItem *currentItem = accountsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    accountsList->setUpdatesEnabled(false);
    accountsList->clear();

    const QFontMetrics metrics(accountsList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    auto staffSummary = [&](const QString &staffId) -> QString {
        if (staffId.isEmpty()) return tr("Chua gan nhan vien");
        QString name;
        for (const auto &staff : staffsCache) {
            const QString staffCode = bridge::toQString(staff.getId());
            if (staffCode.compare(staffId, Qt::CaseInsensitive) == 0) {
                name = bridge::toQString(staff.getFullName());
                break;
            }
        }
        if (name.isEmpty()) return staffId;
        return tr("%1 - %2").arg(staffId, name);
    };

    int restoreRow = -1;
    for (int row = 0; row < accounts.size(); ++row) {
        const auto &acc = accounts[row];
        const QString username = bridge::toQString(acc.getUsername());
        const QString accountId = bridge::toQString(acc.getAccountId());
        const QString role = bridge::toQString(acc.getRole());
        const QString staffId = bridge::toQString(acc.getEmployeeId()).trimmed();
        const QString staffDisplay = staffSummary(staffId);
        const QString createdAt = acc.getCreatedAt().isValid()
                                      ? bridge::toQDateTime(acc.getCreatedAt()).toString(Qt::ISODate)
                                      : tr("Khong ro");
        const QString lastLogin = acc.getLastLogin().isValid()
                                      ? bridge::toQDateTime(acc.getLastLogin()).toString(Qt::ISODate)
                                      : tr("Chua dang nhap");
        const QString headerLine = username;
        const QString metaLine = tr("Vai tro: %1 | Ma tai khoan: %2")
                                      .arg(role.isEmpty() ? tr("Khong ro") : role)
                                      .arg(accountId.isEmpty() ? tr("Khong ro") : accountId);
        const QString detailLine = tr("Nhan vien lien ket: %1").arg(staffDisplay);
        const QString extraDetail = tr("Tao: %1 | Dang nhap cuoi: %2").arg(createdAt, lastLogin);

        const QString statusCode = acc.isActive() ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");
        const QString badgeText = accountActiveText(acc.isActive());

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, username);
        item->setData(kCardRoleId, username);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, extraDetail);
        item->setData(kCardRoleBadgeText, badgeText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, staffId.isEmpty() ? QString() : tr("MSNV %1").arg(staffId));
        item->setData(kCardRolePillColor, QVariant());
        const QString passwordHash = bridge::toQString(acc.getPasswordHash());
        item->setToolTip(QStringList{headerLine,
                                     metaLine,
                                     detailLine,
                                     tr("Trang thai: %1").arg(badgeText),
                                     extraDetail,
                                     tr("Hash mat khau: %1").arg(passwordHash.isEmpty()
                                                                     ? tr("(khong ro)")
                                                                     : passwordHash)}
                             .join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        accountsList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == username) {
            restoreRow = row;
        }
    }

    accountsList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        accountsList->setCurrentRow(restoreRow);
        accountsList->scrollToItem(accountsList->item(restoreRow));
    } else {
        accountsList->clearSelection();
    }
}

void MainWindow::applyReportFilter() {
    QVector<model::ReportRequest> filtered;
    filtered.reserve(reportsCache.size());
    const QString staffTerm = reportStaffFilter ? reportStaffFilter->text().trimmed().toLower() : QString();
    const bool hasFrom = reportFromFilter && reportFromFilter->date() != reportFromFilter->minimumDate();
    const bool hasTo = reportToFilter && reportToFilter->date() != reportToFilter->minimumDate();
    const QDate from = hasFrom ? reportFromFilter->date() : QDate();
    const QDate to = hasTo ? reportToFilter->date() : QDate();

    for (const auto &req : reportsCache) {
        const QString staffUsername = bridge::toQString(req.getStaffUsername());
        const bool fromValid = req.getFromDate().isValid();
        const bool toValid = req.getToDate().isValid();
        const QDate reqFrom = fromValid ? bridge::toQDate(req.getFromDate()) : QDate();
        const QDate reqTo = toValid ? bridge::toQDate(req.getToDate()) : QDate();

        if (!staffTerm.isEmpty() && !staffUsername.trimmed().toLower().contains(staffTerm)) continue;
        if (hasFrom && (!fromValid || reqFrom < from)) continue;
        if (hasTo && (!toValid || reqTo > to)) continue;
        filtered.push_back(req);
    }
    fillReportsList(filtered);
}

void MainWindow::clearReportFilter() {
    if (reportStaffFilter) reportStaffFilter->clear();
    if (reportFromFilter) reportFromFilter->setDate(reportFromFilter->minimumDate());
    if (reportToFilter) reportToFilter->setDate(reportToFilter->minimumDate());
    applyReportFilter();
}

void MainWindow::handleViewReportDetails() {
    if (!reportsList) return;
    const auto rowOpt = currentRow(reportsList);
    if (!rowOpt.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot bao cao."));
        return;
    }
    const QListWidgetItem *item = reportsList->item(rowOpt.value());
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh bao cao da chon."));
        return;
    }
    QString requestId = item->data(kCardRoleId).toString();
    if (requestId.isEmpty()) {
        requestId = item->data(Qt::UserRole).toString();
    }
    if (requestId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh bao cao da chon."));
        return;
    }

    const model::ReportRequest *selectedReport = nullptr;
    for (int i = 0; i < reportsCache.size(); ++i) {
        const auto &report = reportsCache[i];
        if (bridge::toQString(report.getRequestId()).compare(requestId, Qt::CaseInsensitive) == 0) {
            selectedReport = &reportsCache[i];
            break;
        }
    }
    if (!selectedReport) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay bao cao da chon."));
        return;
    }

    const QString statusText = reportStatusText(bridge::toQString(selectedReport->getStatus()));
    ReportDetailsDialog dialog(*selectedReport, statusText, this);
    dialog.exec();
}

void MainWindow::updateStatisticsSummary() { refreshSimpleStats(); }

void MainWindow::refreshSimpleStats() {
    updateStatsCards();
    updateStatsCharts();
    applyStatsFilter();
}

void MainWindow::updateStatsCards() {
    const QLocale locale;
    const QDate today = QDate::currentDate();
    const QDate monthStart = QDate(today.year(), today.month(), 1);

    const int totalBooks = booksCache.size();
    const int totalReaders = readersCache.size();
    
    // Filter loans by date range
    int totalLoans = 0;
    int overdueLoans = 0;
    qint64 totalFines = 0;
    qint64 monthlyFines = 0;

    for (const auto &loan : loansCache) {
        const QDate borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()) : QDate();
        
        // Skip loans outside the selected date range
        if (!borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
            continue;
        }
        
        totalLoans++;
        
        const bool hasReturn = loan.getReturnDate().isValid();
        const QDate dueDate = loan.getDueDate().isValid() ? bridge::toQDate(loan.getDueDate()) : QDate();

        if (dueDate.isValid() && !hasReturn && today > dueDate) {
            overdueLoans++;
        }

        int fine = std::max(0, loan.getFine());
        totalFines += fine;
        
        if (borrowDate.isValid() && borrowDate >= monthStart) {
            monthlyFines += fine;
        }
    }

    // Format với dấu phẩy cho số thường, X,XXX.000đ cho tiền
    QLocale viLocale(QLocale::Vietnamese);
    
    if (totalBooksValue) {
        totalBooksValue->setText(viLocale.toString(totalBooks));
    }
    if (totalReadersValue) {
        totalReadersValue->setText(viLocale.toString(totalReaders));
    }
    if (totalLoansValue) {
        totalLoansValue->setText(viLocale.toString(totalLoans));
    }
    if (totalFinesValue) {
        QString finesText = viLocale.toString(totalFines);
        if (totalFines > 0) {
            finesText += QStringLiteral(".000");
        }
        finesText += QString::fromUtf8("đ");
        totalFinesValue->setText(finesText);
    }
    if (overdueCount) {
        overdueCount->setText(QString::number(overdueLoans) + QStringLiteral(" sach dang tre"));
    }
    if (monthlyFinesValue) {
        QString monthlyText = viLocale.toString(monthlyFines);
        if (monthlyFines > 0) {
            monthlyText += QStringLiteral(".000");
        }
        monthlyText += QString::fromUtf8("đ");
        monthlyFinesValue->setText(monthlyText);
    }
}

void MainWindow::updateStatsCharts() {
    // Update top books chart
    if (topBooksChart) {
        QMap<QString, int> bookBorrowCounts;
        for (const auto &loan : loansCache) {
            const QDate borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()) : QDate();
            
            // Skip loans outside the selected date range
            if (!borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
                continue;
            }
            
            QString bookId = bridge::toQString(loan.getBookId());
            QString bookTitle = bookId;
            
            for (const auto &book : booksCache) {
                if (bridge::toQString(book.getId()) == bookId) {
                    bookTitle = bridge::toQString(book.getTitle());
                    if (bookTitle.length() > 20) {
                        bookTitle = bookTitle.left(17) + "...";
                    }
                    break;
                }
            }
            bookBorrowCounts[bookTitle]++;
        }
        
        // Get top 5
        QList<QPair<QString, int>> items;
        for (auto it = bookBorrowCounts.constBegin(); it != bookBorrowCounts.constEnd(); ++it) {
            items.append(qMakePair(it.key(), it.value()));
        }
        std::sort(items.begin(), items.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
            return a.second > b.second;
        });
        
        QStringList top5Labels;
        QVector<double> top5Values;
        int limit = std::min(5, static_cast<int>(items.size()));
        for (int i = 0; i < limit; ++i) {
            top5Labels.append(items[i].first);
            top5Values.append(items[i].second);
        }
        
        topBooksChart->setCategories(top5Labels);
        StatsChart::Series series;
        series.name = QStringLiteral("So luot muon");
        series.values = top5Values;
        series.color = QColor(0x1d, 0x4e, 0xd8);
        topBooksChart->setSeries({series});
        
        // Populate top books list (bên phải)
        if (topBooksList) {
            topBooksList->clear();
            for (int i = 0; i < limit; ++i) {
                QListWidgetItem *item = new QListWidgetItem(items[i].first);
                item->setData(Qt::UserRole + 1, QString::number(items[i].second));
                topBooksList->addItem(item);
            }
        }
    }
    
    // Update revenue bar chart (show monthly revenue breakdown)
    if (revenueChart) {
        qint64 totalFines = 0;
        for (const auto &loan : loansCache) {
            const QDate borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()) : QDate();
            
            // Skip loans outside the selected date range
            if (!borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
                continue;
            }
            
            totalFines += std::max(0, loan.getFine());
        }
        
        int cardFees = static_cast<int>(totalFines * 0.7 / 1000);  // Convert to thousands
        int fines = static_cast<int>(totalFines * 0.3 / 1000);
        
        QStringList categories;
        categories << QStringLiteral("Lam the") << QStringLiteral("Tien phat");
        
        StatsChart::Series series;
        series.name = QStringLiteral("Doanh thu (nghin dong)");
        series.values = {static_cast<double>(cardFees), static_cast<double>(fines)};
        series.color = QColor(59, 130, 246);  // Blue
        
        revenueChart->setCategories(categories);
        revenueChart->setSeries({series});
        revenueChart->setValueSuffix(QStringLiteral("k"));
    }
    
    // Update active readers list
    if (activeReadersList) {
        activeReadersList->clear();
        
        // Count total borrows per reader
        QMap<QString, int> readerBorrowCounts;
        for (const auto &loan : loansCache) {
            const QDate borrowDate = loan.getBorrowDate().isValid() ? bridge::toQDate(loan.getBorrowDate()) : QDate();
            
            // Skip loans outside the selected date range
            if (!borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
                continue;
            }
            
            QString readerId = bridge::toQString(loan.getReaderId());
            readerBorrowCounts[readerId]++;
        }
        
        // Get top readers
        QList<QPair<QString, int>> readerItems;
        for (const auto &reader : readersCache) {
            if (!reader.isActive()) continue;
            QString readerId = bridge::toQString(reader.getId());
            int borrowCount = readerBorrowCounts.value(readerId, 0);
            if (borrowCount > 0) {
                QString readerName = bridge::toQString(reader.getFullName());
                readerItems.append(qMakePair(readerName, borrowCount));
            }
        }
        
        std::sort(readerItems.begin(), readerItems.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
            return a.second > b.second;
        });
        
        int limit = std::min(5, static_cast<int>(readerItems.size()));
        for (int i = 0; i < limit; ++i) {
            QString readerName = readerItems[i].first;
            int borrowCount = readerItems[i].second;
            
            // Kiểm tra trạng thái trễ hạn
            bool hasOverdue = false;
            QString readerId;
            for (const auto &reader : readersCache) {
                if (bridge::toQString(reader.getFullName()) == readerName) {
                    readerId = bridge::toQString(reader.getId());
                    break;
                }
            }
            
            if (!readerId.isEmpty()) {
                const QDate today = QDate::currentDate();
                for (const auto &loan : loansCache) {
                    if (bridge::toQString(loan.getReaderId()) == readerId) {
                        const QDate dueDate = loan.getDueDate().isValid() ? bridge::toQDate(loan.getDueDate()) : QDate();
                        if (dueDate.isValid() && !loan.getReturnDate().isValid() && today > dueDate) {
                            hasOverdue = true;
                            break;
                        }
                    }
                }
            }
            
            // Format: ● Tên (icon trễ hạn nếu có)      số
            QString icon = hasOverdue ? QString::fromUtf8("⚠") : QString::fromUtf8("●");
            QString displayText = icon + QString(" ") + readerName;
            if (hasOverdue) {
                displayText += QString("\n   Tre han 1 lan");
            }
            
            QListWidgetItem *item = new QListWidgetItem(displayText);
            item->setData(Qt::UserRole + 1, QString::number(borrowCount));
            activeReadersList->addItem(item);
        }
    }
}

void MainWindow::applyStatsFilter() {
    const QDate today = QDate::currentDate();
    
    // Get selected time period
    QString timePeriod = timePeriodCombo ? timePeriodCombo->currentText() : QStringLiteral("Tuan nay");
    
    // Calculate date range
    QDate startDate;
    QDate endDate = today;
    
    if (timePeriod == QStringLiteral("Tuan nay")) {
        startDate = today.addDays(-(today.dayOfWeek() - 1));
    } else if (timePeriod == QStringLiteral("Thang nay")) {
        startDate = QDate(today.year(), today.month(), 1);
    } else if (timePeriod == QStringLiteral("Nam nay")) {
        startDate = QDate(today.year(), 1, 1);
    } else { // Tat ca
        startDate = QDate(2000, 1, 1);
    }
    
    // Store date range for filtering
    statsStartDate = startDate;
    statsEndDate = endDate;
    
    // Refresh all stats with new date range
    updateStatsCards();
    updateStatsCharts();
}

void MainWindow::refreshAccounts() {
    if (!accountsList) return;
    fillAccountsList(accountsCache);
}

void MainWindow::refreshConfigInputs() const {
    if (!maxBorrowDaysSpin || !finePerDaySpin || !maxBooksPerReaderSpin) return;
    maxBorrowDaysSpin->setValue(std::max(1, currentConfig.getMaxBorrowDays()));
    finePerDaySpin->setValue(std::max(0, currentConfig.getFinePerDay()));
    maxBooksPerReaderSpin->setValue(std::max(1, currentConfig.getMaxBooksPerReader()));
}

custom::Optional<int> MainWindow::currentRow(const QListWidget *list) {
    if (!list) return custom::nullopt;
    const int row = list->currentRow();
    if (row < 0) return custom::nullopt;
    return custom::Optional<int>(row);
}

QString MainWindow::nextBookId() const {
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(booksCache.size());
    for (const auto &book : booksCache) {
        ids.pushBack(book.getId());
    }
    return bridge::toQString(core::IdGenerator::nextId(ids, custom::CustomStringLiteral("B"), 3));
}

QString MainWindow::nextReaderId() const {
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(readersCache.size());
    for (const auto &reader : readersCache) {
        ids.pushBack(reader.getId());
    }
    return bridge::toQString(core::IdGenerator::nextId(ids, custom::CustomStringLiteral("R"), 3));
}

QString MainWindow::nextStaffId() const {
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(staffsCache.size());
    for (const auto &s : staffsCache) {
        ids.pushBack(s.getId());
    }
    return bridge::toQString(core::IdGenerator::nextId(ids, custom::CustomStringLiteral("S"), 3));
}

QString MainWindow::nextLoanId() const {
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(loansCache.size());
    for (const auto &loan : loansCache) {
        ids.pushBack(loan.getLoanId());
    }
    return bridge::toQString(core::IdGenerator::nextId(ids, custom::CustomStringLiteral("L"), 3));
}

void MainWindow::handleAddBook() {
    if (!adminRole) return;
    BookDialog dialog(this);
    dialog.presetId(nextBookId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto newBook = dialog.book();
    if (const auto exists = std::any_of(booksCache.cbegin(), booksCache.cend(), [&](const model::Book &b) { return b.getId() == newBook.getId(); })) {
        QMessageBox::warning(this, tr("Trung lap"), tr("Ma sach nay da ton tai."));
        return;
    }
    if (!bookService.addBook(newBook)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the them sach moi."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da them sach moi."), 2000);
}

void MainWindow::configureStaffsTab() {
    configureCardListWidget(staffsList);

    if (staffSearchEdit) {
        connect(staffSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyStaffFilter);
    }
    if (staffStatusFilter) {
        staffStatusFilter->clear();
        staffStatusFilter->addItem(tr("Tat ca trang thai"), QStringLiteral("ALL"));
        staffStatusFilter->addItem(tr("Dang lam viec"), QStringLiteral("ACTIVE"));
        staffStatusFilter->addItem(tr("Da nghi viec"), QStringLiteral("INACTIVE"));
        connect(staffStatusFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyStaffFilter);
    }
    if (ui->staffFilterButton) {
        connect(ui->staffFilterButton, &QPushButton::clicked, this, &MainWindow::applyStaffFilter);
    }

    if (ui->addStaffButton) {
        ui->addStaffButton->setVisible(adminRole);
        connect(ui->addStaffButton, &QPushButton::clicked, this, &MainWindow::handleAddStaff);
    }
    if (ui->editStaffButton) {
        ui->editStaffButton->setVisible(adminRole);
        connect(ui->editStaffButton, &QPushButton::clicked, this, &MainWindow::handleEditStaff);
    }
    if (ui->deleteStaffButton) {
        ui->deleteStaffButton->setVisible(adminRole);
        connect(ui->deleteStaffButton, &QPushButton::clicked, this, &MainWindow::handleDeleteStaff);
    }
    if (ui->toggleStaffStatusButton) {
        ui->toggleStaffStatusButton->setVisible(adminRole);
        connect(ui->toggleStaffStatusButton, &QPushButton::clicked, this, &MainWindow::handleToggleStaffActive);
    }
    if (ui->staffsActionsGroup) {
        ui->staffsActionsGroup->setVisible(adminRole);
    }
    // Use grid/card layout for staffs so entries appear as cards in multiple columns.
    if (staffsList) {
        staffsList->setFlow(QListView::LeftToRight);
        staffsList->setWrapping(true);
        staffsList->setViewMode(QListView::IconMode);
    }
}

void MainWindow::populateStaffs() {
    staffsCache = toQVector(staffService.fetchAll());
    applyStaffFilter();
}

void MainWindow::fillStaffsList(const QVector<model::Staff> &staffs) const {
    if (!staffsList) return;

    const QListWidgetItem *currentItem = staffsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    staffsList->setUpdatesEnabled(false);
    staffsList->clear();

    const QFontMetrics metrics(staffsList->font());
    const int minimumHeight = std::max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < staffs.size(); ++row) {
    const auto &s = staffs[row];
    const QString id = bridge::toQString(s.getId());
    const QString name = bridge::toQString(s.getFullName());
    const QString gender = bridge::toQString(s.getGender());
    const QString address = bridge::toQString(s.getAddress());
    const QString phone = bridge::toQString(s.getPhone());
    const QString email = bridge::toQString(s.getEmail());
    const QString position = bridge::toQString(s.getPosition());
    const QString notes = bridge::toQString(s.getNotes());
    const QString dob = s.getDob().isValid() ? bridge::toQDate(s.getDob()).toString(Qt::ISODate) : tr("Khong ro");
    const QString hireDate = s.getHireDate().isValid() ? bridge::toQDate(s.getHireDate()).toString(Qt::ISODate) : tr("Khong ro");
        const bool active = s.isActive();
        const QString statusText = active ? tr("Dang lam viec") : tr("Da nghi viec");
        const QString statusCode = active ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");

        const QString headerLine = tr("%1 - %2").arg(id, name);
        const QString metaLine = tr("SDT: %1 | Email: %2").arg(phone.isEmpty() ? tr("Khong ro") : phone)
                                                          .arg(email.isEmpty() ? tr("Khong ro") : email);
        const QString detailLine = tr("Dia chi: %1 | Gioi tinh: %2").arg(address.isEmpty() ? tr("Khong ro") : address)
                                                                    .arg(gender.isEmpty() ? tr("Khong ro") : gender);
        const QString secondaryLine = tr("Ngay vao: %1 | Sinh: %2")
                                          .arg(hireDate)
                                          .arg(dob);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, secondaryLine);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, position.isEmpty() ? QString() : tr("Chuc vu: %1").arg(position));
        item->setData(kCardRolePillColor, QVariant());
        QStringList tooltipLines{headerLine, metaLine, detailLine, secondaryLine, tr("Trang thai: %1").arg(statusText)};
        if (!notes.trimmed().isEmpty()) {
            tooltipLines.append(tr("Ghi chu: %1").arg(notes));
        }
        item->setToolTip(tooltipLines.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(std::max(hint.height(), minimumHeight));
        item->setSizeHint(hint);

        staffsList->addItem(item);

        if (restoreRow == -1 && !selectedId.isEmpty() && selectedId == id) {
            restoreRow = row;
        }
    }

    staffsList->setUpdatesEnabled(true);
    if (restoreRow >= 0) {
        staffsList->setCurrentRow(restoreRow);
        staffsList->scrollToItem(staffsList->item(restoreRow));
    } else {
        staffsList->clearSelection();
    }
}

void MainWindow::applyStaffFilter() {
    QVector<model::Staff> filtered;
    filtered.reserve(staffsCache.size());
    const QString term = staffSearchEdit ? staffSearchEdit->text().trimmed().toLower() : QString();
    const QString statusCode = staffStatusFilter ? staffStatusFilter->currentData().toString() : QStringLiteral("ALL");
    const QString statusFilter = statusCode.isEmpty() ? QStringLiteral("ALL") : statusCode;
    for (const auto &staff : staffsCache) {
        if (!term.isEmpty()) {
        const QString haystack = QStringList{bridge::toQString(staff.getId()),
                         bridge::toQString(staff.getFullName()),
                         bridge::toQString(staff.getGender()),
                         bridge::toQString(staff.getPhone()),
                         bridge::toQString(staff.getEmail()),
                         bridge::toQString(staff.getAddress()),
                         bridge::toQString(staff.getPosition()),
                         bridge::toQString(staff.getNotes()),
                         staff.getHireDate().isValid() ? bridge::toQDate(staff.getHireDate()).toString(Qt::ISODate) : QString(),
                         staff.getDob().isValid() ? bridge::toQDate(staff.getDob()).toString(Qt::ISODate) : QString()}
                     .join(' ')
                     .toLower();
            if (!haystack.contains(term)) continue;
        }
        if (statusFilter == QStringLiteral("ACTIVE") && !staff.isActive()) {
            continue;
        }
        if (statusFilter == QStringLiteral("INACTIVE") && staff.isActive()) {
            continue;
        }
        filtered.push_back(staff);
    }
    fillStaffsList(filtered);
}

void MainWindow::handleAddStaff() {
    if (!adminRole) return;
    StaffDialog dialog(this);
    dialog.presetId(nextStaffId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto staff = dialog.staff();
    if (const auto exists = std::any_of(staffsCache.cbegin(), staffsCache.cend(), [&](const model::Staff &s) { return s.getId() == staff.getId(); })) {
        QMessageBox::warning(this, tr("Trung lap"), tr("Ma nhan vien da ton tai."));
        return;
    }
    if (!staffService.addStaff(staff)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the them nhan vien."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da them nhan vien."), 2000);
}

void MainWindow::handleEditStaff() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot nhan vien."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    const auto staffKey = bridge::toCustomString(staffId);
    const auto staffOpt = staffService.findById(staffKey);
    if (!staffOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay nhan vien da chon."));
        return;
    }
    StaffDialog dialog(this);
    dialog.setStaff(*staffOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto updated = dialog.staff();
    if (!staffService.updateStaff(updated)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat nhan vien."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da cap nhat nhan vien."), 2000);
}

void MainWindow::handleDeleteStaff() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot nhan vien."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    const auto staffKey = bridge::toCustomString(staffId);
    // Check if any account is linked to this staff
    const bool linked = std::any_of(accountsCache.cbegin(), accountsCache.cend(), [&](const model::Account &acc) {
        return ::pbl2::bridge::toQString(acc.getEmployeeId()).compare(staffId, Qt::CaseInsensitive) == 0;
    });
    if (linked) {
        QMessageBox::warning(this, tr("Rang buoc"), tr("Khong the xoa: Nhan vien dang duoc gan voi mot tai khoan."));
        return;
    }
    const auto reply = QMessageBox::question(this, tr("Xac nhan"), tr("Ban co chac chan muon xoa nhan vien %1?").arg(staffId));
    if (reply != QMessageBox::Yes) return;
    if (!staffService.removeStaff(staffKey)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the xoa nhan vien."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da xoa nhan vien."), 2000);
}

void MainWindow::handleToggleStaffActive() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot nhan vien."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh nhan vien da chon."));
        return;
    }
    const auto staffKey = bridge::toCustomString(staffId);
    const auto staffOpt = staffService.findById(staffKey);
    if (!staffOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay nhan vien da chon."));
        return;
    }
    const bool newActive = !staffOpt->isActive();
    if (!staffService.setStaffActive(staffKey, newActive)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat trang thai nhan vien."));
        return;
    }
    reloadData();
    const QString message = newActive ? tr("Da danh dau nhan vien dang lam viec.")
                                      : tr("Da danh dau nhan vien da nghi viec.");
    statusBar()->showMessage(message, 2000);
}

void MainWindow::handleEditBook() {
    if (!adminRole) return;
    const auto row = currentRow(booksList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot cuon sach."));
        return;
    }
    const QListWidgetItem *item = booksList->item(row.value());
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh sach da chon."));
        return;
    }
    QString bookId = item->data(kCardRoleId).toString();
    if (bookId.isEmpty()) {
        bookId = item->data(Qt::UserRole).toString();
    }
    if (bookId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh sach da chon."));
        return;
    }
    const auto bookKey = bridge::toCustomString(bookId);
    const auto bookOpt = bookService.findById(bookKey);
    if (!bookOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay sach da chon."));
        return;
    }
    BookDialog dialog(this);
    dialog.setBook(*bookOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto updated = dialog.book();
    if (!bookService.updateBook(updated)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat thong tin sach."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da cap nhat thong tin sach."), 2000);
}

void MainWindow::handleDeleteBook() {
    if (!adminRole) return;
    const auto row = currentRow(booksList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot cuon sach."));
        return;
    }
    const QListWidgetItem *item = booksList->item(row.value());
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh sach da chon."));
        return;
    }
    QString bookId = item->data(kCardRoleId).toString();
    if (bookId.isEmpty()) {
        bookId = item->data(Qt::UserRole).toString();
    }
    if (bookId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh sach da chon."));
        return;
    }
    const auto bookKey = bridge::toCustomString(bookId);
    const auto reply = QMessageBox::question(this, tr("Xac nhan"), tr("Ban co chac chan muon xoa sach %1?").arg(bookId));
    if (reply != QMessageBox::Yes) return;
    if (!bookService.removeBook(bookKey)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the xoa sach."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da xoa sach."), 2000);
}

void MainWindow::handleAddReader() {
    if (!staffRole) return;
    ReaderDialog dialog(this);
    dialog.presetId(nextReaderId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto reader = dialog.reader();
    if (const auto exists = std::any_of(readersCache.cbegin(), readersCache.cend(), [&](const model::Reader &r) { return r.getId() == reader.getId(); })) {
        QMessageBox::warning(this, tr("Trung lap"), tr("Ma ban doc nay da ton tai."));
        return;
    }
    if (!readerService.addReader(reader)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the them ban doc."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da them ban doc."), 2000);
}

void MainWindow::handleEditReader() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot ban doc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    const auto readerKey = bridge::toCustomString(readerId);
    const auto readerOpt = readerService.findById(readerKey);
    if (!readerOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay ban doc da chon."));
        return;
    }
    ReaderDialog dialog(this);
    dialog.setReader(*readerOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto updated = dialog.reader();
    if (!readerService.updateReader(updated)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat ban doc."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da cap nhat ban doc."), 2000);
}

void MainWindow::handleDeleteReader() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot ban doc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    const auto readerKey = bridge::toCustomString(readerId);
    
    // Check if reader has active loans (BORROWED status)
    const bool hasActiveLoans = std::any_of(loansCache.cbegin(), loansCache.cend(), [&](const model::Loan &loan) {
        return bridge::toQString(loan.getReaderId()) == readerId &&
               normalizedStatus(loan.getStatus()) == QStringLiteral("BORROWED");
    });
    
    if (hasActiveLoans) {
        QMessageBox::warning(this, tr("Rang buoc"), 
            tr("Khong the xoa: Ban doc dang co phieu muon chua tra."));
        return;
    }
    
    const auto reply = QMessageBox::question(this, tr("Xac nhan"), 
        tr("Ban co chac chan muon xoa ban doc %1?").arg(readerId));
    if (reply != QMessageBox::Yes) return;
    
    if (!readerService.removeReader(readerKey)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the xoa ban doc."));
        return;
    }
    
    reloadData();
    statusBar()->showMessage(tr("Da xoa ban doc."), 2000);
}

void MainWindow::handleToggleReaderActive() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot ban doc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh ban doc da chon."));
        return;
    }
    const auto readerKey = bridge::toCustomString(readerId);
    const auto readerOpt = readerService.findById(readerKey);
    if (!readerOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay ban doc da chon."));
        return;
    }
    const bool newActive = !readerOpt->isActive();
    if (!readerService.setReaderActive(readerKey, newActive)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat trang thai ban doc."));
        return;
    }
    reloadData();
    const QString message = newActive ? tr("Da mo khoa ban doc.")
                                      : tr("Da tam khoa ban doc.");
    statusBar()->showMessage(message, 2000);
}

void MainWindow::handleNewLoan() {
    if (!staffRole) return;
    if (readersCache.isEmpty() || booksCache.isEmpty()) {
        QMessageBox::warning(this, tr("Thieu du lieu"), tr("Can co it nhat mot ban doc va mot cuon sach."));
        return;
    }
    QVector<model::Reader> availableReaders;
    availableReaders.reserve(readersCache.size());
    for (const auto &reader : readersCache) {
        if (reader.isActive()) availableReaders.append(reader);
    }
    if (availableReaders.isEmpty()) {
        QMessageBox::warning(this, tr("Khong kha dung"), tr("Khong co doc gia dang hoat dong de tao phieu muon."));
        return;
    }
    LoanDialog dialog(availableReaders, booksCache, currentConfig.getMaxBorrowDays(), this);
    dialog.presetLoanId(nextLoanId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    auto loan = dialog.loan();
    if (loanService.findById(loan.getLoanId()).has_value()) {
        QMessageBox::warning(this, tr("Trung lap"), tr("Ma phieu nay da ton tai."));
        return;
    }
    const auto bookOpt = bookService.findById(loan.getBookId());
    if (!bookOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay sach da chon."));
        return;
    }
    if (bookOpt->getQuantity() <= 0) {
        QMessageBox::warning(this, tr("Khong kha dung"), tr("Sach nay da het so luong cho muon."));
        return;
    }
    loan.setStatus(custom::CustomStringLiteral("BORROWED"));
    loan.setFine(0);
    if (!loanService.createLoan(loan)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the tao phieu muon."));
        return;
    }

    model::Book updatedBook = *bookOpt;
    updatedBook.setQuantity(std::max(0, updatedBook.getQuantity() - 1));
    if (!bookService.updateBook(updatedBook)) {
        loanService.removeLoan(loan.getLoanId());
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat so luong sach."));
        return;
    }

    reloadData();
    statusBar()->showMessage(tr("Da tao phieu muon."), 2000);
}

void MainWindow::handleMarkReturned() {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot phieu muon."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    const auto loanKey = bridge::toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay phieu muon."));
        return;
    }
    if (normalizedStatus(loanOpt->getStatus()) == QStringLiteral("RETURNED")) {
        QMessageBox::information(this, tr("Thong bao"), tr("Phieu nay da duoc dong."));
        return;
    }

    const QDate returnDate = QDate::currentDate();
    const auto dueDate = bridge::toQDate(loanOpt->getDueDate());
    const qint64 overdueSpan = dueDate.daysTo(returnDate);
    const int lateDays = overdueSpan > 0 ? static_cast<int>(overdueSpan) : 0;
    const int fine = lateDays * currentConfig.getFinePerDay();

    if (!loanService.updateStatus(loanKey, custom::CustomStringLiteral("RETURNED"), bridge::toCoreDate(returnDate))) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat trang thai phieu."));
        return;
    }
    if (!loanService.applyFine(loanKey, fine)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat tien phat."));
        return;
    }

    const auto bookOpt = bookService.findById(loanOpt->getBookId());
    if (bookOpt.has_value()) {
        model::Book updatedBook = *bookOpt;
        updatedBook.setQuantity(updatedBook.getQuantity() + 1);
        if (!bookService.updateBook(updatedBook)) {
            QMessageBox::warning(this, tr("Canh bao"), tr("Khong the cap nhat so luong sach."));
        }
    }

    reloadData();
    statusBar()->showMessage(tr("Da dong phieu. Tien phat: %1 VND").arg(fine), 3000);
}

void MainWindow::handleExtendLoan() {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot phieu muon."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    bool ok = false;
    const int days = QInputDialog::getInt(this, tr("Gia han phieu"), tr("So ngay bo sung"), 3, 1, 60, 1, &ok);
    if (!ok) return;
    const auto loanKey = bridge::toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay phieu muon."));
        return;
    }
    model::Loan updatedLoan = *loanOpt;
    const QDate adjustedDueDate = bridge::toQDate(updatedLoan.getDueDate()).addDays(days);
    updatedLoan.setDueDate(bridge::toCoreDate(adjustedDueDate));
    updatedLoan.setStatus(custom::CustomStringLiteral("BORROWED"));
    if (!loanService.updateLoan(updatedLoan)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the gia han phieu."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da gia han phieu."), 2000);
}

void MainWindow::handleMarkLost() {
    handleLossOrDamage(QStringLiteral("LOST"));
}

void MainWindow::handleMarkDamaged() {
    handleLossOrDamage(QStringLiteral("DAMAGED"));
}

void MainWindow::handleDeleteLoan() {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot phieu muon."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }

    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    const auto loanKey = bridge::toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay phieu muon."));
        return;
    }
    
    const QString loanStatus = normalizedStatus(loanOpt->getStatus());
    
    // If loan is still BORROWED, need to return the book quantity
    if (loanStatus == QStringLiteral("BORROWED")) {
        const auto reply = QMessageBox::question(this, tr("Xac nhan"), 
            tr("Phieu muon nay dang o trang thai 'Dang muon'. Xoa se tu dong hoan tra sach. Ban co chac chan?"));
        if (reply != QMessageBox::Yes) return;
        
        // Return book quantity
        // handled after removal
    } else {
        const auto reply = QMessageBox::question(this, tr("Xac nhan"), 
            tr("Ban co chac chan muon xoa phieu muon %1?").arg(loanId));
        if (reply != QMessageBox::Yes) return;
    }

    if (!loanService.removeLoan(loanKey)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the xoa phieu muon."));
        return;
    }

    if (loanStatus == QStringLiteral("BORROWED")) {
        const auto bookOpt = bookService.findById(loanOpt->getBookId());
        if (bookOpt.has_value()) {
            model::Book updatedBook = *bookOpt;
            updatedBook.setQuantity(updatedBook.getQuantity() + 1);
            if (!bookService.updateBook(updatedBook)) {
                QMessageBox::warning(this, tr("Canh bao"), tr("Khong the cap nhat so luong sach."));
            }
        }
    }

    reloadData();
    statusBar()->showMessage(tr("Da xoa phieu muon."), 2000);
}

void MainWindow::handleLossOrDamage(const QString &status) {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot phieu muon."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh phieu muon da chon."));
        return;
    }
    const auto loanKey = bridge::toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay phieu muon."));
        return;
    }
    if (normalizedStatus(loanOpt->getStatus()) == QStringLiteral("RETURNED")) {
        QMessageBox::information(this, tr("Thong bao"), tr("Phieu nay da dong, khong the bao mat/hong."));
        return;
    }

    bool ok = false;
    QString reason = QInputDialog::getMultiLineText(this,
                                                   status == QStringLiteral("LOST") ? tr("Bao mat sach") : tr("Bao hu hong"),
                                                   tr("Mo ta chi tiet"),
                                                   QString(),
                                                   &ok).trimmed();
    if (!ok) return;
    if (reason.isEmpty()) {
        reason = tr("Khong co ghi chu bo sung");
    }

    int defaultFee = currentConfig.getFinePerDay() * 10;
    if (defaultFee <= 0) defaultFee = 100000;
    int fee = QInputDialog::getInt(this,
                                   status == QStringLiteral("LOST") ? tr("Tien den bu") : tr("Tien sua chua"),
                                   tr("Nhap so tien VND"),
                                   defaultFee,
                                   0,
                                   10000000,
                                   1000,
                                   &ok);
    if (!ok) return;

    const QDate today = QDate::currentDate();
    const auto statusKey = bridge::toCustomString(status);
    if (!loanService.updateStatus(loanKey, statusKey, bridge::toCoreDate(today))) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat trang thai phieu."));
        return;
    }
    if (!loanService.applyFine(loanKey, fee)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat tien phat."));
        return;
    }

    const auto bookOpt = bookService.findById(loanOpt->getBookId());
    if (bookOpt.has_value()) {
        model::Book updatedBook = *bookOpt;
        updatedBook.setQuantity(std::max(0, updatedBook.getQuantity() - 1));
        updatedBook.setStatus(model::canonicalBookStatus(bridge::toCustomString(status)));
        if (!bookService.updateBook(updatedBook)) {
            QMessageBox::warning(this, tr("Canh bao"), tr("Khong the cap nhat thong tin sach."));
        }
    }

    model::ReportRequest req;
    const QString prefix = status == QStringLiteral("LOST") ? QStringLiteral("MAT") : QStringLiteral("HU");
    const QString requestId = QStringLiteral("%1-%2")
                                  .arg(prefix)
                                  .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmss")));
    req.setRequestId(bridge::toCustomString(requestId));
    req.setStaffUsername(currentAccount.getUsername());
    req.setFromDate(bridge::toCoreDate(QDate::currentDate()));
    req.setToDate(bridge::toCoreDate(QDate::currentDate()));
    req.setHandledLoans(1);
    req.setLostOrDamaged(1);
    req.setOverdueReaders(0);
    const QString statusLabel = status == QStringLiteral("LOST") ? tr("Mat sach") : tr("Hu hong");
    req.setNotes(bridge::toCustomString(
        tr("%1 - phieu %2 - ly do: %3 - tien de xuat: %4 VND")
            .arg(statusLabel)
            .arg(loanId)
            .arg(reason)
            .arg(fee)));
    req.setStatus(custom::CustomStringLiteral("PENDING"));
    req.setCreatedAt(bridge::toCoreDateTime(QDateTime::currentDateTime()));
    if (!reportService.submitRequest(req)) {
        QMessageBox::warning(this, tr("Canh bao"), tr("Khong the ghi nhan bao cao."));
    }

    reloadData();
    statusBar()->showMessage(tr("Da ghi nhan %1.").arg(statusLabel.toLower()), 3000);
}

void MainWindow::handleSubmitReport() {
    if (!staffRole) return;
    ReportRequestDialog dialog(bridge::toQString(currentAccount.getUsername()), this);
    if (dialog.exec() != QDialog::Accepted) return;
    if (!reportService.submitRequest(dialog.reportRequest())) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the gui bao cao."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da gui bao cao."), 2000);
}

void MainWindow::handleReportStatusChange(const QString &status) {
    if (!adminRole) return;
    const auto row = currentRow(reportsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot bao cao."));
        return;
    }
    const QListWidgetItem *item = reportsList ? reportsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh bao cao da chon."));
        return;
    }
    QString requestId = item->data(kCardRoleId).toString();
    if (requestId.isEmpty()) {
        requestId = item->data(Qt::UserRole).toString();
    }
    if (requestId.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh bao cao da chon."));
        return;
    }
    if (!reportService.updateStatus(bridge::toCustomString(requestId), bridge::toCustomString(status))) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat trang thai."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da cap nhat trang thai bao cao."), 2000);
}

void MainWindow::handleLogout() {
    const auto reply = QMessageBox::question(
        this, 
        tr("Đăng xuất"), 
        tr("Bạn có chắc chắn muốn đăng xuất khỏi hệ thống?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No  // Default to No to prevent accidental logout
    );
    
    if (reply != QMessageBox::Yes) return;

    // Show a brief "logging out" message
    QApplication::setOverrideCursor(Qt::WaitCursor);
    statusBar()->showMessage(tr("Đang đăng xuất..."));
    
    // Disable logout controls to avoid re-entry while we perform the transition.
    if (ui) {
        if (ui->logoutButton) ui->logoutButton->setEnabled(false);
        if (ui->logoutAction) ui->logoutAction->setEnabled(false);
    }

    // Restore the cursor and immediately request logout. The application-level
    // handler will close and destroy the main window and restart the login flow.
    QApplication::restoreOverrideCursor();
    emit logoutRequested();
}

void MainWindow::setupNavigationMenu() {
    if (!navigationList || !tabs) return;
    QSignalBlocker blocker(navigationList);
    navigationList->clear();

    const auto resolveIconPath = [](const QString &baseName) -> QString {
        if (baseName.trimmed().isEmpty()) return {};
        static const QStringList kExtensions{QStringLiteral("svg"), QStringLiteral("png"), QStringLiteral("ico")};
        for (const auto &ext : kExtensions) {
            const QString candidate = QStringLiteral(":/icons/%1.%2").arg(baseName, ext);
            if (QFile::exists(candidate)) {
                return candidate;
            }
        }
        return {};
    };
    struct NavEntry {
        QString title;
        QString description;
        QString iconName;
    };

    QVector<NavEntry> entries;
    entries.reserve(tabs->count());
    for (int i = 0; i < tabs->count(); ++i) {
        const QString tabText = tabs->tabText(i);
        QString iconName;
        QString description;
        const QString normalized = tabText.trimmed().toLower();
        if (normalized.contains(QStringLiteral("trang chu"))) {
            iconName = QStringLiteral("home");
            description = tr("Tong quan nhanh ve he thong va hoat dong gan day.");
        } else if (normalized.contains(QStringLiteral("sach"))) {
            iconName = QStringLiteral("book");
            description = tr("Quan ly danh muc sach, trang thai va ton kho.");
        } else if (normalized.contains(QStringLiteral("doc gia"))) {
                
            iconName = QStringLiteral("reader");
            description = tr("Theo doi ho so ban doc va han muc su dung.");
        } else if (normalized.contains(QStringLiteral("phieu")) || normalized.contains(QStringLiteral("muon"))) {
            iconName = QStringLiteral("loan");
            description = tr("Xu ly phieu muon, gia han va tra sach kip thoi.");
        } else if (normalized.contains(QStringLiteral("bao cao"))) {
            iconName = QStringLiteral("report");
            description = tr("Tong hop bao cao hoat dong va chi so chinh.");
        } else if (normalized.contains(QStringLiteral("thong ke")) || normalized.contains(QStringLiteral("tong quan"))) {
            iconName = QStringLiteral("t");
            description = tr("Thong ke chi tiet ve luu thong va su dung.");
        } else if (normalized.contains(QStringLiteral("nhan vien"))) {
            iconName = QStringLiteral("staff");
            description = tr("Cap nhat phan quyen va thong tin nhan su.");
        } else if (normalized.contains(QStringLiteral("tai khoan"))) {
            iconName = QStringLiteral("account");
            description = tr("Quan tri tai khoan dang nhap va trang thai.");
        } else if (normalized.contains(QStringLiteral("cau hinh")) || normalized.contains(QStringLiteral("thiet lap"))) {
            iconName = QStringLiteral("setting");
            description = tr("Dieu chinh tham so he thong va quy tac hoat dong.");
        } else {
            iconName = QStringLiteral("menu");
        }

        entries.push_back({tabText, description, iconName});
    }

    for (const auto &entry : entries) {
        const QString wrappedText = entry.description.isEmpty()
                                        ? entry.title
                                        : QStringLiteral("%1\n%2").arg(entry.title, entry.description);
        auto *item = new QListWidgetItem(wrappedText, navigationList);
        // Make sure tooltip contains the full title so the user can see it
        // when navigation is collapsed (icon-only).
        item->setToolTip(entry.title);
        const QString iconPath = resolveIconPath(entry.iconName);
        if (!iconPath.isEmpty()) {
            item->setIcon(QIcon(iconPath));
        }
        const QSize itemSize = item->sizeHint();
        item->setSizeHint(QSize(itemSize.width(), std::max(itemSize.height(), 72)));
    }

    navigationList->setIconSize(QSize(22, 22));
    navigationList->setCurrentRow(tabs->currentIndex());

    // Populate the navRail (icon-only) so it mirrors the full navigation
    if (navRail) {
        // Clear existing children
        QLayout *l = navRail->layout();
        while (l && l->count() > 0) {
            QLayoutItem *li = l->takeAt(0);
            if (!li) break;
            if (QWidget *w = li->widget()) { w->deleteLater(); }
            delete li;
        }
        if (QVBoxLayout *railLayout = qobject_cast<QVBoxLayout *>(navRail->layout())) {
            // Top spacer
            railLayout->addSpacing(6);
            // create buttons for each nav entry
            for (int i = 0; i < entries.size(); ++i) {
                const NavEntry &e = entries.at(i);
                QToolButton *b = new QToolButton(navRail);
                b->setIcon(QIcon(resolveIconPath(e.iconName)));
                b->setIconSize(QSize(22,22));
                b->setToolButtonStyle(Qt::ToolButtonIconOnly);
                b->setFixedSize(44,44);
                b->setToolTip(e.title);
                // Connect click to select the tab
                connect(b, &QToolButton::clicked, this, [this, i]() {
                    if (tabs && i >= 0 && i < tabs->count()) tabs->setCurrentIndex(i);
                });
                railLayout->addWidget(b);
            }
            railLayout->addStretch(1);
            // Bottom action buttons: reload and logout
            QPushButton *reloadBtn = new QPushButton(tr("Tai lai"), navRail);
            reloadBtn->setFixedHeight(36);
            connect(reloadBtn, &QPushButton::clicked, [this]() { reloadData(); statusBar()->showMessage(tr("Da tai lai."), 1500); });
            railLayout->addWidget(reloadBtn);
            QPushButton *logoutBtn = new QPushButton(tr("Dang xuat"), navRail);
            logoutBtn->setFixedHeight(36);
            connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);
            railLayout->addWidget(logoutBtn);
            railLayout->addSpacing(6);
        }
    }

    repositionNavRailButton();
}

void MainWindow::setNavigationCollapsed(bool collapsed, bool pinned) {
    navCollapsed = collapsed;
    navPinned = pinned;

    if (navigationList) {
        if (!collapsed) {
            if (navExpandedWidth <= 0) {
                navExpandedWidth = navigationList->sizeHintForColumn(0) > 0 ? navigationList->width() : navExpandedWidth;
            }
            navigationList->setVisible(true);
            navigationList->setMinimumWidth(navExpandedWidth);
            navigationList->setMaximumWidth(QWIDGETSIZE_MAX);
        } else {
            navigationList->setVisible(false);
        }
        navigationList->setProperty("navCollapsed", collapsed);
    }

    if (navRail) {
        navRail->setVisible(collapsed);
        navRail->raise();
    }

    if (navRailButton) {
        navRailButton->setVisible(collapsed);
        navRailButton->raise();
    }

    if (navToggleButton) {
        navToggleButton->setText(collapsed ? tr("Hiện") : tr("Ẩn"));
        navToggleButton->setToolTip(collapsed ? tr("Hiển thị thanh điều hướng") : tr("Ẩn thanh điều hướng"));
    }

    repositionNavRailButton();
}

void MainWindow::repositionNavRailButton() const {
    if (!navigationList) return;

    const QPoint topLeft = navigationList->mapTo(this, QPoint(0, 0));
    const QSize navSize = navigationList->size();

    if (navRail) {
        navRail->setGeometry(QRect(topLeft, navSize));
        navRail->raise();
    }

    if (navRailButton) {
        const int railWidth = navRail ? navRail->width() : navSize.width();
        const int buttonX = topLeft.x() + std::max(8, railWidth / 2 - navRailButton->width() / 2);
        const int buttonY = topLeft.y() + 8;
        navRailButton->move(buttonX, buttonY);
        navRailButton->raise();
    }
}

void MainWindow::showNavigationPopupMenu() {
    if (!navRailButton || !navigationList) return;

    QMenu menu(this);
    for (int i = 0; i < navigationList->count(); ++i) {
        QListWidgetItem *item = navigationList->item(i);
        if (!item) continue;
        const QString title = item->text().split('\n').value(0);
        QAction *action = menu.addAction(title);
        connect(action, &QAction::triggered, this, [this, i]() {
            if (navigationList) {
                QSignalBlocker blocker(navigationList);
                navigationList->setCurrentRow(i);
            }
            handleNavigationSelection(i);
            if (navCollapsed && !navPinned) {
                setNavigationCollapsed(true, false);
            }
        });
    }

    if (menu.isEmpty()) return;

    const QPoint globalPos = navRailButton->mapToGlobal(QPoint(navRailButton->width() / 2, navRailButton->height()));
    menu.exec(globalPos);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == navigationList) {
        if (event->type() == QEvent::Move || event->type() == QEvent::Resize || event->type() == QEvent::Show) {
            repositionNavRailButton();
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    repositionNavRailButton();
}

void MainWindow::handleNavigationSelection(const int index) const {
    if (!tabs || index < 0 || index >= tabs->count()) return;
    if (tabs->currentIndex() != index) {
        tabs->setCurrentIndex(index);
    }
}
void MainWindow::handleAddAccount() {
    if (!adminRole) return;
    AccountDialog dialog(this);
    // Provide staff list for selection
    staffsCache = toQVector(staffService.fetchAll());
    dialog.setStaffList(staffsCache);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto usernameKey = dialog.username();
    const QString username = bridge::toQString(usernameKey);
    if (accountService.findByUsername(usernameKey).has_value()) {
        QMessageBox::warning(this, tr("Trung lap"), tr("Ten dang nhap da ton tai."));
        return;
    }
    const QString staffId = bridge::toQString(dialog.staffId()).trimmed();
    custom::Optional<custom::CustomString> staffKey;
    if (!staffId.isEmpty()) {
        staffKey = bridge::toCustomString(staffId);
        if (!staffService.findById(staffKey.value()).has_value()) {
            QMessageBox::warning(this, tr("Khong hop le"), tr("Nhan vien khong ton tai."));
            return;
        }
    }
    const auto passwordKey = dialog.password();
    const auto roleKey = dialog.role();
    bool created = false;
    if (!staffKey.has_value()) {
        created = accountService.createAccount(usernameKey, passwordKey, roleKey, dialog.isActive());
    } else {
        created = accountService.createAccount(usernameKey, passwordKey, roleKey, dialog.isActive(), staffKey.value());
    }
    if (!created) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the tao tai khoan."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da tao tai khoan moi."), 2000);
}

void MainWindow::handleResetPassword() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot tai khoan."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    bool ok = false;
    const QString newPassword = QInputDialog::getText(this, tr("Dat lai mat khau"), tr("Nhap mat khau moi"), QLineEdit::Password, QString(), &ok);
    if (!ok || newPassword.isEmpty()) return;
    if (!accountService.updatePassword(bridge::toCustomString(username), bridge::toCustomString(newPassword))) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the dat lai mat khau."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da dat lai mat khau."), 2000);
}

void MainWindow::handleToggleAccountActive() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot tai khoan."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    const auto usernameKey = bridge::toCustomString(username);
    const auto accountOpt = accountService.findByUsername(usernameKey);
    if (!accountOpt.has_value()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong tim thay tai khoan."));
        return;
    }
    model::Account updated = *accountOpt;
    updated.setActive(!updated.isActive());
    if (!accountService.updateAccount(updated)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the cap nhat tai khoan."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da thay doi trang thai tai khoan."), 2000);
}

void MainWindow::handleDeleteAccount() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        QMessageBox::information(this, tr("Canh bao"), tr("Vui long chon mot tai khoan."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        QMessageBox::warning(this, tr("Khong tim thay"), tr("Khong the xac dinh tai khoan da chon."));
        return;
    }
    if (username == bridge::toQString(currentAccount.getUsername())) {
        QMessageBox::warning(this, tr("Khong hop le"), tr("Khong the xoa tai khoan dang dang nhap."));
        return;
    }
    const auto reply = QMessageBox::question(this, tr("Xac nhan"), tr("Ban co chac chan muon xoa tai khoan %1?").arg(username));
    if (reply != QMessageBox::Yes) return;
    if (!accountService.removeAccount(bridge::toCustomString(username))) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the xoa tai khoan."));
        return;
    }
    reloadData();
    statusBar()->showMessage(tr("Da xoa tai khoan."), 2000);
}

void MainWindow::handleSaveConfig() {
    if (!adminRole || !maxBorrowDaysSpin) return;
    model::SystemConfig cfg;
    cfg.setMaxBorrowDays(maxBorrowDaysSpin->value());
    cfg.setFinePerDay(finePerDaySpin->value());
    cfg.setMaxBooksPerReader(maxBooksPerReaderSpin->value());
    if (!configService.save(cfg)) {
        QMessageBox::warning(this, tr("Khong thanh cong"), tr("Khong the luu cau hinh."));
        return;
    }
    currentConfig = cfg;
    statusBar()->showMessage(tr("Da luu cau hinh."), 2000);
}

}  // namespace pbl2::ui
