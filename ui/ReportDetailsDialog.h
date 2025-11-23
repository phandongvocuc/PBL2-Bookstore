#pragma once

#include <QDialog>
#include <QString>

#include "../core/CustomContainers.h"
#include "model/Book.h"
#include "model/ReportRequest.h"

class QFormLayout;

namespace pbl2::ui {

class ReportDetailsDialog final : public QDialog {
    Q_OBJECT

public:
    ReportDetailsDialog(const model::ReportRequest &report,
                        const QString &statusText,
                        const core::Vector<model::Book> &books,
                        QWidget *parent = nullptr);

private:
    static QString displayOrFallback(const QString &value, const QString &fallback);
};

}  // namespace pbl2::ui
