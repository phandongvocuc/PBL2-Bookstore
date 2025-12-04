#include <algorithm>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyle>
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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDebug>
#include <QRegularExpression>
#include <QSet>
#include <QHeaderView>
#include "core/IdGenerator.h"
#include "../core/DynamicArray.h"
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

struct AffectedBookChange {
    QString id;
    int count{0};
};

core::DynamicArray<AffectedBookChange> parseAffectedBookChanges(const QString &raw) {
    core::DynamicArray<AffectedBookChange> changes;
    QString normalized = raw;
    normalized.replace('\n', ';');
    normalized.replace(',', ';');

    auto appendEntry = [&](const QString &token) {
        if (token.isEmpty()) return;
        const int sep = token.indexOf(':');
        const QString id = (sep >= 0 ? token.left(sep) : token).trimmed();
        const QString countStr = sep >= 0 ? token.mid(sep + 1).trimmed() : QStringLiteral("1");
        if (id.isEmpty()) return;

        bool ok = true;
        const int count = countStr.isEmpty() ? 1 : countStr.toInt(&ok);
        if (!ok) return;

        const QString canonical = id.toUpper();
        for (int i = 0; i < changes.size(); ++i) {
            if (changes[i].id.compare(canonical, Qt::CaseInsensitive) == 0) {
                changes[i].count += count;
                return;
            }
        }
        changes.append(AffectedBookChange{canonical, count});
    };

    QString current;
    for (const QChar ch : normalized) {
        if (ch == QChar(';')) {
            appendEntry(current.trimmed());
            current.clear();
        } else {
            current.append(ch);
        }
    }
    appendEntry(current.trimmed());
    return changes;
}

QColor statusBadgeColor(const QString &statusCode) {
    const QString normalized = normalizedStatus(statusCode);
    if (normalized == QStringLiteral("CÒN")) return {0x28, 0xA7, 0x45};
    if (normalized == QStringLiteral("HẾT")) return {0x96, 0x9D, 0xA6};
    if (normalized == QStringLiteral("MẤT")) return {0xD9, 0x3F, 0x3F};
    if (normalized == QStringLiteral("HỎNG")) return {0xF0, 0x73, 0x24};
    if (normalized == QStringLiteral("BORROWED")) return {0x2F, 0x6A, 0xD0};
    if (normalized == QStringLiteral("RETURNED")) return {0x28, 0xA7, 0x45};
    if (normalized == QStringLiteral("OVERDUE")) return {0xF5, 0x8B, 0x0A};
    if (normalized == QStringLiteral("LOST")) return {0xD9, 0x3F, 0x3F};
    if (normalized == QStringLiteral("DAMAGED")) return {0xF0, 0x73, 0x24};
    if (normalized == QStringLiteral("PENDING")) return {0xFF, 0xA0, 0x2F};
    if (normalized == QStringLiteral("APPROVED")) return {0x28, 0xA7, 0x45};
    if (normalized == QStringLiteral("REJECTED")) return {0xD9, 0x3F, 0x3F};
    if (normalized == QStringLiteral("ACTIVE")) return {0x28, 0xA7, 0x45};
    if (normalized == QStringLiteral("INACTIVE")) return {0x96, 0x9D, 0xA6};
    return {0x2F, 0x6A, 0xD0};
}

QString formatCurrency(int value) {
    const QLocale viLocale(QLocale::Vietnamese, QLocale::Vietnam);
    return viLocale.toString(value) + QStringLiteral(" VND");
}

class CardListDelegate final : public QStyledItemDelegate {
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
    const QColor background = selected ? QColor(0xE6, 0xE9, 0xEB) : QColor(0xE2, 0xE2, 0xE2);
    QColor border = selected ? QColor(0x95, 0xA5, 0xB0) : QColor(0xD0, 0xD5, 0xDA);

        painter->setPen(border);
        painter->setBrush(background);
    painter->drawRoundedRect(outer, 6, 6);

    // Reduce inner padding so text area is larger relative to the card
    QRect inner = outer.adjusted(12, 12, -12, -12);
    const QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    const bool hasIcon = !icon.isNull();
    int textLeft = inner.left();
    // Cover area
    const int coverWidth = 130;
    const int coverHeight = inner.height() - 24;
    const int coverTop = inner.top() + (inner.height() - coverHeight) / 2;
    QRect coverFrame(inner.left(), coverTop, coverWidth, coverHeight);
    QRect coverRect = coverFrame.adjusted(6, 6, -6, -6);
    // Draw a subtle frame so the cover stands out.
    painter->setPen(QPen(QColor(0x70, 0x78, 0x80), 2));
    painter->setBrush(QColor(0xF7, 0xF7, 0xF7));
    painter->drawRoundedRect(coverFrame, 6, 6);
    if (hasIcon) {
        QPixmap pm = icon.pixmap(coverRect.size());
        pm = pm.scaled(coverRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // Center the scaled pixmap inside the rect to avoid cropping.
        const QSize pmSize = pm.size();
        QRect target(coverRect.topLeft(), pmSize);
        target.moveCenter(coverRect.center());
        painter->drawPixmap(target, pm);
    } else {
        painter->fillRect(coverRect, QColor(0x3A, 0x32, 0x32));
    }
    painter->setPen(QPen(QColor(0x70, 0x78, 0x80), 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(coverRect, 4, 4);
    // Shift text area to the right of the cover.
    const int gap = 18;
    inner.setLeft(inner.left() + coverWidth + gap);
    textLeft = inner.left();
    const QString header = index.data(kCardRoleHeader).toString();              // Tiêu đề
    const QString meta = index.data(kCardRoleMeta).toString();                  // Mã
    const QString detail = index.data(kCardRoleDetail).toString();             // Tác giả
    const QString extraDetail = index.data(kCardRoleSecondaryDetail).toString(); // SL còn (hiển thị qua pill)
        const QString badgeText = index.data(kCardRoleBadgeText).toString();
        auto badgeColor = index.data(kCardRoleBadgeColor).value<QColor>();
        if (!badgeColor.isValid()) {
            badgeColor = statusBadgeColor(badgeText);
        }
        const QString pillText = index.data(kCardRolePillText).toString();
        auto pillColor = index.data(kCardRolePillColor).value<QColor>();
        if (!pillColor.isValid()) {
            pillColor = option.palette.alternateBase().color();
        }

        // Fonts: mã nhỏ, tiêu đề lớn, tác giả trung bình
        QFont codeFont = option.font;
        codeFont.setPointSizeF((option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize()) + 1.0);

        QFont titleFont = option.font;
        titleFont.setPointSizeF((option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize()) + 2.5);
        titleFont.setBold(true);

        QFont authorFont = option.font;
        authorFont.setPointSizeF((option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize()) + 1.0);

        QFont badgeFont = option.font;
        badgeFont.setPointSizeF(authorFont.pointSizeF());
        badgeFont.setBold(true);

        QFont pillFont = option.font;
        pillFont.setPointSizeF(option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize());

        QFontMetrics codeMetrics(codeFont);
        QFontMetrics titleMetrics(titleFont);
        QFontMetrics authorMetrics(authorFont);
        QFontMetrics badgeMetrics(badgeFont);
        QFontMetrics pillMetrics(pillFont);
    // Badge (trạng thái) đặt góc phải trên
    const int badgeWidth = badgeMetrics.horizontalAdvance(badgeText) + 18;
    const int badgeHeight = badgeMetrics.height() + 10;
        const int badgeTop = inner.top();
        QRect badgeRect(inner.right() - badgeWidth, badgeTop, badgeWidth, badgeHeight);

        constexpr auto primaryText = QColor(0x22, 0x2C, 0x3A);
    auto secondaryText = QColor(0x43, 0x4D, 0x5A);
    if (selected) {
        secondaryText = QColor(0x1E, 0x40, 0x8A);
    }

    // Text block: mã, tiêu đề, tác giả
    const int titleW = max(inner.width() - badgeWidth - 12, 120);
    int y = inner.top() + 4;

    painter->setFont(codeFont);
    painter->setPen(secondaryText);
    painter->drawText(QRect(textLeft, y, titleW, codeMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, meta);
    y += codeMetrics.height() + 6;

    painter->setFont(titleFont);
    painter->setPen(primaryText);
    painter->drawText(QRect(textLeft, y, titleW, titleMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, header);
    y += titleMetrics.height() + 6;

    painter->setFont(authorFont);
    painter->setPen(primaryText);
    painter->drawText(QRect(textLeft, y, titleW, authorMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, detail);

        if (!pillText.isEmpty()) {
            painter->setFont(pillFont);
            const int pillWidth = pillMetrics.horizontalAdvance(pillText) + 20;
            const int pillHeight = pillMetrics.height() + 12;
            const int pillTop = inner.bottom() - pillHeight;
            QRect pillRect(inner.right() - pillWidth, pillTop, pillWidth, pillHeight);

            QColor pillBg = QColor(0xC7, 0xC2, 0xC2);
            if (selected) pillBg = pillBg.darker(110);
            QColor pillFg = QColor(0xFF, 0xFF, 0xFF);
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
    const int badgeRadius = min(badgeHeight / 2, 8);
    painter->drawRoundedRect(badgeRect, badgeRadius, badgeRadius);
        painter->setPen(badgeFg);
        painter->drawText(badgeRect, Qt::AlignCenter, badgeText);

        painter->restore();
    }

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(index);
        QFont titleFont = option.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        const QFont secondary = option.font;
        const QFontMetrics titleMetrics(titleFont);
        const QFontMetrics bodyMetrics(secondary);
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
                    preferred = clamp(preferred, 220, 420);
                    // Use a slightly larger height to fit the extra line
                    const int preferredHeight = max(height, 180);
                    return {preferred, preferredHeight};
                }
            }
        }
        return {option.rect.width(), max(height, 112)};
    }
};

// Generic text-only delegate for non-book lists
class SimpleCardDelegate final : public QStyledItemDelegate {
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
        const QRect outer = option.rect.adjusted(4, 2, -4, -2);
        const QColor background = selected ? QColor(0xE6, 0xE9, 0xEB) : QColor(0xF1, 0xF5, 0xF9);
        QColor border = selected ? QColor(0x95, 0xA5, 0xB0) : QColor(0xD0, 0xD5, 0xDA);

        painter->setPen(border);
        painter->setBrush(background);
        painter->drawRoundedRect(outer, 6, 6);

        QRect inner = outer.adjusted(12, 12, -12, -12);
        const QString header = index.data(kCardRoleHeader).toString();
        const QString meta = index.data(kCardRoleMeta).toString();
        const QString detail = index.data(kCardRoleDetail).toString();
        const QString extraDetail = index.data(kCardRoleSecondaryDetail).toString();
        const QString badgeText = index.data(kCardRoleBadgeText).toString();
        QColor badgeColor = index.data(kCardRoleBadgeColor).value<QColor>();
        if (!badgeColor.isValid()) badgeColor = statusBadgeColor(badgeText);
        const QString pillText = index.data(kCardRolePillText).toString();

        QFont headerFont = option.font;
        headerFont.setPointSizeF((option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize()) + 1.5);
        headerFont.setBold(true);
        QFont metaFont = option.font;
        metaFont.setPointSizeF(headerFont.pointSizeF() - 1.0);
        QFont detailFont = option.font;
        detailFont.setPointSizeF(option.font.pointSizeF() > 0 ? option.font.pointSizeF() : option.font.pointSize());
        QFont badgeFont = detailFont;
        badgeFont.setBold(true);

        const QFontMetrics headerMetrics(headerFont);
        const QFontMetrics metaMetrics(metaFont);
        const QFontMetrics detailMetrics(detailFont);
        const QFontMetrics badgeMetrics(badgeFont);

        const int badgeWidth = badgeMetrics.horizontalAdvance(badgeText) + 18;
        const int badgeHeight = badgeMetrics.height() + 10;
        const QRect badgeRect(inner.right() - badgeWidth, inner.top(), badgeWidth, badgeHeight);

        const int textWidth = inner.width() - badgeWidth - 12;
        int y = inner.top();

        painter->setFont(metaFont);
        painter->setPen(QColor(0x43, 0x4D, 0x5A));
        painter->drawText(QRect(inner.left(), y, textWidth, metaMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, meta);
        y += metaMetrics.height() + 6;

        painter->setFont(headerFont);
        painter->setPen(QColor(0x22, 0x2C, 0x3A));
        painter->drawText(QRect(inner.left(), y, textWidth, headerMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, header);
        y += headerMetrics.height() + 6;

        painter->setFont(detailFont);
        painter->setPen(QColor(0x22, 0x2C, 0x3A));
        if (!detail.isEmpty()) {
            painter->drawText(QRect(inner.left(), y, textWidth, detailMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, detail);
            y += detailMetrics.height() + 4;
        }
        if (!extraDetail.isEmpty()) {
            painter->drawText(QRect(inner.left(), y, textWidth, detailMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter, extraDetail);
            y += detailMetrics.height() + 4;
        }

        // Badge
        QColor badgeBg = badgeColor.isValid() ? badgeColor : option.palette.highlight().color();
        QColor badgeFg = Qt::white;
        if (badgeBg.lightness() > 200) badgeFg = QColor(0x22, 0x2C, 0x3A);
        painter->setFont(badgeFont);
        painter->setPen(Qt::NoPen);
        painter->setBrush(badgeBg);
        painter->drawRoundedRect(badgeRect, 6, 6);
        painter->setPen(badgeFg);
        painter->drawText(badgeRect, Qt::AlignCenter, badgeText);

        // Pill
        if (!pillText.isEmpty()) {
            const int pillWidth = detailMetrics.horizontalAdvance(pillText) + 18;
            const int pillHeight = detailMetrics.height() + 10;
            QRect pillRect(inner.right() - pillWidth, inner.bottom() - pillHeight, pillWidth, pillHeight);
            QColor pillBg = QColor(0xD1, 0xD5, 0xDB);
            QColor pillFg = QColor(0x22, 0x2C, 0x3A);
            painter->setPen(Qt::NoPen);
            painter->setBrush(pillBg);
            painter->drawRoundedRect(pillRect, 6, 6);
            painter->setPen(pillFg);
            painter->drawText(pillRect, Qt::AlignCenter, pillText);
        }

        painter->restore();
    }

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(index);
        const int h = option.fontMetrics.height();
        const int baseHeight = max(120, h * 5 + 24);

        // In IconMode, QListView sometimes provides a zero/very small width in
        // sizeHint; compute a sane card width based on viewport so the cards
        // remain visible in a grid (similar to CardListDelegate above).
        if (option.widget) {
            if (auto *view = qobject_cast<const QListView *>(option.widget)) {
                if (view->viewMode() == QListView::IconMode) {
                    const int vw = view->viewport() ? view->viewport()->width() : view->width();
                    constexpr int columns = 2;
                    int spacing = 8;
                    if (auto *lw = qobject_cast<const QListWidget *>(view)) {
                        spacing = lw->spacing();
                    }
                    const int totalGaps = (columns + 1) * spacing;
                    int preferred = (vw - totalGaps) / columns;
                    preferred = clamp(preferred, 220, 420);
                    return {preferred, max(baseHeight, 180)};
                }
            }
        }

        return {max(option.rect.width(), 240), baseHeight};
    }
};

class NavigationListDelegate final : public QStyledItemDelegate {
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
        if (opt.widget ? opt.widget->property("navCollapsed").toBool() : false) {
            // Draw only the icon centered inside the item rect.
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            const QRect iconRect = option.rect.adjusted(6, 6, -6, -6);
            if (auto icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole)); !icon.isNull()) {
                const QSize isz = option.decorationSize;
                const QPoint center = iconRect.center();
                const QRect drawRect(center.x() - isz.width() / 2, center.y() - isz.height() / 2, isz.width(), isz.height());
                icon.paint(painter, drawRect);
            }
            painter->restore();
            return;
        }

        opt.text.clear();
        opt.icon = QIcon();  // tránh vẽ icon mặc định, chúng ta tự vẽ bên dưới
        opt.decorationSize = QSize();
        if (opt.widget) {
            opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        } else {
            QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
        }

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        const int padding = 12;
        const QRect contentRect = opt.rect.adjusted(padding, padding, -padding, -padding);
        const QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        const int iconSide = opt.decorationSize.isValid() ? opt.decorationSize.width() : 22;
        QRect iconRect(contentRect.left(),
                       contentRect.center().y() - iconSide / 2,
                       iconSide,
                       iconSide);

        QFont titleFont = opt.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        QFont descriptionFont = opt.font;
        if (descriptionFont.pointSizeF() > 0) {
            descriptionFont.setPointSizeF(max(descriptionFont.pointSizeF() - 0.5, 7.5));
        } else if (descriptionFont.pointSize() > 0) {
            descriptionFont.setPointSize(max(descriptionFont.pointSize() - 1, 7));
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
        if (!icon.isNull()) {
            icon.paint(painter, iconRect);
        }
        QRect textRect = contentRect.adjusted(iconSide + 10, 0, 0, 0);
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, title);

        painter->restore();
    }

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);

        const QStringList lines = opt.text.split('\n', Qt::KeepEmptyParts);
        const QString title = lines.value(0);

        QFont titleFont = opt.font;
        if (titleFont.pointSizeF() > 0) {
            titleFont.setPointSizeF(titleFont.pointSizeF() + 1.0);
        } else if (titleFont.pointSize() > 0) {
            titleFont.setPointSize(titleFont.pointSize() + 1);
        }
        titleFont.setBold(true);

        int measuredWidth = option.rect.width();
        if (measuredWidth <= 0 && opt.widget) {
            measuredWidth = opt.widget->width();
        }
        if (measuredWidth <= 0) {
            measuredWidth = 280;
        }
        if (opt.widget ? opt.widget->property("navCollapsed").toBool() : false) {
            // In collapsed mode, prefer a narrow, icon-only width that fits the
            // decoration size plus a small padding so the list becomes a slim
            // vertical strip of icons.
            const int w = opt.decorationSize.width() + 16;
            const int h = max(72, opt.decorationSize.height() + 12);
            return {w, h};
        }
        const int iconWidth = opt.decorationSize.isValid() ? opt.decorationSize.width() : 22;
        const int availableWidth = max(160, measuredWidth - iconWidth - 80);
        const QFontMetrics titleMetrics(titleFont);
        const QRect titleRect = titleMetrics.boundingRect(0, 0, availableWidth, 0, Qt::TextSingleLine, title);

        const int padding = 24;
        const int rowHeight = max(56, iconWidth + padding);
        return {measuredWidth, max(rowHeight, titleRect.height() + padding)};
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
        compactFont.setPointSizeF(max(9.0, compactFont.pointSizeF()));
    } else if (compactFont.pointSize() > 0) {
        compactFont.setPointSize(max(9, compactFont.pointSize()));
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

    // Delegate will be set per-tab after configuration.
}

QString normalizedStatus(const QString &text) {
    return text.trimmed().toUpper();
}

QString normalizedStatus(const core::CustomString &text) {
    return normalizedStatus(toQString(text));
}

bool containsAllTokens(const QString &haystackLower, const QString &termLower) {
    if (termLower.isEmpty()) return true;
    const auto tokens = termLower.split(QRegularExpression(QStringLiteral("\\s+")), Qt::SkipEmptyParts);
    for (const auto &tok : tokens) {
        if (!haystackLower.contains(tok)) return false;
    }
    return true;
}

QString bookStatusText(const core::CustomString &code) {
    const QString value = toQString(model::canonicalBookStatus(code));
    if (value == QStringLiteral("CÒN")) return QObject::tr("CÒN");
    if (value == QStringLiteral("HẾT")) return QObject::tr("HẾT");
    if (value == QStringLiteral("MẤT")) return QObject::tr("MẤT");
    if (value == QStringLiteral("HỎNG")) return QObject::tr("HỎNG");
    return value;
}

    QString loanStatusText(const QString &code) {
    const QString value = normalizedStatus(code);
    if (value == QStringLiteral("BORROWED")) return QObject::tr("Đang mượn");
    if (value == QStringLiteral("RETURNED")) return QObject::tr("Đã trả");
    if (value == QStringLiteral("OVERDUE")) return QObject::tr("Quá hạn");
    if (value == QStringLiteral("LOST")) return QObject::tr("Báo mất");
    if (value == QStringLiteral("DAMAGED")) return QObject::tr("Báo hư");
    return code;
}

    QString accountActiveText(const bool active) {
    return active ? QObject::tr("Đang hoạt động") : QObject::tr("Đã khóa");
}

    QString reportStatusText(const QString &status) {
    const QString value = normalizedStatus(status);
    if (value == QStringLiteral("PENDING")) return QObject::tr("Chờ duyệt");
    if (value == QStringLiteral("APPROVED")) return QObject::tr("Đã chấp nhận");
    if (value == QStringLiteral("REJECTED")) return QObject::tr("Đã từ chối");
    return status;
}

}

namespace pbl2::ui {

MainWindow::MainWindow(const QString &dataDir, const model::Account &signedInAccount, QWidget *parent)
        : QMainWindow(parent),
            ui(make_unique<Ui::MainWindow>()),
            dataDirectory(toCustomString(dataDir)),
            bookService(dataDirectory),
            readerService(dataDirectory),
            staffService(dataDirectory),
            loanService(dataDirectory),
            accountService(dataDirectory),
        reportService(dataDirectory),
        configService(dataDirectory),
        currentAccount(signedInAccount) {
    const QString roleText = normalizedStatus(toQString(currentAccount.getRole()));
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
        ui->homeHintLabel->setText(tr("Sử dụng các tab hoặc menu bên trái để truy cập nhanh các khu vực quản lý. Dữ liệu mới nhất về sách, bạn đọc và phiếu mượn sẽ được cập nhật ngay sau khi bạn tải lại."));
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
        connect(tabs, &QTabWidget::currentChanged, [this](const int index) {
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
        navigationList->setWordWrap(false);  // giữ text một dòng để icon & chữ thẳng hàng
        navigationList->setUniformItemSizes(true);
        navigationList->setSpacing(4);
        navigationList->setTextElideMode(Qt::ElideNone);
        navigationList->setIconSize(QSize(22, 22));
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
        auto railLayout = new QVBoxLayout(navRail);
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
        ui->refreshButton->setText(tr("Tải lại"));
        connect(ui->refreshButton, &QPushButton::clicked, [this]() {
            notifyEvent(tr("Đang tải lại..."), EventSeverity::Info, 0);
            this->reloadData();
            notifyEvent(tr("Đã tải lại dữ liệu."), EventSeverity::Success, 2000);
        });
    }
    if (ui->logoutButton) {
        ui->logoutButton->setText(tr("Đăng xuất"));
        connect(ui->logoutButton, &QPushButton::clicked, [this]() { handleLogout(); });
    }

    userInfoLabel = new QLabel(this);
    const QString signedInLabel = tr("Đang đăng nhập %1 (%2)")
                                      .arg(toQString(currentAccount.getUsername()).trimmed(), toQString(currentAccount.getRole().trimmed()));
    userInfoLabel->setText(signedInLabel);
    userInfoLabel->setFont(accentFont);
    statusBar()->addPermanentWidget(userInfoLabel);



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
        booksCache = bookService.fetchAll();
    }
    if (readersCache.isEmpty()) {
        readersCache = readerService.fetchAll();
    }
    if (loansCache.isEmpty()) {
        loansCache = loanService.fetchAll();
    }
    
    // Force refresh stats display with real data
    refreshSimpleStats();
    
    configureStaffsTab();
    configureAccountsTab();
    configureSettingsTab();

    auto removeTabIfExists = [this](QWidget *tabWidget) {
        if (!tabs || !tabWidget) return;
        if (const int index = tabs->indexOf(tabWidget); index != -1) {
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

void MainWindow::notifyEvent(const QString &message, const EventSeverity severity, const int durationMs) const {
    Q_UNUSED(severity);
    const int duration = durationMs > 0 ? durationMs : 2000;
    if (statusBar()) {
        statusBar()->showMessage(message, duration);
    }
}

void MainWindow::showInfoDialog(const QString &title, const QString &message) {
    QMessageBox::information(this, title, message);
}

void MainWindow::showWarningDialog(const QString &title, const QString &message) {
    QMessageBox::warning(this, title, message);
}

void MainWindow::showErrorDialog(const QString &title, const QString &message) {
    QMessageBox::critical(this, title, message);
}

QMessageBox::StandardButton MainWindow::askEventQuestion(const QString &title, const QString &message, const QMessageBox::StandardButtons buttons, const QMessageBox::StandardButton defaultButton) {
    return QMessageBox::question(this, title, message, buttons, defaultButton);
}
void MainWindow::configureBooksTab() {
    configureCardListWidget(booksList);
    if (booksList) booksList->setItemDelegate(new CardListDelegate(booksList));
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
        bookStatusFilter->addItem(tr("Tất cả tình trạng"), QStringLiteral("ALL"));
        bookStatusFilter->addItem(tr("CÒN"), QStringLiteral("CÒN"));
        bookStatusFilter->addItem(tr("HẾT"), QStringLiteral("HẾT"));
    }

    if (bookSearchEdit) {
        bookSearchEdit->setPlaceholderText(tr("Tìm kiếm sách (mã, tên, tác giả, thể loại, NXB, trạng thái... )"));
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
    if (ui->restockBookButton) {
        ui->restockBookButton->setVisible(adminRole);
        connect(ui->restockBookButton, &QPushButton::clicked, [this]() { handleRestockBook(); });
    }
    if (ui->booksActionsGroup) {
        ui->booksActionsGroup->setVisible(adminRole);
    }

    if (booksList) {
        disconnect(booksList, &QListWidget::itemDoubleClicked, this, nullptr);
        connect(booksList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            if (!item) return;
            const QString id = item->data(kCardRoleId).toString();
            auto it = std::find_if(booksCache.begin(), booksCache.end(), [&](const model::Book &b) {
                return toQString(b.getId()) == id;
            });
            if (it != booksCache.end()) {
                showBookDetails(*it);
            }
        });
    }
}

void MainWindow::configureReadersTab() {
    configureCardListWidget(readersList);
    if (readersList) readersList->setItemDelegate(new SimpleCardDelegate(readersList));
    // Use grid/card layout for readers: allow wrapping and IconMode so cards
    // form columns similar to Books/Reports.
    if (readersList) {
        readersList->setFlow(QListView::LeftToRight);
        readersList->setWrapping(true);
        readersList->setViewMode(QListView::IconMode);
        disconnect(readersList, &QListWidget::itemDoubleClicked, this, nullptr);
        connect(readersList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            if (!item) return;
            const QString id = item->data(kCardRoleId).toString();
            auto it = std::find_if(readersCache.begin(), readersCache.end(), [&](const model::Reader &r) {
                return toQString(r.getId()) == id;
            });
            if (it != readersCache.end()) {
                showReaderDetails(*it);
            }
        });
    }

    if (readerSearchEdit) {
        readerSearchEdit->setPlaceholderText(tr("Tìm kiếm bạn đọc (mã, tên, tác giả, thể loại, NXB, trạng thái... )"));
        connect(readerSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyReaderFilter);
    }
    if (readerStatusFilter) {
        readerStatusFilter->clear();
        readerStatusFilter->addItem(tr("Tất cả trạng thái"), QStringLiteral("ALL"));
        readerStatusFilter->addItem(tr("Đang hoạt động"), QStringLiteral("ACTIVE"));
        readerStatusFilter->addItem(tr("Tạm khóa"), QStringLiteral("INACTIVE"));
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
    if (loansList) loansList->setItemDelegate(new SimpleCardDelegate(loansList));

    if (loanStatusFilter) {
        loanStatusFilter->clear();
        loanStatusFilter->addItem(tr("Tất cả tình trạng"), QStringLiteral("ALL"));
        loanStatusFilter->addItem(tr("Đang mượn"), QStringLiteral("BORROWED"));
        loanStatusFilter->addItem(tr("Quá hạn"), QStringLiteral("OVERDUE"));
        loanStatusFilter->addItem(tr("Đã trả"), QStringLiteral("RETURNED"));
        loanStatusFilter->addItem(tr("Mất"), QStringLiteral("LOST"));
        loanStatusFilter->addItem(tr("Hư hỏng"), QStringLiteral("DAMAGED"));
        connect(loanStatusFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyLoanFilter);
    }
    if (loanSearchEdit) {
        loanSearchEdit->setPlaceholderText(tr("Tìm kiếm tất cả phiếu mượn (mã, sách, bạn đọc, trạng thái, ... )"));
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
        ui->extendLoanButton->setToolTip(tr("Chỉ gia hạn khi phiếu đang mượn hoặc quá hạn."));
        connect(ui->extendLoanButton, &QPushButton::clicked, [this]() { handleExtendLoan(); });
    }
    if (ui->lostLoanButton) {
        ui->lostLoanButton->setVisible(staffRole);
        ui->lostLoanButton->setToolTip(tr("Chỉ báo mất khi phiếu đang mượn hoặc quá hạn."));
        connect(ui->lostLoanButton, &QPushButton::clicked, [this]() { handleMarkLost(); });
    }
    if (ui->damageLoanButton) {
        ui->damageLoanButton->setVisible(staffRole);
        ui->damageLoanButton->setToolTip(tr("Chỉ báo hư khi phiếu đang mượn hoặc quá hạn."));
        connect(ui->damageLoanButton, &QPushButton::clicked, [this]() { handleMarkDamaged(); });
    }
    if (loansList) {
        connect(loansList, &QListWidget::currentRowChanged, this, &MainWindow::updateLoanActionButtons);
        connect(loansList, &QListWidget::itemActivated, this, &MainWindow::handleViewLoanReceipt);
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
    if (reportsList) reportsList->setItemDelegate(new SimpleCardDelegate(reportsList));
    // Use grid/card layout for reports: allow wrapping and IconMode so cards
    // form columns. CardListDelegate sizeHint adapts to IconMode to allow
    // multi-column display.
    if (reportsList) {
        reportsList->setFlow(QListView::LeftToRight);
        reportsList->setWrapping(true);
        reportsList->setViewMode(QListView::IconMode);
    }

    if (reportsList) {
        // Chỉ dùng itemActivated để tránh gọi 2 lần (doubleClick + activated)
        connect(reportsList, &QListWidget::itemActivated, this, &MainWindow::handleViewReportDetails);
    }

    if (reportFromFilter) {
        reportFromFilter->setCalendarPopup(true);
        reportFromFilter->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        reportFromFilter->setDateRange(QDate(2000, 1, 1), QDate(7999, 12, 31));
        reportFromFilter->setDate(QDate::currentDate());
        connect(reportFromFilter, &QDateEdit::dateChanged, this, &MainWindow::applyReportFilter);
    }
    if (reportToFilter) {
        reportToFilter->setCalendarPopup(true);
        reportToFilter->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
        reportToFilter->setDateRange(QDate(2000, 1, 1), QDate(7999, 12, 31));
        reportToFilter->setDate(QDate::currentDate());
        connect(reportToFilter, &QDateEdit::dateChanged, this, &MainWindow::applyReportFilter);
    }

    if (reportStaffFilter) {
        reportStaffFilter->setPlaceholderText(tr("Tìm kiếm nhân viên lập báo cáo"));
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
    if (ui->deleteReportButton) {
        ui->deleteReportButton->setVisible(adminRole);
        connect(ui->deleteReportButton, &QPushButton::clicked, this, &MainWindow::handleDeleteReport);
    }
}

void MainWindow::configureAccountsTab() {
    configureCardListWidget(accountsList);
    if (accountsList) accountsList->setItemDelegate(new SimpleCardDelegate(accountsList));

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
    const QWidget *searchWidget = ui->statsTab;

    timePeriodCombo = searchWidget->findChild<QComboBox *>(QStringLiteral("timePeriodCombo"));
    const auto customStartDateEdit = searchWidget->findChild<QDateEdit *>(QStringLiteral("customStartDateEdit"));
    const auto customEndDateEdit = searchWidget->findChild<QDateEdit *>(QStringLiteral("customEndDateEdit"));
    if (customStartDateEdit && customEndDateEdit) {
        customStartDateEdit->setVisible(false);
        customEndDateEdit->setVisible(false);
    }
    if (timePeriodCombo) {
        timePeriodCombo->clear();
        timePeriodCombo->addItem(tr("Hôm nay"));
        timePeriodCombo->addItem(tr("7 ngày gần đây"));
        timePeriodCombo->addItem(tr("Tháng này"));
        timePeriodCombo->addItem(tr("Tháng trước"));
        timePeriodCombo->addItem(tr("Tùy chọn..."));
        timePeriodCombo->setCurrentIndex(2);
        if (customStartDateEdit && customEndDateEdit) {
            connect(timePeriodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](const int idx) {
                const bool custom = (idx == 4);
                customStartDateEdit->setVisible(custom);
                customEndDateEdit->setVisible(custom);
            });
        }
    }
    applyFilterButton = searchWidget->findChild<QPushButton *>(QStringLiteral("applyFilterButton"));
    if (applyFilterButton) {
        connect(applyFilterButton, &QPushButton::clicked, this, &MainWindow::applyStatsFilter);
    }

    const QDate today = QDate::currentDate();
    statsStartDate = QDate(today.year(), today.month(), 1);
    statsEndDate = today;

    if (!statsWidget && ui->statsTab) {
        statsWidget = new StatsWidget(ui->statsTab);
        auto *contentLayout = qobject_cast<QVBoxLayout *>(ui->statsTab->findChild<QVBoxLayout *>(QStringLiteral("statsTabLayout")));
        if (!contentLayout && ui->statsTabLayout) {
            contentLayout = ui->statsTabLayout;
        }
        if (!contentLayout) {
            contentLayout = new QVBoxLayout(ui->statsTab);
            contentLayout->setContentsMargins(12, 12, 12, 12);
            contentLayout->setSpacing(10);
        }
        contentLayout->addWidget(statsWidget);
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
    booksCache = bookService.fetchAll();
    applyBookFilter();

    readersCache = readerService.fetchAll();
    applyReaderFilter();

    if (adminRole) {
        staffsCache = staffService.fetchAll();
        applyStaffFilter();
    } else {
        staffsCache.clear();
    }

    loansCache = loanService.fetchAll();
    applyLoanFilter();

    reportsCache = reportService.fetchAll();
    applyReportFilter();
    
    // Update stats tab
    applyStatsFilter();

    if (adminRole) {
        accountsCache = accountService.fetchAll();
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
    // Lightweight diagnostics to confirm data is loaded from the expected path
    qInfo().noquote() << "[DataDiag] dataDir =" << toQString(dataDirectory)
                      << "| books:" << books
                      << "readers:" << readers
                      << "loans:" << loans
                      << "reports:" << reportsCache.size()
                      << "staff:" << staffsCache.size()
                      << "accounts:" << accountsCache.size();

    if (statsLabel) {
        statsLabel->setText(tr("Sách: %1 | Bạn đọc: %2 | Phiếu mượn: %3 | Giới hạn sách/bạn đọc: %4 | Tiền phạt/ngày: %5 VND")
                                .arg(books)
                                .arg(readers)
                                .arg(loans)
                                .arg(currentConfig.getMaxBooksPerReader())
                                .arg(currentConfig.getFinePerDay()));
    }
    updateStatisticsSummary();
}

void MainWindow::populateBooks() {
    booksCache = bookService.fetchAll();
    applyBookFilter();
}

void MainWindow::fillBooksList(const core::DynamicArray<model::Book> &books) {
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
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < books.size(); ++row) {
        const auto &book = books[row];
        const QString id = toQString(book.getId());
        const QString title = toQString(book.getTitle());
        const QString author = toQString(book.getAuthor());
        const QString publisher = toQString(book.getPublisher());
        const int quantity = book.getQuantity();
        const QString coverPath = bridge::toQString(book.getCoverImagePath());
        const core::CustomString statusKey = model::canonicalBookStatus(book.getStatus());
        const QString statusText = bookStatusText(statusKey);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, tr("Tiêu đề: %1").arg(title.isEmpty() ? tr("...") : title));
        // Sử dụng meta làm mã sách, detail là tác giả để paint delegate bố trí theo thiết kế mẫu
        item->setData(kCardRoleMeta, tr("Mã: %1").arg(id));
        item->setData(kCardRoleDetail, author.isEmpty() ? tr("Tác giả: ...") : tr("Tác giả: %1").arg(author));
        item->setData(kCardRoleSecondaryDetail, tr("Số lượng còn: %1").arg(quantity));
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusText));
        item->setData(kCardRolePillText, tr("SL: %1").arg(quantity));
        // Đặt icon là ảnh bìa nếu có
        if (!coverPath.isEmpty()) {
            QPixmap pix(coverPath);
            item->setIcon(QIcon(pix.scaled(80, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        item->setTextAlignment(Qt::AlignCenter);
        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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

void MainWindow::showBookDetails(const model::Book &book) {
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Chi tiết sách"));
    dlg.setModal(true);
    dlg.setMinimumWidth(560);

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(12);

    auto *topRow = new QHBoxLayout;
    topRow->setSpacing(16);

    auto *coverLabel = new QLabel(&dlg);
    coverLabel->setFixedSize(150, 210);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setStyleSheet(QStringLiteral("background: #3a3232; border: 2px solid #707880;"));
    const QString coverPath = bridge::toQString(book.getCoverImagePath());
    if (!coverPath.isEmpty()) {
        QPixmap pm(coverPath);
        if (!pm.isNull()) {
            coverLabel->setPixmap(pm.scaled(coverLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }
    topRow->addWidget(coverLabel, 0, Qt::AlignTop);

    auto *form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignLeft);
    form->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10);
    const auto setRow = [&](const QString &label, const QString &value) {
        auto *v = new QLabel(value, &dlg);
        v->setWordWrap(true);
        form->addRow(label, v);
    };

    setRow(tr("Mã sách"), toQString(book.getId()));
    setRow(tr("Tiêu đề"), toQString(book.getTitle().isEmpty() ? core::CustomStringLiteral("...") : book.getTitle()));
    setRow(tr("Tác giả"), toQString(book.getAuthor().isEmpty() ? core::CustomStringLiteral("...") : book.getAuthor()));
    setRow(tr("Thể loại"), toQString(book.getGenre()));
    setRow(tr("NXB"), toQString(book.getPublisher()));
    setRow(tr("Tình trạng"), bookStatusText(book.getStatus()));
    setRow(tr("Số lượng"), QString::number(book.getQuantity()));
    setRow(tr("Giá gốc"), formatCurrency(book.getOriginalPrice()));

    topRow->addLayout(form, 1);
    root->addLayout(topRow);

    if (!book.getSummary().trimmed().isEmpty()) {
        auto *summaryLabel = new QLabel(tr("Tóm tắt:"), &dlg);
        summaryLabel->setStyleSheet(QStringLiteral("font-weight: 600;"));
        auto *summaryValue = new QLabel(toQString(book.getSummary()), &dlg);
        summaryValue->setWordWrap(true);
        root->addWidget(summaryLabel);
        root->addWidget(summaryValue);
    }

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    root->addWidget(buttons);

    dlg.exec();
}

void MainWindow::showReaderDetails(const model::Reader &reader) const {
    QDialog dlg(const_cast<MainWindow *>(this));
    dlg.setWindowTitle(tr("Chi tiết bạn đọc"));
    dlg.setModal(true);
    dlg.setMinimumWidth(520);

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);

    auto *form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignLeft);
    form->setHorizontalSpacing(14);
    form->setVerticalSpacing(8);
    const auto addRow = [&](const QString &label, const QString &value) {
        auto *v = new QLabel(value, &dlg);
        v->setWordWrap(true);
        form->addRow(label, v);
    };

    addRow(tr("Mã bạn đọc"), toQString(reader.getId()));
    addRow(tr("Họ tên"), toQString(reader.getFullName()));
    addRow(tr("Giới tính"), toQString(reader.getGender()));
    addRow(tr("Địa chỉ"), toQString(reader.getAddress()));
    addRow(tr("SĐT"), toQString(reader.getPhone()));
    addRow(tr("Email"), toQString(reader.getEmail()));
    addRow(tr("CCCD"), toQString(reader.getIdentityCard()));
    addRow(tr("Ghi chú"), toQString(reader.getNotes()));
    if (reader.getDob().isValid()) addRow(tr("Ngày sinh"), toQDate(reader.getDob()).toString(Qt::ISODate));
    if (reader.getCreatedDate().isValid()) addRow(tr("Ngày đăng ký"), toQDate(reader.getCreatedDate()).toString(Qt::ISODate));
    if (reader.getExpiryDate().isValid()) addRow(tr("Ngày hết hạn"), toQDate(reader.getExpiryDate()).toString(Qt::ISODate));
    addRow(tr("Số lần mượn"), QString::number(reader.getTotalBorrowed()));
    addRow(tr("Trạng thái"), reader.isActive() ? tr("Đang hoạt động") : tr("Tạm khóa"));

    root->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    root->addWidget(buttons);

    dlg.exec();
}

void MainWindow::showStaffDetails(const model::Staff &staff) const {
    QDialog dlg(const_cast<MainWindow *>(this));
    dlg.setWindowTitle(tr("Chi tiết nhân viên"));
    dlg.setModal(true);
    dlg.setMinimumWidth(520);

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);

    auto *form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignLeft);
    form->setHorizontalSpacing(14);
    form->setVerticalSpacing(8);
    const auto addRow = [&](const QString &label, const QString &value) {
        auto *v = new QLabel(value, &dlg);
        v->setWordWrap(true);
        form->addRow(label, v);
    };

    addRow(tr("Mã nhân viên"), toQString(staff.getId()));
    addRow(tr("Họ tên"), toQString(staff.getFullName()));
    addRow(tr("Giới tính"), toQString(staff.getGender()));
    addRow(tr("Địa chỉ"), toQString(staff.getAddress()));
    addRow(tr("SĐT"), toQString(staff.getPhone()));
    addRow(tr("Email"), toQString(staff.getEmail()));
    addRow(tr("Chức vụ"), toQString(staff.getPosition()));
    addRow(tr("Ghi chú"), toQString(staff.getNotes()));
    if (staff.getDob().isValid()) addRow(tr("Ngày sinh"), toQDate(staff.getDob()).toString(Qt::ISODate));
    if (staff.getHireDate().isValid()) addRow(tr("Ngày vào làm"), toQDate(staff.getHireDate()).toString(Qt::ISODate));
    addRow(tr("Trạng thái"), staff.isActive() ? tr("Đang làm việc") : tr("Đã nghỉ việc"));

    root->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    root->addWidget(buttons);

    dlg.exec();
}

void MainWindow::applyBookFilter() {
    core::DynamicArray<model::Book> filtered;
    const QString term = bookSearchEdit ? bookSearchEdit->text().trimmed().toLower() : QString();
    const QString statusFilter = bookStatusFilter ? bookStatusFilter->currentData().toString() : QStringLiteral("ALL");
    for (const auto &book : booksCache) {
        const QString bookStatus = toQString(model::canonicalBookStatus(book.getStatus()));
        if (!term.isEmpty()) {
            const QString haystack = QStringList{
                toQString(book.getId()),
                toQString(book.getTitle()),
                toQString(book.getAuthor()),
                toQString(book.getGenre()),
                toQString(book.getPublisher()),
                QString::number(book.getOriginalPrice()),
                bookStatus
            }.join(' ').toLower();
            if (!containsAllTokens(haystack, term)) continue;
        }
        if (statusFilter != QStringLiteral("ALL") && bookStatus != statusFilter) {
            continue;
        }
        filtered.push_back(book);
    }
    fillBooksList(filtered);
}

void MainWindow::populateReaders() {
    readersCache = readerService.fetchAll();
    applyReaderFilter();
}

void MainWindow::fillReadersList(const core::DynamicArray<model::Reader> &readers) const {
    if (!readersList) return;

    const QListWidgetItem *currentItem = readersList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    readersList->setUpdatesEnabled(false);
    readersList->clear();

    const QFontMetrics metrics(readersList->font());
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < readers.size(); ++row) {
        const auto &r = readers[row];
        const QString id = toQString(r.getId());
        const QString name = toQString(r.getFullName());
        const QString gender = toQString(r.getGender());
        const QString address = toQString(r.getAddress());
        const QString phone = toQString(r.getPhone());
        const QString email = toQString(r.getEmail());
        const QString identityCard = toQString(r.getIdentityCard());
        const QString notes = toQString(r.getNotes());
        const QString dob = r.getDob().isValid() ? toQDate(r.getDob()).toString(Qt::ISODate) : tr("Không rõ");
        const QString created = r.getCreatedDate().isValid() ? toQDate(r.getCreatedDate()).toString(Qt::ISODate) : tr("Không rõ");
        const QString expiry = r.getExpiryDate().isValid() ? toQDate(r.getExpiryDate()).toString(Qt::ISODate) : tr("Không rõ");
        const int totalBorrowed = r.getTotalBorrowed();
        const bool active = r.isActive();
        const QString statusText = active ? tr("Đang hoạt động") : tr("Tạm khóa");
        const QString statusCode = active ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");

        const QString headerLine = name.isEmpty() ? id : name;
        const QString metaLine = tr("Mã: %1").arg(id);
        const QString detailLine = tr("SĐT: %1 | Email: %2").arg(phone.isEmpty() ? tr("?") : phone,
                                                                  email.isEmpty() ? tr("?") : email);
        const QString secondaryLine = tr("CCCD: %1").arg(identityCard.isEmpty() ? tr("?") : identityCard);
        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, secondaryLine);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, tr("Mượn: %1").arg(totalBorrowed));
        item->setData(kCardRolePillColor, QVariant());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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
    core::DynamicArray<model::Reader> filtered;
    filtered.reserve(readersCache.size());
    const QString term = readerSearchEdit ? readerSearchEdit->text().trimmed().toLower() : QString();
    const QString statusSelection = readerStatusFilter ? readerStatusFilter->currentData().toString() : QStringLiteral("ALL");
    const QString statusFilter = statusSelection.isEmpty() ? QStringLiteral("ALL") : statusSelection;
    for (const auto &reader : readersCache) {
        const QString statusText = reader.isActive() ? tr("Đang hoạt động") : tr("Tạm khóa");
        const QString statusCode = reader.isActive() ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");
        if (!term.isEmpty()) {
            const QString haystack = QStringList{
                toQString(reader.getId()),
                toQString(reader.getFullName()),
                toQString(reader.getGender()),
                toQString(reader.getPhone()),
                toQString(reader.getEmail()),
                toQString(reader.getAddress()),
                toQString(reader.getIdentityCard()),
                toQString(reader.getNotes()),
                reader.getCreatedDate().isValid() ? toQDate(reader.getCreatedDate()).toString(Qt::ISODate) : QString(),
                reader.getExpiryDate().isValid() ? toQDate(reader.getExpiryDate()).toString(Qt::ISODate) : QString(),
                QString::number(reader.getTotalBorrowed()),
                statusText,
                statusCode
            }.join(' ').toLower();
            if (!containsAllTokens(haystack, term)) continue;
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
    loansCache = loanService.fetchAll();
    applyLoanFilter();
}

void MainWindow::fillLoansList(const core::DynamicArray<model::Loan> &loans) {
    if (!loansList) return;

    const QListWidgetItem *currentItem = loansList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    loansList->setUpdatesEnabled(false);
    loansList->clear();

    const QFontMetrics metrics(loansList->font());
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    auto readerDisplay = [&](const QString &readerId) -> QString {
        QString name;
        for (const auto &reader : readersCache) {
            if (const QString readerIdText = toQString(reader.getId()); readerIdText.compare(readerId, Qt::CaseInsensitive) == 0) {
                name = toQString(reader.getFullName());
                break;
            }
        }
        if (name.isEmpty()) return readerId;
        return tr("%1 (%2)").arg(name, readerId);
    };

    auto bookDisplay = [&](const QString &bookId) -> QString {
        QString title;
        for (const auto &book : booksCache) {
            if (toQString(book.getId()).compare(bookId, Qt::CaseInsensitive) == 0) {
                title = toQString(book.getTitle());
                break;
            }
        }
        if (title.isEmpty()) return bookId;
        return tr("%1 (%2)").arg(title, bookId);
    };

    int restoreRow = -1;
    for (int row = 0; row < loans.size(); ++row) {
        const auto &loan = loans[row];
        const QString loanId = toQString(loan.getLoanId());
        const QString readerId = toQString(loan.getReaderId());
        const QString bookId = toQString(loan.getBookId());
        const QString staffUsername = toQString(loan.getStaffUsername()).trimmed();
        const QString statusCode = normalizedStatus(toQString(loan.getStatus()));
        const QString statusText = loanStatusText(toQString(loan.getStatus()));

        const QString borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()).toString(Qt::ISODate) : tr("Không rõ");
        const QString dueDate = loan.getDueDate().isValid() ? toQDate(loan.getDueDate()).toString(Qt::ISODate) : tr("Không rõ");
        const QString returnDate = loan.getReturnDate().isValid() ? toQDate(loan.getReturnDate()).toString(Qt::ISODate) : tr("Chưa trả");

        const QString headerLine = tr("%1 - %2").arg(loanId, bookDisplay(bookId));
        const QString staffDisplay = staffUsername.isEmpty() ? tr("Không rõ") : staffUsername;
        const QString metaLine = tr("Bạn đọc: %1 | NV: %2").arg(readerDisplay(readerId), staffDisplay);
        const QString detailLine = tr("Mượn: %1 | Hạn: %2").arg(borrowDate, dueDate);
        QString extraDetail;
        if (statusCode == QStringLiteral("OVERDUE")) {
            if (const QDate due = loan.getDueDate().isValid() ? toQDate(loan.getDueDate()) : QDate(); due.isValid()) {
                const int daysLate = max(0, static_cast<int>(due.daysTo(QDate::currentDate())));
                extraDetail = daysLate > 0
                                  ? tr("Quá hạn: %1 ngày").arg(daysLate)
                                  : tr("Quá hạn");
            } else {
                extraDetail = tr("Quá hạn");
            }
        } else {
            extraDetail = tr("Trả: %1").arg(returnDate);
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
        item->setData(kCardRolePillText, tr("Tiền phạt: %1 VND").arg(loan.getFine()));
        item->setData(kCardRolePillColor, QVariant());
        item->setToolTip(QStringList{headerLine,
                                     metaLine,
                                     detailLine,
                                     extraDetail,
                                     tr("Trạng thái: %1").arg(statusText),
                                     tr("Nhân viên lập phiếu: %1").arg(staffDisplay)}
                             .join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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

    updateLoanActionButtons();
}

void MainWindow::applyLoanFilter() {
    core::DynamicArray<model::Loan> filtered;
    filtered.reserve(loansCache.size());
    const QString term = loanSearchEdit ? loanSearchEdit->text().trimmed().toLower() : QString();
    const QString statusFilter = loanStatusFilter ? loanStatusFilter->currentData().toString() : QStringLiteral("ALL");
    const auto readerNameById = [&](const QString &id) -> QString {
        for (const auto &r : readersCache) {
            if (toQString(r.getId()).compare(id, Qt::CaseInsensitive) == 0) {
                return toQString(r.getFullName());
            }
        }
        return {};
    };
    const auto bookTitleById = [&](const QString &id) -> QString {
        for (const auto &b : booksCache) {
            if (toQString(b.getId()).compare(id, Qt::CaseInsensitive) == 0) {
                return toQString(b.getTitle());
            }
        }
        return {};
    };
    for (const auto &loan : loansCache) {
        const QString loanId = toQString(loan.getLoanId());
        const QString readerId = toQString(loan.getReaderId());
        const QString bookId = toQString(loan.getBookId());
        const QString staffUsername = toQString(loan.getStaffUsername());
        const QString statusCode = normalizedStatus(toQString(loan.getStatus()));
        const QString statusText = loanStatusText(toQString(loan.getStatus()));
        const QString readerName = readerNameById(readerId);
        const QString bookTitle = bookTitleById(bookId);

        if (!term.isEmpty()) {
            const QString haystack = QStringList{
                                         loanId,
                                         readerId,
                                         readerName,
                                         bookId,
                                         bookTitle,
                                         staffUsername,
                                         statusCode,
                                         statusText}
                                         .join(' ')
                                         .toLower();
            if (!containsAllTokens(haystack, term)) continue;
        }
        if (statusFilter != QStringLiteral("ALL") && statusCode != statusFilter) {
            continue;
        }
        filtered.push_back(loan);
    }
    fillLoansList(filtered);
    updateLoanActionButtons();
}

void MainWindow::updateLoanActionButtons() {
    const bool enable = staffRole;
    if (ui->lostLoanButton) ui->lostLoanButton->setEnabled(enable);
    if (ui->damageLoanButton) ui->damageLoanButton->setEnabled(enable);
    if (ui->extendLoanButton) ui->extendLoanButton->setEnabled(enable);
}

void MainWindow::populateReports() {
    reportsCache = reportService.fetchAll();
    applyReportFilter();
}

void MainWindow::fillReportsList(const core::DynamicArray<model::ReportRequest> &reports) const {
    if (!reportsList) return;

    const QListWidgetItem *currentItem = reportsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    reportsList->setUpdatesEnabled(false);
    reportsList->clear();

    const QFontMetrics metrics(reportsList->font());
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < reports.size(); ++row) {
        const auto &report = reports[row];
        const QString requestId = toQString(report.getRequestId());
        const QString staff = toQString(report.getStaffUsername());
        const QString fromDate = report.getFromDate().isValid() ? toQDate(report.getFromDate()).toString(Qt::ISODate) : tr("Không rõ");
        const QString toDate = report.getToDate().isValid() ? toQDate(report.getToDate()).toString(Qt::ISODate) : tr("Không rõ");
        const QString createdAt = report.getCreatedAt().isValid() ? toQDateTime(report.getCreatedAt()).toString(Qt::ISODate) : tr("Không rõ");
        const QString statusCode = normalizedStatus(toQString(report.getStatus()));
        const QString statusText = reportStatusText(toQString(report.getStatus()));
        const QString notes = toQString(report.getNotes()).trimmed();

        const QString headerLine = tr("%1 - %2").arg(requestId, staff.isEmpty() ? tr("Nhân viên không rõ") : staff);
        const QString metaLine = tr("Từ: %1 | Đến: %2").arg(fromDate, toDate);
        const QString detailLine = tr("Xử lý: %1 | Mất/Hư: %2 | Quá hạn: %3")
                                      .arg(report.getHandledLoans())
                                      .arg(report.getLostOrDamaged())
                                      .arg(report.getOverdueReaders());
        const QString extraDetail = tr("Tạo: %1").arg(createdAt);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, requestId);
        item->setData(kCardRoleId, requestId);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, extraDetail);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, tr("Ngày tạo: %1").arg(createdAt));
        item->setData(kCardRolePillColor, QVariant());
        QStringList tooltipLines{headerLine, metaLine, detailLine, tr("Trạng thái: %1").arg(statusText), extraDetail};
        if (!notes.isEmpty()) {
            tooltipLines.append(tr("Ghi chú: %1").arg(notes));
        }
        item->setToolTip(tooltipLines.join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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

void MainWindow::fillAccountsList(const core::DynamicArray<model::Account> &accounts) {
    if (!accountsList) return;

    const QListWidgetItem *currentItem = accountsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    accountsList->setUpdatesEnabled(false);
    accountsList->clear();

    const QFontMetrics metrics(accountsList->font());
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    auto staffSummary = [&](const QString &staffId) -> QString {
        if (staffId.isEmpty()) return tr("Chưa gán nhân viên");
        QString name;
        for (const auto &staff : staffsCache) {
            if (const QString staffCode = toQString(staff.getId()); staffCode.compare(staffId, Qt::CaseInsensitive) == 0) {
                name = toQString(staff.getFullName());
                break;
            }
        }
        if (name.isEmpty()) return staffId;
        return tr("%1 - %2").arg(staffId, name);
    };

    int restoreRow = -1;
    for (int row = 0; row < accounts.size(); ++row) {
        const auto &acc = accounts[row];
        const QString username = toQString(acc.getUsername());
        const QString accountId = toQString(acc.getAccountId());
        const QString role = toQString(acc.getRole());
        const QString staffId = toQString(acc.getEmployeeId()).trimmed();
        const QString staffDisplay = staffSummary(staffId);
        const QString createdAt = acc.getCreatedAt().isValid()
                                      ? toQDateTime(acc.getCreatedAt()).toString(Qt::ISODate)
                                      : tr("Không rõ");
        const QString lastLogin = acc.getLastLogin().isValid()
                                      ? toQDateTime(acc.getLastLogin()).toString(Qt::ISODate)
                                      : tr("Chưa đăng nhập");
        const QString& headerLine = username;
        const QString metaLine = tr("Vai trò: %1 | Mã tài khoản: %2")
                                      .arg(role.isEmpty() ? tr("Không rõ") : role, accountId.isEmpty() ? tr("Không rõ") : accountId);
        const QString detailLine = tr("Nhân viên liên kết: %1").arg(staffDisplay);
        const QString extraDetail = tr("Tạo: %1 | Đăng nhập cuối: %2").arg(createdAt, lastLogin);

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
        const QString passwordHash = toQString(acc.getPasswordHash());
        item->setToolTip(QStringList{headerLine,
                                     metaLine,
                                     detailLine,
                                     tr("Trạng thái: %1").arg(badgeText),
                                     extraDetail,
                                     tr("Hash mậtt khẩu: %1").arg(passwordHash.isEmpty()
                                                                     ? tr("(không rõ)")
                                                                     : passwordHash)}
                             .join('\n'));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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
    core::DynamicArray<model::ReportRequest> filtered;
    filtered.reserve(reportsCache.size());
    const QString staffTerm = reportStaffFilter ? reportStaffFilter->text().trimmed().toLower() : QString();
    const bool hasFrom = reportFromFilter && reportFromFilter->date() != reportFromFilter->minimumDate();
    const bool hasTo = reportToFilter && reportToFilter->date() != reportToFilter->minimumDate();
    const QDate from = hasFrom ? reportFromFilter->date() : QDate();
    const QDate to = hasTo ? reportToFilter->date() : QDate();

    const auto staffNameByUsername = [&](const QString &username) -> QString {
        for (const auto &s : staffsCache) {
            if (toQString(s.getId()).compare(username, Qt::CaseInsensitive) == 0 ||
                toQString(s.getEmail()).compare(username, Qt::CaseInsensitive) == 0) {
                return toQString(s.getFullName());
            }
        }
        return {};
    };

    for (const auto &req : reportsCache) {
        const QString staffUsername = toQString(req.getStaffUsername());
        const QString statusCode = normalizedStatus(toQString(req.getStatus()));
        const QString statusText = reportStatusText(toQString(req.getStatus()));
        const QString notes = toQString(req.getNotes());
        const QString staffName = staffNameByUsername(staffUsername);
        const QString haystack = QStringList{
                                     toQString(req.getRequestId()),
                                     staffUsername,
                                     staffName,
                                     statusCode,
                                     statusText,
                                     notes,
                                 }
                                     .join(' ')
                                     .toLower();
        const bool fromValid = req.getFromDate().isValid();
        const bool toValid = req.getToDate().isValid();
        const QDate reqFrom = fromValid ? toQDate(req.getFromDate()) : QDate();
        const QDate reqTo = toValid ? toQDate(req.getToDate()) : QDate();

        if (!staffTerm.isEmpty() && !containsAllTokens(haystack, staffTerm)) continue;
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
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một báo cáo."));
        return;
    }
    const QListWidgetItem *item = reportsList->item(rowOpt.value());
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }
    QString requestId = item->data(kCardRoleId).toString();
    if (requestId.isEmpty()) {
        requestId = item->data(Qt::UserRole).toString();
    }
    if (requestId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }

    const model::ReportRequest *selectedReport = nullptr;
    for (auto & i : reportsCache) {
        if (const auto &report = i; toQString(report.getRequestId()).compare(requestId, Qt::CaseInsensitive) == 0) {
            selectedReport = &i;
            break;
        }
    }
    if (!selectedReport) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy báo cáo đã chọn."));
        return;
    }

    const QString statusText = reportStatusText(toQString(selectedReport->getStatus()));
    ReportDetailsDialog dialog(*selectedReport, statusText, booksCache, this);
    dialog.exec();
}

void MainWindow::updateStatisticsSummary() { refreshSimpleStats(); }

void MainWindow::refreshSimpleStats() {
    // Use the same logic as when the user clicks the filter button
    // so date range, cards, charts and the dashboard widget stay in sync.
    applyStatsFilter();
}

void MainWindow::updateStatsCards() {
    if (!statsWidget) return;

    const QDate today = QDate::currentDate();
    const auto monthStart = QDate(today.year(), today.month(), 1);

    const int totalBooks = booksCache.size();
    const int totalReaders = readersCache.size();

    int totalLoans = 0;
    int overdueLoans = 0;
    qint64 totalFines = 0;

    for (const auto &loan : loansCache) {
        const QDate borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()) : QDate();
        if (!borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) continue;

        totalLoans++;

        const bool hasReturn = loan.getReturnDate().isValid();
        const QDate dueDate = loan.getDueDate().isValid() ? toQDate(loan.getDueDate()) : QDate();
        if (dueDate.isValid() && !hasReturn && today > dueDate) {
            overdueLoans++;
        }

        totalFines += max(0, loan.getFine());
    }

    statsWidget->updateStats(totalBooks, totalReaders, totalLoans, overdueLoans, totalFines);
}

void MainWindow::updateStatsCharts() {
    // Charts are now rendered inside StatsWidget; reuse dashboard updater.
    updateStatsDashboardWidget();
}

void MainWindow::updateStatsDashboardWidget() {
    if (!statsWidget) return;

    const QDate today = QDate::currentDate();
    const int totalBooks = booksCache.size();
    const int totalReaders = readersCache.size();
    const int totalLoans = loansCache.size();

    int overdueLoans = 0;
    qint64 totalFines = 0;

    // Aggregate high‑level numbers
    for (const auto &loan : loansCache) {
        const QDate dueDate = loan.getDueDate().isValid() ? toQDate(loan.getDueDate()) : QDate();
        const bool hasReturn = loan.getReturnDate().isValid();

        const QString status = normalizedStatus(loan.getStatus());
        const bool isBorrowed = status == QStringLiteral("BORROWED");
        const bool isOverdueStatus = status == QStringLiteral("OVERDUE");

        if (const bool overdueByDate = (!hasReturn && dueDate.isValid() && today > dueDate); isOverdueStatus || overdueByDate) {
            ++overdueLoans;
        }

        totalFines += max(0, loan.getFine());
    }

    statsWidget->updateStats(totalBooks, totalReaders, totalLoans, overdueLoans, totalFines);

    // Borrow counts by category (genre) within the current filter range
    core::Map<QString, int> categoryCounts;
    QSet<QString> knownGenres;
    // Khởi tạo tất cả thể loại từ kho sách (đếm 0) để luôn hiển thị tên thể loại
    for (const auto &book : booksCache) {
        QString genre = toQString(book.getGenre()).trimmed();
        if (genre.isEmpty()) continue;
        const QString keyLower = genre.toLower();
        if (knownGenres.contains(keyLower)) continue;
        knownGenres.insert(keyLower);
        categoryCounts[genre] = 0;
    }

    for (const auto &loan : loansCache) {
        // Skip loans outside the selected date range or without a valid borrow date
        if (const QDate borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()) : QDate(); !borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
            continue;
        }

        const QString bookId = toQString(loan.getBookId());
        QString bookGenre;
        for (const auto &book : booksCache) {
            if (toQString(book.getId()) == bookId) {
                bookGenre = toQString(book.getGenre());
                break;
            }
        }
        if (bookGenre.isEmpty()) {
            bookGenre = tr("Khác");
        }
        const QString keyLower = bookGenre.toLower();
        if (!knownGenres.contains(keyLower)) {
            knownGenres.insert(keyLower);
            categoryCounts[bookGenre] = 0;
        }

        const int current = categoryCounts.value(bookGenre, 0);
        categoryCounts[bookGenre] = current + 1;
    }
    statsWidget->updateCategoryChart(categoryCounts);

    // Top borrowed books (by title) within the current filter range
    core::Map<QString, int> bookBorrowCounts;
    for (const auto &loan : loansCache) {
        // Skip loans outside the selected date range or without a valid borrow date
        if (const QDate borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()) : QDate(); !borrowDate.isValid() || borrowDate < statsStartDate || borrowDate > statsEndDate) {
            continue;
        }

        const QString bookId = toQString(loan.getBookId());
        QString title;
        for (const auto &book : booksCache) {
            if (toQString(book.getId()) == bookId) {
                title = toQString(book.getTitle());
                break;
            }
        }
        if (title.isEmpty()) {
            title = bookId;
        }

        const int current = bookBorrowCounts.value(title, 0);
        bookBorrowCounts[title] = current + 1;
    }
    statsWidget->updateTopBooksChart(bookBorrowCounts);

    // Monthly borrow counts for the current year
    int monthlyRaw[12] = {0};
    for (const auto &loan : loansCache) {
        const QDate borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()) : QDate();
        if (!borrowDate.isValid() || borrowDate.year() != today.year()) {
            continue;
        }
        if (const int monthIndex = borrowDate.month() - 1; monthIndex >= 0 && monthIndex < 12) {
            ++monthlyRaw[monthIndex];
        }
    }

    core::DynamicArray<int> monthlyCounts;
    for (int i : monthlyRaw) {
        monthlyCounts.append(i);
    }
    statsWidget->updateMonthlyChart(monthlyCounts);
}

void MainWindow::applyStatsFilter() {
    const QDate today = QDate::currentDate();
    // Get selected time period
    const QString timePeriod = timePeriodCombo ? timePeriodCombo->currentText() : QStringLiteral("Hôm nay");
    // Get selected genre if available
    QString selectedGenre;
    if (!genreFilterCombo) {
        const QWidget *searchWidget = ui->statsTab;
        genreFilterCombo = searchWidget->findChild<QComboBox *>(QStringLiteral("genreFilterCombo"));
    }
    if (genreFilterCombo) selectedGenre = genreFilterCombo->currentText();
    // Store for chart update
    this->statsSelectedGenre = selectedGenre;
    // Calculate date range
    QDate startDate;
    QDate endDate = today;
    // Lấy 2 QDateEdit custom nếu có
    const QWidget *searchWidget = ui->statsTab;
    const auto *customStartDateEdit = searchWidget->findChild<QDateEdit *>(QStringLiteral("customStartDateEdit"));
    const auto *customEndDateEdit = searchWidget->findChild<QDateEdit *>(QStringLiteral("customEndDateEdit"));

    if (timePeriod == QStringLiteral("Hôm nay")) {
        startDate = today;
    } else if (timePeriod == QStringLiteral("7 ngày gần đây")) {
        startDate = today.addDays(-6);
    } else if (timePeriod == QStringLiteral("Tháng này")) {
        startDate = QDate(today.year(), today.month(), 1);
    } else if (timePeriod == QStringLiteral("Tháng trước")) {
        int prevMonth = today.month() - 1;
        int year = today.year();
        if (prevMonth == 0) { prevMonth = 12; year--; }
        startDate = QDate(year, prevMonth, 1);
        endDate = QDate(year, prevMonth, QDate(year, prevMonth, 1).daysInMonth());
    } else if (timePeriod == QStringLiteral("Tùy chọn...") && customStartDateEdit && customEndDateEdit) {
        startDate = customStartDateEdit->date();
        endDate = customEndDateEdit->date();
    } else {
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
    maxBorrowDaysSpin->setValue(max(1, currentConfig.getMaxBorrowDays()));
    finePerDaySpin->setValue(max(0, currentConfig.getFinePerDay()));
    maxBooksPerReaderSpin->setValue(max(1, currentConfig.getMaxBooksPerReader()));
}

core::Optional<int> MainWindow::currentRow(const QListWidget *list) {
    if (!list) return core::nullopt;
    const int row = list->currentRow();
    if (row < 0) return core::nullopt;
    return core::Optional(row);
}

QString MainWindow::nextBookId() const {
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(booksCache.size());
    for (const auto &book : booksCache) {
        ids.pushBack(book.getId());
    }
    return toQString(core::IdGenerator::nextId(ids, core::CustomStringLiteral("B"), 3));
}

QString MainWindow::nextReaderId() const {
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(readersCache.size());
    for (const auto &reader : readersCache) {
        ids.pushBack(reader.getId());
    }
    return toQString(core::IdGenerator::nextId(ids, core::CustomStringLiteral("R"), 3));
}

QString MainWindow::nextStaffId() const {
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(staffsCache.size());
    for (const auto &s : staffsCache) {
        ids.pushBack(s.getId());
    }
    return toQString(core::IdGenerator::nextId(ids, core::CustomStringLiteral("S"), 3));
}

QString MainWindow::nextLoanId() const {
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(loansCache.size());
    for (const auto &loan : loansCache) {
        ids.pushBack(loan.getLoanId());
    }
    return toQString(core::IdGenerator::nextId(ids, core::CustomStringLiteral("L"), 3));
}

void MainWindow::handleAddBook() {
    if (!adminRole) return;
    BookDialog dialog(this);
    dialog.presetId(nextBookId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto newBook = dialog.book();
    if (const auto exists = ranges::any_of(as_const(booksCache), [&](const model::Book &b) { return b.getId() == newBook.getId(); })) {
        showWarningDialog(tr("Trùng lặp"), tr("Mã sách này đã tồn tại."));
        return;
    }
    if (!bookService.addBook(newBook)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể thêm sách mới."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã thêm sách mới."), EventSeverity::Success, 2000);
}

void MainWindow::configureStaffsTab() {
    configureCardListWidget(staffsList);
    if (staffsList) staffsList->setItemDelegate(new SimpleCardDelegate(staffsList));

    if (staffSearchEdit) {
        connect(staffSearchEdit, &QLineEdit::textChanged, this, &MainWindow::applyStaffFilter);
    }
    if (staffStatusFilter) {
        staffStatusFilter->clear();
        staffStatusFilter->addItem(tr("Tất cả trạng thái"), QStringLiteral("ALL"));
        staffStatusFilter->addItem(tr("Đang làm việc"), QStringLiteral("ACTIVE"));
        staffStatusFilter->addItem(tr("Đã nghỉ việc"), QStringLiteral("INACTIVE"));
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

        disconnect(staffsList, &QListWidget::itemDoubleClicked, this, nullptr);
        connect(staffsList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            if (!item) return;
            const QString id = item->data(kCardRoleId).toString();
            auto it = std::find_if(staffsCache.begin(), staffsCache.end(), [&](const model::Staff &s) {
                return toQString(s.getId()) == id;
            });
            if (it != staffsCache.end()) {
                showStaffDetails(*it);
            }
        });
    }
}

void MainWindow::populateStaffs() {
    staffsCache = staffService.fetchAll();
    applyStaffFilter();
}

void MainWindow::fillStaffsList(const core::DynamicArray<model::Staff> &staffs) const {
    if (!staffsList) return;

    const QListWidgetItem *currentItem = staffsList->currentItem();
    const QString selectedId = currentItem ? currentItem->data(kCardRoleId).toString() : QString();

    staffsList->setUpdatesEnabled(false);
    staffsList->clear();

    const QFontMetrics metrics(staffsList->font());
    const int minimumHeight = max(metrics.lineSpacing() * 4 + 24, 120);

    int restoreRow = -1;
    for (int row = 0; row < staffs.size(); ++row) {
        const auto &s = staffs[row];
        const QString id = toQString(s.getId());
        const QString name = toQString(s.getFullName());
        const QString gender = toQString(s.getGender());
        const QString address = toQString(s.getAddress());
        const QString phone = toQString(s.getPhone());
        const QString email = toQString(s.getEmail());
        const QString position = toQString(s.getPosition());
        const QString notes = toQString(s.getNotes());
        const QString dob = s.getDob().isValid() ? toQDate(s.getDob()).toString(Qt::ISODate) : tr("Không rõ");
        const QString hireDate = s.getHireDate().isValid() ? toQDate(s.getHireDate()).toString(Qt::ISODate) : tr("Không rõ");
        const bool active = s.isActive();
        const QString statusText = active ? tr("Đang làm việc") : tr("Đã nghỉ việc");
        const QString statusCode = active ? QStringLiteral("ACTIVE") : QStringLiteral("INACTIVE");

        const QString headerLine = name.isEmpty() ? id : name;
        const QString metaLine = tr("Mã: %1").arg(id);
        const QString detailLine = tr("Chức vụ: %1 | SĐT: %2")
                                        .arg(position.isEmpty() ? tr("?") : position,
                                             phone.isEmpty() ? tr("?") : phone);
        const QString secondaryLine = tr("Email: %1").arg(email.isEmpty() ? tr("?") : email);

        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setData(kCardRoleId, id);
        item->setData(kCardRoleHeader, headerLine);
        item->setData(kCardRoleMeta, metaLine);
        item->setData(kCardRoleDetail, detailLine);
        item->setData(kCardRoleSecondaryDetail, secondaryLine);
        item->setData(kCardRoleBadgeText, statusText);
        item->setData(kCardRoleBadgeColor, statusBadgeColor(statusCode));
        item->setData(kCardRolePillText, position.isEmpty() ? QString() : position);
        item->setData(kCardRolePillColor, QVariant());

        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QSize hint = item->sizeHint();
        hint.setHeight(max(hint.height(), minimumHeight));
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
    core::DynamicArray<model::Staff> filtered;
    filtered.reserve(staffsCache.size());
    const QString term = staffSearchEdit ? staffSearchEdit->text().trimmed().toLower() : QString();
    const QString statusCode = staffStatusFilter ? staffStatusFilter->currentData().toString() : QStringLiteral("ALL");
    const QString statusFilter = statusCode.isEmpty() ? QStringLiteral("ALL") : statusCode;
    for (const auto &staff : staffsCache) {
        if (!term.isEmpty()) {
        const QString haystack = QStringList{toQString(staff.getId()),
                         toQString(staff.getFullName()),
                         toQString(staff.getGender()),
                         toQString(staff.getPhone()),
                         toQString(staff.getEmail()),
                         toQString(staff.getAddress()),
                         toQString(staff.getPosition()),
                         toQString(staff.getNotes()),
                         staff.getHireDate().isValid() ? toQDate(staff.getHireDate()).toString(Qt::ISODate) : QString(),
                         staff.getDob().isValid() ? toQDate(staff.getDob()).toString(Qt::ISODate) : QString()}
                     .join(' ')
                     .toLower();
            if (!containsAllTokens(haystack, term)) continue;
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
    if (ranges::any_of(as_const(staffsCache), [&](const model::Staff &s) { return s.getId() == staff.getId(); })) {
        showWarningDialog(tr("Trùng lặp"), tr("Mã nhân viên đã tồn tại."));
        return;
    }
    if (!staffService.addStaff(staff)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể thêm nhân viên."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã thêm nhân viên."), EventSeverity::Success, 2000);
}

void MainWindow::handleEditStaff() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một nhân viên."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định nhân viên đã chọn."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định nhân viên đã chọn."));
        return;
    }
    const auto staffKey = toCustomString(staffId);
    const auto staffOpt = staffService.findById(staffKey);
    if (!staffOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy nhân viên đã chọn."));
        return;
    }
    StaffDialog dialog(this);
    dialog.setStaff(*staffOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto updated = dialog.staff();
    if (!staffService.updateStaff(updated)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật nhân viên."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã cập nhật nhân viên."), EventSeverity::Success, 2000);
}

void MainWindow::handleDeleteStaff() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một nhân viên."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định nhân viên đã chọn."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định nhân viên đã chọn."));
        return;
    }
    const auto staffKey = toCustomString(staffId);
    // Check if any account is linked to this staff
    const bool linked = ranges::any_of(as_const(accountsCache), [&](const model::Account &acc) {
        return toQString(acc.getEmployeeId()).compare(staffId, Qt::CaseInsensitive) == 0;
    });
    if (linked) {
        showWarningDialog(tr("Ràng buộc"), tr("Không thể xóa: Nhân viên đang được gắn với một tài khoản."));
        return;
    }
    if (askEventQuestion(tr("Xác nhận"), tr("Bạn có chắc muốn xóa nhân viên %1?").arg(staffId)) != QMessageBox::Yes) return;
    if (!staffService.removeStaff(staffKey)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa nhân viên."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã xóa nhân viên."), EventSeverity::Success, 2000);
}

void MainWindow::handleToggleStaffActive() {
    if (!adminRole) return;
    const auto row = currentRow(staffsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một nhân viên."));
        return;
    }
    const QListWidgetItem *item = staffsList ? staffsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định nhân viên đã chọn."));
        return;
    }
    QString staffId = item->data(kCardRoleId).toString();
    if (staffId.isEmpty()) {
        staffId = item->data(Qt::UserRole).toString();
    }
    if (staffId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Khổng thể xác định nhân viên đã chọn."));
        return;
    }
    const auto staffKey = toCustomString(staffId);
    const auto staffOpt = staffService.findById(staffKey);
    if (!staffOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy nhân viên đã chọn."));
        return;
    }
    const bool newActive = !staffOpt->isActive();
    const QString confirmMessage = newActive
        ? tr("Bạn có chắc chắn muốn mở tài khoản nhân viên %1?").arg(staffId)
        : tr("Bạn cso chắc chắn muốn khóa tài khoản nhân viên %1?").arg(staffId);
    if (askEventQuestion(tr("Xác nhận"), confirmMessage) != QMessageBox::Yes) return;
    if (!staffService.setStaffActive(staffKey, newActive)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật trạng thái nhân viên."));
        return;
    }
    reloadData();
    const QString message = newActive ? tr("Đã mở tài khoản nhân viên %1.").arg(staffId)
                                      : tr("Đã khóa tài khoản nhân viên %1.").arg(staffId);
    notifyEvent(message, EventSeverity::Success, 2000);
}

void MainWindow::handleEditBook() {
    if (!adminRole) return;
    const auto row = currentRow(booksList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một cuốn sách."));
        return;
    }
    const QListWidgetItem *item = booksList->item(row.value());
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    QString bookId = item->data(kCardRoleId).toString();
    if (bookId.isEmpty()) {
        bookId = item->data(Qt::UserRole).toString();
    }
    if (bookId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    const auto bookKey = toCustomString(bookId);
    const auto bookOpt = bookService.findById(bookKey);
    if (!bookOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy sách đã chọn"));
        return;
    }
    BookDialog dialog(this);
    dialog.setBook(*bookOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    if (const auto updated = dialog.book(); !bookService.updateBook(updated)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật thông tin sách."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã cập nhật thông tin sách."), EventSeverity::Success, 2000);
}

void MainWindow::handleDeleteBook() {
    if (!adminRole) return;
    const auto row = currentRow(booksList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một cuốn sách."));
        return;
    }
    const QListWidgetItem *item = booksList->item(row.value());
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    QString bookId = item->data(kCardRoleId).toString();
    if (bookId.isEmpty()) {
        bookId = item->data(Qt::UserRole).toString();
    }
    if (bookId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    const auto bookKey = toCustomString(bookId);
    if (askEventQuestion(tr("Xác nhận"), tr("Bạn có chắc chắn muốn xóa sách %1?").arg(bookId)) != QMessageBox::Yes) return;
    if (!bookService.removeBook(bookKey)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa sách."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã xóa sách."), EventSeverity::Success, 2000);
}

void MainWindow::handleRestockBook() {
    if (!adminRole) return;
    const auto row = currentRow(booksList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một cuốn sách."));
        return;
    }
    const QListWidgetItem *item = booksList ? booksList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    QString bookId = item->data(kCardRoleId).toString();
    if (bookId.isEmpty()) {
        bookId = item->data(Qt::UserRole).toString();
    }
    if (bookId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định sách đã chọn."));
        return;
    }
    const auto bookKey = toCustomString(bookId);
    const auto bookOpt = bookService.findById(bookKey);
    if (!bookOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy sách đã chọn"));
        return;
    }

    bool ok = false;
    const int amount = QInputDialog::getInt(
        this,
        tr("Nhập hàng"),
        tr("Số lượng nhập thêm"),
        10,
        1,
        100000,
        1,
        &ok);
    if (!ok || amount <= 0) return;

    model::Book updated = *bookOpt;
    updated.setQuantity(updated.getQuantity() + amount);
    if (!bookService.updateBook(updated)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật thông tin sách."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã nhập thêm %1 bản cho %2.").arg(amount).arg(bookId), EventSeverity::Success, 2000);
}

void MainWindow::handleAddReader() {
    if (!staffRole) return;
    ReaderDialog dialog(this);
    dialog.presetId(nextReaderId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto reader = dialog.reader();
    if (ranges::any_of(as_const(readersCache), [&](const model::Reader &r) { return r.getId() == reader.getId(); })) {
        showWarningDialog(tr("Trùng lặp"), tr("Mã bạn đọc này đã tồn tại."));
        return;
    }
    if (!readerService.addReader(reader)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể thêm bạn đọc."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã thêm bạn đọc."), EventSeverity::Success, 2000);
}

void MainWindow::handleEditReader() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một bạn đọc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    const auto readerKey = toCustomString(readerId);
    const auto readerOpt = readerService.findById(readerKey);
    if (!readerOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể tìm thấy bạn đọc đã chọn."));
        return;
    }
    ReaderDialog dialog(this);
    dialog.setReader(*readerOpt, true);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto updated = dialog.reader();
    if (!readerService.updateReader(updated)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật bạn đọc."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã cập nhật bạn đọc."), EventSeverity::Success, 2000);
}

void MainWindow::handleDeleteReader() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một bạn đọc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    const auto readerKey = toCustomString(readerId);
    
    // Check if reader has active loans (BORROWED status)
    const bool hasActiveLoans = ranges::any_of(as_const(loansCache), [&](const model::Loan &loan) {
        return toQString(loan.getReaderId()) == readerId &&
               normalizedStatus(loan.getStatus()) == QStringLiteral("BORROWED");
    });
    
    if (hasActiveLoans) {
        showWarningDialog(tr("Ràng buộc"), 
            tr("Không thể xóa: Bạn đọc đang có phiếu mượn chưa trả."));
        return;
    }
    
    if (askEventQuestion(tr("Xác nhận"),
                         tr("Bạn có chắc chắn muốn xóa bạn đọc %1?").arg(readerId)) != QMessageBox::Yes) {
        return;
    }
    
    if (!readerService.removeReader(readerKey)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa bạn đọc."));
        return;
    }
    
    reloadData();
    notifyEvent(tr("Đã xóa bạn đọc."), EventSeverity::Success, 2000);
}

void MainWindow::handleToggleReaderActive() {
    if (!staffRole) return;
    const auto row = currentRow(readersList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một bạn đọc."));
        return;
    }
    const QListWidgetItem *item = readersList ? readersList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    QString readerId = item->data(kCardRoleId).toString();
    if (readerId.isEmpty()) {
        readerId = item->data(Qt::UserRole).toString();
    }
    if (readerId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định bạn đọc đã chọn."));
        return;
    }
    const auto readerKey = toCustomString(readerId);
    const auto readerOpt = readerService.findById(readerKey);
    if (!readerOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy bạn đọc đã chọn."));
        return;
    }
    const bool newActive = !readerOpt->isActive();
    const QString confirmMessage = newActive
        ? tr("Bạn có chắc chắn muốn mở khóa bạn đọc %1?").arg(readerId)
        : tr("Bạn có chắc chắn muốn khóa thẻ bạn đọc %1?").arg(readerId);
    if (askEventQuestion(tr("Xác nhận"), confirmMessage) != QMessageBox::Yes) return;
    if (!readerService.setReaderActive(readerKey, newActive)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật trạng thái bạn đọc."));
        return;
    }
    reloadData();
    const QString message = newActive ? tr("Đã mở khóa tài khoản bạn đọc %1.").arg(readerId)
                                      : tr("Đã tạm khóa bạn đọc %1.").arg(readerId);
    notifyEvent(message, EventSeverity::Success, 2000);
}

void MainWindow::handleNewLoan() {
    if (!staffRole) return;
    if (readersCache.isEmpty() || booksCache.isEmpty()) {
        showWarningDialog(tr("Thiếu dữ liệu"), tr("Cần có ít nhất một bạn đọc và một cuốn sách."));
        return;
    }
    core::DynamicArray<model::Reader> availableReaders;
    availableReaders.reserve(readersCache.size());
    for (const auto &reader : readersCache) {
        if (reader.isActive()) availableReaders.append(reader);
    }
    if (availableReaders.isEmpty()) {
        showWarningDialog(tr("Không khả dụng"), tr("Không có độc giả đang hoạt động để tạo phiếu mượn."));
        return;
    }
    const QString staffDisplay = toQString(currentAccount.getUsername()).trimmed();
    LoanDialog dialog(availableReaders, booksCache, currentConfig.getMaxBorrowDays(), staffDisplay, this);
    dialog.presetLoanId(nextLoanId(), true);
    if (dialog.exec() != QDialog::Accepted) return;
    auto loan = dialog.loan();
    loan.setStaffUsername(currentAccount.getUsername());
    if (loanService.findById(loan.getLoanId()).has_value()) {
        showWarningDialog(tr("Trùng lặp"), tr("Mã phiếu này đã tồn tại."));
        return;
    }
    const auto bookOpt = bookService.findById(loan.getBookId());
    if (!bookOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy sách đã chọn"));
        return;
    }
    if (bookOpt->getQuantity() <= 0) {
        showWarningDialog(tr("Không khả dụng"), tr("Sách này đã hết số lượng cho mượn."));
        return;
    }
    loan.setStatus(core::CustomStringLiteral("BORROWED"));
    loan.setFine(0);
    if (!loanService.createLoan(loan)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể tạo phiếu mượn."));
        return;
    }

    model::Book updatedBook = *bookOpt;
    updatedBook.setQuantity(max(0, updatedBook.getQuantity() - 1));
    if (!bookService.updateBook(updatedBook)) {
        loanService.removeLoan(loan.getLoanId());
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật số lượng sách."));
        return;
    }

    reloadData();
    notifyEvent(tr("Đã tạo phiếu mượn."), EventSeverity::Success, 2000);
}

void MainWindow::handleMarkReturned() {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một phiếu mượn."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    const auto loanKey = toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy phiểu mượn."));
        return;
    }
    if (normalizedStatus(loanOpt->getStatus()) == QStringLiteral("RETURNED")) {
        showInfoDialog(tr("Thông báo"), tr("Phiếu này đã được đóng."));
        return;
    }

    const QDate returnDate = QDate::currentDate();
    const auto dueDate = toQDate(loanOpt->getDueDate());
    const qint64 overdueSpan = dueDate.daysTo(returnDate);
    const int lateDays = overdueSpan > 0 ? static_cast<int>(overdueSpan) : 0;
    const int fine = lateDays * currentConfig.getFinePerDay();

    if (!loanService.updateStatus(loanKey, core::CustomStringLiteral("RETURNED"), toCoreDate(returnDate))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật trạng thái phiếu."));
        return;
    }
    if (!loanService.applyFine(loanKey, fine)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật tiền phạt."));
        return;
    }

    const auto bookOpt = bookService.findById(loanOpt->getBookId());
    if (bookOpt.has_value()) {
        model::Book updatedBook = *bookOpt;
        updatedBook.setQuantity(updatedBook.getQuantity() + 1);
        if (!bookService.updateBook(updatedBook)) {
            showWarningDialog(tr("Cảnh báo"), tr("Không thể cập nhật số lượng sách."));
        }
    }

    reloadData();
    notifyEvent(tr("Đã đóng phiếu. Tiền phạt: %1 VND").arg(fine), EventSeverity::Success, 3000);
}

void MainWindow::handleExtendLoan() {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một phiếu mượn."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    bool ok = false;
    const auto loanKey = toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy phiếu mượn."));
        return;
    }
    const QString statusCode = normalizedStatus(loanOpt->getStatus());
    if (statusCode != QStringLiteral("BORROWED") && statusCode != QStringLiteral("OVERDUE")) {
        showInfoDialog(tr("Thông báo"), tr("Phiếu này đã đóng, không thể gia hạn."));
        return;
    }

    const int days = QInputDialog::getInt(this, tr("Gia hạn phiếu"), tr("Số ngày bổ sung"), 3, 1, 60, 1, &ok);
    if (!ok) return;
    model::Loan updatedLoan = *loanOpt;
    const QDate adjustedDueDate = toQDate(updatedLoan.getDueDate()).addDays(days);
    updatedLoan.setDueDate(toCoreDate(adjustedDueDate));
    updatedLoan.setStatus(core::CustomStringLiteral("BORROWED"));
    if (!loanService.updateLoan(updatedLoan)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể gia hạn phiếu."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã gia hạn phiếu."), EventSeverity::Success, 2000);
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
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một phiều mượn."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }

    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    const auto loanKey = toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy phiếu mượn."));
        return;
    }
    
    const QString loanStatus = normalizedStatus(loanOpt->getStatus());
    
    // If loan is still BORROWED, need to return the book quantity
    if (loanStatus == QStringLiteral("BORROWED")) {
        if (askEventQuestion(
                tr("Xác nhận"),
                tr("Phiếu mượn này đang ở trạng thái 'Đang mượn'. Xóa sẽ tự động hoàn trả sách. Bạn có chắc chắn?")) !=
            QMessageBox::Yes) {
            return;
        }
        
        // Return book quantity
        // handled after removal
    } else {
        if (askEventQuestion(tr("Xác nhận"), tr("Bạn có chắc chắn muốn xóa phiếu mượn %1?").arg(loanId)) !=
            QMessageBox::Yes) {
            return;
        }
    }

    if (!loanService.removeLoan(loanKey)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa phiếu mượn."));
        return;
    }

    if (loanStatus == QStringLiteral("BORROWED")) {
        const auto bookOpt = bookService.findById(loanOpt->getBookId());
        if (bookOpt.has_value()) {
            model::Book updatedBook = *bookOpt;
            updatedBook.setQuantity(updatedBook.getQuantity() + 1);
            if (!bookService.updateBook(updatedBook)) {
                showWarningDialog(tr("Cảnh báo"), tr("Không thể cập nhật số lượng sách."));
            }
        }
    }

    reloadData();
    notifyEvent(tr("Đã xóa phiếu mượn."), EventSeverity::Success, 2000);
}

void MainWindow::handleLossOrDamage(const QString &status) {
    if (!staffRole) return;
    const auto row = currentRow(loansList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một phiếu mượn."));
        return;
    }
    const QListWidgetItem *item = loansList ? loansList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    const auto loanKey = toCustomString(loanId);
    const auto loanOpt = loanService.findById(loanKey);
    if (!loanOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy phiếu mượn."));
        return;
    }
    const QString previousStatus = normalizedStatus(loanOpt->getStatus());
    if (previousStatus != QStringLiteral("BORROWED") && previousStatus != QStringLiteral("OVERDUE")) {
        showInfoDialog(tr("Thông báo"), tr("Phiếu này đã được đóng hoặc báo mất/hư."));
        return;
    }
    const bool alreadyDamaged = previousStatus == QStringLiteral("DAMAGED");
    const bool isDamaged = status == QStringLiteral("DAMAGED");

    bool ok = false;
    QString reason = QInputDialog::getMultiLineText(this,
                                                   status == QStringLiteral("LOST") ? tr("Báo mất sách") : tr("Báo hư hỏng"),
                                                   tr("Mô tả chi tiết"),
                                                   QString(),
                                                   &ok).trimmed();
    if (!ok) return;
    if (reason.isEmpty()) {
        reason = tr("Không có ghi chú bổ sung");
    }

    const QDate today = QDate::currentDate();
    int fee = 0;
    if (status == QStringLiteral("LOST")) {
        // Tiền đền bù mất sách = giá gốc * 2 + tiền trễ (nếu có)
        const auto dueDate = toQDate(loanOpt->getDueDate());
        const qint64 overdueSpan = dueDate.daysTo(today);
        const int lateDays = overdueSpan > 0 ? static_cast<int>(overdueSpan) : 0;
        const int lateFee = lateDays * currentConfig.getFinePerDay();

        int bookPrice = 0;
        const auto bookOpt = bookService.findById(loanOpt->getBookId());
        if (bookOpt.has_value()) {
            bookPrice = max(0, bookOpt->getOriginalPrice());
        }
        fee = bookPrice * 2 + lateFee;
        ok = true;
    } else {
        int defaultFee = currentConfig.getFinePerDay() * 10;
        if (defaultFee <= 0) defaultFee = 100000;
        fee = QInputDialog::getInt(this,
                                   tr("Tiền sửa chữa"),
                                   tr("Nhập số tiền VND"),
                                   defaultFee,
                                   0,
                                   10000000,
                                   1000,
                                   &ok);
    }
    if (!ok) return;

    const auto statusKey = toCustomString(status);
    if (!loanService.updateStatus(loanKey, statusKey, toCoreDate(today))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật trạng thái phiếu."));
        return;
    }
    if (!loanService.applyFine(loanKey, fee)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật tiền phạt."));
        return;
    }

    model::ReportRequest req;
    const QString prefix = status == QStringLiteral("LOST") ? QStringLiteral("MẤT") : QStringLiteral("HƯ");
    const QString requestId = QStringLiteral("%1-%2")
                                  .arg(prefix, QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmss")));
    req.setRequestId(toCustomString(requestId));
    req.setStaffUsername(currentAccount.getUsername());
    req.setFromDate(toCoreDate(QDate::currentDate()));
    req.setToDate(toCoreDate(QDate::currentDate()));
    req.setHandledLoans(1);
    req.setLostOrDamaged(1);
    req.setOverdueReaders(0);
    const QString affectedId = toQString(loanOpt->getBookId()).trimmed();
    if (!affectedId.isEmpty()) {
        req.setAffectedBooks(toCustomString(QStringLiteral("%1:1").arg(affectedId)));
    }
    const QString statusLabel = status == QStringLiteral("LOST") ? tr("Mất sách") : tr("Hư hỏng");
    req.setNotes(toCustomString(
        tr("%1 - phiếu %2 - lý do: %3 - tiền đề xuất: %4 VND")
            .arg(statusLabel, loanId, reason)
            .arg(fee)));
    req.setStatus(core::CustomStringLiteral("PENDING"));
    req.setCreatedAt(toCoreDateTime(QDateTime::currentDateTime()));
    if (!reportService.submitRequest(req)) {
        showWarningDialog(tr("Cảnh báo"), tr("Không thể ghi nhận báo cáo."));
    }

    reloadData();
    notifyEvent(tr("Đã ghi nhận %1.").arg(statusLabel.toLower()), EventSeverity::Success, 3000);

    // Disable thao tác lại trên item vừa thao tác (nếu có)
    if (loansList && row.has_value()) {
        if (QListWidgetItem *list_widget_item = loansList->item(row.value())) {
            list_widget_item->setFlags(list_widget_item->flags() & ~Qt::ItemIsEnabled);
        }
    }
    updateLoanActionButtons();
}

void MainWindow::handleViewLoanReceipt() {
    if (!loansList) return;
    const QListWidgetItem *item = loansList->currentItem();
    if (!item) return;

    QString loanId = item->data(kCardRoleId).toString();
    if (loanId.isEmpty()) {
        loanId = item->data(Qt::UserRole).toString();
    }
    if (loanId.isEmpty()) return;

    const auto loanOpt = loanService.findById(toCustomString(loanId));
    if (!loanOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định phiếu mượn đã chọn"));
        return;
    }
    const auto loan = *loanOpt;

    auto findReader = [&](const core::CustomString &id) -> core::Optional<model::Reader> {
        const QString target = toQString(id);
        for (const auto &r : readersCache) {
            if (toQString(r.getId()).compare(target, Qt::CaseInsensitive) == 0) return core::Optional<model::Reader>(r);
        }
        return readerService.findById(id);
    };
    auto findBook = [&](const core::CustomString &id) -> core::Optional<model::Book> {
        const QString target = toQString(id);
        for (const auto &b : booksCache) {
            if (toQString(b.getId()).compare(target, Qt::CaseInsensitive) == 0) return core::Optional<model::Book>(b);
        }
        return bookService.findById(id);
    };

    const auto readerOpt = findReader(loan.getReaderId());
    const auto bookOpt = findBook(loan.getBookId());

    const QString readerName = readerOpt.has_value() ? toQString(readerOpt->getFullName()) : tr("Không rõ");
    const QString readerId = toQString(loan.getReaderId());
    const QString readerContact = readerOpt.has_value() ? toQString(readerOpt->getPhone()) : tr("?");
    const QString statusText = loanStatusText(toQString(loan.getStatus()));
    const QString borrowDate = loan.getBorrowDate().isValid() ? toQDate(loan.getBorrowDate()).toString(QStringLiteral("dd/MM/yyyy"))
                                                              : tr("Không rõ");
    const QString dueDate = loan.getDueDate().isValid() ? toQDate(loan.getDueDate()).toString(QStringLiteral("dd/MM/yyyy"))
                                                        : tr("Không rõ");
    const QString returnDate = loan.getReturnDate().isValid() ? toQDate(loan.getReturnDate()).toString(QStringLiteral("dd/MM/yyyy"))
                                                              : tr("Chưa trả");
    const QString staffUsername = toQString(loan.getStaffUsername()).isEmpty() ? tr("Không rõ")
                                                                               : toQString(loan.getStaffUsername());
    const QString bookId = toQString(loan.getBookId());
    const QString bookTitle = bookOpt.has_value() ? toQString(bookOpt->getTitle()) : tr("Không rõ");
    const QString bookPublisher = bookOpt.has_value() ? toQString(bookOpt->getPublisher()) : tr("Không rõ");
    const QString noteText = tr("Trạng thái: %1 | Hạn trả: %2 | Ngày trả: %3")
                                 .arg(statusText, dueDate, returnDate);

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Phiếu mượn %1").arg(loanId));
    dlg.setModal(true);
    dlg.setMinimumWidth(720);

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto *title = new QLabel(tr("PHIẾU MƯỢN SÁCH"), &dlg);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(QStringLiteral("font-weight: 700; font-size: 18px; letter-spacing: 1px; color: #1d3b8b;"));
    root->addWidget(title);

    auto *infoGrid = new QGridLayout;
    infoGrid->setHorizontalSpacing(12);
    infoGrid->setVerticalSpacing(8);
    const auto addField = [&](int row, int col, const QString &label, const QString &value) {
        auto *lbl = new QLabel(label, &dlg);
        lbl->setStyleSheet(QStringLiteral("color: #1d3b8b; font-weight: 600;"));
        auto *val = new QLabel(value, &dlg);
        val->setStyleSheet(QStringLiteral("font-weight: 600; color: #0f172a; border-bottom: 1px dashed #1d3b8b; padding-bottom: 4px;"));
        val->setWordWrap(true);
        infoGrid->addWidget(lbl, row, col * 2);
        infoGrid->addWidget(val, row, col * 2 + 1);
    };

    addField(0, 0, tr("Số phiếu:"), loanId);
    addField(0, 1, tr("Ngày mượn:"), borrowDate);
    addField(1, 0, tr("Bạn đọc:"), readerName);
    addField(1, 1, tr("Mã bạn đọc:"), readerId);
    addField(2, 0, tr("SĐT:"), readerContact);
    addField(2, 1, tr("Nhân viên lập:"), staffUsername);
    addField(3, 0, tr("Hạn trả:"), dueDate);
    addField(3, 1, tr("Trạng thái:"), statusText);
    addField(4, 0, tr("Tiền phạt:"), formatCurrency(loan.getFine()));
    addField(4, 1, tr("Ngày trả:"), returnDate);

    root->addLayout(infoGrid);

    auto *divider = new QFrame(&dlg);
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    divider->setStyleSheet(QStringLiteral("color: #8ea0d5;"));
    root->addWidget(divider);

    auto *table = new QTableWidget(1, 5, &dlg);
    table->setHorizontalHeaderLabels({tr("STT"), tr("Mã sách"), tr("Tên sách"), tr("NXB"), tr("Ghi chú")});
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setAlternatingRowColors(true);
    table->setFixedHeight(120);
    table->setStyleSheet(
        "QTableWidget {"
        " border: 1px solid #8ea0d5;"
        " gridline-color: #8ea0d5;"
        " alternate-background-color: #e7f7e7;"
        " selection-background-color: #dbeafe;"
        "}"
        "QHeaderView::section {"
        " background: #e7ecff;"
        " color: #1d3b8b;"
        " font-weight: 700;"
        " border: 1px solid #8ea0d5;"
        " padding: 6px;"
        "}"
        "QTableWidget::item { padding: 6px; }");

    table->setItem(0, 0, new QTableWidgetItem(QStringLiteral("1")));
    table->setItem(0, 1, new QTableWidgetItem(bookId));
    table->setItem(0, 2, new QTableWidgetItem(bookTitle));
    table->setItem(0, 3, new QTableWidgetItem(bookPublisher));
    table->setItem(0, 4, new QTableWidgetItem(noteText));
    table->resizeRowsToContents();

    root->addWidget(table);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    root->addWidget(buttons);

    dlg.exec();
}

void MainWindow::handleSubmitReport() {
    if (!staffRole) return;
    core::DynamicArray<core::CustomString> bookIds;
    bookIds.reserve(booksCache.size());
    for (const auto &book : booksCache) {
        bookIds.append(book.getId());
    }
    ReportRequestDialog dialog(toQString(currentAccount.getUsername()), bookIds, this);
    dialog.setLoansData(loansCache);
    if (dialog.exec() != QDialog::Accepted) return;
    if (!reportService.submitRequest(dialog.reportRequest())) {
        showWarningDialog(tr("Không thành công"), tr("Không thể gửi báo cáo."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã gửi báo cáo."), EventSeverity::Success, 2000);
}

bool MainWindow::applyBookAdjustmentsForReport(const model::ReportRequest &report, QStringList *errors) {
    const auto changes = parseAffectedBookChanges(toQString(report.getAffectedBooks()));
    const QString reportId = toQString(report.getRequestId()).toUpper();
    const bool damageReport = reportId.startsWith(QStringLiteral("HƯ-")) || reportId.startsWith(QStringLiteral("HU-")) || reportId.contains(QStringLiteral("DAMAGED"));
    const bool lostReport = reportId.startsWith(QStringLiteral("MẤT-")) || reportId.startsWith(QStringLiteral("MAT-")) || reportId.contains(QStringLiteral("LOST"));
    bool ok = true;
    for (const auto &change : changes) {
        const auto bookOpt = bookService.findById(toCustomString(change.id));
        if (!bookOpt.has_value()) {
            ok = false;
            if (errors) errors->append(tr("Không tìm thấy sách %1.").arg(change.id));
            continue;
        }
        model::Book updated = *bookOpt;
        int delta = -change.count;  // Default: trừ số lượng đã báo
        if (damageReport) {
            delta = change.count;  // Hư: trả lại số lượng sau khi được duyệt
        } else if (lostReport) {
            delta = 0 ;     // Mất:mất sách không thay đổi số lượng
        }
        if (delta != 0) {
            const int newQty = max(0, updated.getQuantity() + delta);
            updated.setQuantity(newQty);
            if (model::isAvailabilityStatus(updated.getStatus())) {
                updated.setStatus(model::availabilityStatusForQuantity(newQty));
            }
            if (!bookService.updateBook(updated)) {
                ok = false;
                if (errors) errors->append(tr("Không thể cập nhật sách %1.").arg(change.id));
            }
        }
    }
    return ok;
}

void MainWindow::handleReportStatusChange(const QString &status) {
    if (!adminRole) return;
    const auto row = currentRow(reportsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một báo cáo."));
        return;
    }
    const QListWidgetItem *item = reportsList ? reportsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }
    QString requestId = item->data(kCardRoleId).toString();
    if (requestId.isEmpty()) {
        requestId = item->data(Qt::UserRole).toString();
    }
    if (requestId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }

    const model::ReportRequest *selectedReport = nullptr;
    for (const auto &report : reportsCache) {
        if (toQString(report.getRequestId()).compare(requestId, Qt::CaseInsensitive) == 0) {
            selectedReport = &report;
            break;
        }
    }
    if (!selectedReport) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể tìm thấy dữ liệu báo cáo."));
        return;
    }

    const QString targetStatus = normalizedStatus(status);
    if (targetStatus.isEmpty()) {
        showWarningDialog(tr("Không thành công"), tr("Trạng thái không hợp lệ."));
        return;
    }
    const QString previousStatus = normalizedStatus(selectedReport->getStatus());

    if (!reportService.updateStatus(toCustomString(requestId), toCustomString(targetStatus))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật trạng thái."));
        return;
    }

    if (targetStatus == QStringLiteral("APPROVED") && previousStatus != QStringLiteral("APPROVED")) {
        QStringList errors;
        if (!applyBookAdjustmentsForReport(*selectedReport, &errors)) {
            // Roll back status if we could not update inventory
            const bool rolledBack = reportService.updateStatus(toCustomString(requestId), selectedReport->getStatus());
            if (!rolledBack) {
                errors.append(tr("Không thể khôi phục trạng thái báo cáo sau khi lỗi."));
            }
            reloadData();
            showWarningDialog(tr("Không thành công"),
                              errors.isEmpty() ? tr("Không thể cập nhật số lượng sách.")
                                               : errors.join('\n'));
            return;
        }
    }

    reloadData();
    notifyEvent(tr("Đã cập nhật trạng thái báo cáo."), EventSeverity::Success, 2000);
}

void MainWindow::handleDeleteReport() {
    if (!adminRole) return;
    const auto row = currentRow(reportsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một báo cáo."));
        return;
    }
    const QListWidgetItem *item = reportsList ? reportsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }
    QString requestId = item->data(kCardRoleId).toString();
    if (requestId.isEmpty()) {
        requestId = item->data(Qt::UserRole).toString();
    }
    if (requestId.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định báo cáo đã chọn."));
        return;
    }

    if (askEventQuestion(tr("Xác nhận"),
                         tr("Bạn có chắc muốn xóa báo cáo %1? Hành động này không thể hoàn tác.")
                             .arg(requestId)) != QMessageBox::Yes) {
        return;
    }

    if (!reportService.removeItem(toCustomString(requestId))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa báo cáo."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã xóa báo cáo."), EventSeverity::Success, 2000);
}

void MainWindow::handleLogout() {
    const auto reply = askEventQuestion(
        tr("Đăng xuất"),
        tr("Bạn có chắc chắn muốn đăng xuất khỏi hệ thống?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No  // Default to No to prevent accidental logout
    );

    if (reply != QMessageBox::Yes) return;

    // Show a brief "logging out" message
    QApplication::setOverrideCursor(Qt::WaitCursor);
    notifyEvent(tr("Đang đăng xuất..."), EventSeverity::Info, 0);
    
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
            if (const QString candidate = QStringLiteral(":/icons/%1.%2").arg(baseName, ext); QFile::exists(candidate)) {
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

    core::DynamicArray<NavEntry> entries;
    entries.reserve(tabs->count());
    for (int i = 0; i < tabs->count(); ++i) {
        const QString tabText = tabs->tabText(i);
        QString iconName;
        QString description;
        if (const QString normalized = tabText.trimmed().toLower();
            normalized.contains(QStringLiteral("trang chu")) || normalized.contains(QStringLiteral("trang chủ"))) {
            iconName = QStringLiteral("home");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("sach")) || normalized.contains(QStringLiteral("sách"))) {
            iconName = QStringLiteral("book");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("doc gia")) || normalized.contains(QStringLiteral("độc giả"))) {
            iconName = QStringLiteral("reader");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("phieu")) || normalized.contains(QStringLiteral("phiếu")) ||
                   normalized.contains(QStringLiteral("muon")) || normalized.contains(QStringLiteral("mượn"))) {
            iconName = QStringLiteral("loan");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("bao cao")) || normalized.contains(QStringLiteral("báo cáo"))) {
            iconName = QStringLiteral("report");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("thong ke")) || normalized.contains(QStringLiteral("thống kê")) ||
                   normalized.contains(QStringLiteral("tong quan")) || normalized.contains(QStringLiteral("tổng quan"))) {
            iconName = QStringLiteral("t");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("nhan vien")) || normalized.contains(QStringLiteral("nhân viên"))) {
            iconName = QStringLiteral("staff");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("tai khoan")) || normalized.contains(QStringLiteral("tài khoản"))) {
            iconName = QStringLiteral("account");
            description = tr("");
        } else if (normalized.contains(QStringLiteral("cau hinh")) || normalized.contains(QStringLiteral("cấu hình")) ||
                   normalized.contains(QStringLiteral("thiet lap")) || normalized.contains(QStringLiteral("thiết lập")) ||
                   normalized.contains(QStringLiteral("cai dat")) || normalized.contains(QStringLiteral("cài đặt"))) {
            iconName = QStringLiteral("setting");
            description = tr("");
        } else {
            iconName = QStringLiteral("menu");
        }

        entries.push_back({tabText, description, iconName});
    }

    for (const auto &[title, description, iconName] : entries) {
        const QString wrappedText = description.isEmpty()
                                        ? title
                                        : QStringLiteral("%1\n%2").arg(title, description);
        auto *item = new QListWidgetItem(wrappedText, navigationList);
        // Make sure tooltip contains the full title so the user can see it
        // when navigation is collapsed (icon-only).
        item->setToolTip(title);
        if (const QString iconPath = resolveIconPath(iconName); !iconPath.isEmpty()) {
            item->setIcon(QIcon(iconPath));
        }
        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        item->setSizeHint(QSize(220, 56));
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
        if (auto *railLayout = qobject_cast<QVBoxLayout *>(navRail->layout())) {
            // Top spacer
            railLayout->addSpacing(6);
            // create buttons for each nav entry
            for (int i = 0; i < entries.size(); ++i) {
                const NavEntry &e = entries.at(i);
                auto *b = new QToolButton(navRail);
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
            auto *reloadBtn = new QPushButton(tr("Tải lại"), navRail);
            reloadBtn->setFixedHeight(36);
            connect(reloadBtn, &QPushButton::clicked, [this]() {
                reloadData();
                notifyEvent(tr("Đã tải lại."), EventSeverity::Success, 1500);
            });
            railLayout->addWidget(reloadBtn);
            auto *logoutBtn = new QPushButton(tr("Đăng xuất"), navRail);
            logoutBtn->setFixedHeight(36);
            connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);
            railLayout->addWidget(logoutBtn);
            railLayout->addSpacing(6);
        }
    }

    repositionNavRailButton();
}

void MainWindow::setNavigationCollapsed(const bool collapsed, const bool pinned) {
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
        const int buttonX = topLeft.x() + max(8, railWidth / 2 - navRailButton->width() / 2);
        const int buttonY = topLeft.y() + 8;
        navRailButton->move(buttonX, buttonY);
        navRailButton->raise();
    }
}

void MainWindow::showNavigationPopupMenu() {
    if (!navRailButton || !navigationList) return;

    QMenu menu(this);
    for (int i = 0; i < navigationList->count(); ++i) {
        const QListWidgetItem *item = navigationList->item(i);
        if (!item) continue;
        const QString title = item->text().split('\n').value(0);
        const QAction *action = menu.addAction(title);
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
    staffsCache = staffService.fetchAll();
    dialog.setStaffList(staffsCache);
    if (dialog.exec() != QDialog::Accepted) return;
    const auto usernameKey = dialog.username();
    if (accountService.findByUsername(usernameKey).has_value()) {
        showWarningDialog(tr("Trùng lặp"), tr("Tên đăng nhập đã tồn tại."));
        return;
    }
    const QString staffId = toQString(dialog.staffId()).trimmed();
    core::Optional<core::CustomString> staffKey;
    if (!staffId.isEmpty()) {
        staffKey = toCustomString(staffId);
        if (!staffService.findById(staffKey.value()).has_value()) {
            showWarningDialog(tr("Không hợp lệ"), tr("Nhân viên không tồn tại."));
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
        showWarningDialog(tr("Không thành công"), tr("Không thể tạo tài khoản."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã tạo tài khoản mới."), EventSeverity::Success, 2000);
}

void MainWindow::handleResetPassword() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một tài khoản."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    bool ok = false;
    const QString newPassword = QInputDialog::getText(this, tr("Đặt lại mật khẩu"), tr("Nhập mật khẩu mới"), QLineEdit::Password, QString(), &ok);
    if (!ok || newPassword.isEmpty()) return;
    if (!accountService.updatePassword(toCustomString(username), toCustomString(newPassword))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể đặt lại mật khẩu."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã đặt lại mật khẩu."), EventSeverity::Success, 2000);
}

void MainWindow::handleToggleAccountActive() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một tài khoản."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    const auto usernameKey = toCustomString(username);
    const auto accountOpt = accountService.findByUsername(usernameKey);
    if (!accountOpt.has_value()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không tìm thấy tài khoản."));
        return;
    }
    model::Account updated = *accountOpt;
    updated.setActive(!updated.isActive());
    const QString confirmMessage = updated.isActive()
        ? tr("Bạn có chắc chắn muốn mở tài khoản %1?").arg(username)
        : tr("Bạn có chắc chắn muốn khóa tài khoản %1?").arg(username);
    if (askEventQuestion(tr("Xác nhận"), confirmMessage) != QMessageBox::Yes) return;
    if (!accountService.updateAccount(updated)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể cập nhật tài khoản."));
        return;
    }
    reloadData();
    const QString statusMessage = updated.isActive()
        ? tr("Đã mở khóa tài khoản %1.").arg(username)
        : tr("Đã khóa tài khoản %1.").arg(username);
    notifyEvent(statusMessage, EventSeverity::Success, 2000);
}

void MainWindow::handleDeleteAccount() {
    if (!adminRole) return;
    const auto row = currentRow(accountsList);
    if (!row.has_value()) {
        showInfoDialog(tr("Cảnh báo"), tr("Vui lòng chọn một tài khoản."));
        return;
    }
    const QListWidgetItem *item = accountsList ? accountsList->item(row.value()) : nullptr;
    if (!item) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    QString username = item->data(kCardRoleId).toString();
    if (username.isEmpty()) {
        username = item->data(Qt::UserRole).toString();
    }
    if (username.isEmpty()) {
        showWarningDialog(tr("Không tìm thấy"), tr("Không thể xác định tài khoản đã chọn."));
        return;
    }
    if (username == toQString(currentAccount.getUsername())) {
        showWarningDialog(tr("Không hợp lệ"), tr("Không thể xóa tài khoản đang đăng nhập."));
        return;
    }
    if (askEventQuestion(tr("Xác nhận"), tr("Bạn có chắc chắn muốn xóa tài khoản %1?").arg(username)) != QMessageBox::Yes) {
        return;
    }
    if (!accountService.removeAccount(toCustomString(username))) {
        showWarningDialog(tr("Không thành công"), tr("Không thể xóa tài khoản."));
        return;
    }
    reloadData();
    notifyEvent(tr("Đã xóa tài khoản."), EventSeverity::Success, 2000);
}

void MainWindow::handleSaveConfig() {
    if (!adminRole || !maxBorrowDaysSpin) return;
    model::SystemConfig cfg;
    cfg.setMaxBorrowDays(maxBorrowDaysSpin->value());
    cfg.setFinePerDay(finePerDaySpin->value());
    cfg.setMaxBooksPerReader(maxBooksPerReaderSpin->value());
    if (!configService.save(cfg)) {
        showWarningDialog(tr("Không thành công"), tr("Không thể lưu cấu hình."));
        return;
    }
    currentConfig = cfg;
    notifyEvent(tr("Đã lưu cấu hình."), EventSeverity::Success, 2000);
}

}  // namespace pbl2::ui
