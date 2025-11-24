#include "StatsChart.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QtMath>

namespace pbl2::ui {

StatsChart::StatsChart(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(180);
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
        painter.setPen(QPen(palette().mid().color()));
        painter.drawText(rect(), Qt::AlignCenter, QObject::tr("Không có dữ liệu"));
        return;
    }

    constexpr int leftMargin = 60;
    constexpr int rightMargin = 24;
    const int topMargin = title_.isEmpty() ? 16 : 40;
    constexpr int bottomMargin = 40;
    const QRect plotArea = rect().adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);
    if (plotArea.width() <= 0 || plotArea.height() <= 0) return;

    drawAxes(painter, plotArea);

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
    }

    if (showLegend_) {
        drawLegend(painter, plotArea);
    }
    drawTitle(painter, rect().adjusted(leftMargin, 0, -rightMargin, -plotArea.height()));
}

void StatsChart::drawBackground(QPainter &painter) const {
    painter.fillRect(rect(), palette().base());
    painter.setPen(QPen(palette().midlight().color()));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void StatsChart::drawTitle(QPainter &painter, const QRect &area) const {
    if (title_.isEmpty()) return;
    QFont f = painter.font();
    f.setBold(true);
    f.setPointSize(qMax(10, f.pointSize() + 1));
    painter.setFont(f);
    painter.setPen(QColor(0x1F, 0x29, 0x37));

    // Add some padding and center the title
    const QRect titleRect = area.adjusted(0, 8, 0, 0);
    painter.drawText(titleRect, Qt::AlignCenter, title_);
}

void StatsChart::drawAxes(QPainter &painter, const QRect &plotArea) const {

    // Draw main axes
    painter.setPen(QPen(QColor(0x9C, 0xA3, 0xAF), 2));
    painter.drawLine(plotArea.bottomLeft(), plotArea.bottomRight());
    painter.drawLine(plotArea.bottomLeft(), plotArea.topLeft());

    // Draw Y-axis value labels
    const double maxVal = findMaxValue();
    painter.setPen(QColor(0x6B, 0x72, 0x80));
    QFont axisFont = painter.font();
    axisFont.setPointSize(qMax(7, axisFont.pointSize() - 2));
    painter.setFont(axisFont);

    for (int i = 0; i <= 4; ++i) {
        const double value = (maxVal * i) / 4.0;
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);

        // Draw tick mark
        painter.drawLine(QPointF(plotArea.left() - 5, y), QPointF(plotArea.left(), y));

        // Draw value label
        QString valueText = QString::number(static_cast<int>(value));
        if (!valueSuffix_.isEmpty()) {
            valueText += valueSuffix_;
        }
        painter.drawText(QRectF(plotArea.left() - 50, y - 10, 45, 20),
                        Qt::AlignRight | Qt::AlignVCenter, valueText);
    }

    // Draw Y axis label if provided
    QFont yLabelFont = painter.font();
    yLabelFont.setPointSize(13);
    yLabelFont.setBold(true);
    painter.setFont(yLabelFont);
    painter.save();
    painter.translate(plotArea.left() - 60, plotArea.top() + plotArea.height() / 2);
    painter.rotate(-90);
    if (!yAxisLabel_.isEmpty()) {
        painter.drawText(QRect(-60, -40, 120, 80), Qt::AlignCenter, yAxisLabel_);
    }
    painter.restore();
}

double StatsChart::findMaxValue() const {
    double maxVal = 0.0;
    for (const auto &series : series_) {
        for (double v : series.values) {
            maxVal = qMax(maxVal, v);
        }
    }
    if (qFuzzyIsNull(maxVal)) maxVal = 1.0;
    return maxVal * 1.1;  // add headroom
}

void StatsChart::drawBarChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount == 0) return;

    const double groupWidth = plotArea.width() / static_cast<double>(categoryCount);
    const int seriesCount = series_.size();
    const double barWidth = qMax(8.0, (groupWidth - 12.0) / qMax(1, seriesCount));

    // Draw grid lines for better readability
    painter.setPen(QPen(QColor(0xF3, 0xF4, 0xF6), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    // Draw bars with gradient and shadow effect
    // Nếu chỉ có 1 series và nhiều thể loại, tô mỗi cột 1 màu để phân biệt
    static core::DynamicArray defaultColors = []() {
        core::DynamicArray<QColor> colors;
        colors.append(QColor(0xEF5350));
        colors.append(QColor(0xAB47BC));
        colors.append(QColor(0x42A5F5));
        colors.append(QColor(0x26A69A));
        colors.append(QColor(0xFFB300));
        colors.append(QColor(0x8D6E63));
        colors.append(QColor(0x5C6BC0));
        colors.append(QColor(0x00897B));
        colors.append(QColor(0x8BC34A));
        colors.append(QColor(0xFF7043));
        colors.append(QColor(0x5E35B1));
        colors.append(QColor(0x00838F));
        return colors;
    }();
    // Vẽ từng cột cho từng ngày, mỗi màu là 1 thể loại
    for (int cat = 0; cat < categoryCount; ++cat) {
        double baseX = plotArea.left() + cat * groupWidth;
        for (int si = 0; si < seriesCount; ++si) {
            if (cat >= series_[si].values.size()) continue;
            double value = series_[si].values[cat];
            double ratio = value / maxVal;
            double barHeight = ratio * plotArea.height();
            double x = baseX + si * barWidth + (groupWidth - seriesCount * barWidth) / 2.0;
            QRectF bar(x, plotArea.bottom() - barHeight, barWidth - 2.0, barHeight);

            QLinearGradient gradient(bar.topLeft(), bar.bottomLeft());
            QColor baseColor = series_[si].color.isValid() ? series_[si].color : QColor(0x25, 0x63, 0xEB);
            if (seriesCount == 1) {
                baseColor = defaultColors[cat % defaultColors.size()];
            }
            gradient.setColorAt(0, baseColor.lighter(120));
            gradient.setColorAt(1, baseColor.darker(110));

            QRectF shadow = bar.translated(2, 2);
            painter.fillRect(shadow, QColor(0, 0, 0, 30));
            painter.fillRect(bar, QBrush(gradient));
            painter.setPen(QPen(baseColor.darker(120), 1));
            painter.drawRect(bar);
        }
        // Nhãn dưới cột là ngày/tháng
        painter.setPen(QColor(0x37, 0x41, 0x51));
        QFont labelFont = painter.font();
        labelFont.setPointSize(qMax(8, labelFont.pointSize() - 1));
        painter.setFont(labelFont);
        painter.drawText(QRectF(baseX, plotArea.bottom() + 5, groupWidth, 25.0), Qt::AlignCenter, categories_[cat]);
    }

    // Vẽ legend cho từng thể loại ở góc phải
    int legendX = plotArea.right() + 40;
    int legendY = plotArea.top() + 10;
    int legendBox = 18;
    int legendSpacing = 8;
    QFont legendFont = painter.font();
    legendFont.setPointSize(11);
    painter.setFont(legendFont);
    painter.setPen(QColor(0x37, 0x41, 0x51));

    if (seriesCount == 1) {
        painter.drawText(legendX, legendY - legendBox - 8, QStringLiteral("Chú thích màu theo thể loại"));
        for (int cat = 0; cat < categoryCount; ++cat) {
            QColor color = defaultColors[cat % defaultColors.size()];
            QRect colorRect(legendX, legendY + cat * (legendBox + legendSpacing), legendBox, legendBox);
            painter.fillRect(colorRect, color);
            painter.setPen(QPen(color.darker(120), 1));
            painter.drawRect(colorRect);
            painter.setPen(QColor(0x37, 0x41, 0x51));
            painter.drawText(legendX + legendBox + 8,
                             legendY + cat * (legendBox + legendSpacing) + legendBox - 3,
                             categories_[cat]);
        }
    } else {
        painter.drawText(legendX, legendY - legendBox - 8, QStringLiteral("Chú thích màu theo series"));
        for (int si = 0; si < seriesCount; ++si) {
            QColor color = series_[si].color.isValid() ? series_[si].color : QColor(0x25, 0x63, 0xEB);
            QRect colorRect(legendX, legendY + si * (legendBox + legendSpacing), legendBox, legendBox);
            painter.fillRect(colorRect, color);
            painter.setPen(Qt::white);
            painter.drawRect(colorRect);
            painter.setPen(QColor(0x37, 0x41, 0x51));
            painter.drawText(legendX + legendBox + 8,
                             legendY + si * (legendBox + legendSpacing) + legendBox - 3,
                             series_[si].name);
        }
    }

    // Vẽ chú thích "Ngày/Tháng" bên ngoài trục X
    painter.setPen(QColor(0x1F, 0x29, 0x37));
    QFont dateFont = painter.font();
    dateFont.setPointSize(13);
    dateFont.setBold(true);
    painter.setFont(dateFont);
    if (!xAxisLabel_.isEmpty()) {
        painter.drawText(QRectF(plotArea.left(), plotArea.bottom() + 35, plotArea.width(), 30),
                         Qt::AlignCenter, xAxisLabel_);
    }

    // Draw value labels on top of bars if there's enough space
    if (barWidth > 20) {
        painter.setPen(QColor(0x1F, 0x29, 0x37));
        QFont valueFont = painter.font();
        valueFont.setPointSize(qMax(7, valueFont.pointSize() - 2));
        painter.setFont(valueFont);

        for (int cat = 0; cat < categoryCount; ++cat) {
            const double baseX = plotArea.left() + cat * groupWidth;
            for (int si = 0; si < seriesCount; ++si) {
                if (cat >= series_[si].values.size()) continue;
                if (const double value = series_[si].values[cat]; value > 0) {
                    const double ratio = value / maxVal;
                    const double barHeight = ratio * plotArea.height();
                    const double x = baseX + si * barWidth + (groupWidth - seriesCount * barWidth) / 2.0;
                    const double y = plotArea.bottom() - barHeight - 5;

                    painter.drawText(QRectF(x, y - 15, barWidth - 2.0, 15),
                                   Qt::AlignCenter, QString::number(static_cast<int>(value)));
                }
            }
        }
    }
}

void StatsChart::drawStackedBarChart(QPainter &painter, const QRect &plotArea) const {
    const double maxVal = findMaxValue();
    const int categoryCount = categories_.size();
    if (categoryCount == 0) return;

    const double groupWidth = plotArea.width() / static_cast<double>(categoryCount);
    const double barWidth = qMax(20.0, groupWidth - 20.0);

    // Draw grid lines
    painter.setPen(QPen(QColor(0xF3, 0xF4, 0xF6), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    for (int cat = 0; cat < categoryCount; ++cat) {
        double accumulatedHeight = 0.0;
        const double baseX = plotArea.left() + cat * groupWidth + (groupWidth - barWidth) / 2.0;

        for (const auto & serie : series_) {
            if (cat >= serie.values.size()) continue;
            const double value = serie.values[cat];
            const double ratio = value / maxVal;
            const double segmentHeight = ratio * plotArea.height();

            QRectF segment(baseX,
                          plotArea.bottom() - accumulatedHeight - segmentHeight,
                          barWidth,
                          segmentHeight);

            // Create gradient for each segment
            QLinearGradient gradient(segment.topLeft(), segment.bottomLeft());
            QColor baseColor = serie.color.isValid() ? serie.color : palette().highlight().color();
            gradient.setColorAt(0, baseColor.lighter(120));
            gradient.setColorAt(1, baseColor.darker(110));

            // Draw shadow
            QRectF shadow = segment.translated(1, 1);
            painter.fillRect(shadow, QColor(0, 0, 0, 20));

            // Draw segment with gradient
            painter.fillRect(segment, QBrush(gradient));

            // Add border
            painter.setPen(QPen(baseColor.darker(120), 1));
            painter.drawRect(segment);

            accumulatedHeight += segmentHeight;
        }

        // Draw category label
        painter.setPen(QColor(0x37, 0x41, 0x51));
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

    painter.setPen(QPen(palette().midlight().color(), 1, Qt::DashLine));
    for (int i = 1; i <= 4; ++i) {
        const double y = plotArea.bottom() - (plotArea.height() * i / 4.0);
        painter.drawLine(QPointF(plotArea.left(), y), QPointF(plotArea.right(), y));
    }

    const double stepX = plotArea.width() / static_cast<double>(categoryCount - 1);

    for (const auto &series : series_) {
        if (series.values.size() < categoryCount) continue;
        QPainterPath path;
        for (int i = 0; i < categoryCount; ++i) {
            const double value = series.values[i];
            const double ratio = value / maxVal;
            const QPointF point(plotArea.left() + i * stepX,
                                plotArea.bottom() - ratio * plotArea.height());
            if (i == 0) path.moveTo(point);
            else path.lineTo(point);
        }
        QPen pen(series.color.isValid() ? series.color : palette().highlight().color(), 2);
        painter.setPen(pen);
        painter.drawPath(path);
    }

    painter.setPen(palette().mid().color());
    for (int i = 0; i < categoryCount; ++i) {
        painter.drawText(QRectF(plotArea.left() + i * stepX - 20, plotArea.bottom(), 40, 20), Qt::AlignCenter, categories_[i]);
    }
}

void StatsChart::drawLegend(QPainter &painter, const QRect &plotArea) const {
    // Luôn vẽ legend, kể cả khi chỉ có 1 series

    constexpr int legendHeight = 18;
    constexpr int legendSpacing = 4;

    // Calculate legend position (top-right)
    const int totalHeight = series_.size() * (legendHeight + legendSpacing) - legendSpacing;
    const int x = plotArea.right() - 160;
    int y = plotArea.top() + 10;

    // Draw legend background
    const QRect legendBg(x - 8, y - 4, 150, totalHeight + 8);
    painter.fillRect(legendBg, QColor(255, 255, 255, 200));
    painter.setPen(QPen(QColor(0xE5, 0xE7, 0xEB), 1));
    painter.drawRect(legendBg);

    for (const auto & series : series_) {
        constexpr int boxSize = 12;
        QColor color = series.color.isValid() ? series.color : palette().highlight().color();

        // Draw color box with gradient
        QRect colorBox(x, y + 2, boxSize, boxSize);
        QLinearGradient boxGradient(colorBox.topLeft(), colorBox.bottomLeft());
        boxGradient.setColorAt(0, color.lighter(120));
        boxGradient.setColorAt(1, color.darker(110));
        painter.fillRect(colorBox, QBrush(boxGradient));
        painter.setPen(QPen(color.darker(150), 1));
        painter.drawRect(colorBox);

        // Draw text
        painter.setPen(QColor(0x37, 0x41, 0x51));
        QFont legendFont = painter.font();
        legendFont.setPointSize(qMax(8, legendFont.pointSize() - 1));
        painter.setFont(legendFont);
        painter.drawText(QRect(x + boxSize + 8, y, 100, legendHeight),
                        Qt::AlignLeft | Qt::AlignVCenter, series.name);

        y += legendHeight + legendSpacing;
    }
}

}  // namespace pbl2::ui
