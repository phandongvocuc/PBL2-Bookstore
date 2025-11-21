#pragma once

#include <QString>

class QWidget;

namespace pbl2::ui {

QString dialogStylesheet();
void applyDialogPalette(const QWidget *widget);
void applyDialogTheme(QWidget *widget);

}  // namespace pbl2::ui
