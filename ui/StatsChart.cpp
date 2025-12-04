#include "StatsChart.h"

#include <QLineF>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QtMath>
#include <cmath>

namespace pbl2::ui {

StatsChart::StatsChart(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumHeight(200);
}

void StatsChart::setMode(const Mode mode) {
    if (mode_ == mode) return;
    mode_ = mode;
    update();
}

void StatsChart::setTitle(const QString &title) {
    if (title_ == title) return;
    title_ = title;
    update();
}

void StatsChart::setCategories(const QStringList &categories) {
    categories_ = categories;
    update();
}

void StatsChart::setSeries(const core::DynamicArray<Series> &series) {
    series_ = series;
    update();
}

void StatsChart::setValueSuffix(const QString &suffix) {
    valueSuffix_ = suffix;
    update();
}

void StatsChart::setAxisLabels(const QString &xLabel, const QString &yLabel) {
    bool changed = false;
    if (xAxisLabel_ != xLabel) {
        xAxisLabel_ = xLabel;
        changed = true;
    }
    if (yAxisLabel_ != yLabel) {
        yAxisLabel_ = yLabel;
        changed = true;
    }
    if (changed) update();
}

void StatsChart::setShowLegend(const bool show) {
    if (showLegend_ == show) return;
    showLegend_ = show;
    update();
}

void StatsChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    drawBackground(painter);

    if (categories_.isEmpty() || series_.isEmpty()) {
        painter.setPen(QPen(QColor(0x7E, 0x8A, 0x9A)));
        painter.drawText(rect(), Qt::AlignCenter, QObject::tr("Không có dữ liệu"));
        return;
    }

    const int leftMargin = mode_ == Mode::HorizontalBar ? 120 : 64;
    const int rightMargin = 32;
    const int topMargin = title_.isEmpty() ? 16 : 42;
    const int bottomMargin = mode_ == Mode::HorizontalBar ? 26 : 60;

    const QRect plotArea = rect().adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);
    if (plotArea.width() <= 0 || plotArea.height() <= 0) return;

    if (mode_ != Mode::HorizontalBar) {
        drawAxes(painter, plotArea);
    }

    switch (mode_) {
    case Mode::Bar:
        drawBarChart(painter, plotArea);
        break;
    case Mode::Line:
        drawLineChart(painter, plotArea);
        break;
    case Mode::StackedBar:
        drawStackedBarChart(painter, plotArea);
        break;
    case Mode::HorizontalBar:
        drawHorizontalBarChart(painter, plotArea);
        break;
    }

    if (showLegend_) {
        drawLegend(painter, plotArea);
    }
    drawTitle(painter, rect().adjusted(leftMargin, 0, -rightMargin, -plotArea.height()));
}

void StatsChart::drawBackground(QPainter &painter) const {
    painter.fillRect(rect(), QColor(0xF5, 0xFA, 0xFF));
    painter.setPen(QPen(QColor(0xE5, 0xE7, 0xEB)));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void StatsChart::drawTitle(QPainter &painter, const QRect &area) const {
    if (title_.isEmpty()) return;
    QFont f = painter.font();
    f.setBold(true);
    f.setPointSize(qMax(10, f.pointSize() + 1));
    painter.setFont(f);
    painter.setPen(QColor(0x0B, 0x2B, 0x52));

    const QRect titleRect = area.adjusted(0, 8, 0, 0);
    painter.drawText(titleRect, Qt::AlignCenter, title_);
}

void StatsChart::drawAxes(QPainter &painter, const QRect &plotArea) const {
    painter.setPen(QPen(QColor(0x9C, 0xA3, 0xAF), 2));
    constexpr int axisExtension = 12;
    const QPointF origin = plotArea.bottomLeft();
    const QPointF xAxisEnd = QPointF(plotArea.right() + axisExtension, plotArea.bottom());
    const QPointF yAxisEnd = QPointF(plotArea.left(), plotArea.top() - axisExtension);
    painter.drawLine(origin, xAxisEnd);
    painter.drawLine(origin, yAxisEnd);

    const auto drawArrow = [&](const QPointF &from, const QPointF &to) {
        constexpr double arrowSize = 8.0;
        const double arrowAngle = qDegreesToRadians(30.0);
        QLineF line(from, to);
        const double angle = std::atan2(line.dy(), line.dx());
        const QPointF p1 = to - QPointF(std::cos(angle - arrowAngle) * arrowSize,
                                        std::sin(angle - arrowAngle) * arrowSize);
        const QPointF p2 = to - QPointF(std::cos(angle + arrowAngle) * arrowSize,
                                        std::sin(angle + arrowAngle) * arrowSize);
        painter.drawLine(to, p1);
        painter.drawLine(to, p2);
    };
    drawArrow(origin, xAxisEnd);
    drawArrow(origin, yAxisEnd);

    const double maxVal = findMaxValue();
    painter.setPen(QColor(0x6B, 0x72, 0x80));
    QFont axisFont = painter.font();
    axisFont.setPointSize(qMax(7, axisFont.pointSize() - 2));
    painter.setFont(axisFont);

    const int axisMaxInt = qMax(1, static_cast<int>(std::ceil(maxVal)));
    // Nếu giá trị nhỏ (<=6), hiển thị đủ mốc nguyên 0..axisMaxInt; ngược lại dùng 5 mốc chuẩn.
    const bool denseTicks = axisMaxInt <= 6;
    const int tickSteps = denseTicks ? axisMaxInt : 4;
    for (int i = 0; i <= tickSteps; ++i) {
        const double value = (axisMaxInt * i) / static_cast<double>(tickSteps);
        const double y = plotArea.bottom() - (plotArea.height() * i / static_cast<double>(tickSteps));

        painter.drawLine(QPointF(plotArea.left() - 5, y), QPointF(plotArea.left(), y));

        QString valueText = QString::number(qRound(value));
        if (!valueSuffix_.isEmpty()) valueText += valueSuffix_;
        painter.drawText(QRectF(plotArea.left() - 54, y - 10, 48, 20),
                         Qt::AlignRight | Qt::AlignVCenter, valueText);
    }

    QFont yLabelFont = painter.font();
    yLabelFont.setPointSize(13);
    yLabelFont.setBold(true);
    painter.setFont(yLabelFont);
    painter.save();
    painter.setPen(QColor(0x2C, 0x3E, 0x59));
    painter.translate(plotArea.left() - 60, plotArea.top() + plotArea.height() / 2);
    painter.rotate(-90);
    if (!yAxisLabel_.isEmpty()) {
        painter.drawText(QRect(-60, -40, 120, 80), Qt::AlignCenter, yAxisLabel_);
    }
    painter.restore();

    if (!xAxisLabel_.isEmpty()) {
        QFont xLabelFont = painter.font();
        xLabelFont.setPointSize(12);
        xLabelFont.setBold(true);
        painter.setFont(xLabelFont);
        painter.setPen(QColor(0x2C, 0x3E, 0x59));
        const int xLabelTop = plotArea.bottom() + 34;
        painter.drawText(QRect(plotArea.left(), xLabelTop, plotArea.width(), 20),
                         Qt::AlignCenter, xAxisLabel_);
    }
}

double StatsChart::findMaxValue() const {
    double maxVal = 0.0;
    for (const auto &serie : series_) {
        for (double v : serie.values) {
            maxVal = qMax(maxVal, v);
        }
    }
    if (qFuzzyIsNull(maxVal)) return 5.0;
    // Luôn có tối thiểu 0..5 trên trục để thấy các mốc 1..5 ngay cả khi dữ liệu nhỏ.
    return static_cast<double>(qMax(5, static_cast<int>(std::ceil(maxVal))));
}

void StatsChart::drawBarChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount == 0) return;

    const double groupWidth = plotArea.width() / static_cast<double>(categoryCount);
    const int seriesCount = series_.size();
    const double barWidth = qMax(8.0, (groupWidth - 12.0) / qMax(1, seriesCount));

    painter.setPen(QPen(QColor(0xEC, 0xEE, 0xF1), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    static core::DynamicArray defaultColors = []() {
        core::DynamicArray<QColor> colors;
        colors.append(QColor(0x20, 0x9C, 0xF0));
        colors.append(QColor(0x22, 0xC5, 0x67));
        colors.append(QColor(0xF9, 0xA8, 0x0E));
        colors.append(QColor(0xFB, 0x72, 0x4C));
        colors.append(QColor(0x8B, 0x5C, 0xE0));
        colors.append(QColor(0x14, 0xB8, 0xA6));
        colors.append(QColor(0xF4, 0x43, 0x36));
        return colors;
    }();

    for (int cat = 0; cat < categoryCount; ++cat) {
        const double baseX = plotArea.left() + cat * groupWidth;
        for (int si = 0; si < seriesCount; ++si) {
            if (cat >= series_[si].values.size()) continue;
            const double value = series_[si].values[cat];
            const double ratio = value / maxVal;
            const double barHeight = ratio * plotArea.height();
            const double x = baseX + si * barWidth + (groupWidth - seriesCount * barWidth) / 2.0;
            QRectF bar(x, plotArea.bottom() - barHeight, barWidth - 2.0, barHeight);

            QLinearGradient gradient(bar.topLeft(), bar.bottomLeft());
            QColor baseColor = series_[si].color.isValid() ? series_[si].color : QColor(0x25, 0x63, 0xEB);
            if (seriesCount == 1) baseColor = defaultColors[cat % defaultColors.size()];
            gradient.setColorAt(0, baseColor.lighter(120));
            gradient.setColorAt(1, baseColor.darker(115));

            QRectF shadow = bar.translated(2, 2);
            painter.fillRect(shadow, QColor(0, 0, 0, 28));
            painter.fillRect(bar, QBrush(gradient));
            painter.setPen(QPen(baseColor.darker(140), 1));
            painter.drawRect(bar);
        }

        painter.setPen(QColor(0x2C, 0x3E, 0x59));
        QFont labelFont = painter.font();
        labelFont.setPointSize(qMax(8, labelFont.pointSize() - 1));
        painter.setFont(labelFont);
        painter.drawText(QRectF(baseX, plotArea.bottom() + 5, groupWidth, 25.0),
                         Qt::AlignCenter, categories_[cat]);
    }

    // Không vẽ số trên đỉnh cột để cột chạm trục tối đa rõ ràng hơn
}

void StatsChart::drawStackedBarChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount == 0) return;

    const double groupWidth = plotArea.width() / static_cast<double>(categoryCount);
    const double barWidth = qMax(20.0, groupWidth - 20.0);

    painter.setPen(QPen(QColor(0xEC, 0xEE, 0xF1), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    for (int cat = 0; cat < categoryCount; ++cat) {
        double accumulatedHeight = 0.0;
        const double baseX = plotArea.left() + cat * groupWidth + (groupWidth - barWidth) / 2.0;

        for (const auto &serie : series_) {
            if (cat >= serie.values.size()) continue;
            const double value = serie.values[cat];
            const double ratio = value / maxVal;
            const double segmentHeight = ratio * plotArea.height();

            QRectF segment(baseX,
                           plotArea.bottom() - accumulatedHeight - segmentHeight,
                           barWidth,
                           segmentHeight);

            QLinearGradient gradient(segment.topLeft(), segment.bottomLeft());
            QColor baseColor = serie.color.isValid() ? serie.color : palette().highlight().color();
            gradient.setColorAt(0, baseColor.lighter(120));
            gradient.setColorAt(1, baseColor.darker(110));

            QRectF shadow = segment.translated(1, 1);
            painter.fillRect(shadow, QColor(0, 0, 0, 20));
            painter.fillRect(segment, QBrush(gradient));

            painter.setPen(QPen(baseColor.darker(120), 1));
            painter.drawRect(segment);

            accumulatedHeight += segmentHeight;
        }

        painter.setPen(QColor(0x2C, 0x3E, 0x59));
        QFont labelFont = painter.font();
        labelFont.setPointSize(qMax(8, labelFont.pointSize() - 1));
        painter.setFont(labelFont);
        painter.drawText(QRectF(plotArea.left() + cat * groupWidth, plotArea.bottom() + 5, groupWidth, 25),
                         Qt::AlignCenter, categories_[cat]);
    }
}

void StatsChart::drawLineChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount < 2) return;

    painter.setPen(QPen(QColor(0xEC, 0xEE, 0xF1), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    const double stepX = plotArea.width() / static_cast<double>(categoryCount - 1);

    for (const auto &serie : series_) {
        if (serie.values.size() < categoryCount) continue;
        QPainterPath path;
        for (int i = 0; i < categoryCount; ++i) {
            const double value = serie.values[i];
            const double ratio = value / maxVal;
            const QPointF point(plotArea.left() + i * stepX,
                                plotArea.bottom() - ratio * plotArea.height());
            if (i == 0) path.moveTo(point);
            else path.lineTo(point);
        }
        QColor base = serie.color.isValid() ? serie.color : QColor(0x25, 0x63, 0xEB);
        QPen pen(base, 2.4);
        painter.setPen(pen);
        painter.drawPath(path);

        painter.setBrush(base);
        painter.setPen(Qt::NoPen);
        for (int i = 0; i < categoryCount; ++i) {
            const double value = serie.values[i];
            const double ratio = value / maxVal;
            const QPointF point(plotArea.left() + i * stepX,
                                plotArea.bottom() - ratio * plotArea.height());
            painter.drawEllipse(point, 4.5, 4.5);

            QString valueText = QString::number(qRound(value));
            if (!valueSuffix_.isEmpty()) valueText += valueSuffix_;
            QFont valueFont = painter.font();
            valueFont.setPointSize(qMax(8, valueFont.pointSize() - 1));
            painter.setFont(valueFont);
            painter.setPen(base.darker(140));
            painter.drawText(QRectF(point.x() - 14, point.y() - 18, 28, 14),
                             Qt::AlignCenter, valueText);
        }
    }

    painter.setPen(QColor(0x2C, 0x3E, 0x59));
    QFont labelFont = painter.font();
    labelFont.setPointSize(qMax(8, labelFont.pointSize() - 1));
    painter.setFont(labelFont);
    for (int i = 0; i < categoryCount; ++i) {
        painter.drawText(QRectF(plotArea.left() + i * stepX - 20, plotArea.bottom() + 2, 40, 20),
                         Qt::AlignCenter, categories_[i]);
    }
}

void StatsChart::drawHorizontalBarChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount == 0 || series_.isEmpty()) return;

    const auto &serie = series_[0];

    const double rowHeight = plotArea.height() / static_cast<double>(categoryCount);
    const double barHeight = qMax(16.0, rowHeight * 0.62);

    painter.setPen(QPen(QColor(0xEC, 0xEE, 0xF1), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    static core::DynamicArray defaultColors = []() {
        core::DynamicArray<QColor> colors;
        colors.append(QColor(0x20, 0x9C, 0xF0));
        colors.append(QColor(0x22, 0xC5, 0x67));
        colors.append(QColor(0xF9, 0xA8, 0x0E));
        colors.append(QColor(0xFB, 0x72, 0x4C));
        colors.append(QColor(0x8B, 0x5C, 0xE0));
        colors.append(QColor(0x14, 0xB8, 0xA6));
        return colors;
    }();

    for (int i = 0; i < categoryCount; ++i) {
        const double value = serie.values.value(i, 0.0);
        const double ratio = value / maxVal;
        const double width = ratio * plotArea.width();
        const double y = plotArea.top() + i * rowHeight + (rowHeight - barHeight) / 2.0;
        QRectF bar(plotArea.left(), y, width, barHeight);

        QColor baseColor = serie.color.isValid() ? serie.color
                                                 : defaultColors[i % defaultColors.size()];
        QLinearGradient gradient(bar.topLeft(), bar.topRight());
        gradient.setColorAt(0, baseColor.lighter(120));
        gradient.setColorAt(1, baseColor.darker(110));
        painter.fillRect(bar.translated(1.5, 1.5), QColor(0, 0, 0, 25));
        painter.fillRect(bar, gradient);
        painter.setPen(QPen(baseColor.darker(140), 1));
        painter.drawRect(bar);

        painter.setPen(QColor(0x2C, 0x3E, 0x59));
        QFont labelFont = painter.font();
        labelFont.setPointSize(10);
        painter.setFont(labelFont);
        painter.drawText(QRectF(plotArea.left() - 110, y - 2, 102, barHeight + 4),
                         Qt::AlignRight | Qt::AlignVCenter, categories_[i]);
        painter.drawText(QRectF(bar.right() + 6, y - 2, 60, barHeight + 4),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         QString::number(static_cast<int>(value)));
    }
}

void StatsChart::drawLegend(QPainter &painter, const QRect &plotArea) const {
    constexpr int legendHeight = 18;
    constexpr int legendSpacing = 4;
    const int totalHeight = series_.size() * (legendHeight + legendSpacing) - legendSpacing;
    const int x = plotArea.right() - 160;
    int y = plotArea.top() + 10;

    const QRect legendBg(x - 8, y - 4, 150, totalHeight + 8);
    painter.fillRect(legendBg, QColor(255, 255, 255, 230));
    painter.setPen(QPen(QColor(0xE5, 0xE7, 0xEB), 1));
    painter.drawRect(legendBg);

    for (const auto &serie : series_) {
        constexpr int boxSize = 12;
        QColor color = serie.color.isValid() ? serie.color : palette().highlight().color();

        QRect colorBox(x, y + 2, boxSize, boxSize);
        QLinearGradient boxGradient(colorBox.topLeft(), colorBox.bottomLeft());
        boxGradient.setColorAt(0, color.lighter(120));
        boxGradient.setColorAt(1, color.darker(110));
        painter.fillRect(colorBox, QBrush(boxGradient));
        painter.setPen(QPen(color.darker(150), 1));
        painter.drawRect(colorBox);

        painter.setPen(QColor(0x2C, 0x3E, 0x59));
        QFont legendFont = painter.font();
        legendFont.setPointSize(qMax(8, legendFont.pointSize() - 1));
        painter.setFont(legendFont);
        painter.drawText(QRect(x + boxSize + 8, y, 100, legendHeight),
                         Qt::AlignLeft | Qt::AlignVCenter, serie.name);

        y += legendHeight + legendSpacing;
    }
}

}  // namespace pbl2::ui
