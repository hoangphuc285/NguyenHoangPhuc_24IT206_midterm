# BÀI TẬP GIỮA KỲ: THỰC HIỆN LỆNH `ls` TRÊN UNIX

* **Môn học:** Lập trình hệ thống 

* **Họ và tên sinh viên:** Nguyễn Hoàng Phúc

* **MSSV: 24IT206**

* **GitHub:** https://github.com/hoangphuc285/NguyenHoangPhuc_24IT206_midterm

## 1. TỔNG QUAN DỰ ÁN

Dự án này thực hiện việc viết lại công cụ dòng lệnh `ls` trong hệ điều hành UNIX/Linux từ đầu bằng ngôn ngữ C, dựa trên tập hợp tính năng trong tài liệu tả kỹ thuật của NetBSD.

Chương trình cho phép người dùng liệt kê danh sách tập tin/thư mục, hiển thị thông tin chi tiết (quyền truy cập, chủ sở hữu, nhóm, kích thước, thời gian, inode, số block,...), hỗ trợ lọc, định dạng dữ liệu và sắp xếp theo nhiều tiêu chí khác nhau.

## 2. CẤU TRÚC MÃ NGUỒN VÀ THIẾT KẾ MÔ-ĐUN

Dự án được phân chia thành các mô-đun độc lập:

```
NguyenHoangPhuc_24IT206_midterm/
├── main.c           # Điều khiển luồng thực thi, phân loại file/thư mục, duyệt đệ quy (-R)
├── options.h        # Khai báo cấu trúc lưu trữ cờ lệnh (Options)
├── options.c        # Phân tích cờ lệnh bằng getopt() và xử lý độ ưu tiên đè cờ
├── entry.h          # Định nghĩa cấu trúc dữ liệu FileEntry và danh sách động EntryList
├── entry.c          # Thu thập thông tin tập tin/thư mục qua stat/lstat
├── sort.h           # Khai báo các hàm sắp xếp
├── sort.c           # Logic sắp xếp (qsort) theo tên, kích thước (-S), thời gian (-t), đảo ngược (-r)
├── display.h        # Khai báo các hàm định dạng đầu ra
├── display.c        # Định dạng và in danh sách (-l, -n, -F, -q, -w, -i, -s)
├── Makefile         # Kịch bản tự động hóa biên dịch
├── .gitignore       # Cấu hình bỏ qua các file thực thi và file object (.o)
└── README.md        # Báo cáo dự án

```

## 3. DANH SÁCH CÁC CỜ LỆNH ĐÃ HIỆN THỰC

Chương trình hỗ trợ đầy đủ các cờ lệnh theo yêu cầu trong bản tả kỹ thuật NetBSD `ls(1)`:

| Cờ (Option) | Chức năng chi tiết | 
| ----- | ----- | 
| **`-a`** | Liệt kê tất cả tập tin, bao gồm tập tin ẩn (`.`) và các thư mục `.`, `..`. | 
| **`-A`** | Liệt kê tất cả tập tin ngoại trừ `.` và `..`. | 
| **`-l`** | Hiển thị dạng danh sách dài (Long format): loại file, phân quyền, số link, owner, group, kích thước, thời gian và tên. | 
| **`-n`** | Tương tự `-l`, nhưng hiển thị UID và GID dưới dạng số. | 
| **`-d`** | Xem thư mục như file thông thường (không đọc đệ quy nội dung bên trong). | 
| **`-R`** | Duyệt và hiển thị đệ quy toàn bộ thư mục con. | 
| **`-r`** | Đảo ngược thứ tự sắp xếp. | 
| **`-t`** | Sắp xếp theo thời gian chỉnh sửa (`mtime`) mới nhất trước. | 
| **`-u`** | Dùng thời gian truy cập (`atime`) thay cho `mtime` khi sắp xếp (`-t`) hoặc hiển thị (`-l`). | 
| **`-c`** | Dùng thời gian thay đổi trạng thái (`ctime`) thay cho `mtime`. | 
| **`-S`** | Sắp xếp theo kích thước file giảm dần. | 
| **`-f`** | Xuất danh sách không qua sắp xếp. | 
| **`-F`** | Thêm ký tự đánh dấu loại file (`/`, `*`, `@`, `=`, `|`). | 
| **`-i`** | In số Serial Inode ở đầu mỗi file. | 
| **`-s`** | In số block 512-byte mà file thực sự sử dụng trên đĩa. | 
| **`-h`** | Hiển thị kích thước file/block theo định dạng dễ đọc (K, M, G). | 
| **`-k`** | Định dạng kích thước block theo đơn vị Kilobytes. | 
| **`-q`** | In ký tự không in được dưới dạng `?` (mặc định khi xuất ra màn hình terminal). | 
| **`-w`** | In mã thô của ký tự không in được (mặc định khi redirect đầu ra). | 

## 4. HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY CHƯƠNG TRÌNH

### Biên dịch mã nguồn:

Sử dụng `Makefile` được cung cấp sẵn để biên dịch:

```
make

```

Lệnh này sẽ biên dịch tất cả các file `.c` và tạo ra file thực thi `ls_custom`.

### Chạy chương trình:

Chạy file thực thi `./ls_custom` cùng các cờ lệnh mong muốn:

```
# Xem thư mục hiện tại
./ls_custom

# Chi tiết tất cả tập tin kể cả file ẩn
./ls_custom -la

# Sắp xếp theo dung lượng giảm dần và hiển thị phân loại file
./ls_custom -lSF

# Duyệt đệ quy tất cả thư mục con
./ls_custom -R /path/to/directory

```

### Dọn dẹp file biên dịch:

Để xóa các file object (`.o`) và file thực thi sau khi hoàn thành:

```
make clean

```
