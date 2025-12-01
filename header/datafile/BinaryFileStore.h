#pragma once

#include <cstdint>

#include "core/Date.h"
#include "core/DateTime.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"
#include "header/SystemConfig.h"
#include "header/model/Account.h"
#include "header/model/Book.h"
#include "header/model/Loan.h"
#include "header/model/Reader.h"
#include "header/model/ReportRequest.h"
#include "header/model/Staff.h"

namespace pbl2::serialization {

#pragma pack(push, 1)
struct DateRecord {
    int32_t year;
    int32_t month;
    int32_t day;
};

struct DateTimeRecord {
    int32_t year;
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t minute;
    int32_t second;
    int32_t millisecond;
};

struct BookRecord {
    char id[32];
    char title[256];
    char author[128];
    char genre[64];
    char publisher[128];
    DateRecord publishDate;
    int32_t quantity;
    int32_t originalPrice;
    char status[32];
    char summary[512];
    char coverImagePath[256]; // Đường dẫn ảnh bìa
};

struct ReaderRecord {
    char id[32];
    char fullName[128];
    DateRecord dob;
    uint8_t active;
    char gender[16];
    char email[128];
    char address[256];
    char phone[32];
    char identityCard[32];
    DateRecord createdDate;
    DateRecord expiryDate;
    int32_t totalBorrowed;
    char notes[512];
};

struct LoanRecord {
    char loanId[32];
    char readerId[32];
    char bookId[32];
    DateRecord borrowDate;
    DateRecord dueDate;
    DateRecord returnDate;
    char status[32];
    int32_t fine;
    char staffUsername[64];
};

struct AccountRecord {
    char id[32];
    char fullName[128];
    DateRecord dob;
    uint8_t active;
    char accountId[32];
    char username[64];
    char passwordHash[128];
    char role[32];
    char employeeId[32];
    DateTimeRecord createdAt;
    DateTimeRecord lastLogin;
};

struct StaffRecord {
    char id[32];
    char fullName[128];
    DateRecord dob;
    uint8_t active;
    char gender[16];
    char email[128];
    char address[256];
    char phone[32];
    DateRecord hireDate;
    char position[64];
    char notes[256];
};

struct ReportRequestRecord {
    char requestId[32];
    char staffUsername[64];
    DateRecord fromDate;
    DateRecord toDate;
    int32_t handledLoans;
    int32_t lostOrDamaged;
    int32_t overdueReaders;
    char affectedBooks[512];
    char notes[256];
    char status[32];
    DateTimeRecord createdAt;
};

struct ConfigRecord {
    int32_t maxBorrowDays;
    int32_t finePerDay;
    int32_t maxBooksPerReader;
};
#pragma pack(pop)

class BinaryFileStore {
public:
    static bool writeBooks(const core::DynamicArray<model::Book> &items, const core::CustomString &path);
    static bool writeReaders(const core::DynamicArray<model::Reader> &items, const core::CustomString &path);
    static bool writeLoans(const core::DynamicArray<model::Loan> &items, const core::CustomString &path);
    static bool writeAccounts(const core::DynamicArray<model::Account> &items, const core::CustomString &path);
    static bool writeStaff(const core::DynamicArray<model::Staff> &items, const core::CustomString &path);
    static bool writeReports(const core::DynamicArray<model::ReportRequest> &items, const core::CustomString &path);
    static bool writeConfig(const model::SystemConfig &item, const core::CustomString &path);

    static core::DynamicArray<model::Book> readBooks(const core::CustomString &path);
    static core::DynamicArray<model::Reader> readReaders(const core::CustomString &path);
    static core::DynamicArray<model::Loan> readLoans(const core::CustomString &path);
    static core::DynamicArray<model::Account> readAccounts(const core::CustomString &path);
    static core::DynamicArray<model::Staff> readStaff(const core::CustomString &path);
    static core::DynamicArray<model::ReportRequest> readReports(const core::CustomString &path);
    static model::SystemConfig readConfig(const core::CustomString &path);

protected:
    static DateRecord packDate(const core::Date &value);
    static core::Date unpackDate(const DateRecord &record);

    static DateTimeRecord packDateTime(const core::DateTime &value);
    static core::DateTime unpackDateTime(const DateTimeRecord &record);

    static void assignText(char *destination, size_t capacity, const core::CustomString &value);

    static BookRecord packBook(const model::Book &value);
    static model::Book unpackBook(const BookRecord &record);

    static ReaderRecord packReader(const model::Reader &value);
    static model::Reader unpackReader(const ReaderRecord &record);

    static LoanRecord packLoan(const model::Loan &value);
    static model::Loan unpackLoan(const LoanRecord &record);

    static AccountRecord packAccount(const model::Account &value);
    static model::Account unpackAccount(const AccountRecord &record);

    static StaffRecord packStaff(const model::Staff &value);
    static model::Staff unpackStaff(const StaffRecord &record);

    static ReportRequestRecord packReportRequest(const model::ReportRequest &value);
    static model::ReportRequest unpackReportRequest(const ReportRequestRecord &record);

    static ConfigRecord packConfig(const model::SystemConfig &value);
    static model::SystemConfig unpackConfig(const ConfigRecord &record);
};

}  // namespace pbl2::serialization
