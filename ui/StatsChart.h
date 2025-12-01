#pragma once

#include <QColor>
#include <QWidget>
#include <QStringList>
#include "../core/DynamicArray.h"

namespace pbl2::ui {

class StatsChart final : public QWidget {
    Q_OBJECT

public:
    enum class Mode { Bar, Line, StackedBar, HorizontalBar };

    struct Series {
        QString name;
        core::DynamicArray<double> values;
        QColor color;
    };

    explicit StatsChart(QWidget *parent = nullptr);

    void setMode(Mode mode);
    void setTitle(const QString &title);
    void setCategories(const QStringList &categories);
    void setSeries(const core::DynamicArray<Series> &series);
    void setValueSuffix(const QString &suffix);
    void setAxisLabels(const QString &xLabel, const QString &yLabel);
    void setShowLegend(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Mode mode_{Mode::Bar};
    QString title_;
    QString valueSuffix_;
    QStringList categories_;
    core::DynamicArray<Series> series_;
    QString xAxisLabel_{QStringLiteral("Ngày/Tháng")};
    QString yAxisLabel_{QStringLiteral("Sách")};
    bool showLegend_{true};

    void drawBackground(QPainter &painter) const;
    void drawTitle(QPainter &painter, const QRect &area) const;
    void drawAxes(QPainter &painter, const QRect &plotArea) const;
    void drawBarChart(QPainter &painter, const QRect &plotArea) const;
    void drawStackedBarChart(QPainter &painter, const QRect &plotArea) const;
    void drawLineChart(QPainter &painter, const QRect &plotArea) const;
    void drawHorizontalBarChart(QPainter &painter, const QRect &plotArea) const;
    void drawLegend(QPainter &painter, const QRect &plotArea) const;
    [[nodiscard]] double findMaxValue() const;
};

}  // namespace pbl2::ui

using StatsChart = pbl2::ui::StatsChart;
