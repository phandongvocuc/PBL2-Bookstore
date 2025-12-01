#include "datafile/BinaryFileStore.h"

#include <cstdio>
#include <cstring>
using namespace std;
using pbl2::core::CustomString;
using pbl2::core::DynamicArray;

namespace pbl2::serialization {
namespace {

struct FileHeader {
    uint32_t recordSize;
    uint32_t count;
};

// Legacy book record without originalPrice (pre-price addition)
struct LegacyBookRecord {
    char id[32];
    char title[256];
    char author[128];
    char genre[64];
    char publisher[128];
    DateRecord publishDate;
    int32_t quantity;
    char status[32];
    char summary[512];
};

// Legacy report record without affectedBooks (pre-affectedBooks addition)
struct LegacyReportRequestRecord {
    char requestId[32];
    char staffUsername[64];
    DateRecord fromDate;
    DateRecord toDate;
    int32_t handledLoans;
    int32_t lostOrDamaged;
    int32_t overdueReaders;
    char notes[256];
    char status[32];
    DateTimeRecord createdAt;
};

template <typename R>
bool writefile(const CustomString& path, const DynamicArray<R>& records) {
    FILE* file = fopen(path.cStr(), "wb");
    if (!file) {
        return false;
    }

    const FileHeader header{
        static_cast<uint32_t>(sizeof(R)),
        static_cast<uint32_t>(records.size())
    };

    if (fwrite(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return false;
    }

    if (header.count > 0) {
        if (fwrite(records.data(), sizeof(R), records.size(), file) != records.size()) {
            fclose(file);
            return false;
        }
    }

    fclose(file);
    return true;
}

template <typename R>
bool readfile(const CustomString& path, DynamicArray<R>& out) {
    FILE* file = fopen(path.cStr(), "rb");
    if (!file) {
        out.clear();
        return false;
    }

    FileHeader header{};
    if (fread(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        out.clear();
        return false;
    }

    if (header.recordSize != sizeof(R)) {
        fclose(file);
        out.clear();
        return false;
    }

    if (header.count == 0) {
        out.clear();
        fclose(file);
        return true;
    }

    out.resize(header.count);
    const size_t read = fread(out.data(), sizeof(R), header.count, file);
    fclose(file);
    if (read != header.count) {
        out.clear();
        return false;
    }

    return true;
}

template <typename M, typename R>
bool writeCollection(const DynamicArray<M>& models,
                     const CustomString& path,
                     R (*pack)(const M&)) {
    DynamicArray<R> records(models.size());
    for (size_t i = 0; i < models.size(); ++i) {
        records[i] = pack(models[i]);
    }
    return writefile(path, records);
}

template <typename M, typename R>
DynamicArray<M> readCollection(const CustomString& path,
                               M (*unpack)(const R&)) {
    DynamicArray<R> records;
    if (!readfile(path, records)) {
        return {};
    }

    DynamicArray<M> models(records.size());
    for (size_t i = 0; i < records.size(); ++i) {
        models[i] = unpack(records[i]);
    }
    return models;
}

}  // namespace

DateRecord BinaryFileStore::packDate(const core::Date &value) {
    return DateRecord{ value.year(), value.month(), value.day() };
}

core::Date BinaryFileStore::unpackDate(const DateRecord &record) {
    return {record.year, record.month, record.day};
}

DateTimeRecord BinaryFileStore::packDateTime(const core::DateTime &value) {
    return DateTimeRecord{
        value.year(),
        value.month(),
        value.day(),
        value.hour(),
        value.minute(),
        value.second(),
        value.millisecond()
    };
}

core::DateTime BinaryFileStore::unpackDateTime(const DateTimeRecord &record) {
    return {
        record.year,
        record.month,
        record.day,
        record.hour,
        record.minute,
        record.second,
        record.millisecond
    };
}

void BinaryFileStore::assignText(char *destination, size_t capacity, const CustomString &value) {
    if (!destination || capacity == 0U) return;
    memset(destination, 0, capacity);
    if (const size_t copyCount = value.length() >= capacity ? capacity - 1U : value.length(); copyCount > 0U) {
        memcpy(destination, value.cStr(), copyCount);
    }
}

BookRecord BinaryFileStore::packBook(const model::Book &value) {
    BookRecord record{};
    assignText(record.id, sizeof(record.id), value.getId());
    assignText(record.title, sizeof(record.title), value.getTitle());
    assignText(record.author, sizeof(record.author), value.getAuthor());
    assignText(record.genre, sizeof(record.genre), value.getGenre());
    assignText(record.publisher, sizeof(record.publisher), value.getPublisher());
    record.publishDate = packDate(value.getPublishDate());
    // publishYear removed
    record.quantity = value.getQuantity();
    record.originalPrice = value.getOriginalPrice();
    assignText(record.status, sizeof(record.status), value.getStatus());
    assignText(record.summary, sizeof(record.summary), value.getSummary());
    assignText(record.coverImagePath, sizeof(record.coverImagePath), value.getCoverImagePath());
    return record;
}

model::Book BinaryFileStore::unpackBook(const BookRecord &record) {
    model::Book value;
    value.setId(CustomString(record.id));
    value.setTitle(CustomString(record.title));
    value.setAuthor(CustomString(record.author));
    value.setGenre(CustomString(record.genre));
    value.setPublisher(CustomString(record.publisher));
    value.setPublishDate(unpackDate(record.publishDate));
    // publishYear removed
    value.setQuantity(record.quantity);
    value.setOriginalPrice(record.originalPrice);
    value.setStatus(CustomString(record.status));
    value.setSummary(CustomString(record.summary));
    value.setCoverImagePath(CustomString(record.coverImagePath));
    return value;
}

ReaderRecord BinaryFileStore::packReader(const model::Reader &value) {
    ReaderRecord record{};
    assignText(record.id, sizeof(record.id), value.getId());
    assignText(record.fullName, sizeof(record.fullName), value.getFullName());
    record.dob = packDate(value.getDob());
    record.active = value.isActive() ? 1U : 0U;
    assignText(record.gender, sizeof(record.gender), value.getGender());
    assignText(record.email, sizeof(record.email), value.getEmail());
    assignText(record.address, sizeof(record.address), value.getAddress());
    assignText(record.phone, sizeof(record.phone), value.getPhone());
    assignText(record.identityCard, sizeof(record.identityCard), value.getIdentityCard());
    record.createdDate = packDate(value.getCreatedDate());
    record.expiryDate = packDate(value.getExpiryDate());
    record.totalBorrowed = value.getTotalBorrowed();
    assignText(record.notes, sizeof(record.notes), value.getNotes());
    return record;
}

model::Reader BinaryFileStore::unpackReader(const ReaderRecord &record) {
    model::Reader value;
    value.setId(CustomString(record.id));
    value.setFullName(CustomString(record.fullName));
    value.setDob(unpackDate(record.dob));
    value.setActive(record.active != 0U);
    value.setGender(CustomString(record.gender));
    value.setEmail(CustomString(record.email));
    value.setAddress(CustomString(record.address));
    value.setPhone(CustomString(record.phone));
    value.setIdentityCard(CustomString(record.identityCard));
    value.setCreatedDate(unpackDate(record.createdDate));
    value.setExpiryDate(unpackDate(record.expiryDate));
    value.setTotalBorrowed(record.totalBorrowed);
    value.setNotes(CustomString(record.notes));
    return value;
}

LoanRecord BinaryFileStore::packLoan(const model::Loan &value) {
    LoanRecord record{};
    assignText(record.loanId, sizeof(record.loanId), value.getLoanId());
    assignText(record.readerId, sizeof(record.readerId), value.getReaderId());
    assignText(record.bookId, sizeof(record.bookId), value.getBookId());
    record.borrowDate = packDate(value.getBorrowDate());
    record.dueDate = packDate(value.getDueDate());
    record.returnDate = packDate(value.getReturnDate());
    assignText(record.status, sizeof(record.status), value.getStatus());
    record.fine = value.getFine();
    assignText(record.staffUsername, sizeof(record.staffUsername), value.getStaffUsername());
    return record;
}

model::Loan BinaryFileStore::unpackLoan(const LoanRecord &record) {
    model::Loan value;
    value.setLoanId(CustomString(record.loanId));
    value.setReaderId(CustomString(record.readerId));
    value.setBookId(CustomString(record.bookId));
    value.setBorrowDate(unpackDate(record.borrowDate));
    value.setDueDate(unpackDate(record.dueDate));
    value.setReturnDate(unpackDate(record.returnDate));
    value.setStatus(CustomString(record.status));
    value.setFine(record.fine);
    value.setStaffUsername(CustomString(record.staffUsername));
    return value;
}

AccountRecord BinaryFileStore::packAccount(const model::Account &value) {
    AccountRecord record{};
    assignText(record.id, sizeof(record.id), value.getId());
    assignText(record.fullName, sizeof(record.fullName), value.getFullName());
    record.dob = packDate(value.getDob());
    record.active = value.isActive() ? 1U : 0U;
    assignText(record.accountId, sizeof(record.accountId), value.getAccountId());
    assignText(record.username, sizeof(record.username), value.getUsername());
    assignText(record.passwordHash, sizeof(record.passwordHash), value.getPasswordHash());
    assignText(record.role, sizeof(record.role), value.getRole());
    assignText(record.employeeId, sizeof(record.employeeId), value.getEmployeeId());
    record.createdAt = packDateTime(value.getCreatedAt());
    record.lastLogin = packDateTime(value.getLastLogin());
    return record;
}

model::Account BinaryFileStore::unpackAccount(const AccountRecord &record) {
    model::Account value;
    value.setId(CustomString(record.id));
    value.setFullName(CustomString(record.fullName));
    value.setDob(unpackDate(record.dob));
    value.setActive(record.active != 0U);
    value.setAccountId(CustomString(record.accountId));
    value.setUsername(CustomString(record.username));
    value.setPasswordHash(CustomString(record.passwordHash));
    value.setRole(CustomString(record.role));
    value.setEmployeeId(CustomString(record.employeeId));
    value.setCreatedAt(unpackDateTime(record.createdAt));
    value.setLastLogin(unpackDateTime(record.lastLogin));
    return value;
}

StaffRecord BinaryFileStore::packStaff(const model::Staff &value) {
    StaffRecord record{};
    assignText(record.id, sizeof(record.id), value.getId());
    assignText(record.fullName, sizeof(record.fullName), value.getFullName());
    record.dob = packDate(value.getDob());
    record.active = value.isActive() ? 1U : 0U;
    assignText(record.gender, sizeof(record.gender), value.getGender());
    assignText(record.email, sizeof(record.email), value.getEmail());
    assignText(record.address, sizeof(record.address), value.getAddress());
    assignText(record.phone, sizeof(record.phone), value.getPhone());
    record.hireDate = packDate(value.getHireDate());
    assignText(record.position, sizeof(record.position), value.getPosition());
    assignText(record.notes, sizeof(record.notes), value.getNotes());
    return record;
}

model::Staff BinaryFileStore::unpackStaff(const StaffRecord &record) {
    model::Staff value;
    value.setId(CustomString(record.id));
    value.setFullName(CustomString(record.fullName));
    value.setDob(unpackDate(record.dob));
    value.setActive(record.active != 0U);
    value.setGender(CustomString(record.gender));
    value.setEmail(CustomString(record.email));
    value.setAddress(CustomString(record.address));
    value.setPhone(CustomString(record.phone));
    value.setHireDate(unpackDate(record.hireDate));
    value.setPosition(CustomString(record.position));
    value.setNotes(CustomString(record.notes));
    return value;
}

ReportRequestRecord BinaryFileStore::packReportRequest(const model::ReportRequest &value) {
    ReportRequestRecord record{};
    assignText(record.requestId, sizeof(record.requestId), value.getRequestId());
    assignText(record.staffUsername, sizeof(record.staffUsername), value.getStaffUsername());
    record.fromDate = packDate(value.getFromDate());
    record.toDate = packDate(value.getToDate());
    record.handledLoans = value.getHandledLoans();
    record.lostOrDamaged = value.getLostOrDamaged();
    record.overdueReaders = value.getOverdueReaders();
    assignText(record.affectedBooks, sizeof(record.affectedBooks), value.getAffectedBooks());
    assignText(record.notes, sizeof(record.notes), value.getNotes());
    assignText(record.status, sizeof(record.status), value.getStatus());
    record.createdAt = packDateTime(value.getCreatedAt());
    return record;
}

model::ReportRequest BinaryFileStore::unpackReportRequest(const ReportRequestRecord &record) {
    model::ReportRequest value;
    value.setRequestId(CustomString(record.requestId));
    value.setStaffUsername(CustomString(record.staffUsername));
    value.setFromDate(unpackDate(record.fromDate));
    value.setToDate(unpackDate(record.toDate));
    value.setHandledLoans(record.handledLoans);
    value.setLostOrDamaged(record.lostOrDamaged);
    value.setOverdueReaders(record.overdueReaders);
    value.setAffectedBooks(CustomString(record.affectedBooks));
    value.setNotes(CustomString(record.notes));
    value.setStatus(CustomString(record.status));
    value.setCreatedAt(unpackDateTime(record.createdAt));
    return value;
}

ConfigRecord BinaryFileStore::packConfig(const model::SystemConfig &value) {
    return ConfigRecord{
        value.getMaxBorrowDays(),
        value.getFinePerDay(),
        value.getMaxBooksPerReader()
    };
}

model::SystemConfig BinaryFileStore::unpackConfig(const ConfigRecord &record) {
    model::SystemConfig value;
    value.setMaxBorrowDays(record.maxBorrowDays);
    value.setFinePerDay(record.finePerDay);
    value.setMaxBooksPerReader(record.maxBooksPerReader);
    return value;
}

bool BinaryFileStore::writeBooks(const DynamicArray<model::Book> &items, const CustomString &path) {
    return writeCollection<model::Book, BookRecord>(items, path, packBook);
}

DynamicArray<model::Book> BinaryFileStore::readBooks(const CustomString &path) {
    // Try reading the latest format (with originalPrice)
    DynamicArray<model::Book> current = readCollection<model::Book, BookRecord>(path, unpackBook);
    if (!current.isEmpty()) {
        return current;
    }

    // Fallback: legacy format without originalPrice/coverImagePath
    DynamicArray<LegacyBookRecord> legacyRecords;
    if (!readfile(path, legacyRecords)) {
        return current;  // Could be empty new file
    }
    DynamicArray<model::Book> models(legacyRecords.size());
    for (size_t i = 0; i < legacyRecords.size(); ++i) {
        const auto &record = legacyRecords[i];
        model::Book value;
        value.setId(CustomString(record.id));
        value.setTitle(CustomString(record.title));
        value.setAuthor(CustomString(record.author));
        value.setGenre(CustomString(record.genre));
        value.setPublisher(CustomString(record.publisher));
        value.setPublishDate(unpackDate(record.publishDate));
        // publishYear removed
        value.setQuantity(record.quantity);
        value.setOriginalPrice(0);
        value.setStatus(CustomString(record.status));
        value.setSummary(CustomString(record.summary));
        value.setCoverImagePath(core::CustomString("")); // Trường mới, mặc định rỗng
        models[i] = value;
    }
    // Tự động cập nhật file sang định dạng mới
    writeBooks(models, path);
    return models;
}

bool BinaryFileStore::writeReaders(const DynamicArray<model::Reader> &items, const CustomString &path) {
    return writeCollection<model::Reader, ReaderRecord>(items, path, packReader);
}

DynamicArray<model::Reader> BinaryFileStore::readReaders(const CustomString &path) {
    return readCollection<model::Reader, ReaderRecord>(path, unpackReader);
}

bool BinaryFileStore::writeLoans(const DynamicArray<model::Loan> &items, const CustomString &path) {
    return writeCollection<model::Loan, LoanRecord>(items, path, packLoan);
}

DynamicArray<model::Loan> BinaryFileStore::readLoans(const CustomString &path) {
    return readCollection<model::Loan, LoanRecord>(path, unpackLoan);
}

bool BinaryFileStore::writeAccounts(const DynamicArray<model::Account> &items, const CustomString &path) {
    return writeCollection<model::Account, AccountRecord>(items, path, packAccount);
}

DynamicArray<model::Account> BinaryFileStore::readAccounts(const CustomString &path) {
    return readCollection<model::Account, AccountRecord>(path, unpackAccount);
}

bool BinaryFileStore::writeStaff(const DynamicArray<model::Staff> &items, const CustomString &path) {
    return writeCollection<model::Staff, StaffRecord>(items, path, packStaff);
}

DynamicArray<model::Staff> BinaryFileStore::readStaff(const CustomString &path) {
    return readCollection<model::Staff, StaffRecord>(path, unpackStaff);
}

bool BinaryFileStore::writeReports(const DynamicArray<model::ReportRequest> &items, const CustomString &path) {
    return writeCollection<model::ReportRequest, ReportRequestRecord>(items, path, packReportRequest);
}

DynamicArray<model::ReportRequest> BinaryFileStore::readReports(const CustomString &path) {
    // Try reading new format (with affectedBooks)
    DynamicArray<model::ReportRequest> current = readCollection<model::ReportRequest, ReportRequestRecord>(path, unpackReportRequest);
    if (!current.isEmpty()) {
        return current;
    }

    // Fallback: legacy format without affectedBooks
    DynamicArray<LegacyReportRequestRecord> legacyRecords;
    if (!readfile(path, legacyRecords)) {
        return {};
    }
    DynamicArray<model::ReportRequest> models(legacyRecords.size());
    for (size_t i = 0; i < legacyRecords.size(); ++i) {
        const auto &record = legacyRecords[i];
        model::ReportRequest value;
        value.setRequestId(CustomString(record.requestId));
        value.setStaffUsername(CustomString(record.staffUsername));
        value.setFromDate(unpackDate(record.fromDate));
        value.setToDate(unpackDate(record.toDate));
        value.setHandledLoans(record.handledLoans);
        value.setLostOrDamaged(record.lostOrDamaged);
        value.setOverdueReaders(record.overdueReaders);
        value.setAffectedBooks(core::CustomStringLiteral(""));  // Legacy records không lưu sách cụ thể
        value.setNotes(CustomString(record.notes));
        value.setStatus(CustomString(record.status));
        value.setCreatedAt(unpackDateTime(record.createdAt));
        models[i] = value;
    }
    return models;
}

bool BinaryFileStore::writeConfig(const model::SystemConfig &item, const CustomString &path) {
    DynamicArray<ConfigRecord> records(1);
    records[0] = packConfig(item);
    return writefile(path, records);
}

model::SystemConfig BinaryFileStore::readConfig(const CustomString &path) {
    DynamicArray<ConfigRecord> records;
    if (!readfile(path, records)) {
        return model::SystemConfig{};
    }
    return unpackConfig(records[0]);
}

}  // namespace pbl2::serialization
