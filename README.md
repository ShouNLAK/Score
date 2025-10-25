# 🎓 HUIT-Score - Hệ Thống Quản Lý Điểm Sinh Viên

[![GitHub](https://img.shields.io/badge/GitHub-ShouNLAK/HUIT--Score-blue?logo=github)](https://github.com/ShouNLAK/HUIT-Score)
[![Website](https://img.shields.io/badge/Website-shounlak--score.vercel.app-green?logo=vercel)](https://shounlak-score.vercel.app)

**HUIT-Score** là hệ thống quản lý và tra cứu điểm sinh viên toàn diện, bao gồm hai phần chính:
- **Console - Terminal**: Ứng dụng C chạy trên Windows Console để quản lý điểm cá nhân
- **Website**: Nền tảng web tra cứu thông tin học vụ, bảng điểm và thời gian biểu trực tuyến

---

## 📂 Cấu Trúc Dự Án

```
HUIT-Score/
├── Console - Terminal/     # Ứng dụng quản lý điểm console
│   ├── Score.c            # Mã nguồn chương trình C
│   └── Stats.csv          # File lưu trữ dữ liệu điểm
│
└── Website/               # Ứng dụng web tra cứu điểm
    ├── index.html         # Giao diện chính
    ├── tsconfig.json      # Cấu hình TypeScript
    ├── vercel.json        # Cấu hình deploy Vercel
    └── api/               # API endpoints
        ├── captcha.ts     # Xử lý mã bảo vệ
        ├── huit.ts        # Tra cứu điểm HUIT
        ├── init.ts        # Khởi tạo phiên
        ├── lookup.ts      # Tra cứu sinh viên
        └── schedule.ts    # Lấy thời gian biểu
```

---

# 🖥️ Phần 1: Console - Terminal

## 📋 Giới Thiệu

**Score** là chương trình quản lý điểm sinh viên chạy trên Windows Console với giao diện ASCII đẹp mắt, hỗ trợ quản lý điểm cá nhân, tính GPA, và gợi ý điểm cần đạt để hoàn thành mục tiêu học tập.

## ✨ Tính Năng Nổi Bật

### 📊 Quản Lý Điểm
- ✅ Thêm, sửa, xóa môn học
- ✅ Quản lý điểm giữa kỳ, cuối kỳ, tỷ lệ điểm
- ✅ Hỗ trợ tên môn học tiếng Việt
- ✅ Lưu trữ tự động vào file CSV

### 🎯 Tính Toán Thông Minh
- ✅ Tự động tính điểm tổng kết hệ 10
- ✅ Quy đổi GPA hệ 4.0 chuẩn
- ✅ Tính điểm tích lũy toàn khóa
- ✅ Thử nghiệm điểm (What-if Analysis)

### 🎨 Giao Diện Chuyên Nghiệp
- ✅ Bảng điểm ASCII với màu sắc nổi bật
- ✅ Căn lề tự động hỗ trợ tiếng Việt
- ✅ Màu sắc phân biệt trạng thái điểm
- ✅ Menu điều hướng trực quan

### 🚀 Công Cụ Gợi Ý
- ✅ Gợi ý điểm tối thiểu để đạt GPA mục tiêu
- ✅ Phân tích điểm cần thiết cho từng môn
- ✅ Dự đoán kết quả học tập

## 🖼️ Giao Diện Mẫu

```
======= BANG DIEM SINH VIEN =======

+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+
| STT | TEN MON HOC                                                  | TC | GK (%)        | CK (%)        | TK    | GPA   |
+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+
| 1   | Toán cao cấp 1                                               | 4  | 8.5 (30)      | 9.0 (70)      | 8.8   | 4.0   |
| 2   | Vật lý đại cương                                             | 3  | ? (40)        | 7.5 (60)      | ?     | ?     |
| 3   | Lập trình căn bản                                            | 3  | 7.0 (50)      | ? (50)        | ?     | ?     |
+-----+--------------------------------------------------------------+----+---------------+---------------+-------+-------+

> Diem toan khoa hien tai (he 10): 8.80
> GPA tong ket tuong duong (he 4): 3.50
```

**Chú thích màu sắc:**
- 🟢 **Màu xanh**: Điểm đã có
- 🔴 **Màu đỏ**: Điểm chưa có / Điểm giả định
- 🟡 **Màu vàng**: Cảnh báo / Thông báo
- 🔵 **Màu xanh dương**: Tiêu đề / Đường viền

## ⚙️ Hướng Dẫn Sử Dụng

### 1️⃣ Yêu Cầu Hệ Thống
- **Hệ điều hành**: Windows 7/8/10/11
- **Trình biên dịch**: GCC (MinGW), MSVC, hoặc tương đương
- **Bộ nhớ**: 2MB trở lên

### 2️⃣ Biên Dịch Chương Trình

**Sử dụng GCC (MinGW):**
```batch
cd "Console - Terminal"
gcc Score.c -o Score.exe
```

**Sử dụng MSVC (Visual Studio):**
```batch
cl.exe /Zi /EHsc /nologo /Fe:Score.exe Score.c
```

### 3️⃣ Chạy Chương Trình

```batch
Score.exe
```

### 4️⃣ Menu Chức Năng

Khi chạy chương trình, bạn sẽ thấy menu với các tùy chọn:

```
======= QUAN LY DIEM SINH VIEN =======
1. Xem bang diem
2. Them mon hoc moi
3. Chinh sua mon hoc
4. Xem diem tong ket hien tai
5. Thu diem cho mon chua biet (What-if)
6. Goi y diem de dat diem toan khoa muc tieu
0. Thoat chuong trinh
```

#### 📖 Chi Tiết Từng Chức Năng

**1. Xem bảng điểm**
- Hiển thị toàn bộ môn học với điểm số
- Tính tự động điểm tổng kết và GPA
- Phân biệt môn đã có điểm và chưa có điểm

**2. Thêm môn học mới**
- Nhập tên môn học (hỗ trợ tiếng Việt)
- Nhập số tín chỉ
- Nhập tỷ lệ điểm giữa kỳ/cuối kỳ (%)
- Nhập điểm (dùng `-2` nếu chưa có, `-1` nếu không áp dụng)

**3. Chỉnh sửa môn học**
- Chọn môn cần sửa từ danh sách
- Cập nhật thông tin hoặc điểm số
- Tự động tính lại GPA

**4. Xem điểm tổng kết**
- Hiển thị điểm trung bình hệ 10
- Hiển thị GPA hệ 4.0
- Thống kê số môn còn thiếu điểm

**5. Thử điểm (What-if)**
- Chọn môn chưa có điểm
- Nhập điểm giả định
- Xem trước GPA dự kiến

**6. Gợi ý điểm mục tiêu**
- Nhập GPA mong muốn
- Hệ thống tính toán và gợi ý
- Hiển thị điểm tối thiểu cần đạt cho từng môn

## 📁 Quản Lý Dữ Liệu

### File `Stats.csv`
Dữ liệu được lưu tự động vào file `Stats.csv` với cấu trúc:

```csv
Ten Mon Hoc,So Tin Chi,Diem Giua Ky,Ty Le Giua Ky,Diem Cuoi Ky
"Toán cao cấp 1",4,8.5,30,9.0
"Vật lý đại cương",3,UNKNOWN,40,7.5
"Lập trình căn bản",3,7.0,50,UNKNOWN
```

**Lợi ích:**
- ✅ Dễ dàng backup và phục hồi
- ✅ Có thể chỉnh sửa bằng Excel/LibreOffice
- ✅ Import/Export dữ liệu nhanh chóng

## 💡 Lưu Ý Quan Trọng

### Quy Ước Nhập Điểm
- **`-2`**: Chưa có điểm (UNKNOWN)
- **`-1`**: Không áp dụng (NONE)
- **`0-10`**: Điểm thực tế

### Thang Điểm GPA 4.0
| Điểm Hệ 10 | GPA 4.0 |
|------------|---------|
| 8.5 - 10   | 4.0     |
| 8.0 - 8.4  | 3.5     |
| 7.0 - 7.9  | 3.0     |
| 6.5 - 6.9  | 2.5     |
| 5.5 - 6.4  | 2.0     |
| 5.0 - 5.4  | 1.5     |
| 4.0 - 4.9  | 1.0     |
| < 4.0      | 0.0     |

### Mẹo Sử Dụng
- 🔹 Backup file `Stats.csv` thường xuyên
- 🔹 Sử dụng chức năng "Thử điểm" để lập kế hoạch học tập
- 🔹 Cập nhật điểm ngay khi biết kết quả
- 🔹 Dùng chức năng "Gợi ý" để đặt mục tiêu rõ ràng

---

# 🌐 Phần 2: Website

## 🚀 Truy Cập Website

**🔗 Website chính thức**: [shounlak-score.vercel.app](https://shounlak-score.vercel.app)

## 📋 Giới Thiệu

Website HUIT-Score là nền tảng tra cứu thông tin học vụ trực tuyến, cho phép sinh viên xem bảng điểm, thời gian biểu và thông tin cá nhân một cách nhanh chóng và tiện lợi.

## ✨ Tính Năng Chính

### 🎓 Tra Cứu Thông Tin Sinh Viên
- ✅ Tra cứu bằng MSSV và mã bảo vệ
- ✅ Hiển thị thông tin cá nhân đầy đủ
- ✅ Thông tin chuyên ngành và đào tạo
- ✅ Dữ liệu được bảo mật và an toàn

### 📊 Bảng Điểm Chi Tiết
- ✅ Xem điểm theo từng học kỳ
- ✅ Thống kê GPA tích lũy
- ✅ Tính năng thử nghiệm điểm trực tiếp
- ✅ Xuất dữ liệu ra file CSV
- ✅ Phân biệt môn học loại trừ (GDQP, GDTC)

### 📅 Thời Gian Biểu
- ✅ Lịch học theo tuần
- ✅ Lịch học trực tuyến
- ✅ Lịch thi
- ✅ Lịch tạm ngưng học
- ✅ Điều hướng nhanh giữa các tuần

### 🎨 Giao Diện Hiện Đại
- ✅ Thiết kế responsive, tương thích mọi thiết bị
- ✅ Giao diện gradient đẹp mắt
- ✅ Animation mượt mà
- ✅ Dark mode tự động
- ✅ UX/UI được tối ưu hóa

## 🖼️ Tính Năng Nổi Bật

### 1. Tra Cứu Thông Tin
<details>
<summary>Xem chi tiết</summary>

- Nhập MSSV (6-12 chữ số)
- Nhập mã bảo vệ (captcha)
- Hệ thống tự động xác thực và hiển thị thông tin
- Dữ liệu được cache để tăng tốc độ truy cập
</details>

### 2. Bảng Điểm Tương Tác
<details>
<summary>Xem chi tiết</summary>

- Xem điểm theo từng học kỳ
- Thử nghiệm điểm trực tiếp trên bảng
- Tính toán GPA tự động
- Highlight môn học loại trừ
- Xuất CSV với một click
</details>

### 3. Thời Gian Biểu Thông Minh
<details>
<summary>Xem chi tiết</summary>

- Lọc theo loại lịch (học, thi, trực tuyến, nghỉ)
- Điều hướng theo tuần
- Hiển thị chi tiết phòng học, giảng viên
- Card view và table view
- Đánh dấu buổi học hôm nay
</details>

## 🛠️ Công Nghệ Sử Dụng

### Frontend
- **HTML5**: Cấu trúc trang web
- **CSS3**: Styling và animation
- **JavaScript (ES6+)**: Logic xử lý
- **Responsive Design**: Tương thích mọi màn hình

### Backend (API)
- **TypeScript**: Type-safe code
- **Vercel Serverless**: Deploy nhanh chóng
- **Node.js**: Runtime environment
- **Fetch API**: Gọi API HUIT

### Bảo Mật
- ✅ HTTPS end-to-end
- ✅ Captcha verification
- ✅ Session management
- ✅ Data sanitization
- ✅ CORS protection

## 📱 Hướng Dẫn Sử Dụng Website

### Bước 1: Truy Cập Website
Mở trình duyệt và truy cập: **[shounlak-score.vercel.app](https://shounlak-score.vercel.app)**

### Bước 2: Tra Cứu Thông Tin
1. Nhập **Mã số sinh viên** (MSSV)
2. Nhập **Mã bảo vệ** hiển thị trên ảnh
3. Click **🔍 Tra cứu MSSV**

### Bước 3: Xem Thông Tin
Sau khi tra cứu thành công, bạn có thể:
- Tab **👤 Thông tin sinh viên**: Xem thông tin cá nhân
- Tab **📅 Thời gian biểu**: Xem lịch học, lịch thi
- Tab **📊 Bảng điểm**: Xem điểm và thử nghiệm điểm

### Bước 4: Sử Dụng Tính Năng
- **Thử điểm**: Nhập điểm giả định vào ô input trên bảng
- **Xuất CSV**: Click nút "📥 Xuất CSV" để tải file
- **Xem lịch**: Chuyển đổi giữa các tuần bằng nút điều hướng

## 🔧 Cài Đặt & Deploy

### Chạy Local

1. **Clone repository**
```bash
git clone https://github.com/ShouNLAK/HUIT-Score.git
cd HUIT-Score/Website
```

2. **Cài đặt dependencies** (nếu có)
```bash
npm install
```

3. **Chạy local server**
```bash
# Sử dụng Python
python -m http.server 8000

# Hoặc sử dụng Node.js
npx serve
```

4. **Truy cập**
Mở trình duyệt: `http://localhost:8000`

### Deploy lên Vercel

1. **Install Vercel CLI**
```bash
npm i -g vercel
```

2. **Deploy**
```bash
cd Website
vercel --prod
```

3. **Cấu hình**
File `vercel.json` đã được cấu hình sẵn:
```json
{
  "functions": {
    "api/**/*.ts": {
      "memory": 1024,
      "maxDuration": 10
    }
  }
}
```

## 🎯 API Endpoints

Website sử dụng các API endpoints sau:

| Endpoint | Method | Mô tả |
|----------|--------|-------|
| `/api/init` | GET | Khởi tạo session và lấy cookies |
| `/api/captcha` | POST | Lấy ảnh mã bảo vệ |
| `/api/lookup` | POST | Tra cứu thông tin sinh viên |
| `/api/huit` | POST | Lấy dữ liệu bảng điểm |
| `/api/schedule` | POST | Lấy thời gian biểu |

---

## 🤝 Đóng Góp

Mọi đóng góp đều được chào đón! Hãy:
1. **Fork** repository
2. Tạo **branch** mới (`git checkout -b feature/AmazingFeature`)
3. **Commit** thay đổi (`git commit -m 'Add some AmazingFeature'`)
4. **Push** lên branch (`git push origin feature/AmazingFeature`)
5. Mở **Pull Request**

### 📝 Báo Lỗi

Nếu bạn gặp lỗi, vui lòng:
- Mở [Issue](https://github.com/ShouNLAK/HUIT-Score/issues) mới
- Mô tả chi tiết lỗi
- Đính kèm screenshot nếu có
- Ghi rõ môi trường (OS, Browser, ...)

---

## 📄 Giấy Phép

Dự án này được phân phối dưới giấy phép **MIT License**. Xem file `LICENSE` để biết thêm chi tiết.

---

## ⚠️ Disclaimer

> **Lưu ý quan trọng**: 
> - Dữ liệu điểm số chỉ mang tính chất **tham khảo**
> - Không đảm bảo **tính chính xác 100%** do cách tính điểm khác nhau giữa các trường
> - Vui lòng kiểm tra lại với phòng đào tạo để có thông tin chính thức
> - Website **không lưu trữ** thông tin cá nhân hay mật khẩu của người dùng

---

## 👨‍💻 Tác Giả

**ShouNLAK**
- GitHub: [@ShouNLAK](https://github.com/ShouNLAK)
- Website: [shounlak-score.vercel.app](https://shounlak-score.vercel.app)

---

## 🙏 Cảm Ơn

- Cảm ơn tất cả những người đã đóng góp cho dự án
- Cảm ơn cộng đồng sinh viên HUIT đã sử dụng và phản hồi
- Cảm ơn Vercel đã cung cấp hosting miễn phí

---

## 📞 Liên Hệ & Hỗ Trợ

Nếu có bất kỳ câu hỏi nào, vui lòng:
- Mở [Issue](https://github.com/ShouNLAK/HUIT-Score/issues) trên GitHub
- Liên hệ qua GitHub Profile

---

<div align="center">

**⭐ Nếu thấy hữu ích, hãy cho dự án một ngôi sao!**

Made with ❤️ by [ShouNLAK](https://github.com/ShouNLAK)

</div>
