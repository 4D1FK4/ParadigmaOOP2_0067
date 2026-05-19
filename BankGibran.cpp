/*
 * ============================================================
 *  BANK GIBRAN JAYA - Sistem Otomatisasi Biaya Admin Akhir Bulan
 * ============================================================
 *  Arsitektur   : Abstract Base Class + Pure Virtual Function
 *  Produk       : Rekening Syariah, Konvensional, Premium
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// =============================================================
//  UTILITAS: Format angka ke format Rupiah
// =============================================================
string formatRupiah(double nominal) {
    string hasil = "Rp ";
    long long bulat = (long long)nominal;
    string angka  = to_string(bulat);
    int sisaDigit = angka.size() % 3;
    for (int i = 0; i < (int)angka.size(); i++) {
        if (i != 0 && (i - sisaDigit) % 3 == 0) hasil += ".";
        hasil += angka[i];
    }
    return hasil;
}

// =============================================================
//  ABSTRACT BASE CLASS: RekeningBank
// =============================================================
class RekeningBank {
protected:
    string nomorRekening;
    string namaNasabah;
    double saldo;

public:
    RekeningBank(const string& nomor, const string& nama, double saldoAwal)
        : nomorRekening(nomor), namaNasabah(nama), saldo(saldoAwal) {}

    // Pure virtual function — wajib diimplementasikan oleh setiap subclass
    virtual void potongAdmin() = 0;

    // Virtual function untuk menampilkan info rekening
    virtual void tampilInfo() const {
        cout << "  No. Rekening  : " << nomorRekening   << "\n"
             << "  Nama Nasabah  : " << namaNasabah     << "\n"
             << "  Saldo         : " << formatRupiah(saldo) << "\n";
    }

    // Getter
    string getNomorRekening() const { return nomorRekening; }
    string getNamaNasabah()   const { return namaNasabah;   }
    double getSaldo()         const { return saldo;          }

    // Virtual destructor (best practice untuk base class)
    virtual ~RekeningBank() {}
};

// =============================================================
//  SUBCLASS 1: RekeningKonvensional
//  Aturan: Potong Rp 15.000 setiap bulan
// =============================================================
class RekeningKonvensional : public RekeningBank {
private:
    static constexpr double BIAYA_ADMIN = 15000.0;

public:
    RekeningKonvensional(const string& nomor, const string& nama, double saldoAwal)
        : RekeningBank(nomor, nama, saldoAwal) {}

    void potongAdmin() override {
        cout << "\n[KONVENSIONAL] " << namaNasabah << "\n";
        cout << "  Saldo sebelum : " << formatRupiah(saldo) << "\n";

        if (saldo >= BIAYA_ADMIN) {
            saldo -= BIAYA_ADMIN;
            cout << "  Potongan admin: " << formatRupiah(BIAYA_ADMIN) << "\n";
        } else {
            // Saldo tidak mencukupi — tidak dipotong agar tidak negatif
            cout << "  [!] Saldo tidak mencukupi untuk biaya admin. Potongan dilewati.\n";
        }

        cout << "  Saldo sesudah : " << formatRupiah(saldo) << "\n";
    }

    void tampilInfo() const override {
        cout << "  Jenis         : Rekening Konvensional\n";
        RekeningBank::tampilInfo();
    }
};

// =============================================================
//  SUBCLASS 2: RekeningSyariah
//  Aturan: Bebas biaya admin (saldo tetap utuh)
// =============================================================
class RekeningSyariah : public RekeningBank {
public:
    RekeningSyariah(const string& nomor, const string& nama, double saldoAwal)
        : RekeningBank(nomor, nama, saldoAwal) {}

    void potongAdmin() override {
        cout << "\n[SYARIAH] " << namaNasabah << "\n";
        cout << "  Saldo         : " << formatRupiah(saldo) << "\n";
        cout << "  Biaya admin   : Rp 0 (Bebas — prinsip Syariah)\n";
        cout << "  Saldo tetap   : " << formatRupiah(saldo) << "\n";
    }

    void tampilInfo() const override {
        cout << "  Jenis         : Rekening Syariah\n";
        RekeningBank::tampilInfo();
    }
};

// =============================================================
//  SUBCLASS 3: RekeningPremium  ← PRODUK BARU
//  Aturan:
//    - Saldo > Rp 10.000.000  → bebas biaya admin
//    - Saldo ≤ Rp 10.000.000  → potong Rp 50.000
// =============================================================
class RekeningPremium : public RekeningBank {
private:
    static constexpr double BATAS_BEBAS_ADMIN = 10000000.0;
    static constexpr double BIAYA_ADMIN       =    50000.0;

public:
    RekeningPremium(const string& nomor, const string& nama, double saldoAwal)
        : RekeningBank(nomor, nama, saldoAwal) {}

    void potongAdmin() override {
        cout << "\n[PREMIUM] " << namaNasabah << "\n";
        cout << "  Saldo sebelum : " << formatRupiah(saldo) << "\n";

        if (saldo > BATAS_BEBAS_ADMIN) {
            // Saldo di atas batas — BEBAS biaya admin
            cout << "  Status        : Saldo > " << formatRupiah(BATAS_BEBAS_ADMIN)
                 << " → Bebas biaya admin\n";
            cout << "  Potongan admin: Rp 0\n";
        } else {
            // Saldo di bawah/sama dengan batas — DIPOTONG Rp 50.000
            cout << "  Status        : Saldo ≤ " << formatRupiah(BATAS_BEBAS_ADMIN)
                 << " → Dikenakan biaya admin\n";
            if (saldo >= BIAYA_ADMIN) {
                saldo -= BIAYA_ADMIN;
                cout << "  Potongan admin: " << formatRupiah(BIAYA_ADMIN) << "\n";
            } else {
                cout << "  [!] Saldo tidak mencukupi untuk biaya admin. Potongan dilewati.\n";
            }
        }

        cout << "  Saldo sesudah : " << formatRupiah(saldo) << "\n";
    }

    void tampilInfo() const override {
        cout << "  Jenis         : Rekening Premium\n";
        RekeningBank::tampilInfo();
    }
};

// =============================================================
//  SERVER BANK: Proses semua rekening via pointer base class
// =============================================================
void prosesAkhirBulan(const vector<RekeningBank*>& daftarRekening) {
    cout << "\n"
         << "╔══════════════════════════════════════════════════╗\n"
         << "║     PROSES BIAYA ADMIN AKHIR BULAN               ║\n"
         << "║     Bank Gibran Jaya — Sistem Otomatisasi         ║\n"
         << "╚══════════════════════════════════════════════════╝\n";

    for (RekeningBank* rek : daftarRekening) {
        rek->potongAdmin();   // Polymorphism: memanggil implementasi yang tepat
    }

    cout << "\n"
         << "══════════════════════════════════════════════════\n"
         << "  REKAP SALDO AKHIR BULAN\n"
         << "══════════════════════════════════════════════════\n";

    double totalSaldo = 0;
    for (const RekeningBank* rek : daftarRekening) {
        cout << "\n";
        rek->tampilInfo();
        totalSaldo += rek->getSaldo();
    }

    cout << "\n══════════════════════════════════════════════════\n"
         << "  Total Dana Kelolaan : " << formatRupiah(totalSaldo) << "\n"
         << "══════════════════════════════════════════════════\n";
}

// =============================================================
//  MAIN
// =============================================================
int main() {
    // Inisialisasi data nasabah
    vector<RekeningBank*> serverRekening = {
        // Rekening Konvensional
        new RekeningKonvensional("KON-001", "Budi Santoso",   5000000.0),
        new RekeningKonvensional("KON-002", "Siti Rahayu",      10000.0),  // saldo minim

        // Rekening Syariah
        new RekeningSyariah("SYR-001", "Ahmad Fauzi",        8000000.0),
        new RekeningSyariah("SYR-002", "Dewi Lestari",         750000.0),

        // Rekening Premium — produk baru
        new RekeningPremium("PRM-001", "Hendra Wijaya",    15000000.0),  // > 10 jt → bebas
        new RekeningPremium("PRM-002", "Rina Kusumawati",   9500000.0),  // ≤ 10 jt → potong
        new RekeningPremium("PRM-003", "Fajar Nugroho",    10000000.0),  // == 10 jt → potong
    };

    // Server bank memproses semua rekening via pointer base class
    prosesAkhirBulan(serverRekening);

    // Bersihkan memori
    for (RekeningBank* rek : serverRekening) {
        delete rek;
    }

    return 0;
}