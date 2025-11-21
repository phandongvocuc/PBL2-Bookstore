import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DATA_DIR = ROOT / "data"


def pack_str(text: str, size: int) -> bytes:
    if text is None:
        text = ""
    b = text.encode("utf-8")
    if len(b) >= size:
        b = b[: size - 1]
    return b + b"\x00" * (size - len(b))


def pack_date(year: int, month: int, day: int) -> bytes:
    return struct.pack("<iii", int(year), int(month), int(day))


def write_books():
    books = [
        ("B001", "Dragon Ball Tập 1", "Akira Toriyama", "Truyện tranh", "Kim Đồng", 1990, 5),
        ("B002", "Dragon Ball Tập 2", "Akira Toriyama", "Truyện tranh", "Kim Đồng", 1990, 5),
        ("B003", "One Piece Tập 1", "Eiichiro Oda", "Truyện tranh", "Kim Đồng", 1997, 5),
        ("B004", "One Piece Tập 2", "Eiichiro Oda", "Truyện tranh", "Kim Đồng", 1997, 5),
        ("B005", "Conan Tập 1", "Gosho Aoyama", "Truyện tranh", "Kim Đồng", 1994, 4),
        ("B006", "Conan Tập 2", "Gosho Aoyama", "Truyện tranh", "Kim Đồng", 1994, 4),
        ("B007", "Doraemon Tập 1", "Fujiko F. Fujio", "Truyện tranh", "Kim Đồng", 1980, 6),
        ("B008", "Doraemon Tập 2", "Fujiko F. Fujio", "Truyện tranh", "Kim Đồng", 1980, 6),
        ("B009", "Thám Tử Lừng Danh Kindaichi 1", "Seimaru Amagi", "Truyện tranh", "Trẻ", 1992, 3),
        ("B010", "Naruto Tập 1", "Masashi Kishimoto", "Truyện tranh", "Kim Đồng", 1999, 5),
        ("B011", "Naruto Tập 2", "Masashi Kishimoto", "Truyện tranh", "Kim Đồng", 1999, 5),
        ("B012", "Tớ Muốn Ăn Tụy Của Cậu", "Yoru Sumino", "Tiểu thuyết", "IPM", 2014, 3),
        ("B013", "5 Centimet Trên Giây", "Shinkai Makoto", "Tiểu thuyết", "IPM", 2007, 3),
        ("B014", "Your Name", "Shinkai Makoto", "Tiểu thuyết", "IPM", 2016, 3),
        ("B015", "Nhà Giả Kim", "Paulo Coelho", "Tiểu thuyết", "Tổng Hợp", 1988, 4),
        ("B016", "Đắc Nhân Tâm", "Dale Carnegie", "Kỹ năng", "First News", 1936, 6),
        ("B017", "Quẳng Gánh Lo Đi Và Vui Sống", "Dale Carnegie", "Kỹ năng", "First News", 1948, 4),
        ("B018", "7 Thói Quen Hiệu Quả", "Stephen R. Covey", "Kỹ năng", "Trẻ", 1989, 4),
        ("B019", "Không Gia Đình", "Hector Malot", "Thiếu nhi", "Trẻ", 1878, 5),
        ("B020", "Hoàng Tử Bé", "Antoine de Saint-Exupéry", "Thiếu nhi", "Kim Đồng", 1943, 5),
        ("B021", "Dế Mèn Phiêu Lưu Ký", "Tô Hoài", "Thiếu nhi", "Kim Đồng", 1941, 5),
        ("B022", "Lão Hạc", "Nam Cao", "Truyện ngắn", "Văn Học", 1943, 4),
        ("B023", "Chí Phèo", "Nam Cao", "Truyện ngắn", "Văn Học", 1941, 4),
        ("B024", "Tắt Đèn", "Ngô Tất Tố", "Tiểu thuyết", "Văn Học", 1937, 3),
        ("B025", "Số Đỏ", "Vũ Trọng Phụng", "Tiểu thuyết", "Văn Học", 1936, 3),
        ("B026", "Tuổi Trẻ Đáng Giá Bao Nhiêu", "Rosie Nguyễn", "Kỹ năng", "Trẻ", 2016, 6),
        ("B027", "Mình Nói Gì Khi Nói Về Hạnh Phúc", "Nguyễn Văn A", "Tản văn", "Trẻ", 2020, 4),
        ("B028", "Trên Đường Băng", "Tony Buổi Sáng", "Kỹ năng", "Trẻ", 2013, 5),
        ("B029", "Đi Tìm Lẽ Sống", "Viktor E. Frankl", "Tâm lý", "First News", 1946, 3),
        ("B030", "Tư Duy Nhanh Và Chậm", "Daniel Kahneman", "Kinh tế", "Nhà Xuất Bản Trẻ", 2011, 3),
        ("B031", "Cha Giàu Cha Nghèo", "Robert Kiyosaki", "Kinh tế", "Trẻ", 1997, 5),
        ("B032", "Dạy Con Làm Giàu 2", "Robert Kiyosaki", "Kinh tế", "Trẻ", 1998, 3),
        ("B033", "Bí Mật Của May Mắn", "Alex Rovira", "Kỹ năng", "Trẻ", 2004, 4),
        ("B034", "Thép Đã Tôi Thế Đấy", "Nikolai Ostrovsky", "Tiểu thuyết", "Văn Học", 1932, 3),
        ("B035", "Chiến Tranh Và Hòa Bình", "Lev Tolstoy", "Tiểu thuyết", "Văn Học", 1869, 2),
        ("B036", "Anh Em Nhà Karamazov", "Fyodor Dostoevsky", "Tiểu thuyết", "Văn Học", 1880, 2),
        ("B037", "Trăm Năm Cô Đơn", "Gabriel Garcia Marquez", "Tiểu thuyết", "Văn Học", 1967, 2),
        ("B038", "Hai Số Phận", "Jeffrey Archer", "Tiểu thuyết", "NXB Văn Học", 1979, 3),
        ("B039", "Đồi Gió Hú", "Emily Bronte", "Tiểu thuyết", "Văn Học", 1847, 3),
        ("B040", "Kỳ Án Ánh Trăng", "Quỷ Cổ Nữ", "Trinh thám", "Trẻ", 2006, 3),
        ("B041", "Đừng Lựa Chọn An Nhàn Khi Còn Trẻ", "Cường Tử", "Kỹ năng", "Trẻ", 2018, 5),
        ("B042", "Ngừng Viện Cớ", "Brian Tracy", "Kỹ năng", "First News", 2010, 4),
        ("B043", "Nhà Thờ Đức Bà Paris", "Victor Hugo", "Tiểu thuyết", "Văn Học", 1831, 2),
        ("B044", "Những Người Khốn Khổ", "Victor Hugo", "Tiểu thuyết", "Văn Học", 1862, 2),
        ("B045", "Science Of Interstellar", "Kip Thorne", "Khoa học", "Trẻ", 2014, 2),
        ("B046", "Vũ Trụ Trong Vỏ Hạt Dẻ", "Stephen Hawking", "Khoa học", "Trẻ", 2001, 3),
        ("B047", "Lược Sử Thời Gian", "Stephen Hawking", "Khoa học", "Trẻ", 1988, 3),
        ("B048", "Sapiens", "Yuval Noah Harari", "Khoa học", "Omega", 2011, 3),
        ("B049", "Hiểu Về Trái Tim", "Minh Niệm", "Tâm lý", "First News", 2011, 4),
        ("B050", "Thiên Tài Bên Trái Kẻ Điên Bên Phải", "Điền Kỳ", "Tâm lý", "Trẻ", 2010, 3),
    ]

    fmt = "<32s256s128s64s128siiiii32s512s"
    fmt = fmt.replace(" ", "")
    assert struct.calcsize(fmt) == 1172

    records = []
    for bid, title, author, genre, publisher, year, qty in books:
        status = "CON" if qty > 0 else "HET"
        summary = ""
        y, m, d = year, 1, 1
        rec = struct.pack(
            fmt,
            pack_str(bid, 32),
            pack_str(title, 256),
            pack_str(author, 128),
            pack_str(genre, 64),
            pack_str(publisher, 128),
            int(y),
            int(m),
            int(d),
            int(year),
            int(qty),
            pack_str(status, 32),
            pack_str(summary, 512),
        )
        records.append(rec)

    path = DATA_DIR / "books.bin"
    header = struct.pack("<II", 1172, len(records))
    path.write_bytes(header + b"".join(records))


def write_readers():
    readers = [
        ("R001", "Nguyễn Văn An", "2000-05-12", "Quận 1 - TP.HCM", "0901234567", "an.nguyen@example.com", "Nam"),
        ("R002", "Trần Thị Bình", "1999-11-03", "Quận 3 - TP.HCM", "0902345678", "binh.tran@example.com", "Nữ"),
        ("R003", "Lê Hoàng Nam", "2001-02-20", "Quận 7 - TP.HCM", "0903456789", "nam.le@example.com", "Nam"),
        ("R004", "Phạm Thu Hà", "1998-09-15", "Quận Bình Thạnh - TP.HCM", "0904567890", "ha.pham@example.com", "Nữ"),
        ("R005", "Võ Minh Quân", "2002-01-30", "Quận Tân Bình - TP.HCM", "0905678901", "quan.vo@example.com", "Nam"),
        ("R006", "Đỗ Thị Mai", "2000-07-22", "Quận Gò Vấp - TP.HCM", "0906789012", "mai.do@example.com", "Nữ"),
        ("R007", "Hoàng Gia Huy", "1997-12-05", "Quận 10 - TP.HCM", "0907890123", "huy.hoang@example.com", "Nam"),
    ]

    parts = [
        "32s",  # id
        "128s",  # fullName
        "i",
        "i",
        "i",  # dob
        "B",  # active
        "16s",  # gender
        "128s",  # email
        "256s",  # address
        "32s",  # phone
        "32s",  # identityCard
        "i",
        "i",
        "i",  # createdDate
        "i",
        "i",
        "i",  # expiryDate
        "i",  # totalBorrowed
        "512s",  # notes
    ]
    fmt = "<" + "".join(parts)
    assert struct.calcsize(fmt) == 1177

    records = []
    for rid, name, dob_str, addr, phone, email, gender in readers:
        year, month, day = map(int, dob_str.split("-"))
        dob = pack_date(year, month, day)
        created = pack_date(2023, 1, 1)
        expiry = pack_date(2026, 1, 1)
        total_borrowed = 0
        identity = f"ID{rid[1:]}"
        notes = ""

        rec = struct.pack(
            fmt,
            pack_str(rid, 32),
            pack_str(name, 128),
            *struct.unpack("<iii", dob),
            1,  # active
            pack_str(gender, 16),
            pack_str(email, 128),
            pack_str(addr, 256),
            pack_str(phone, 32),
            pack_str(identity, 32),
            *struct.unpack("<iii", created),
            *struct.unpack("<iii", expiry),
            int(total_borrowed),
            pack_str(notes, 512),
        )
        records.append(rec)

    path = DATA_DIR / "readers.bin"
    header = struct.pack("<II", 1177, len(records))
    path.write_bytes(header + b"".join(records))


def write_loans():
    loans = [
        ("L001", "R001", "B001", "2025-10-01", "2025-10-15", "ĐANG_MƯỢN"),
        ("L002", "R001", "B016", "2025-10-01", "2025-10-15", "ĐÃ_TRẢ"),
        ("L003", "R002", "B003", "2025-10-03", "2025-10-17", "ĐANG_MƯỢN"),
        ("L004", "R002", "B020", "2025-10-03", "2025-10-17", "ĐÃ_TRẢ"),
        ("L005", "R003", "B010", "2025-10-05", "2025-10-19", "ĐANG_MƯỢN"),
        ("L006", "R003", "B021", "2025-10-05", "2025-10-19", "ĐANG_MƯỢN"),
        ("L007", "R004", "B015", "2025-10-07", "2025-10-21", "ĐÃ_TRẢ"),
        ("L008", "R004", "B026", "2025-10-07", "2025-10-21", "ĐANG_MƯỢN"),
        ("L009", "R005", "B028", "2025-10-09", "2025-10-23", "ĐANG_MƯỢN"),
        ("L010", "R005", "B031", "2025-10-09", "2025-10-23", "ĐÃ_TRẢ"),
        ("L011", "R006", "B041", "2025-10-10", "2025-10-24", "ĐANG_MƯỢN"),
        ("L012", "R006", "B042", "2025-10-10", "2025-10-24", "ĐANG_MƯỢN"),
        ("L013", "R007", "B035", "2025-10-11", "2025-10-25", "ĐÃ_TRẢ"),
        ("L014", "R007", "B046", "2025-10-11", "2025-10-25", "ĐANG_MƯỢN"),
        ("L015", "R001", "B002", "2025-10-12", "2025-10-26", "ĐANG_MƯỢN"),
        ("L016", "R002", "B004", "2025-10-13", "2025-10-27", "ĐANG_MƯỢN"),
        ("L017", "R003", "B018", "2025-10-13", "2025-10-27", "ĐÃ_TRẢ"),
        ("L018", "R004", "B029", "2025-10-14", "2025-10-28", "ĐANG_MƯỢN"),
        ("L019", "R005", "B049", "2025-10-14", "2025-10-28", "ĐANG_MƯỢN"),
        ("L020", "R006", "B050", "2025-10-15", "2025-10-29", "ĐANG_MƯỢN"),
    ]

    parts = [
        "32s",  # loanId
        "32s",  # readerId
        "32s",  # bookId
        "i",
        "i",
        "i",  # borrowDate
        "i",
        "i",
        "i",  # dueDate
        "i",
        "i",
        "i",  # returnDate
        "32s",  # status
        "i",  # fine
        "64s",  # staffUsername
    ]
    fmt = "<" + "".join(parts)
    assert struct.calcsize(fmt) == 232

    def normalize_status(s: str) -> str:
        s = s.upper()
        if "ĐÃ" in s or "DA" in s:
            return "RETURNED"
        if "TRẢ" in s or "TRA" in s:
            return "RETURNED"
        return "BORROWED"

    records = []
    for lid, rid, bid, borrow_str, due_str, status in loans:
        by, bm, bd = map(int, borrow_str.split("-"))
        dy, dm, dd = map(int, due_str.split("-"))
        borrow = pack_date(by, bm, bd)
        due = pack_date(dy, dm, dd)
        norm_status = normalize_status(status)
        if norm_status == "RETURNED":
            ret = due
        else:
            ret = pack_date(0, 0, 0)
        fine = 0
        staff_username = "admin"

        rec = struct.pack(
            fmt,
            pack_str(lid, 32),
            pack_str(rid, 32),
            pack_str(bid, 32),
            *struct.unpack("<iii", borrow),
            *struct.unpack("<iii", due),
            *struct.unpack("<iii", ret),
            pack_str(norm_status, 32),
            int(fine),
            pack_str(staff_username, 64),
        )
        records.append(rec)

    path = DATA_DIR / "loans.bin"
    header = struct.pack("<II", 232, len(records))
    path.write_bytes(header + b"".join(records))


def main():
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    write_books()
    write_readers()
    write_loans()
    print("Generated books.bin, readers.bin, loans.bin in", DATA_DIR)


if __name__ == "__main__":
    main()
