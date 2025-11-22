#pragma once

#include "core/Date.h"
#include "core/DateTime.h"

#include <QDateTime>
#include <QString>

#include "../core/CustomString.h"

namespace pbl2::bridge {

core::CustomString toCustomString(const QString &value);
QString toQString(const core::CustomString &value);

core::Date toCoreDate(const QDate &value);
QDate toQDate(const core::Date &value);

core::DateTime toCoreDateTime(const QDateTime &value);
QDateTime toQDateTime(const core::DateTime &value);

}  // namespace bridge
