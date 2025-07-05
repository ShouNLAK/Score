# 🎓 Score - Quản Lý Điểm Sinh Viên

**Score** là chương trình quản lý điểm sinh viên chạy trên Windows Console, hỗ trợ nhập/xem/sửa điểm, tính GPA, thử điểm giả định (what-if), và gợi ý điểm cần đạt để đạt mục tiêu. Giao diện bảng điểm được thiết kế đẹp mắt, chuyên nghiệp với màu sắc nổi bật, dễ nhìn.

---

## 🚀 Tính Năng Nổi Bật

- **Quản lý môn học**: Thêm, sửa thông tin môn học, điểm số, số tín chỉ, tỷ lệ giữa kỳ/cuối kỳ.
- **Bảng điểm chuyên nghiệp**: Hiển thị bảng điểm dạng bảng ASCII, căn lề đẹp, hỗ trợ tên môn học tiếng Việt, tô màu các cột điểm.
- **Tính toán tự động**: Tự động tính điểm tổng kết hệ 10, quy đổi GPA hệ 4.
- **Thử điểm (What-if)**: Nhập điểm giả định cho các môn chưa biết để xem trước GPA dự kiến.
- **Gợi ý điểm mục tiêu**: Gợi ý điểm tối thiểu cần đạt ở các môn còn lại để đạt GPA mong muốn.
- **Lưu trữ CSV**: Dữ liệu được lưu tự động vào file `Stats.csv`, dễ dàng backup hoặc chỉnh sửa ngoài.

---

## 🖥️ Giao Diện Mẫu

```
======= BANG DIEM SINH VIEN =======

+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+
| STT | TEN MON HOC                                                  | TC | GK (%)        | CK (%)        | TK    | GPA   |
+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+
| 1   | Toán cao cấp 1                                               | 4  | 8.5 (30)      | 9.0 (70)      | 8.8   | 4.0   |
| 2   | Vật lý đại cương                                             | 3  | ? (40)        | 7.5 (60)      | ?     | ?     |
| 3   | Tin học cơ sở                                                | 2  | 7.0 (50)      | ? (50)        | ?     | ?     |
+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+
```
- **Màu sắc**: Điểm đã có màu xanh, điểm chưa có màu vàng/đỏ, điểm giả định màu đỏ.
- **Căn lề**: Tên môn học căn đều, hỗ trợ tiếng Việt.
- **Bảng rõ ràng**: Đường viền bảng xanh dương, tiêu đề nổi bật.

---

## ⚙️ Hướng Dẫn Sử Dụng

### 1. Biên Dịch

Yêu cầu: Windows, trình biên dịch C (MSVC, MinGW, ...)

```sh
gcc Score.c -o Score.exe
```

### 2. Chạy Chương Trình

```sh
Score.exe
```

### 3. Menu Chức Năng

- **1. Xem bảng điểm**: Hiển thị toàn bộ môn học, điểm số, GPA.
- **2. Thêm môn học mới**: Nhập tên, tín chỉ, tỷ lệ, điểm số.
- **3. Chỉnh sửa môn học**: Sửa thông tin hoặc điểm từng môn.
- **4. Xem điểm tổng kết hiện tại**: Tính GPA hệ 10 và hệ 4.
- **5. Thử điểm cho môn chưa biết**: Nhập điểm giả định, xem GPA dự kiến.
- **6. Gợi ý điểm để đạt mục tiêu**: Nhập GPA mục tiêu, chương trình gợi ý điểm tối thiểu cần đạt.
- **0. Thoát chương trình**

---

## 📁 Lưu Trữ Dữ Liệu

- Dữ liệu lưu tại file `Stats.csv` cùng thư mục.
- Có thể mở file này bằng Excel hoặc Notepad để chỉnh sửa nhanh.

---

## 💡 Lưu Ý

- Điểm giữa kỳ/cuối kỳ nhập `-2` nếu chưa có, `-1` nếu không áp dụng.
- Chương trình tự động lưu sau mỗi thay đổi.
- Giao diện tối ưu cho Windows Console, hỗ trợ tiếng Việt.

---

## 🏆 Đóng Góp

Mọi ý kiến đóng góp, báo lỗi hoặc cải tiến UI, vui lòng tạo issue hoặc pull request!

---
>  *Sản phẩm của ShouNLAK - Giá trị chỉ nên dùng để tham khảo chứ không thể khẳng định tính chính xác 100% do cách tính riêng mỗi trường*
