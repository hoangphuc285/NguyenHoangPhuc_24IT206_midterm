# BÀI TẬP GIỮA KỲ: HIỆN THỰC LỆNH `ls` TRÊN UNIX

* **Môn học:** Lập trình Hệ thống 
* **Họ và tên sinh viên:** Nguyễn Hoàng Phúc
* **Mã số sinh viên (MSSV):** [Điền MSSV Của Bạn]
* **GitHub:** `https://github.com/hoangphuc285/NguyenHoangPhuc_24IT206_midterm`
---

## 1. TỔNG QUAN DỰ ÁN

Dự án này thực hiện việc viết lại công cụ dòng lệnh `ls(1)` trong hệ điều hành UNIX/Linux từ đầu bằng ngôn ngữ C, dựa trên tài liệu mô tả kỹ thuật (Manual Page) của NetBSD.

Chương trình cung cấp giao diện dòng lệnh cho phép người dùng liệt kê nội dung của thư mục, hiển thị thông tin chi tiết về tập tin/thư mục (phân quyền, chủ sở hữu, kích thước, thời gian chỉnh sửa, inode,...), hỗ trợ nhiều chế độ sắp xếp và lọc dữ liệu.

### Mục tiêu đạt được:
- Hiểu và thao tác thành thạo với **Hệ thống tập tin UNIX (UNIX Filesystem)** thông qua các System Call chuẩn POSIX (`stat`, `lstat`, `opendir`, `readdir`,...).
- Áp dụng phương pháp **Lập trình C dạng mô-đun (Modular C)**, tách biệt cấu trúc dữ liệu và xử lý logic vào các file `.h` và `.c` riêng biệt.
- Quản lý bộ nhớ động an toàn, chống rò rỉ bộ nhớ (memory leaks) và ngăn ngừa lỗi truy cập bộ nhớ nghiêm trọng (`Segmentation Fault`).
- Tự động hóa quá trình biên dịch dự án bằng `Makefile`.
- Quản lý phiên bản mã nguồn chuyên nghiệp bằng `Git` và `GitHub`.

---

## 2. CẤU TRÚC DỰ ÁN VÀ THIẾT KẾ MÔ-ĐUN

Mã nguồn dự án được tổ chức chặt chẽ thành các mô-đun độc lập theo chức năng:


Name_studentID_midterm/
├── main.c           # Luồng điều khiển chính, xử lý đối số đường dẫn và đệ quy
├── options.h        # Định nghĩa cấu trúc lưu trữ cờ lệnh (Options)
├── options.c        # Phân tích tham số dòng lệnh (getopt) và xử lý độ ưu tiên cờ
├── entry.h          # Khai báo cấu trúc dữ liệu FileEntry và EntryList
├── entry.c          # Thu thập thông tin file/thư mục (stat, lstat, readdir)
├── sort.h           # Khai báo các hàm sắp xếp
├── sort.c           # Hiện thực logic sắp xếp (qsort) theo tên, thời gian, kích thước
├── display.h        # Khai báo các hàm định dạng và hiển thị
├── display.c        # In dữ liệu (chế độ -l, -n, ký hiệu -F, chuyển đổi ký tự -q/-w)
├── Makefile         # Kịch bản tự động hóa biên dịch dự án
├── .gitignore       # Cấu hình loại bỏ file rác, file object (.o) và file thực thi
└── README.md        # Báo cáo chi tiết dự án (File này)


Cờ (Option),Mô tả chi tiết chức năng
-a,"Liệt kê tất cả các tập tin, bao gồm cả tập tin ẩn bắt đầu bằng dấu chấm (.) và các thư mục đặc biệt (., ..)."
-A,Liệt kê tất cả các tập tin ngoại trừ hai thư mục đặc biệt . và ...
-l,"Hiển thị theo định dạng danh sách dài (Long format): Phân quyền, số link, owner, group, kích thước (bytes), thời gian chỉnh sửa và tên file."
-n,"Giống như -l, nhưng hiển thị UID và GID dưới dạng số thay vì tên người dùng/nhóm."
-d,Hiển thị thư mục như một file thông thường chứ không đọc nội dung bên trong thư mục đó.
-R,Duyệt và hiển thị đệ quy tất cả các thư mục con encountered.
-r,"Đảo ngược thứ tự sắp xếp (ví dụ: từ Z-A thay vì A-Z, nhỏ nhất/cũ nhất trước)."
-t,Sắp xếp danh sách theo thời gian chỉnh sửa gần nhất (most recently modified) trước.
-u,Sử dụng thời gian truy cập gần nhất (atime) thay cho thời gian chỉnh sửa (mtime) khi sắp xếp (-t) hoặc in (-l).
-c,Sử dụng thời gian thay đổi trạng thái file (ctime) khi sắp xếp (-t) hoặc in (-l).
-S,Sắp xếp danh sách theo kích thước tập tin (file lớn nhất xếp trước).
-f,Xuất danh sách không qua sắp xếp (giữ nguyên thứ tự đọc từ hệ thống tập tin).
-F,"Thêm ký tự đánh dấu loại file vào sau tên: / (thư mục), * (file thi hành), @ (symlink), = (socket), | (FIFO)."
-i,In số Serial của file (Inode number) ở đầu mỗi dòng.
-s,Hiển thị số block hệ thống (mỗi block 512 bytes) mà file thực sự sử dụng. In tổng số block ở đầu danh sách.
-h,"Thay đổi định dạng hiển thị dung lượng file/block sang dạng dễ đọc cho con người (K, M, G)."
-k,Bắt buộc hiển thị dung lượng theo đơn vị Kilobytes (KB).
-q,Bắt buộc in các ký tự không in được (non-printable) dưới dạng dấu ? (Mặc định khi xuất ra terminal).
-w,In trực tiếp mã raw của các ký tự không in được (Mặc định khi redirect đầu ra không phải terminal).

## Compilation and Running
To build the project executable:
```bash
make

