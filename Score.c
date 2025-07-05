#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

// =================== CÁC ĐỊNH NGHĨA VÀ CẤU TRÚC ===================

#define MAX_SUBJECTS 100
#define MAX_NAME 100
#define CSV_FILE "Stats.csv"

// Mã màu cho Console
#define COLOR_DEFAULT 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_YELLOW 14

// Loại điểm cần highlight trong bảng phụ
#define HIGHLIGHT_NONE 0
#define HIGHLIGHT_MIDTERM 1
#define HIGHLIGHT_FINAL 2
#define HIGHLIGHT_BOTH 3

typedef struct {
    char name[MAX_NAME];
    int credits;
    float midterm;      // Điểm giữa kỳ. -2: Chưa có (UNKNOWN), -1: Không áp dụng (NONE)
    int midterm_ratio;
    float final;        // Điểm cuối kỳ.
    float total;        // Điểm tổng kết hệ 10
    float gpa;          // Điểm GPA hệ 4
} Subject;

Subject subjects[MAX_SUBJECTS];
int subject_count = 0;
HANDLE hConsole;

// Khai báo trước tất cả các hàm
void set_color(int color);
void clear_screen();
void print_header(const char* title);
void press_enter_to_continue();
void get_string_input(const char* prompt, char* buffer, int size);
int get_integer_input(const char* prompt);
float get_float_input(const char* prompt);
float score_to_gpa(float score);
void calculate_subject_scores(Subject* s);
float calc_standard_with_cases(float* case_scores, int* case_idxs, int n_cases);
float calc_gpa_with_cases(float* case_scores, int* case_idxs, int n_cases);
void save_data();
void load_data();
void show_partial_grade_table(int* subject_indices, int count, float* highlight_scores, int* highlight_types);
void show_grade_table();
void add_new_subject();
void edit_subject();
void show_current_gpa();
void try_score();
void suggest_min_scores(float standard_target, int *unknown_subject_idxs, int n_unknown_subjects);
void suggest_scores_for_target_gpa();

// =================== CÁC HÀM TIỆN ÍCH GIAO DIỆN (UI UTILITIES) ===================

void set_color(int color) { SetConsoleTextAttribute(hConsole, color); }
void clear_screen() { system("cls"); }

void print_header(const char* title) {
    set_color(COLOR_CYAN);
    char line[100];
    sprintf(line, "======= %s =======", title);
    printf("\n\n%s\n", line);
    set_color(COLOR_DEFAULT);
}

void press_enter_to_continue() {
    set_color(COLOR_YELLOW); printf("\n\nNhan Enter de tiep tuc..."); set_color(COLOR_DEFAULT);
    getchar();
}

// =================== CÁC HÀM NHẬP LIỆU AN TOÀN ===================

// HÀM MỚI: Đếm số ký tự trực quan trong chuỗi UTF-8
int count_utf8_chars(const char *s) {
    int count = 0;
    while (*s) {
        // Một byte UTF-8 hợp lệ không bắt đầu bằng '10' (binary) là một ký tự mới.
        // (0xC0 = 11000000, 0x80 = 10000000)
        if ((*s & 0xC0) != 0x80) {
            count++;
        }
        s++;
    }
    return count;
}

void get_string_input(const char* prompt, char* buffer, int size) {
    fflush(stdin); printf("%s", prompt);
    fgets(buffer, size, stdin); buffer[strcspn(buffer, "\n")] = 0;
}

int get_integer_input(const char* prompt) {
    char buffer[100]; int value;
    while (1) {
        printf("%s", prompt); fflush(stdin);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) return value;
        }
        set_color(COLOR_RED); printf("Loi: Vui long chi nhap so nguyen. Hay thu lai.\n"); set_color(COLOR_DEFAULT);
    }
}

float get_float_input(const char* prompt) {
    char buffer[100]; float value;
    while (1) {
        printf("%s", prompt); fflush(stdin);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &value) == 1) return value;
        }
        set_color(COLOR_RED); printf("Loi: Vui long nhap so thuc (vi du: 8.5). Hay thu lai.\n"); set_color(COLOR_DEFAULT);
    }
}

// =================== CÁC HÀM XỬ LÝ LOGIC CỐT LÕI ===================

float score_to_gpa(float score) {
    float rounded_score = roundf(score * 10) / 10.0f;
    if (rounded_score >= 8.5) return 4.0; if (rounded_score >= 8.0) return 3.5;
    if (rounded_score >= 7.0) return 3.0; if (rounded_score >= 6.5) return 2.5;
    if (rounded_score >= 5.5) return 2.0; if (rounded_score >= 5.0) return 1.5;
    if (rounded_score >= 4.0) return 1.0; return 0.0;
}

void calculate_subject_scores(Subject* s) {
    if (s->midterm == -2 || s->final == -2) {
        s->total = -2; s->gpa = -2; return;
    }
    float mt = (s->midterm < 0) ? 0 : s->midterm;
    float fn = (s->final < 0) ? 0 : s->final;
    int final_ratio = 100 - s->midterm_ratio;
    s->total = (float)(((double)mt * s->midterm_ratio / 100.0) + ((double)fn * final_ratio / 100.0));
    s->gpa = score_to_gpa(s->total);
}

float calc_standard_with_cases(float* case_scores, int* case_idxs, int n_cases) {
    double total_weighted_score = 0; double total_credits = 0;
    for (int i = 0; i < subject_count; ++i) {
        float current_score = subjects[i].total;
        for (int j = 0; j < n_cases; ++j) {
            if (i == case_idxs[j]) { current_score = case_scores[j]; break; }
        }
        if (current_score >= 0) {
            total_weighted_score += (double)current_score * subjects[i].credits;
            total_credits += subjects[i].credits;
        }
    }
    return (total_credits == 0) ? 0 : (float)(total_weighted_score / total_credits);
}

float calc_gpa_with_cases(float* case_scores, int* case_idxs, int n_cases) {
    double total_weighted_score = 0; double total_credits = 0;
    for (int i = 0; i < subject_count; ++i) {
        float current_score = subjects[i].gpa;
        for (int j = 0; j < n_cases; ++j) {
            if (i == case_idxs[j]) { current_score = case_scores[j]; break; }
        }
        if (current_score >= 0) {
            total_weighted_score += (double)current_score * subjects[i].credits;
            total_credits += subjects[i].credits;
        }
    }
    return (total_credits == 0) ? 0 : (float)(total_weighted_score / total_credits);
}

// =================== QUẢN LÝ FILE CSV ===================

void save_data() {
    FILE* f = fopen(CSV_FILE, "w");
    if (!f) { perror("Loi: Khong the mo file de ghi"); return; }
    fprintf(f, "Ten Mon Hoc,So Tin Chi,Diem Giua Ky,Ty Le Giua Ky,Diem Cuoi Ky\n");
    for (int i = 0; i < subject_count; ++i) {
        Subject* s = &subjects[i];
        fprintf(f, "\"%s\",%d,", s->name, s->credits);
        if (s->midterm == -2) fprintf(f, "UNKNOWN,"); else fprintf(f, "%.1f,", s->midterm);
        fprintf(f, "%d,", s->midterm_ratio);
        if (s->final == -2) fprintf(f, "UNKNOWN\n"); else fprintf(f, "%.1f\n", s->final);
    }
    fclose(f);
}

void load_data() {
    FILE* f = fopen(CSV_FILE, "r");
    if (!f) return;
    char line[512];
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f) && subject_count < MAX_SUBJECTS) {
        line[strcspn(line, "\r\n")] = 0;
        Subject* s = &subjects[subject_count];
        char* token;
        token = strtok(line, ",");
        if (token) {
            if (token[0] == '"') token++;
            if (token[strlen(token) - 1] == '"') token[strlen(token) - 1] = '\0';
            strncpy(s->name, token, MAX_NAME);
        }
        token = strtok(NULL, ","); s->credits = atoi(token);
        token = strtok(NULL, ",");
        if (token && strcmp(token, "UNKNOWN") == 0) s->midterm = -2; else s->midterm = atof(token);
        token = strtok(NULL, ","); s->midterm_ratio = atoi(token);
        token = strtok(NULL, ",");
        if (token && strcmp(token, "UNKNOWN") == 0) s->final = -2; else s->final = atof(token);
        calculate_subject_scores(s);
        subject_count++;
    }
    fclose(f);
}

// =================== CÁC CHỨC NĂNG CHÍNH CỦA MENU ===================

// ========= BỘ ĐỊNH NGHĨA CẤU TRÚC BẢNG (Chỉnh sửa ở đây) =========
const int W_STT = 3;
const int W_NAME = 60;
const int W_TC = 2;
const int W_GK = 13;
const int W_CK = 13;
const int W_TK = 5;
const int W_GPA = 5;
// =================================================================

 // Hàm nội tuyến để vẽ một đường ngang
void print_horizontal_line() {
    printf("+"); for (int i = 0; i < W_STT + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_NAME + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_TC + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_GK + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_CK + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_TK + 2; i++) printf("-");
    printf("+"); for (int i = 0; i < W_GPA + 2; i++) printf("-");
    printf("+\n");
}

// Hàm trợ giúp mới để vẽ đường viền bảng ASCII một cách linh hoạt
void print_table_border(const int name_width) {
    set_color(COLOR_BLUE);
    printf("+"); for (int i = 0; i < 5; i++) printf("-"); // Cột STT
    printf("+"); for (int i = 0; i < name_width + 2; i++) printf("-"); // Cột Tên Môn Học
    printf("+"); for (int i = 0; i < 4; i++) printf("-"); // Cột TC
    printf("+"); for (int i = 0; i < 16; i++) printf("-"); // Cột GK
    printf("+"); for (int i = 0; i < 16; i++) printf("-"); // Cột CK
    printf("+"); for (int i = 0; i < 7; i++) printf("-"); // Cột TK
    printf("+"); for (int i = 0; i < 7; i++) printf("-"); // Cột GPA
    printf("+\n");
    set_color(COLOR_DEFAULT);
}

void show_partial_grade_table(int* subject_indices, int count, float* highlight_scores, int* highlight_types) {
    if (count == 0) {
        set_color(COLOR_YELLOW);
        printf("Khong co mon hoc nao de hien thi.\n");
        return;
    }

    set_color(COLOR_BLUE);
    print_horizontal_line();
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           W_STT, "STT", W_NAME, "TEN MON HOC", W_TC, "TC", W_GK, "GK (%)", W_CK, "CK (%)", W_TK, "TK", W_GPA, "GPA");
    print_horizontal_line();
    set_color(COLOR_DEFAULT);

    for (int i = 0; i < count; i++) {
        int subj_idx = subject_indices[i];
        Subject* s = &subjects[subj_idx];

        char gk_buffer[20], ck_buffer[20], tk_buffer[10], gpa_buffer[10];
        int gk_color = COLOR_DEFAULT, ck_color = COLOR_DEFAULT, tk_color = COLOR_DEFAULT, gpa_color = COLOR_DEFAULT;

        // Chuẩn bị chuỗi và màu cho các cột
        if (highlight_types && (highlight_types[i] & HIGHLIGHT_MIDTERM)) {
            gk_color = COLOR_RED; sprintf(gk_buffer, "%.2f (%d)", highlight_scores[i], s->midterm_ratio);
        } else {
            if(s->midterm == -2) { gk_color = COLOR_RED; sprintf(gk_buffer, "? (%d)", s->midterm_ratio); }
            else if(s->midterm == -1) { sprintf(gk_buffer, "-"); }
            else { gk_color = COLOR_GREEN; sprintf(gk_buffer, "%.1f (%d)", s->midterm, s->midterm_ratio); }
        }

        if (highlight_types && (highlight_types[i] & HIGHLIGHT_FINAL)) {
            ck_color = COLOR_RED; sprintf(ck_buffer, "%.2f (%d)", highlight_scores[i], 100 - s->midterm_ratio);
        } else {
            if(s->final == -2) { ck_color = COLOR_RED; sprintf(ck_buffer, "? (%d)", 100 - s->midterm_ratio); }
            else if(s->final == -1) { sprintf(ck_buffer, "-"); }
            else { ck_color = COLOR_GREEN; sprintf(ck_buffer, "%.1f (%d)", s->final, 100 - s->midterm_ratio); }
        }

        if(s->total == -2) { tk_color=COLOR_YELLOW; gpa_color=COLOR_YELLOW; sprintf(tk_buffer, "?"); sprintf(gpa_buffer, "?"); }
        else { tk_color=COLOR_GREEN; gpa_color=COLOR_GREEN; sprintf(tk_buffer, "%.1f", s->total); sprintf(gpa_buffer, "%.1f", s->gpa); }

        // **LOGIC MỚI: TÍNH TOÁN TK VÀ GPA GIẢ ĐỊNH KHI Ở CHẾ ĐỘ GỢI Ý**
        if (highlight_types != NULL) {
            float temp_gk, temp_ck;

            // Lấy điểm GK để tính toán: ưu tiên điểm đã có, nếu không có thì lấy điểm gợi ý
            if (s->midterm >= 0) temp_gk = s->midterm;
            else temp_gk = (highlight_types[i] & HIGHLIGHT_MIDTERM) ? highlight_scores[i] : 0;

            // Lấy điểm CK để tính toán: ưu tiên điểm đã có, nếu không có thì lấy điểm gợi ý
            if (s->final >= 0) temp_ck = s->final;
            else temp_ck = (highlight_types[i] & HIGHLIGHT_FINAL) ? highlight_scores[i] : 0;
            
            // Trường hợp cả 2 đều thiếu, gán điểm gợi ý cho cả 2
            if (highlight_types[i] == HIGHLIGHT_BOTH) {
                temp_gk = highlight_scores[i];
                temp_ck = highlight_scores[i];
            }
            
            float hypothetical_total = (float)(((double)temp_gk * s->midterm_ratio / 100.0) + ((double)temp_ck * (100 - s->midterm_ratio) / 100.0));
            float hypothetical_gpa = score_to_gpa(hypothetical_total);

            tk_color = COLOR_RED; // Tô màu đỏ cho kết quả giả định
            gpa_color = COLOR_RED;
            sprintf(tk_buffer, "%.1f", hypothetical_total);
            sprintf(gpa_buffer, "%.1f", hypothetical_gpa);

        } else { // Logic hiển thị bình thường cho các case khác
            if(s->total == -2) { tk_color=COLOR_YELLOW; gpa_color=COLOR_YELLOW; sprintf(tk_buffer, "?"); sprintf(gpa_buffer, "?"); }
            else { tk_color=COLOR_GREEN; gpa_color=COLOR_GREEN; sprintf(tk_buffer, "%.1f", s->total); sprintf(gpa_buffer, "%.1f", s->gpa); }
        }

        // In dữ liệu
        printf("| %-*d | %s", W_STT, subj_idx + 1, s->name);
        
        int padding = W_NAME - count_utf8_chars(s->name);
        for(int p = 0; p < padding; p++) { printf(" "); }
        printf(" | %-*d | ", W_TC, s->credits);

        set_color(gk_color); printf("%-*s", W_GK, gk_buffer); set_color(COLOR_DEFAULT);
        printf(" | ");
        set_color(ck_color); printf("%-*s", W_CK, ck_buffer); set_color(COLOR_DEFAULT);
        printf(" | ");
        set_color(tk_color); printf("%-*s", W_TK, tk_buffer); set_color(COLOR_DEFAULT);
        printf(" | ");
        set_color(gpa_color); printf("%-*s", W_GPA, gpa_buffer); set_color(COLOR_DEFAULT);
        printf(" |\n");
    }
    
    set_color(COLOR_BLUE);
    print_horizontal_line();
    set_color(COLOR_DEFAULT);
}

void show_grade_table() {
    clear_screen();
    print_header("BANG DIEM SINH VIEN");
    int* all_indices = malloc(subject_count * sizeof(int));
    if (all_indices == NULL) { printf("Loi cap phat bo nho!"); return; }
    for (int i = 0; i < subject_count; i++) all_indices[i] = i;
    show_partial_grade_table(all_indices, subject_count, NULL, NULL);
    free(all_indices);
}

void add_new_subject() {
    clear_screen();
    print_header("THEM MON HOC MOI");
    if (subject_count >= MAX_SUBJECTS) {
        set_color(COLOR_RED); printf("Da dat so luong mon hoc toi da!\n"); return;
    }
    Subject* s = &subjects[subject_count];
    get_string_input("> Nhap ten mon hoc: ", s->name, MAX_NAME);
    s->credits = get_integer_input("> Nhap so tin chi: ");
    do {
        s->midterm_ratio = get_integer_input("> Nhap ty le giua ky (%): ");
        if(s->midterm_ratio < 0 || s->midterm_ratio > 100) {
            set_color(COLOR_RED); printf("Loi: Ty le phai tu 0 den 100.\n"); set_color(COLOR_DEFAULT);
        }
    } while(s->midterm_ratio < 0 || s->midterm_ratio > 100);
    printf("  -> Ty le cuoi ky: %d%%\n", 100 - s->midterm_ratio);
    s->midterm = get_float_input("> Nhap diem giua ky (-2 neu chua co, -1 neu khong co): ");
    s->final = get_float_input("> Nhap diem cuoi ky (-2 neu chua co, -1 neu khong co): ");
    calculate_subject_scores(s);
    subject_count++;
    save_data();
    set_color(COLOR_GREEN); printf("\nDa them mon hoc '%s' thanh cong!\n", s->name);
}

void edit_subject() {
    clear_screen();
    print_header("CHINH SUA MON HOC");
    show_grade_table();
    if (subject_count == 0) return;
    int choice = get_integer_input("\n> Chon STT mon hoc can sua (nhap 0 de quay lai): ");
    if (choice < 1 || choice > subject_count) return;
    Subject* s = &subjects[choice - 1];
    clear_screen();
    print_header("CHINH SUA MON HOC");
    printf("--- Dang sua mon: "); set_color(COLOR_YELLOW); printf("%s\n\n", s->name); set_color(COLOR_DEFAULT);
    printf("1. Ten mon hoc (Hien tai: %s)\n", s->name);
    printf("2. So tin chi (Hien tai: %d)\n", s->credits);
    printf("3. Ty le giua ky (Hien tai: %d%%)\n", s->midterm_ratio);
    printf("4. Diem giua ky (Hien tai: %.1f)\n", s->midterm);
    printf("5. Diem cuoi ky (Hien tai: %.1f)\n", s->final);
    printf("0. Quay lai\n");
    int edit_choice = get_integer_input("\n> Chon thong tin can sua: ");
    switch (edit_choice) {
        case 1: get_string_input("> Nhap ten moi: ", s->name, MAX_NAME); break;
        case 2: s->credits = get_integer_input("> Nhap so tin chi moi: "); break;
        case 3: s->midterm_ratio = get_integer_input("> Nhap ty le giua ky moi (%): "); break;
        case 4: s->midterm = get_float_input("> Nhap diem giua ky moi (-2 neu chua co): "); break;
        case 5: s->final = get_float_input("> Nhap diem cuoi ky moi (-2 neu chua co): "); break;
        case 0: return;
        default: set_color(COLOR_RED); printf("Lua chon khong hop le.\n"); set_color(COLOR_DEFAULT); return;
    }
    calculate_subject_scores(s);
    save_data();
    set_color(COLOR_GREEN); printf("\nDa cap nhat thanh cong!\n");
}

void show_current_overall() {
    clear_screen();
    print_header("DIEM TONG KET HIEN TAI");
    float gpa_10 = calc_standard_with_cases(NULL, NULL, 0);
    float gpa_4 = calc_gpa_with_cases(NULL, NULL, 0);
    int unknown_count = 0;
    for(int i=0; i<subject_count; ++i) if(subjects[i].total == -2) unknown_count++;

    printf("> Diem toan khoa hien tai (he 10): ");
    set_color(COLOR_GREEN); printf("%.2f\n", gpa_10); set_color(COLOR_DEFAULT);

    printf("> GPA tong ket tuong duong (he 4): ");
    set_color(COLOR_GREEN); printf("%.2f\n", gpa_4); set_color(COLOR_DEFAULT);

    if (unknown_count > 0) {
        set_color(COLOR_YELLOW); printf("\n> Con %d mon chua co diem tong ket.\n", unknown_count);
    }
}

void try_score() {
    clear_screen();
    print_header("THU DIEM (WHAT-IF)");
    int unknown_idxs[MAX_SUBJECTS], n_unknown = 0;
    for (int i = 0; i < subject_count; ++i) if (subjects[i].total == -2) unknown_idxs[n_unknown++] = i;
    
    show_partial_grade_table(unknown_idxs, n_unknown, NULL, NULL);
    if (n_unknown == 0) return;

    float user_trial_scores[MAX_SUBJECTS]; int user_trial_idxs[MAX_SUBJECTS]; int n_user_trials = 0;
    while (1) {
        int choice = get_integer_input("\n> Chon STT mon muon thu diem (nhap 0 de ket thuc): ");
        if (choice == 0) break;
        int subject_idx = choice - 1; int found = 0;
        for(int i=0; i<n_unknown; ++i) if(unknown_idxs[i] == subject_idx) found = 1;
        if(!found) { set_color(COLOR_RED); printf("Lua chon khong hop le hoac mon da co diem!\n"); set_color(COLOR_DEFAULT); continue; }
        
        user_trial_idxs[n_user_trials] = subject_idx;
        Subject *s = &subjects[subject_idx];
        
        printf("\n--- Mon: %s ---\n", s->name);
        float gk = s->midterm, ck = s->final;
        if (s->midterm == -2) gk = get_float_input("> Nhap diem GK du kien: ");
        if (s->final == -2) ck = get_float_input("> Nhap diem CK du kien: ");
        float tong = (float)(((double)gk * s->midterm_ratio / 100.0) + ((double)ck * (100 - s->midterm_ratio) / 100.0));
        
        user_trial_scores[n_user_trials] = tong;
        n_user_trials++;

        float gpa_projected = calc_gpa_with_cases(user_trial_scores, user_trial_idxs, n_user_trials);
        printf("  -> GPA du kien (tinh tren cac mon da co diem va dang thu): "); 
        set_color(COLOR_GREEN); printf("%.2f\n", gpa_projected); set_color(COLOR_DEFAULT);
    }
}

void suggest_min_scores(float standard_target, int *unknown_subject_idxs, int n_unknown_subjects) {
    double sum_known_points = 0; double sum_all_credits = 0;
    for (int i = 0; i < subject_count; i++) {
        sum_all_credits += subjects[i].credits;
        if (subjects[i].total >= 0) sum_known_points += (double)subjects[i].total * subjects[i].credits;
    }
    double sum_partial_points = 0; double total_credit_weight_missing = 0;
    for (int i = 0; i < n_unknown_subjects; i++) {
        Subject* s = &subjects[unknown_subject_idxs[i]];
        if (s->midterm >= 0) {
            sum_partial_points += ((double)s->midterm * s->midterm_ratio / 100.0) * s->credits;
            total_credit_weight_missing += ((double)(100 - s->midterm_ratio) / 100.0) * s->credits;
        } else if (s->final >= 0) {
            sum_partial_points += ((double)s->final * (100 - s->midterm_ratio) / 100.0) * s->credits;
            total_credit_weight_missing += ((double)s->midterm_ratio / 100.0) * s->credits;
        } else { total_credit_weight_missing += (double)s->credits; }
    }
    double required_total_points = (double)standard_target * sum_all_credits;
    double required_points_from_missing = required_total_points - sum_known_points - sum_partial_points;
    if (total_credit_weight_missing == 0) return;
    double required_avg_score = required_points_from_missing / total_credit_weight_missing;

    printf("\n> De dat diem toan khoa %.2f, diem trung binh cho moi thanh phan con thieu can la: ", standard_target);
    set_color(COLOR_GREEN); printf("%.2f\n", required_avg_score); set_color(COLOR_DEFAULT);

    if (required_avg_score > 10.0 || required_avg_score < 0) {
        set_color(COLOR_RED); printf("  (Goi y khong kha thi! Diem yeu cau nam ngoai khoang [0, 10])\n");
    } else {
        printf("\n> Bang diem goi y chi tiet:\n");
        float* highlight_scores = malloc(n_unknown_subjects * sizeof(float));
        int* highlight_types = malloc(n_unknown_subjects * sizeof(int));
        if (highlight_scores == NULL || highlight_types == NULL) { if(highlight_scores) free(highlight_scores); if(highlight_types) free(highlight_types); return; }
        
        for (int i = 0; i < n_unknown_subjects; i++) {
            Subject* s = &subjects[unknown_subject_idxs[i]];
            highlight_scores[i] = required_avg_score;
            if (s->midterm == -2 && s->final >= 0) highlight_types[i] = HIGHLIGHT_MIDTERM;
            else if (s->midterm >= 0 && s->final == -2) highlight_types[i] = HIGHLIGHT_FINAL;
            else { highlight_types[i] = HIGHLIGHT_BOTH; }
        }
        show_partial_grade_table(unknown_subject_idxs, n_unknown_subjects, highlight_scores, highlight_types);
        free(highlight_scores);
        free(highlight_types);
    }
}

void suggest_scores_for_target_gpa() {
    clear_screen();
    print_header("GOI Y DIEM DE DAT MUC TIEU");
    int unknown_idxs[MAX_SUBJECTS], n_unknown = 0;
    for (int i = 0; i < subject_count; ++i) if (subjects[i].total == -2) unknown_idxs[n_unknown++] = i;
    if (n_unknown == 0) {
        set_color(COLOR_YELLOW); printf("Khong co mon nao 'chua biet diem' de goi y.\n"); return;
    }
    float max_scores_case[MAX_SUBJECTS];
    for (int i = 0; i < n_unknown; i++) {
        Subject* s = &subjects[unknown_idxs[i]];
        float max_gk = (s->midterm == -2) ? 10.0 : s->midterm;
        float max_ck = (s->final == -2) ? 10.0 : s->final;
        max_scores_case[i] = (float)(((double)max_gk * s->midterm_ratio / 100.0) + ((double)max_ck * (100-s->midterm_ratio) / 100.0));
    }
    float standard_max = calc_standard_with_cases(max_scores_case, unknown_idxs, n_unknown);
    float worst_scores_case[MAX_SUBJECTS]; 
    for (int i = 0; i < n_unknown; ++i) {
        Subject* s = &subjects[unknown_idxs[i]];
        float worst_gk = (s->midterm == -2) ? 0.0 : s->midterm;
        float worst_ck = (s->final == -2) ? 0.0 : s->final;
        worst_scores_case[i] = (float)(((double)worst_gk * s->midterm_ratio / 100.0) + ((double)worst_ck * (100-s->midterm_ratio) / 100.0));
    }
    float standard_worst = calc_standard_with_cases(worst_scores_case, unknown_idxs, n_unknown);

    printf("> Diem toan khoa toi da THUC TE (neu cac diem con lai deu 10): "); set_color(COLOR_GREEN); printf("%.2f\n", standard_max); set_color(COLOR_DEFAULT);
    printf("> Diem toan khoa toi thieu (neu cac diem con lai deu 0):  %.2f\n", standard_worst);

    float standard_target = get_float_input("\n> Nhap diem toan khoa ban mong muon: ");
    if (standard_target > standard_max) {
        set_color(COLOR_RED); printf("Khong the dat duoc diem nay! Muc tieu cao hon diem toi da co the dat.\n");
    } else if (standard_target < standard_worst) {
        set_color(COLOR_YELLOW); printf("Ban da dat duoc diem nay ngay ca khi 0 diem cac mon con lai!\n");
    }
    else {
        suggest_min_scores(standard_target, unknown_idxs, n_unknown);
    }
}

// =================== MAIN ===================

void main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleOutputCP(65001); // Dòng này không cần thiết cho UI ASCII
    SetConsoleTitleA("Chuong trinh Quan ly Diem sinh vien");
    load_data();
    int choice;
    do {
        clear_screen();
        print_header("QUAN LY DIEM SINH VIEN");
        set_color(COLOR_YELLOW);
        printf("1. Xem bang diem\n");
        printf("2. Them mon hoc moi\n");
        printf("3. Chinh sua mon hoc\n");
        printf("4. Xem diem tong ket hien tai\n");
        printf("5. Thu diem cho mon chua biet (What-if)\n");
        printf("6. Goi y diem de dat diem toan khoa muc tieu\n");
        printf("0. Thoat chuong trinh\n");
        set_color(COLOR_DEFAULT);

        choice = get_integer_input("\n> Nhap lua chon cua ban: ");

        switch (choice) {
            case 1: show_grade_table(); press_enter_to_continue(); break;
            case 2: add_new_subject(); press_enter_to_continue(); break;
            case 3: edit_subject(); press_enter_to_continue(); break;
            case 4: show_current_overall(); press_enter_to_continue(); break;
            case 5: try_score(); press_enter_to_continue(); break;
            case 6: suggest_scores_for_target_gpa(); press_enter_to_continue(); break;
            case 0: break;
            default:
                set_color(COLOR_RED); printf("Lua chon khong hop le. Vui long chon lai.\n");
                set_color(COLOR_DEFAULT); Sleep(1000);
        }
    } while (choice != 0);
    set_color(COLOR_CYAN); printf("\nTam biet!\n\n"); set_color(COLOR_DEFAULT);
}