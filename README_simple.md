
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
- Không xử lý logic, chỉ giao tiếp với tầng Logic/Business

**File code**: `ui/MainWindow.cpp`, `ui/BookDialog.cpp`, `ui/LoginDialog.cpp`

### **Tầng 2: Logic/Business - Xử Lý Nghiệp Vụ**  
**Nhiệm vụ**: Gom Service + Manager thành một tầng xử lý hoàn chỉnh
- Cung cấp API đơn giản cho UI, đồng thời triển khai toàn bộ rule nghiệp vụ
- Kiểm tra dữ liệu đầu vào, sinh ID, áp dụng quota mượn sách, cập nhật trạng thái
- Quản lý cache/tạm thời và chuẩn bị dữ liệu trước khi lưu
- Chuyển đổi kiểu dữ liệu giữa UI và Repository khi cần

**File code**: `service/BookService.cpp`, `service/LoanService.cpp`, `service/AccountService.cpp`, `service/ReaderService.cpp`, `service/StaffService.cpp`, `service/ReportService.cpp`

### **Tầng 3: Repository - Truy Cập Dữ Liệu**
**Nhiệm vụ**: Gộp Repository + Store để xử lý toàn bộ việc lưu trữ
- Chuyển đối tượng Book/Loan thành table data và ngược lại
- Quản lý việc đọc/ghi file an toàn, backup, validate format
- Cung cấp API thống nhất cho tầng Logic/Business
- Dễ dàng thay thế backend (file → database) mà không ảnh hưởng UI/Logic

**File code**: `repository/BooksRepository.cpp`, `repository/LoansRepository.cpp`, `scoure/datafile/BinaryFileStore.cpp`, `header/datafile/BinaryFileStore.h`

#### **Tầng phụ: FileStore - Hệ Thống File Thấp Nhất**
- Tách riêng phần I/O nhị phân để dễ thay thế bằng backend khác.
- Chịu trách nhiệm đọc/ghi file `.bin`, tạo backup, khôi phục nếu lỗi.
- Được Repository layer gọi thông qua API `BinaryFileStore::read/write`.

**File code**: `scoure/datafile/BinaryFileStore.cpp`, `header/datafile/BinaryFileStore.h`

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
    for (auto &book : books) {
        normalized = normalizeAvailability(book) || normalized;
    }
    if (normalized) repository.saveAll(books);
    return books;
}
```

**Giải thích Logic/Business**:
- Service giờ đảm nhận luôn phần nghiệp vụ: validate, chuẩn hóa trạng thái, sinh mã, quản lý cache tạm.
- Hàm helper `ensureLoaded()/persist()` gom logic đọc-ghi và đảm bảo dữ liệu được normalize trước khi trả cho UI.
- Không còn lớp Manager riêng, nên UI gọi trực tiếp Service và chỉ có đúng một tầng Logic để debug/test.

### 3. Repository (BooksRepository.cpp + BinaryFileStore.cpp) - Tầng Truy Cập Dữ Liệu
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
- Kết hợp Repository (mapping object ↔ bảng dữ liệu) và BinaryFileStore (đọc/ghi binary) thành một tầng thống nhất.
- **Serialization/Deserialization**: chuyển đổi dữ liệu, validate format, gom thao tác theo batch để tăng tốc.
- **Error handling**: backup/restore, exception handling, đảm bảo dữ liệu toàn vẹn trước khi trả về Logic layer.
- **Backend abstraction**: muốn đổi sang SQLite/PostgreSQL chỉ cần thay BinaryFileStore, UI và Logic giữ nguyên.

Phần BinaryFileStore đảm nhận I/O nhị phân cấp thấp:
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

## Luồng Dữ Liệu Hoàn Chỉnh: UI → Repository

Khi user thêm sách, dữ liệu đi qua 3 tầng:

```
1. UI: User nhập "Harry Potter" → Book object
   ↓
2. Logic/Business: BookService nhận yêu cầu, validate + sinh ID → Book hoàn chỉnh
   ↓  
3. Repository: BooksRepository map Book → table, BinaryFileStore ghi binary xuống disk
```

**Dữ liệu tại mỗi tầng**:
- **UI**: `Book{title="Harry Potter", author="J.K.Rowling"}`
- **Logic/Business**: `Book{id="B001", title="Harry Potter", status="available"}` sau khi áp dụng rule
- **Repository**: `["B001", "Harry Potter", "J.K.Rowling", "Fantasy", "", "", "5", "available"]` → `[4][B001][12][Harry Potter]...` (binary)

## Tại Sao Chia 3 Tầng (và có thể bung thành 5 khi cần)?

1. **Separation of Concerns**: Vẫn giữ rõ 3 trách nhiệm chính
   - UI: Giao diện/input-output  
   - Logic/Business: Toàn bộ nghiệp vụ và API cho UI
   - Repository: Mapping dữ liệu + I/O

2. **Easy Maintenance**: Ít tầng hơn nên dễ đọc nhưng vẫn dễ mở rộng
   - Đổi UI Qt → Web? Chỉ chạm vào UI
   - Đổi file → Database? Chỉ sửa Repository
   - Thêm business rule? Chỉ thao tác Logic/Business

3. **Testability**: Test từng tầng riêng biệt
   ```cpp
   // Test Repository mapping
   Book book = createTestBook();
   auto row = repository.bookToRow(book);
   EXPECT_EQ(row[0], "B001");
   
   // Test Logic/Business (BookService)  
   bool result = bookService.addBook(emptyTitleBook);
   EXPECT_FALSE(result); // Should reject
   
   // Test BinaryFileStore I/O
   Table testData = createTestTable();
   store.write("test.bin", testData);
   auto loaded = store.read("test.bin");
   EXPECT_EQ(loaded, testData);
   ```

4. **Team Collaboration**: Chia việc gọn hơn
   - Frontend dev: UI layer
   - Core/backend dev: Logic/Business (Service + Manager codebase)
   - Data engineer: Repository layer (Repository + BinaryFileStore)
   - QA engineer: Viết test cho từng tầng

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
   Khi dự án mở rộng, chỉ cần bung tầng Logic/Business thành Service + Manager riêng hoặc tách Repository thành Repository + Store như cũ để phục vụ nhu cầu phức tạp hơn.

## Cấu Trúc Thư Mục

```
Pbl2_book/
├── ui/                      # Tầng 1 - Giao diện Qt
│   ├── MainWindow.cpp
│   ├── BookDialog.cpp
│   └── LoginDialog.cpp
├── service/                 # Tầng Logic/Business hợp nhất
│   ├── BookService.cpp
│   ├── LoanService.cpp
│   ├── AccountService.cpp
│   ├── ReaderService.cpp
│   ├── StaffService.cpp
│   └── ReportService.cpp
├── repository/             # Tầng Repository - mapping object ↔ bảng
│   ├── BooksRepository.cpp
│   ├── LoansRepository.cpp
│   ├── AccountsRepository.cpp
│   └── ...
├── scoure/
│   ├── datafile/              # Tiện ích đọc/ghi file
│   │   ├── BinaryFileStore.cpp
│   │   └── DataPaths.cpp
│   └── ...
├── header/
│   ├── datafile/
│   │   ├── BinaryFileStore.h
│   │   └── DataPaths.h
│   └── ...
├── model/                   # Đối tượng dữ liệu
│   ├── Book.cpp
│   └── Loan.cpp
└── core/                    # Thư viện hỗ trợ (Date, IdGenerator,...)
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
