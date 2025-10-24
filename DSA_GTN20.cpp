#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
using namespace std;

#define MAX_BACSI 12
#define MAX_LICHKHAM 1000
#define MAX_CHUYENKHOA 5

// Thông tin bác sĩ
struct BacSi {
    int maSo;
    char hoTen[50];
    int chuyenKhoa;   // 1: Rang, 2: Mat, 3: Noi, 4: Nhi, 5: Ngoai
    int caLamViec;    // 1: Sang, 2: Chieu, 3: Ca ngay
    int phongKham;
};

// Thông tin bệnh nhân
struct BenhNhan {
    char hoTen[50];
    long long cccd;
    int namSinh;
    char sdt[15];// số điện thoại
    char diaChi[100];
};

struct ThoiGianDangKy {
    int gio, phut, giay;
    int ngay, thang, nam;
};

// Thông tin đặt lịch
struct LichKham {
    BenhNhan benhnhan;
    int tenBenh;
    int chuyenKhoaBenh;
    BacSi bacsi;
    int ngay, thang, nam;
    int caKham;
    ThoiGianDangKy tgdk;
};

struct Node {
    LichKham data;
    Node* next;
};

// Tạo node mới
Node* createNode(LichKham lk) {
    Node* p = new Node;
    p->data = lk;
    p->next = NULL;
    return p;
}

// Kiểm tra ngày có hợp lệ không
bool NgayHopLe(int d, int m, int y) {
    if (m < 1 || m > 12 || d < 1 || d > 31) 
        return false;
    // Nhập số ngày trong tháng
    int ngayTrongThang[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    // Tính năm nhuận
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
        if (d > 29) 
            return false;
    } 
    else {
        if (d > ngayTrongThang[m-1]) 
            return false;
    }
    // So sánh với thời gian hiện tại
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int dHienTai = ltm->tm_mday;
    int mHienTai = ltm->tm_mon + 1;
    int yHienTai = ltm->tm_year + 1900;
    if (y < yHienTai) 
        return false;
    if (y == yHienTai && m < mHienTai) 
        return false;
    if (y == yHienTai && m == mHienTai && d < dHienTai) 
        return false;
    return true;
}

// Thời gian lúc đặt lịch thành công
void ThoiGianHeThong(ThoiGianDangKy &tg) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    tg.gio = ltm->tm_hour;
    tg.phut = ltm->tm_min;
    tg.giay = ltm->tm_sec;
    tg.ngay = ltm->tm_mday;
    tg.thang = ltm->tm_mon + 1;
    tg.nam = ltm->tm_year + 1900;
}

class QuanLyLichKham {
public:
    Node* head;
    Node* tail;
    QuanLyLichKham() : head(NULL), tail(NULL) {}
    
    int demSoLich(int maSo, int ngay, int thang, int nam, int caKham) {
        int dem = 0;
        Node* p = head;
        while (p) {
            if (
                p->data.bacsi.maSo == maSo &&
                p->data.ngay == ngay &&
                p->data.thang == thang &&
                p->data.nam == nam &&
                p->data.caKham == caKham
            ) dem++;
            p = p->next;
        }
        return dem;
    }

    void datLich(BacSi dsBacSi[], int soBacSi);
    void huyLich(BacSi dsBacSi[], int soBacSi);
    void suaLich(BacSi dsBacSi[], int soBacSi);
    void timKiemLichKhamTheoCCCD(long long cccd);
    void hienThiTatCa();
};

// Đặt lịch khám bệnh
void QuanLyLichKham::datLich(BacSi dsBacSi[], int soBacSi) {
    BenhNhan bn;
    cout << "Nhap ho ten benh nhan: ";
    cin.getline(bn.hoTen, 50);
    cout << "Nhap nam sinh: ";
    cin >> bn.namSinh; 
    cin.ignore();
    cout << "Nhap so CCCD: ";
    cin >> bn.cccd; 
    cin.ignore();
    cout << "Nhap so dien thoai (sdt): ";
    cin.getline(bn.sdt, 15);
    cout << "Nhap dia chi (diaChi): ";
    cin.getline(bn.diaChi, 100);
    // Nhập và kiểm tra ngày khám
    int d, m, y;
    while (true) {
        string ngayStr;
        cout << "Nhap ngay kham (dd/mm/yyyy): ";
        getline(cin, ngayStr);
        if (sscanf(ngayStr.c_str(), "%d/%d/%d", &d, &m, &y) != 3) {
            cout << "Dinh dang khong hop le. Vui long nhap lai!\n";
            continue;
        }
        if (!NgayHopLe(d, m, y)) {
            cout << "Ngay kham khong hop le. Vui long nhap lai!\n";
            continue;
        }
        break;
    }
    // Nhập ca khám
    int caKham;
    while (true) {
        cout << "Nhap ca kham (Nhap 1 de chon ca sang, nhap 2 de chon ca chieu): ";
        cin >> caKham;
        cin.ignore();
        if (caKham == 1 || caKham == 2) 
        break;
        cout << "Chi duoc nhap 1 (sang) hoac 2 (chieu)! Vui long nhap lai.\n";
    }
    
    // Nhập tên bệnh và chuyên khoa
    LichKham lk;
    lk.benhnhan = bn;
    lk.ngay = d; lk.thang = m; lk.nam = y;
    lk.caKham = caKham;
    cout << "Nhap ten benh(1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai): ";
    cin>>(lk.tenBenh);
    while (true) {
        cout << "Nhap chuyen khoa ( Nhap cac so sau de chon chuyen khoa kham (1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai)): ";
        cin >> lk.chuyenKhoaBenh; 
        cin.ignore();
        // Kiểm tra bệnh có đúng chuyên khoa không
        if(lk.tenBenh!=lk.chuyenKhoaBenh){
            cout<<"Ten benh va chuyen khoa benh khong khop.Hay nhap lai!";
            continue;
        }
         if (lk.chuyenKhoaBenh >= 1 && lk.chuyenKhoaBenh <= MAX_CHUYENKHOA) 
        break;
        cout << "Chuyen khoa khong hop le! Nhap lai.\n";
    }
    
    // In danh sách bác sĩ thuộc chuyên khoa và ca khám đã chọn
    cout << "\nDanh sach bac si co ma chuyen khoa [" << lk.chuyenKhoaBenh << "] || ca [" << caKham << "] || ngay "
         << d << "/" << m << "/" << y << ":\n";
    int a[MAX_BACSI], n = 0;
    for (int i = 0; i < soBacSi; ++i) {
        if (dsBacSi[i].chuyenKhoa == lk.chuyenKhoaBenh &&
           (dsBacSi[i].caLamViec == caKham || dsBacSi[i].caLamViec == 3)) {
            int soLuot = demSoLich(dsBacSi[i].maSo, d, m, y, caKham);
            cout << "Ma BS: " << dsBacSi[i].maSo
                 << " | Ho ten: " << dsBacSi[i].hoTen
                 << " | Ma chuyen khoa: " << dsBacSi[i].chuyenKhoa
                 << " | Ca lam viec: " << dsBacSi[i].caLamViec
                 << " | Phong kham: " << dsBacSi[i].phongKham
                 << " | Luot da dat: " << soLuot << "/25\n";
            a[n] = i;
            n++;
        }
    }
    if (n == 0) {
        cout << "Khong co bac si nao thuoc chuyen khoa va ca nay!\n";
        return;
    }
    
    // Chọn bác sĩ muốn khám
    BacSi bsChon;
    while (true) {
        cout << "Nhap ma so bac si de chon: ";
        int maBS;
        cin >> maBS; 
        cin.ignore();
        bool timDuoc = false;
        for (int i = 0; i < n; ++i) {
            if (dsBacSi[a[i]].maSo == maBS) {
                bsChon = dsBacSi[a[i]];
                timDuoc = true;
                break;
            }
        }
        if (!timDuoc) 
        cout << "Khong tim thay bac si phu hop. Vui long nhap lai!\n";
        else 
            break;
    }
    
    // Đếm số lượt đặt lịch của 1 bác sĩ trong 1 ca khám trong 1 ngày
    int luotDaDat = demSoLich(bsChon.maSo, d, m, y, caKham);
    if (luotDaDat >= 25) {
        cout << "Bac si da dat toi da 25 luot/ca/ngay. Vui long chon bac si hoac ca khac!\n";
        return;
    }

    lk.bacsi = bsChon;
    ThoiGianHeThong(lk.tgdk);

    Node* node = createNode(lk);
    if (!head) 
        head = tail = node;
    else 
        tail->next = node; 
        tail = node;
    cout << "Dat lich thanh cong!\n";
    cout << "Thoi gian dat lich: " << lk.tgdk.gio << ":" << lk.tgdk.phut << ":" << lk.tgdk.giay << "|| "
         << lk.tgdk.ngay << "/" << lk.tgdk.thang << "/" << lk.tgdk.nam << endl;
}

// Hủy lịch khám
void QuanLyLichKham::huyLich(BacSi dsBacSi[], int soBacSi) {
    cout << "Nhap CCCD benh nhan can huy lich: ";
    long long cccd;
    cin >> cccd; 
    cin.ignore();
    Node* a[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) 
        a[n] = p;
        n++;
        p = p->next;
    }
    if (n == 0) {
        cout << "Khong tim thay benh nhan voi CCCD nay.\n";
        return;
    }
    // In danh sách đặt lịch khám của bệnh nhân 
    cout << "Danh sach lich kham cua benh nhan:\n";
    for (int i = 0; i < n; ++i) {
        LichKham& lk = a[i]->data;
        cout << i + 1 << ". " << lk.ngay << "/" << lk.thang << "/" << lk.nam
             << " || Ca: " << lk.caKham
             << " || Bac si: " << lk.bacsi.hoTen << " | Ma BS: " << lk.bacsi.maSo
             << " || Ma chuyen khoa: " << lk.chuyenKhoaBenh
             << " || Phong kham: " << lk.bacsi.phongKham
             << " || Thoi gian dat: " << lk.tgdk.gio << ":" << lk.tgdk.phut << ":" << lk.tgdk.giay << " || "
             << lk.tgdk.ngay << "/" << lk.tgdk.thang << "/" << lk.tgdk.nam << endl;
    }
    // Chọn lịch muốn hủy
    cout << "Nhap so thu tu lich kham muon huy: ";
    int m;
    cin >> m; 
    cin.ignore();
    if (m < 1 || m > n) {
        cout << "So thu tu khong hop le.\n";
        return;
    }
    Node* nodeHuy = a[m - 1];
    if (nodeHuy == head) 
        head = nodeHuy->next;
    else {
        Node* prev = head;
        while (prev->next != nodeHuy){
            prev = prev->next;
        }
            prev->next = nodeHuy->next;
        if (nodeHuy == tail) 
            tail = prev;
    }
    cout << "Da huy lich kham.\n";
}

// Sửa lịch khám
void QuanLyLichKham::suaLich(BacSi dsBacSi[], int soBacSi) {
    cout << "Nhap CCCD benh nhan can sua lich: ";
    long long cccd;
    cin >> cccd; 
    cin.ignore();
    Node* a[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) 
        a[n] = p;
        n++;
        p = p->next;
    }
    if (n == 0) {
        cout << "Khong tim thay benh nhan voi CCCD nay.\n";
        return;
    }
    // In danh sách đặt lịch của bệnh nhân
    cout << "Danh sach lich kham cua benh nhan:\n";
    for (int i = 0; i < n; ++i) {
        LichKham& lk = a[i]->data;
        cout << i + 1 << ". " << lk.ngay << "/" << lk.thang << "/" << lk.nam
             << " | Ca: " << lk.caKham
             << " | Bac si: " << lk.bacsi.hoTen << " | Ma BS: " << lk.bacsi.maSo
             << " | Ma chuyen khoa: " << lk.chuyenKhoaBenh
             << " | Phong kham: " << lk.bacsi.phongKham
             << " | Thoi gian dat: " << lk.tgdk.gio << ":" << lk.tgdk.phut << ":" << lk.tgdk.giay << " || "
             << lk.tgdk.ngay << "/" << lk.tgdk.thang << "/" << lk.tgdk.nam << endl;
    }
    // Chọn lịch muốn sửa
    cout << "Nhap so thu tu lich kham muon sua: ";
    int m;
    cin >> m; 
    cin.ignore();
    if (m < 1 || m > n) {
        cout << "So thu tu khong hop le.\n";
        return;
    }
    Node* nodeSua = a[m - 1];
    LichKham& lk = nodeSua->data;

    // Cho phép sửa thông tin bệnh nhân
    cout << "Ban co muon sua thong tin benh nhan (ho ten, CCCD, nam sinh, sdt, dia chi)? (y/n): ";
    char ch;
    cin >> ch;
    cin.ignore();
    if (ch == 'y' || ch == 'Y') {
        cout << "Nhap ho ten benh nhan moi: ";
        cin.getline(lk.benhnhan.hoTen, 50);
        cout << "Nhap nam sinh moi: ";
        cin >> lk.benhnhan.namSinh;
        cin.ignore();
        cout << "Nhap so CCCD moi: ";
        cin >> lk.benhnhan.cccd;
        cin.ignore();
        cout << "Nhap so dien thoai (sdt) moi: ";
        cin.getline(lk.benhnhan.sdt, 15);
        cout << "Nhap dia chi (diaChi) moi: ";
        cin.getline(lk.benhnhan.diaChi, 100);
    }
    
    // Nhập ngày khám mới
    int d, mth, y;
    while (true) {
        string ngayStr;
        cout << "Nhap ngay kham moi (dd/mm/yyyy): ";
        getline(cin, ngayStr);
        if (sscanf(ngayStr.c_str(), "%d/%d/%d", &d, &mth, &y) != 3) {
            cout << "Dinh dang khong hop le. Vui long nhap lai!\n";
            continue;
        }
        if (!NgayHopLe(d, mth, y)) {
            cout << "Ngay kham khong hop le. Vui long nhap lai!\n";
            continue;
        }
        break;
    }
    lk.ngay = d; lk.thang = mth; lk.nam = y;
    
    // Nhập tên bệnh mới
    cout << "Nhap ten benh moi(1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai): ";
    cin>>(lk.tenBenh);
    
    // Nhập chuyên khoa mới
    while (true) {
        cout << "Nhap chuyen khoa (Nhap cac so sau de chon chuyen khoa kham (1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai)): ";
        cin >> lk.chuyenKhoaBenh; 
        cin.ignore();
        // Kiểm tra tên bệnh và chuyên khoa có khớp không
        if(lk.tenBenh!=lk.chuyenKhoaBenh){
            cout<<"Ten benh va chuyen khoa benh khong khop.Hay nhap lai!";
            continue;
        }
        if (lk.chuyenKhoaBenh >= 1 && lk.chuyenKhoaBenh <= MAX_CHUYENKHOA)
        break;
        cout << "Chuyen khoa khong hop le! Nhap lai.\n";
    }
    // Nhập ca khám mới
     while (true) {
        cout << "Nhap ca kham moi (Nhap 1 de chon ca sang, nhap 2 de chon ca chieu): ";
        cin >> lk.caKham; 
        cin.ignore();
        if (lk.caKham == 1 || lk.caKham == 2) 
            break;
        cout << "Chi duoc nhap 1 (sang) hoac 2 (chieu)! Vui long nhap lai.\n";
    }
    // In danh sách các bác sĩ thuộc chuyên khoa mới và ca khám mới mà bệnh nhân đã sửa
    int b[MAX_BACSI], k = 0;
    for (int i = 0; i < soBacSi; ++i) {
        if (dsBacSi[i].chuyenKhoa == lk.chuyenKhoaBenh &&
           (dsBacSi[i].caLamViec == lk.caKham || dsBacSi[i].caLamViec == 3)) {
            cout << "Ma BS: " << dsBacSi[i].maSo << " | Ho ten: " << dsBacSi[i].hoTen
                 << " || Ca lam viec: " << dsBacSi[i].caLamViec
                 << "|| Chuyen khoa: " <<dsBacSi[i].chuyenKhoa
                 << " || Phong kham: " << dsBacSi[i].phongKham << endl;
            b[k] = i;
            k++;
        }
    }
    if (k == 0) {
        cout << "Khong co bac si nao thuoc chuyen khoa va ca nay!\n";
        return;
    }
    // Nhập mã số để chọn bác sĩ
    BacSi bsChon;
    while (true) {
        cout << "Nhap ma so bac si de chon: ";
        int maBS;
        cin >> maBS; 
        cin.ignore();
        bool timDuoc = false;
        for (int i = 0; i < k; ++i) {
            if (dsBacSi[b[i]].maSo == maBS) {
                bsChon = dsBacSi[b[i]];
                timDuoc = true;
                break;
            }
        }
        if (!timDuoc) 
            cout << "Khong tim thay bac si phu hop. Vui long nhap lai!\n";
        else break;
    }
     lk.bacsi = bsChon;

    ThoiGianHeThong(lk.tgdk);
    cout << "Da sua lich kham.\n";
}

// Tìm kiếm lịch khám của bệnh nhân theo căn cước công dân
void QuanLyLichKham::timKiemLichKhamTheoCCCD(long long cccd) {
    LichKham ds[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) 
        ds[n] = p->data;
        n++;
        p = p->next;
    }
    if (n == 0) {
        cout << "Khong co lich kham nao voi CCCD: " << cccd << endl;
        return;
    }
    // Sắp xếp lịch khám đặt theo ngày tháng năm khám bệnh, theo ca khám, theo bác sĩ, theo thời gian đặt lịch thành công
    for (int i = 0; i < n-1; ++i)
        for (int j = i+1; j < n; ++j) {
            LichKham &a = ds[i], &b = ds[j];
            if (
                a.nam > b.nam ||
                (a.nam == b.nam && a.thang > b.thang) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay > b.ngay) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham > b.caKham) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo > b.bacsi.maSo) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo == b.bacsi.maSo &&
                 (a.tgdk.nam > b.tgdk.nam ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang > b.tgdk.thang) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay > b.tgdk.ngay) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio > b.tgdk.gio) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio == b.tgdk.gio && a.tgdk.phut > b.tgdk.phut) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio == b.tgdk.gio && a.tgdk.phut == b.tgdk.phut && a.tgdk.giay > b.tgdk.giay)
                )
            )) {
                LichKham t = ds[i]; 
                         ds[i] = ds[j]; 
                         ds[j] = t;
            }
        }
    int ngayHienTai = -1, thangHienTai = -1, namHienTai = -1;
    int caHienTai = -1, bacsiHienTai = -1;
    for (int k = 0; k < n; ++k) {
        LichKham& lk = ds[k];
        if (!(lk.ngay == ngayHienTai && lk.thang == thangHienTai && lk.nam == namHienTai)) {
            cout << "\n=== Ngay kham: " << lk.ngay << "/" << lk.thang << "/" << lk.nam << " ===\n";
            ngayHienTai = lk.ngay; thangHienTai = lk.thang; namHienTai = lk.nam;
            caHienTai = bacsiHienTai = -1;
        }
        if (lk.caKham != caHienTai) {
            cout << "-- Ca kham: " << lk.caKham << " --\n";
            caHienTai = lk.caKham;
            bacsiHienTai = -1;
        }
        // In thông tin đặt lịch khám bệnh
        cout << "Ho ten: " << lk.benhnhan.hoTen << " | Tuoi: " << (2025 - lk.benhnhan.namSinh)
             << " | CCCD: " << lk.benhnhan.cccd << " | SDT: " << lk.benhnhan.sdt << " | Dia chi: " << lk.benhnhan.diaChi << endl;
             
        cout << "Bac si: " << lk.bacsi.hoTen
             << " | Ma so: " << lk.bacsi.maSo
             << " | Ma chuyen khoa: " << lk.bacsi.chuyenKhoa
             << " | Phong kham: " << lk.bacsi.phongKham << endl;
             
        cout << "Ten benh: " << lk.tenBenh << " | Ma khoa: " << lk.chuyenKhoaBenh
             << " | Thoi gian dang ky: " << lk.tgdk.gio << ":" << lk.tgdk.phut << ":" << lk.tgdk.giay << " "
             << lk.tgdk.ngay << "/" << lk.tgdk.thang << "/" << lk.tgdk.nam << endl;
        cout << "------------------------\n";
    }
}

// Hiển thị tất cả các lịch khám
void QuanLyLichKham::hienThiTatCa() {
    LichKham ds[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    while (p) {
        ds[n] = p->data;
        n++;
        p = p->next;
    }
    // Sắp xếp lịch khám đặt theo ngày tháng năm khám bệnh, theo ca khám, theo bác sĩ, theo thời gian đặt lịch thành công
    for (int i = 0; i < n-1; ++i)
        for (int j = i+1; j < n; ++j) {
            LichKham &a = ds[i], &b = ds[j];
            if (
                a.nam > b.nam ||
                (a.nam == b.nam && a.thang > b.thang) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay > b.ngay) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham > b.caKham) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo > b.bacsi.maSo) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo == b.bacsi.maSo &&
                 (a.tgdk.nam > b.tgdk.nam ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang > b.tgdk.thang) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay > b.tgdk.ngay) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio > b.tgdk.gio) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio == b.tgdk.gio && a.tgdk.phut > b.tgdk.phut) ||
                  (a.tgdk.nam == b.tgdk.nam && a.tgdk.thang == b.tgdk.thang && a.tgdk.ngay == b.tgdk.ngay && a.tgdk.gio == b.tgdk.gio && a.tgdk.phut == b.tgdk.phut && a.tgdk.giay > b.tgdk.giay)
                )
            )) {
                LichKham t = ds[i]; ds[i] = ds[j]; ds[j] = t;
            }
        }
    int ngayHienTai = -1, thangHienTai = -1, namHienTai = -1;
    int caHienTai = -1, bacsiHienTai = -1;
    for (int k = 0; k < n; ++k) {
        LichKham& lk = ds[k];
        if (!(lk.ngay == ngayHienTai && lk.thang == thangHienTai && lk.nam == namHienTai)) {
            cout << "\n=== Ngay kham: " << lk.ngay << "/" << lk.thang << "/" << lk.nam << " ===\n";
            ngayHienTai = lk.ngay; thangHienTai = lk.thang; namHienTai = lk.nam;
            caHienTai = bacsiHienTai = -1;
        }
        if (lk.caKham != caHienTai) {
            cout << "-- Ca kham: " << lk.caKham << " --\n";
            caHienTai = lk.caKham;
            bacsiHienTai = -1;
        }
        
        // In thông tin đặt lịch khám
        cout << "Ho ten: " << lk.benhnhan.hoTen << " | Tuoi: " << (2025 - lk.benhnhan.namSinh)
             << " | CCCD: " << lk.benhnhan.cccd << " | SDT: " << lk.benhnhan.sdt << " | Dia chi: " << lk.benhnhan.diaChi << endl;

        cout << "Bac si: " << lk.bacsi.hoTen
             << " | Ma so: " << lk.bacsi.maSo
             << " | Ma chuyen khoa: " << lk.bacsi.chuyenKhoa
             << " | Phong kham: " << lk.bacsi.phongKham << endl;

        cout << "Ten benh: " << lk.tenBenh << " | Ma khoa: " << lk.chuyenKhoaBenh
             << " | Thoi gian dang ky: " << lk.tgdk.gio << ":" << lk.tgdk.phut << ":" << lk.tgdk.giay << " "
             << lk.tgdk.ngay << "/" << lk.tgdk.thang << "/" << lk.tgdk.nam << endl;
        cout << "------------------------\n";
    }
}

int main() {
    BacSi dsBacSi[MAX_BACSI] = {
        // Thông tin bác sĩ{ Mã số bác sĩ, "Họ tên bác sĩ", "chuyên khoa","ca khám"," Phòng khám"}
        {101, "Nguyen Van An", 1, 1, 101},
        {102, "Tran Thi Thao Vy", 1, 2, 101},
        {103, "Le Van Dung", 2, 3, 201},
        {104, "Pham Thi Thu Duyen", 3, 1, 301},
        {105, "Hoang Van Viet", 3, 3, 302},
        {106, "Ly Hoang Anh", 3, 1, 303},
        {107, "Trieu Minh Tien", 3, 2, 301},
        {108, "Ho Viet Quoc", 4, 1, 401},
        {109, "Ngo Trong Nghia", 4, 2, 401},
        {110, "Phuong Nguyen", 5, 3, 501},
        {111, "Vo Thao Nguyen", 5, 1, 502},
        {112, "Bui Huyen Huyen", 5, 2, 503}
    };
    int soBacSi = 12;

    QuanLyLichKham ql;
    int chon;
    do {
        cout << "\n\n====== MENU QUAN LY DAT LICH KHAM BENH CUA PHONG KHAM ======\n";
        cout << "1. Dat lich kham\n";
        cout << "2. Huy lich kham\n";
        cout << "3. Sua lich kham\n";
        cout << "4. Tim kiem lich kham theo CCCD\n";
        cout << "5. Xem tat ca lich kham (da sap xep, phan loai)\n";
        cout << "0. Thoat\n";
        cout << "Chon chuc nang: ";
        cin >> chon; cin.ignore();
        switch (chon) {
            case 1:
                ql.datLich(dsBacSi, soBacSi);
                break;
            case 2:
                ql.huyLich(dsBacSi, soBacSi);
                break;
            case 3:
                ql.suaLich(dsBacSi, soBacSi);
                break;
            case 4: {
                cout << "Nhap CCCD benh nhan: ";
                long long cccd;
                cin >> cccd; cin.ignore();
                ql.timKiemLichKhamTheoCCCD(cccd);
                break;
            }
            case 5:
                ql.hienThiTatCa();
                break;
            case 0:
                cout << "Thoat chuong trinh.\n";
                break;
            default:
                cout << "Chuc nang khong hop le!\n";
        }
    } while (chon != 0);
    return 0;
}