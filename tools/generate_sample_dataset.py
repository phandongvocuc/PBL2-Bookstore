#!/usr/bin/env python3
"""
Sinh dữ liệu mẫu tiếng Việt cho ứng dụng thư viện:
- Sách với nhiều thể loại, tên/ tác giả/ NXB tiếng Việt
- Bạn đọc, nhân viên, tài khoản đăng nhập
- Phiếu mượn (nhiều trạng thái) với ngày mượn ngẫu nhiên 11/09/2025–30/11/2025
- Yêu cầu báo cáo (report_requests.bin + reports.bin để tương thích)

Chạy:  python3 tools/generate_sample_dataset.py
"""

import random
import struct
from datetime import date, datetime, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA_DIR = ROOT / "data"
random.seed(2025)


# Helpers --------------------------------------------------------------------
def pad(text: str, size: int) -> bytes:
    data = text.encode("utf-8")
    if len(data) >= size:
        data = data[: size - 1]
    return data + b"\x00" * (size - len(data))


def pack_date(value: date | None) -> tuple[int, int, int]:
    if value is None:
        return 0, 0, 0
    return value.year, value.month, value.day


def pack_datetime(value: datetime | None) -> tuple[int, int, int, int, int, int, int]:
    if value is None:
        return 0, 0, 0, 0, 0, 0, 0
    return (
        value.year,
        value.month,
        value.day,
        value.hour,
        value.minute,
        value.second,
        int(value.microsecond / 1000),
    )


def write_file(path: Path, record_fmt: str, records: list[bytes]) -> None:
    record_size = struct.calcsize(record_fmt)
    header = struct.pack("<II", record_size, len(records))
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as fh:
        fh.write(header)
        for rec in records:
            if len(rec) != record_size:
                raise ValueError(f"Bad record length for {path.name}: {len(rec)} vs {record_size}")
            fh.write(rec)


# Data builders --------------------------------------------------------------
def build_books() -> tuple[list[bytes], int, int]:
    fmt = "<32s256s128s64s128s3i2i32s512s256s"
    genres = [
        "Tiểu thuyết", "Kinh doanh", "Lịch sử", "Khoa học", "Công nghệ",
        "Tâm lý", "Thiếu nhi", "Viễn tưởng", "Trinh thám", "Kỹ năng sống",
    ]
    publishers = ["NXB Trẻ", "NXB Kim Đồng", "NXB Tổng Hợp", "Alpha Books", "First News", "NXB Lao Động"]
    authors = [
        "Nguyễn Nhật Ánh", "Hà Thủy Nguyên", "Nguyễn Nhật Minh", "Phạm Anh Tuấn",
        "Trần Thu Trang", "Lê Hoàng Nam", "Vũ Bích Hạnh", "Đinh Tiến Dũng",
        "Nguyễn Phi Vân", "Trần Đình Hiệp", "Phạm Lữ Ân", "Nguyễn Thành Phong"
    ]
    titles = [
        "Mắt Biếc", "Tôi Thấy Hoa Vàng Trên Cỏ Xanh", "Người Lái Đò Sông Đà", "Sống Mòn",
        "Dế Mèn Phiêu Lưu Ký", "Nhà Giả Kim", "Hai Số Phận", "Bí Mật Của May Mắn",
        "Nghệ Thuật Tinh Tế Của Việc Đếch Quan Tâm", "Khởi Nghiệp Tinh Gọn", "Đắc Nhân Tâm",
        "Không Gia Đình", "Homo Deus", "Sapiens", "Trò Chơi Vương Quyền", "Bí Kíp Thuyết Trình",
        "Lược Sử Thời Gian", "Nhà Thờ Đức Bà Paris", "Sherlock Holmes Tuyển Tập", "Ông Già Và Biển Cả",
        "Đi Tìm Lẽ Sống", "Bố Già", "Chí Phèo", "Vợ Nhặt", "Bàn Về Khế Ước Xã Hội", "Tư Duy Nhanh Và Chậm",
        "Bí Ẩn Mã Da Vinci", "Harry Potter Và Hòn Đá Phù Thủy", "Cuốn Theo Chiều Gió", "Thám Tử Lừng Danh Conan",
    ]

    records: list[bytes] = []
    available = 0
    unavailable = 0
    total_books = 60
    for idx in range(total_books):
        title = titles[idx % len(titles)]
        title = f"{title} #{idx + 1}"
        genre = genres[idx % len(genres)]
        author = authors[idx % len(authors)]
        publisher = publishers[idx % len(publishers)]
        publish_date = date(2012 + (idx % 10), (idx % 12) + 1, (idx % 27) + 1)

        # Trạng thái: đa số CÒN, một ít HẾT/MẤT/HỎNG
        if idx % 12 == 0:
            status = "HẾT"; quantity = 0
            unavailable += 1
        elif idx % 15 == 0:
            status = "MẤT"; quantity = 0
            unavailable += 1
        elif idx % 18 == 0:
            status = "HỎNG"; quantity = 0
            unavailable += 1
        else:
            status = "CÒN"; quantity = random.randint(3, 12)
            available += 1

        record = struct.pack(
            fmt,
            pad(f"B{idx + 1:03d}", 32),
            pad(title, 256),
            pad(author, 128),
            pad(genre, 64),
            pad(publisher, 128),
            *pack_date(publish_date),
            quantity,
            65000 + (idx % 12) * 7000,
            pad(status, 32),
            pad(f"Tóm tắt ngắn gọn cho sách {idx + 1} về chủ đề {genre.lower()}.", 512),
            pad("", 256),
        )
        records.append(record)
    return records, available, unavailable


def build_readers() -> list[bytes]:
    fmt = "<32s128s3iB16s128s256s32s32s3i3ii512s"
    names = [
        "Nguyễn Minh Tú", "Trần Bảo Trâm", "Phan Quang Huy", "Đỗ Thu Ngân",
        "Phạm Hải Đăng", "Võ Ngọc Anh", "Lê Minh Châu", "Lý Tường Vy",
        "Ngô Hoài Nam", "Trần Khánh Hà", "Phạm Đức Long", "Đỗ Thị Lan",
        "Hoàng Bảo Khang", "Vũ Minh Châu", "Lê Gia Hân", "Đặng Mỹ An",
        "Nguyễn Quỳnh Hoa", "Phạm Hoàng Tuấn", "Trịnh Thanh Bình", "Tạ Bích Ngọc",
        "Đinh Công Minh", "Bùi Thảo My", "Đoàn Quốc Huy", "Nguyễn Như Quỳnh",
    ]
    genders = ["Nam", "Nữ"]
    records: list[bytes] = []
    for idx, name in enumerate(names):
        created = date(2024, (idx % 12) + 1, (idx % 27) + 1)
        expiry = created + timedelta(days=540)
        record = struct.pack(
            fmt,
            pad(f"R{idx + 1:03d}", 32),
            pad(name, 128),
            *pack_date(date(1990 + (idx % 15), (idx % 12) + 1, (idx % 25) + 1)),
            1 if idx % 11 != 0 else 0,
            pad(genders[idx % len(genders)], 16),
            pad(f"user{idx + 1:02d}@thuviendemo.vn", 128),
            pad(f"Phường {idx + 1}, Q.{(idx % 12) + 1}, Đà Nẵng", 256),
            pad(f"09{idx:02d}{idx + 10:02d}5678", 32),
            pad(f"0790{idx + 234567:06d}", 32),
            *pack_date(created),
            *pack_date(expiry),
            random.randint(3, 25),
            pad("Bạn đọc thân thiết", 512),
        )
        records.append(record)
    return records


def build_staffs() -> list[bytes]:
    fmt = "<32s128s3iB16s128s256s32s3i64s256s"
    staff_data = [
        ("S001", "Trần Thị Thu Hà", "Nữ", "thuhavp", "Thủ thư "),
        ("S002", "Nguyễn Văn Hùng", "Nam", "hungnv", "Thủ thư"),
        ("S003", "Phạm Thanh Tùng", "Nam", "tungpt", "Quản lý"),
        ("S004", "Lê Ngọc Anh", "Nữ", "anhlibrary", "Nhân viên"),
        ("S005", "Đỗ Minh Đức", "Nam", "ducdm", "Nhân viên"),
    ]
    records: list[bytes] = []
    for idx, (sid, name, gender, user, notes) in enumerate(staff_data):
        dob = date(1988 + idx, (idx % 12) + 1, (idx % 27) + 1)
        hire = date(2022, (idx % 12) + 1, (idx % 27) + 1)
        record = struct.pack(
            fmt,
            pad(sid, 32),
            pad(name, 128),
            *pack_date(dob),
            1,
            pad(gender, 16),
            pad(f"{user}@thuviendemo.vn", 128),
            pad(f"Số {12 + idx} Pasteur, Q.1, Đà Nẵng", 256),
            pad(f"0908{idx:04d}", 32),
            *pack_date(hire),
            pad("Nhân viên thư viện", 64),
            pad(notes, 256),
        )
        records.append(record)
    return records


def build_accounts() -> list[bytes]:
    # AccountRecord: id[32], fullName[128], dob[3i], active[uint8],
    # accountId[32], username[64], passwordHash[128], role[32], employeeId[32],
    # createdAt[7i], lastLogin[7i]
    fmt = "<32s128s3iB32s64s128s32s32s7i7i"
    accounts_data = [
        ("admin", "Quản trị hệ thống", "ADMIN", "", "A001", "AC001"),
        ("thuhavp", "Trần Thị Thu Hà", "STAFF", "S001", "A002", "AC002"),
        ("hungnv", "Nguyễn Văn Hùng", "STAFF", "S002", "A003", "AC003"),
    ]
    records: list[bytes] = []
    created_at = datetime(2025, 1, 5, 9, 0, 0)
    for username, full_name, role, staff_id, id_field, acc_id in accounts_data:
        record = struct.pack(
            fmt,
            pad(id_field, 32),
            pad(full_name, 128),
            1990, 1, 1,  # dob placeholder
            1,  # active
            pad(acc_id, 32),
            pad(username, 64),
            pad("123456", 128),  # password lưu dạng plaintext trong app này
            pad(role, 32),
            pad(staff_id, 32),
            *pack_datetime(created_at),
            *pack_datetime(created_at),
        )
        records.append(record)
    return records


def build_loans(total_readers: int, total_books: int) -> list[bytes]:
    fmt = "<32s32s32s3i3i3i32si64s"
    borrow_start = date(2025, 9, 11)
    borrow_end = date(2025, 11, 30)
    total_days = (borrow_end - borrow_start).days

    statuses = (
        ["BORROWED"] * 10
        + ["RETURNED"] * 10
        + ["OVERDUE"] * 6
        + ["LOST"] * 4
        + ["DAMAGED"] * 5
    )

    staff_pool = ["thuhavp", "hungnv", "admin"]
    records: list[bytes] = []

    for idx, status in enumerate(statuses):
        borrow_date = borrow_start + timedelta(days=random.randint(0, total_days))
        due_date = borrow_date + timedelta(days=random.randint(10, 20))
        return_date: date | None = None
        fine = 0

        if status == "RETURNED":
            return_date = borrow_date + timedelta(days=random.randint(5, 25))
            fine = max(0, (return_date - due_date).days) * 5000
        elif status == "OVERDUE":
            # chưa trả, nên không có return_date
            fine = max(0, (borrow_end - due_date).days) * 3000
        elif status == "LOST":
            return_date = borrow_date + timedelta(days=random.randint(8, 22))
            fine = 150000 + (idx % 3) * 20000
        elif status == "DAMAGED":
            return_date = borrow_date + timedelta(days=random.randint(6, 18))
            fine = 70000 + (idx % 2) * 15000

        reader_id = f"R{(idx % total_readers) + 1:03d}"
        book_id = f"B{(idx * 2 % total_books) + 1:03d}"

        record = struct.pack(
            fmt,
            pad(f"L{idx + 1:03d}", 32),
            pad(reader_id, 32),
            pad(book_id, 32),
            *pack_date(borrow_date),
            *pack_date(due_date),
            *pack_date(return_date),
            pad(status, 32),
            fine,
            pad(staff_pool[idx % len(staff_pool)], 64),
        )
        records.append(record)
    return records


def build_reports() -> list[bytes]:
    fmt = "<32s64s3i3i3i512s256s32s7i"
    reports = [
        {
            "id": "BC-09-2025",
            "staff": "thuhavp",
            "from": date(2025, 9, 1),
            "to": date(2025, 9, 30),
            "handled": 28,
            "lost": 2,
            "overdue": 5,
            "affected": "B003,B012,B025",
            "notes": "Tổng hợp mượn trả tháng 9.",
            "status": "APPROVED",
            "created_at": datetime(2025, 10, 2, 10, 0, 0),
        },
        {
            "id": "BC-10-2025",
            "staff": "hungnv",
            "from": date(2025, 10, 1),
            "to": date(2025, 10, 31),
            "handled": 30,
            "lost": 1,
            "overdue": 6,
            "affected": "B008,B020,B031,B044",
            "notes": "Tập trung xử lý quá hạn và hỏng.",
            "status": "PENDING",
            "created_at": datetime(2025, 11, 3, 9, 30, 0),
        },
        {
            "id": "BC-11-2025",
            "staff": "admin",
            "from": date(2025, 11, 1),
            "to": date(2025, 11, 30),
            "handled": 34,
            "lost": 2,
            "overdue": 7,
            "affected": "B006,B018,B027,B050",
            "notes": "Báo cáo cuối quý, có thu phí hỏng/mất.",
            "status": "APPROVED",
            "created_at": datetime(2025, 12, 1, 15, 0, 0),
        },
    ]

    records: list[bytes] = []
    for entry in reports:
        records.append(
            struct.pack(
                fmt,
                pad(entry["id"], 32),
                pad(entry["staff"], 64),
                *pack_date(entry["from"]),
                *pack_date(entry["to"]),
                entry["handled"],
                entry["lost"],
                entry["overdue"],
                pad(entry["affected"], 512),
                pad(entry["notes"], 256),
                pad(entry["status"], 32),
                *pack_datetime(entry["created_at"]),
            )
        )
    return records


# Main -----------------------------------------------------------------------
def main() -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    books, available, unavailable = build_books()
    readers = build_readers()
    staffs = build_staffs()
    accounts = build_accounts()
    loans = build_loans(total_readers=len(readers), total_books=len(books))
    reports = build_reports()

    write_file(DATA_DIR / "books.bin", "<32s256s128s64s128s3i2i32s512s256s", books)
    write_file(DATA_DIR / "readers.bin", "<32s128s3iB16s128s256s32s32s3i3ii512s", readers)
    write_file(DATA_DIR / "staffs.bin", "<32s128s3iB16s128s256s32s3i64s256s", staffs)
    write_file(DATA_DIR / "users.bin", "<32s128s3iB32s64s128s32s32s7i7i", accounts)
    write_file(DATA_DIR / "loans.bin", "<32s32s32s3i3i3i32si64s", loans)
    # App đọc report_requests.bin; giữ reports.bin để tương thích
    write_file(DATA_DIR / "report_requests.bin", "<32s64s3i3i3i512s256s32s7i", reports)
    write_file(DATA_DIR / "reports.bin", "<32s64s3i3i3i512s256s32s7i", reports)

    print(f"Wrote {len(books)} books (CÒN: {available}, HẾT/MẤT/HỎNG: {unavailable})")
    print(f"Wrote {len(readers)} readers")
    print(f"Wrote {len(staffs)} staffs")
    print(f"Wrote {len(accounts)} accounts (users.bin)")
    print(f"Wrote {len(loans)} loans (dates 11/09/2025–30/11/2025)")
    print(f"Wrote {len(reports)} reports to report_requests.bin & reports.bin")
    print(f"Data directory: {DATA_DIR}")


if __name__ == "__main__":
    main()
