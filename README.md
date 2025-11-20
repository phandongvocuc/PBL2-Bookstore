
## Chương Trình Làm Được Gì?

- ✅ **Quản lý sách**: Thêm, sửa, xóa thông tin sách
- ✅ **Quản lý độc giả**: Đăng ký thẻ thư viện, thông tin bạn đọc
- ✅ **Mượn/trả sách**: Theo dõi ai mượn sách gì, hạn trả
- ✅ **Báo cáo**: Xem thống kê sách, độc giả
- ✅ **Đăng nhập**: Phân quyền cho thủ thư và admin

## Cách Tổ Chức Code - 3 Tầng

### **Tầng 1: UI - Giao Diện** 
**Nhiệm vụ**: Hiển thị thông tin và nhận input từ người dùng
- Tạo các cửa sổ, nút bấm, bảng hiển thị
- Nhận dữ liệu từ người dùng (form thêm sách, dialog đăng nhập)
- Hiển thị kết quả (danh sách sách, thông báo lỗi)
- Không xử lý logic, chỉ giao tiếp với Service

**File code**: `ui/MainWindow.cpp`, `ui/BookDialog.cpp`, `ui/LoginDialog.cpp`

### **Tầng 2: Logic/Business - Service**  
**Nhiệm vụ**: Giữ toàn bộ logic nghiệp vụ và API cho UI
- Nhận yêu cầu từ UI (thêm sách, tìm kiếm, đăng nhập) và tự xử lý validation/quy tắc
- Sinh ID, chuẩn hóa trạng thái, áp dụng rule mượn trả, cập nhật cache
- Giao tiếp trực tiếp với Repository layer để đọc/ghi dữ liệu
- Chuyển đổi kiểu dữ liệu giữa UI và tầng lưu trữ khi cần

**File code**: `service/BookService.cpp`, `service/LoanService.cpp`, `service/AccountService.cpp`, `service/ReaderService.cpp`, `service/StaffService.cpp`, `service/ReportService.cpp`

### **Tầng 3: Repository - Lưu Trữ Dữ Liệu**
**Nhiệm vụ**: Chuyển đổi giữa đối tượng và dữ liệu file
- Chuyển đối tượng Book thành dòng text để lưu file
- Đọc dòng text từ file và tạo thành đối tượng Book
- Quản lý việc đọc/ghi file an toàn
- Validate dữ liệu khi đọc từ file

**File code**: `repository/BooksRepository.cpp`, `repository/LoansRepository.cpp`, `scoure/datafile/BinaryFileStore.cpp`, `header/datafile/BinaryFileStore.h`

## Ví Dụ Cụ Thể: Thêm Sách Mới

### 1. UI (MainWindow.cpp) - Tầng Giao Diện
```cpp
void MainWindow::onAddBookClicked() {
    // Khi user click nút "Thêm sách", hàm này được gọi
    
    // Tạo và hiển thị dialog để user nhập thông tin sách
    BookDialog dialog;
    if (dialog.exec() == QDialog::Accepted) { // User click OK
        
        // Lấy thông tin sách từ dialog (title, author, genre...)
        Book newBook = dialog.getBook();
        
        // UI KHÔNG xử lý logic - chỉ gọi Service
        bool success = bookService.addBook(newBook);
        
        // Hiển thị kết quả cho user
        if (success) {
            QMessageBox::information(this, "Thành công", "Đã thêm sách!");
            refreshBookList(); // Cập nhật danh sách hiển thị
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể thêm sách!");
        }
    }
}
```

**Giải thích UI**:
- UI chỉ quan tâm **hiển thị** và **nhận input**
- Không có logic kiểm tra, tính toán
- Chỉ gọi Service và hiển thị kết quả

### 2. Logic/Business (BookService.cpp) - Tầng Xử Lý
```cpp
bool BookService::addBook(const model::Book &book) const {
    auto books = ensureLoaded();

    model::Book copy = book;
    const auto target = copy.getId();
    for (const auto &existing : books) {
        if (existing.getId().compare(target, custom::CaseSensitivity::Insensitive) == 0) {
            return false; // Tránh trùng ID
        }
    }

    normalizeAvailability(copy);
    books.pushBack(copy);
    persist(books);
    return true;
}

custom::DynamicArray<model::Book> BookService::ensureLoaded() const {
    auto books = repository.loadAll();
    bool normalized = false;
    for (auto &b : books) {
        normalized = normalizeAvailability(b) || normalized;
    }
    if (normalized) repository.saveAll(books);
    return books;
}
```

**Giải thích Logic/Business**:
- Service giờ kiêm luôn business logic: validate input, sinh trạng thái, giới hạn số lượng, cập nhật cache.
- Các helper như `ensureLoaded()` bảo đảm dữ liệu nhất quán trước khi trả ngược lên UI.
- Không còn lớp Manager riêng nên việc test/debug gói gọn trong service.

### 3. Repository (BooksRepository.cpp) - Tầng Lưu Trữ
```cpp
bool BooksRepository::save(const Book& book) {
    // Repository chuyên về "dịch thuật" object ↔ file data
    
    // BƯỚC 1: Load dữ liệu hiện tại
    auto allBooks = loadAll();
    
    // BƯỚC 2: Kiểm tra trùng ID
    for (int i = 0; i < allBooks.size(); i++) {
        if (allBooks[i].getId() == book.getId()) {
            allBooks[i] = book; // Update existing
            return saveAll(allBooks);
        }
    }
    
    // BƯỚC 3: Thêm sách mới
    allBooks.push_back(book);
    return saveAll(allBooks);
}

bool BooksRepository::saveAll(const DynamicArray<Book>& books) {
    // Chuyển đổi từ objects sang table data
    core::Table tableData;
    
    for (const auto& book : books) {
        core::Row row;
        row.push_back(book.getId());           // "B001"
        row.push_back(book.getTitle());        // "Harry Potter"
        row.push_back(book.getAuthor());       // "J.K. Rowling"
        row.push_back(book.getGenre());        // "Fantasy"
        row.push_back(book.getPublisher());    // "Bloomsbury"
        row.push_back(book.getPublishDate().toIsoString()); // "1997-06-26"
        row.push_back(String::fromInt(book.getQuantity())); // "5"
        row.push_back(book.getStatus());       // "available"
        
        tableData.push_back(row);
    }
    
    // Ghi xuống file binary thông qua BinaryFileStore
    return BinaryFileStore::write(dataPath, tableData);
}

DynamicArray<Book> BooksRepository::loadAll() {
    // Đọc từ file và chuyển thành objects
    DynamicArray<Book> books;
    
    // Đọc table data từ file
    auto tableData = BinaryFileStore::read(dataPath);
    
    // Chuyển đổi từng row thành Book object
    for (const auto& row : tableData) {
        if (row.size() < 8) continue; // Skip invalid rows
        
        Book book;
        book.setId(row[0]);                    // "B001" → ID
        book.setTitle(row[1]);                 // "Harry Potter" → Title
        book.setAuthor(row[2]);                // "J.K. Rowling" → Author
        book.setGenre(row[3]);                 // "Fantasy" → Genre
        book.setPublisher(row[4]);             // "Bloomsbury" → Publisher
        book.setPublishDate(Date::fromIsoString(row[5])); // "1997-06-26" → Date
        book.setQuantity(String::toInt(row[6])); // "5" → int 5
        book.setStatus(row[7]);                // "available" → Status
        
        books.push_back(book);
    }
    
    return books;
}
```

**Giải thích Repository**:
- **Data mapping**: Object ↔ File format
- **Serialization/Deserialization**: Chuyển đổi kiểu dữ liệu
- **Data validation**: Kiểm tra format khi đọc file
- **Error handling**: Xử lý file corrupt, missing
- **Performance**: Batch operations thay vì từng record

#### Tầng phụ: FileStore (BinaryFileStore.cpp) - Hạ tầng File
- Chỉ phụ trách đọc/ghi binary, không hiểu mô hình dữ liệu.
- Quản lý backup/restore, đảm bảo an toàn khi ghi file.
- Có thể thay thế bằng driver khác (SQLite, REST…) mà không ảnh hưởng Repository.
- **File code**: `scoure/datafile/BinaryFileStore.cpp`, `header/datafile/BinaryFileStore.h`

### 5. Store (BinaryFileStore.cpp) - Tầng File I/O
```cpp
class BinaryFileStore {
    // Store là tầng thấp nhất - chỉ biết đọc/ghi file binary
    
public:
    static core::Table read(const CustomString& filePath) {
        // BƯỚC 1: Mở file binary để đọc
        std::ifstream file(filePath.toStdString(), std::ios::binary);
        if (!file.is_open()) {
            return core::Table(); // Trả về table rỗng nếu lỗi
        }
        
        core::Table table;
        
        // BƯỚC 2: Đọc số lượng rows
        int rowCount;
        file.read(reinterpret_cast<char*>(&rowCount), sizeof(rowCount));
        
        // BƯỚC 3: Đọc từng row
        for (int i = 0; i < rowCount; i++) {
            core::Row row;
            
            // Đọc số lượng columns trong row này
            int colCount;
            file.read(reinterpret_cast<char*>(&colCount), sizeof(colCount));
            
            // Đọc từng column (string)
            for (int j = 0; j < colCount; j++) {
                // Đọc độ dài string
                int stringLength;
                file.read(reinterpret_cast<char*>(&stringLength), sizeof(stringLength));
                
                // Đọc nội dung string
                char* buffer = new char[stringLength + 1];
                file.read(buffer, stringLength);
                buffer[stringLength] = '\0';
                
                row.push_back(CustomString(buffer));
                delete[] buffer;
            }
            
            table.push_back(row);
        }
        
        file.close();
        return table;
    }
    
    static bool write(const CustomString& filePath, const core::Table& table) {
        // BƯỚC 1: Mở file binary để ghi
        std::ofstream file(filePath.toStdString(), std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        
        // BƯỚC 2: Ghi số lượng rows
        int rowCount = table.size();
        file.write(reinterpret_cast<const char*>(&rowCount), sizeof(rowCount));
        
        // BƯỚC 3: Ghi từng row
        for (const auto& row : table) {
            // Ghi số lượng columns
            int colCount = row.size();
            file.write(reinterpret_cast<const char*>(&colCount), sizeof(colCount));
            
            // Ghi từng column
            for (const auto& cell : row) {
                // Ghi độ dài string
                int stringLength = cell.length();
                file.write(reinterpret_cast<const char*>(&stringLength), sizeof(stringLength));
                
                // Ghi nội dung string
                file.write(cell.toStdString().c_str(), stringLength);
            }
        }
        
        file.close();
        return true;
    }
    
    // Version an toàn hơn với error handling
    static core::Table readSafe(const CustomString& filePath) {
        try {
            return read(filePath);
        } catch (const std::exception& e) {
            qWarning() << "Error reading file:" << filePath.toQString() << e.what();
            return core::Table(); // Trả về empty table
        }
    }
    
    static bool writeSafe(const CustomString& filePath, const core::Table& table) {
        try {
            // Tạo backup trước khi ghi
            CustomString backupPath = filePath + ".backup";
            if (QFile::exists(filePath.toQString())) {
                QFile::copy(filePath.toQString(), backupPath.toQString());
            }
            
            bool result = write(filePath, table);
            
            if (result) {
                // Ghi thành công, xóa backup
                QFile::remove(backupPath.toQString());
            } else {
                // Ghi thất bại, restore backup
                if (QFile::exists(backupPath.toQString())) {
                    QFile::remove(filePath.toQString());
                    QFile::rename(backupPath.toQString(), filePath.toQString());
                }
            }
            
            return result;
        } catch (const std::exception& e) {
            qWarning() << "Error writing file:" << filePath.toQString() << e.what();
            return false;
        }
    }
};
```

**File Binary Format**:
```
[4 bytes: số rows]  
  [4 bytes: số cols của row 1]
    [4 bytes: độ dài string 1][string 1 content]
    [4 bytes: độ dài string 2][string 2 content]
    ...
  [4 bytes: số cols của row 2]  
    [4 bytes: độ dài string 1][string 2 content]
    ...
...
```

**Ví dụ file books.bin**:
```
Số rows: 2
Row 1: 4 columns
  - "B001" (4 chars)
  - "Harry Potter" (12 chars)  
  - "J.K. Rowling" (12 chars)
  - "5" (1 char)
Row 2: 4 columns
  - "B002" (4 chars)
  - "The Hobbit" (10 chars)
  - "J.R.R. Tolkien" (14 chars)
  - "3" (1 char)
```

**Giải thích Store**:
- **Lowest level**: Chỉ biết binary I/O, không hiểu data structure
- **Format agnostic**: Lưu bất kỳ Table nào, không quan tâm là Book hay Loan  
- **Error handling**: Backup/restore, exception handling
- **Performance**: Binary format nhanh hơn text
- **Reliability**: Atomic operations, consistency checks

## Luồng Dữ Liệu Hoàn Chỉnh: UI → Store

Khi user thêm sách, dữ liệu đi qua tất cả 5 tầng:

```
1. UI: User nhập "Harry Potter" → Book object
   ↓
2. Service: book.addBook(harryPotter) → chuyển cho Manager  
   ↓
3. Manager: Kiểm tra + sinh ID → book với ID "B001"
   ↓  
4. Repository: Book object → ["B001","Harry Potter","Rowling","5"]
   ↓
5. Store: String array → Binary file trên disk
```

**Dữ liệu tại mỗi tầng**:
- **UI**: `Book{title="Harry Potter", author="J.K.Rowling"}`
- **Service**: Same Book object (pass-through)
- **Manager**: `Book{id="B001", title="Harry Potter", status="available"}`  
- **Repository**: `["B001", "Harry Potter", "J.K.Rowling", "Fantasy", "", "", "5", "available"]`
- **Store**: `[4][B001][12][Harry Potter][12][J.K.Rowling]...` (binary)

## Tại Sao Chia 3 Tầng?

1. **Separation of Concerns**: Giữ rõ 3 trách nhiệm chính
   - UI: Giao diện/input-output  
   - Logic/Business (Service): xử lý nghiệp vụ, validate, sinh ID
   - Repository: mapping dữ liệu + I/O

2. **Easy Maintenance**: Ít tầng hơn nhưng vẫn dễ mở rộng
   - Đổi UI Qt → Web? Chỉ đụng UI
   - Đổi file → Database? Chỉ sửa Repository
   - Thêm business rule? Chỉ sửa Service

3. **Testability**: Test từng tầng riêng biệt
   ```cpp
   // Test Repository mapping
   Book book = createTestBook();
   auto row = repository.bookToRow(book);
   EXPECT_EQ(row[0], "B001");
   
   // Test Logic/Business (BookService)  
   bool result = bookService.addBook(emptyTitleBook);
   EXPECT_FALSE(result); // Should reject
   
   // Test File I/O
   Table testData = createTestTable();
   store.write("test.bin", testData);
   auto loaded = store.read("test.bin");
   EXPECT_EQ(loaded, testData);
   ```

4. **Team Collaboration**: Chia việc gọn hơn
   - Frontend dev: UI layer
   - Core/backend dev: Book/Loan/Account Service
   - Data engineer: Repository + Binary store
   - QA engineer: Test từng tầng

5. **Reusability**: Logic có thể dùng nhiều nơi
   ```cpp
   // Desktop app
   QtMainWindow → BookService
   
   // Web API  
   RestController → BookService
   
   // Command line
   CLI → BookService
   
   // Same business logic, khác interface!
   ```

## Cấu Trúc Thư Mục

```
Pbl2_book/
├── ui/              # Giao diện (tầng 1)
│   ├── MainWindow.cpp
│   ├── BookDialog.cpp
│   └── LoginDialog.cpp
├── service/         # Logic/Business (tầng 2)
│   ├── BookService.cpp
│   ├── LoanService.cpp
│   ├── AccountService.cpp
│   └── ...
├── repository/     # Repository (tầng 3)
│   ├── BooksRepository.cpp
│   ├── LoansRepository.cpp
│   └── ...
├── scoure/datafile/   # Tiện ích làm việc với file
│   ├── BinaryFileStore.cpp
│   └── DataPaths.cpp
├── header/datafile/
│   ├── BinaryFileStore.h
│   └── DataPaths.h
├── model/           # Đối tượng dữ liệu
│   ├── Book.cpp
│   └── Loan.cpp
└── core/            # Thư viện hỗ trợ
    ├── Date.cpp
    └── IdGenerator.cpp
```

## Cách Chạy Chương Trình

### Yêu Cầu
- Qt 6.2 trở lên
- CMake 3.20 trở lên
- C++17

### Lệnh Build
```bash
# Tạo thư mục build
mkdir build
cd build

# Configure
cmake ..

# Compile
make

# Chạy
./pbl2_book
```

## Các Đối Tượng Chính

### Book (Sách)
```cpp
class Book {
    string id;          // B001, B002...
    string title;       // Tên sách
    string author;      // Tác giả
    string genre;       // Thể loại
    int quantity;       // Số lượng
    string status;      // available/unavailable
};
```

### Loan (Mượn sách)
```cpp
class Loan {
    string id;          // L001, L002...
    string bookId;      // ID sách được mượn
    string readerId;    // ID người mượn
    Date borrowDate;    // Ngày mượn
    Date dueDate;       // Hạn trả
    string status;      // active/returned
};
```

### Reader (Độc giả)
```cpp
class Reader {
    string id;          // R001, R002...
    string name;        // Họ tên
    string studentId;   // Mã sinh viên
    string phone;       // Số điện thoại
    bool active;        // Còn hoạt động không
};
```

## Container Tự Chế - DynamicArray

Dự án sử dụng `custom::DynamicArray` thay vì `std::vector`. Đây là container tự implement:

### Cách Hoạt Động Của DynamicArray
```cpp
template <typename T>
class DynamicArray {
private:
    T* _data;           // Con trỏ đến vùng nhớ chứa dữ liệu
    int _size;          // Số phần tử hiện tại
    int _capacity;      // Dung lượng tối đa hiện tại

public:
    DynamicArray() : _data(nullptr), _size(0), _capacity(0) {}
    
    void push_back(const T& item) {
        // Nếu hết chỗ, tự động mở rộng
        if (_size >= _capacity) {
            int newCapacity = (_capacity == 0) ? 4 : _capacity * 2;
            reallocate(newCapacity);
        }
        
        _data[_size] = item;  // Thêm phần tử mới
        _size++;
    }
    
private:
    void reallocate(int newCapacity) {
        // Tạo vùng nhớ mới lớn hơn
        T* newData = new T[newCapacity];
        
        // Copy dữ liệu cũ sang vùng nhớ mới
        for (int i = 0; i < _size; i++) {
            newData[i] = _data[i];
        }
        
        // Xóa vùng nhớ cũ
        delete[] _data;
        
        // Cập nhật con trỏ và capacity
        _data = newData;
        _capacity = newCapacity;
    }
};
```

### Sử Dụng DynamicArray
```cpp
// Tạo danh sách sách
DynamicArray<Book> books;

// Thêm sách - tự động mở rộng khi cần
books.push_back(harryPotter);   // Capacity: 4
books.push_back(hobbit);        // Capacity: 4  
books.push_back(lordOfRings);   // Capacity: 4
books.push_back(gameOfThrones); // Capacity: 4
books.push_back(narnia);        // Capacity: 8 (tự động x2)

// Truy cập dữ liệu
cout << books.size();           // 5
cout << books[0].getTitle();    // "Harry Potter"

// Duyệt qua tất cả sách
for (int i = 0; i < books.size(); i++) {
    cout << books[i].getTitle();
}

// Hoặc dùng range-based for loop
for (const auto& book : books) {
    cout << book.getTitle();
}
```

### Tại Sao Tự Chế Container?

1. **Mục đích học tập**: Hiểu cách `std::vector` hoạt động bên trong
2. **Kiểm soát memory**: Biết chính xác khi nào allocate/deallocate
3. **Tương thích Qt**: Có method `append()`, `isEmpty()` giống Qt
4. **Custom optimization**: Có thể optimize cho use-case cụ thể

### So Sánh Với std::vector

| Feature | DynamicArray | std::vector |
|---------|--------------|-------------|
| `push_back()` | ✅ | ✅ |
| `size()` | ✅ | ✅ |
| `operator[]` | ✅ | ✅ |
| `append()` | ✅ | ❌ |
| `isEmpty()` | ✅ | ❌ (có `empty()`) |
| Iterator | ✅ | ✅ |
| Exception safety | Basic | Strong |
| Performance | Good | Excellent |

## Core Utilities - Các Tiện Ích Hỗ Trợ

### IdGenerator - Sinh ID Tự Động
```cpp
class IdGenerator {
private:
    static int bookCounter;    // Đếm số sách
    static int loanCounter;    // Đếm số lần mượn
    static int readerCounter;  // Đếm số độc giả

public:
    static CustomString generateBookId() {
        return CustomString("B") + CustomString::number(++bookCounter);
        // Tạo ra: B001, B002, B003...
    }
    
    static CustomString generateLoanId() {
        return CustomString("L") + CustomString::number(++loanCounter);
        // Tạo ra: L001, L002, L003...
    }
    
    static CustomString generateReaderId() {
        return CustomString("R") + CustomString::number(++readerCounter);
        // Tạo ra: R001, R002, R003...
    }
};

// Sử dụng
CustomString newBookId = IdGenerator::generateBookId();   // "B001"
CustomString newLoanId = IdGenerator::generateLoanId();   // "L001"
```

### Date - Xử Lý Ngày Tháng
```cpp
class Date {
private:
    int year, month, day;

public:
    // Tạo Date từ string ISO "1997-06-26"
    static Date fromIsoString(const CustomString& isoString) {
        // Parse "1997-06-26" thành year=1997, month=6, day=26
    }
    
    // Chuyển Date thành string "1997-06-26"
    CustomString toIsoString() const {
        return CustomString::number(year) + "-" + 
               CustomString::number(month) + "-" + 
               CustomString::number(day);
    }
    
    // Thêm số ngày
    Date addDays(int days) const {
        // Logic tính toán ngày tháng (xử lý tháng 30/31 ngày, năm nhuận...)
    }
    
    // Lấy ngày hôm nay
    static Date today() {
        // Lấy ngày hiện tại từ hệ thống
    }
};

// Sử dụng
Date publishDate = Date::fromIsoString("1997-06-26");
Date dueDate = Date::today().addDays(14);  // 2 tuần sau
CustomString dateString = dueDate.toIsoString();
```

### CustomString - String Tùy Chỉnh
```cpp
class CustomString {
private:
    char* _data;
    int _length;

public:
    // Chuyển đổi với Qt
    QString toQString() const {
        return QString::fromUtf8(_data, _length);
    }
    
    static CustomString fromQString(const QString& qstr) {
        QByteArray utf8 = qstr.toUtf8();
        return CustomString(utf8.data(), utf8.length());
    }
    
    // Chuyển đổi kiểu số
    int toInt(bool* ok = nullptr) const {
        return QString::fromUtf8(_data, _length).toInt(ok);
    }
    
    static CustomString number(int value) {
        return fromQString(QString::number(value));
    }
    
    // Các operation cơ bản
    bool isEmpty() const { return _length == 0; }
    bool operator==(const CustomString& other) const;
    CustomString operator+(const CustomString& other) const;
};

// Sử dụng
CustomString bookId = "B001";
CustomString quantity = "5";

int number = quantity.toInt();        // "5" → 5
QString qtStr = bookId.toQString();   // Cho Qt widgets
CustomString combined = bookId + "_" + quantity; // "B001_5"
```
