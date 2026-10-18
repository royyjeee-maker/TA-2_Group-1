#include <stdio.h>     // Library untuk input/output (printf, scanf, fopen, dll)
#include <stdlib.h>    // Library untuk fungsi-fungsi umum (system, exit, dll)
#include <string.h>    // Library untuk operasi string (strcmp, strcspn, dll)

// Struktur data untuk menyimpan informasi siswa
// struct adalah cara mengelompokkan beberapa variabel jadi satu kesatuan
struct Siswa {
    char nama[50];              // Array karakter untuk nama (max 50 karakter)
    int umur;                   // Variabel integer untuk menyimpan umur
    char nomorRegistrasi[20];   // Array karakter untuk nomor registrasi (max 20 karakter)
};

// Fungsi untuk menampilkan menu utama
// void = fungsi ini tidak mengembalikan nilai apapun
void tampilkanMenu() {
    printf("\n=== SISTEM MANAJEMEN DATABASE SISWA ===\n");  // Judul program
    printf("1. Tambah Data Siswa\n");                       // Pilihan menu 1
    printf("2. Hapus Data Siswa\n");                        // Pilihan menu 2
    printf("3. Cari Data Siswa\n");                         // Pilihan menu 3
    printf("4. Tampilkan Semua Data Siswa\n");              // Pilihan menu 4
    printf("5. Keluar\n");                                  // Pilihan menu 5
    printf("Pilih menu (1-5): ");                           // Minta input dari user
}

// Fungsi 1: Menambah data siswa ke file
// void = tidak mengembalikan nilai
void tambahSiswa() {
    struct Siswa siswa;  // Membuat variabel siswa dengan tipe struct Siswa untuk menyimpan data
    FILE *file;          // Pointer untuk menangani file (FILE adalah tipe data khusus untuk file)
    
    printf("\n--- Tambah Data Siswa ---\n");  // Header/judul untuk fitur tambah data
    
    // INPUT DATA SISWA DARI USER
    printf("Masukkan Nama: ");              // Tampilkan prompt untuk nama
    getchar();                              // Membersihkan buffer input (sisa newline dari scanf sebelumnya)
    fgets(siswa.nama, sizeof(siswa.nama), stdin);  // Baca nama lengkap (bisa ada spasi) dari keyboard
    siswa.nama[strcspn(siswa.nama, "\n")] = 0;     // Hapus karakter newline (\n) di akhir string yang diinput fgets
    
    printf("Masukkan Umur: ");              // Tampilkan prompt untuk umur
    scanf("%d", &siswa.umur);               // Baca umur (integer) dan simpan ke siswa.umur (&siswa.umur = alamat memori)
    
    printf("Masukkan Nomor Registrasi: ");  // Tampilkan prompt untuk nomor registrasi
    scanf("%s", siswa.nomorRegistrasi);     // Baca nomor registrasi (string tanpa spasi)
    
    // PROSES PENYIMPANAN KE FILE
    file = fopen("database_siswa.txt", "a");  // Buka file dalam mode "a" (append = tambah di akhir, tidak timpa data lama)
    
    // Cek apakah file berhasil dibuka
    if (file == NULL) {                     // NULL = gagal membuka file
        printf("Error: Tidak bisa membuka file!\n");  // Tampilkan pesan error
        return;                             // Keluar dari fungsi (berhenti di sini)
    }
    
    // Tulis data ke file dengan format: Nama|Umur|NomorRegistrasi
    // fprintf = seperti printf tapi hasilnya ditulis ke file, bukan ke layar
    fprintf(file, "%s|%d|%s\n",            // Format string: %s (string) | %d (integer) | %s (string) \n (newline)
            siswa.nama,                     // Data nama
            siswa.umur,                     // Data umur
            siswa.nomorRegistrasi);         // Data nomor registrasi
    
    // Tutup file untuk mengamankan data
    fclose(file);  // PENTING! Selalu tutup file setelah selesai digunakan
    
    printf("Data siswa berhasil ditambahkan!\n");  // Konfirmasi ke user bahwa data berhasil disimpan
}

// Fungsi 2: Menghapus data siswa berdasarkan nomor registrasi
// Metode: Menggunakan file temporary untuk keamanan data
void hapusSiswa() {
    char nomorCari[20];     // Array untuk menyimpan nomor registrasi yang akan dicari dan dihapus
    struct Siswa siswa;     // Variabel untuk membaca data siswa dari file
    FILE *fileAsli, *fileTemp;  // Pointer untuk file original dan file temporary
    int ditemukan = 0;      // Flag/penanda apakah data ditemukan (0 = belum, 1 = sudah)
    
    printf("\n--- Hapus Data Siswa ---\n");  // Header untuk fitur hapus data
    printf("Masukkan Nomor Registrasi yang akan dihapus: ");  // Minta input nomor registrasi
    scanf("%s", nomorCari);  // Baca nomor registrasi dari user dan simpan ke nomorCari
    
    // BUKA FILE ORIGINAL (DATABASE ASLI)
    fileAsli = fopen("database_siswa.txt", "r");  // Mode "r" = read (baca saja)
    if (fileAsli == NULL) {  // Cek apakah file bisa dibuka
        printf("Error: File database tidak ditemukan!\n");  // Error jika file tidak ada
        return;  // Keluar dari fungsi
    }
    
    // BUAT FILE TEMPORARY (SEMENTARA)
    fileTemp = fopen("temp.txt", "w");  // Mode "w" = write (tulis, buat file baru)
    if (fileTemp == NULL) {  // Cek apakah file temporary bisa dibuat
        printf("Error: Tidak bisa membuat file temporary!\n");  // Error jika gagal buat file temp
        fclose(fileAsli);  // Tutup file original sebelum keluar
        return;  // Keluar dari fungsi
    }
    
    // LOOP: BACA SETIAP BARIS DARI FILE ORIGINAL
    // fscanf membaca data terformat dari file
    // %[^|] = baca semua karakter sampai ketemu | (untuk nama yang bisa ada spasi)
    // %d = baca integer (umur)
    // %[^\n] = baca semua karakter sampai ketemu newline (untuk nomor registrasi)
    // == 3 artinya harus berhasil baca 3 item (nama, umur, noReg)
    while (fscanf(fileAsli, "%[^|]|%d|%[^\n]\n", 
                  siswa.nama, &siswa.umur, siswa.nomorRegistrasi) == 3) {
        
        // BANDINGKAN NOMOR REGISTRASI
        // strcmp = string compare (membandingkan 2 string)
        // Return 0 jika sama, bukan 0 jika beda
        // != 0 artinya TIDAK SAMA
        if (strcmp(siswa.nomorRegistrasi, nomorCari) != 0) {
            // Jika TIDAK SAMA dengan yang dicari, COPY data ini ke file temporary
            // (Data ini TIDAK dihapus, jadi kita simpan)
            fprintf(fileTemp, "%s|%d|%s\n", 
                    siswa.nama, siswa.umur, siswa.nomorRegistrasi);
        } else {
            // Jika SAMA dengan yang dicari, JANGAN copy ke file temp
            // (Data ini yang mau dihapus, jadi SKIP/lewati)
            ditemukan = 1;  // Set flag jadi 1 (true) karena data ditemukan
        }
    }
    
    // TUTUP KEDUA FILE
    fclose(fileAsli);   // Tutup file original
    fclose(fileTemp);   // Tutup file temporary
    
    // REPLACE FILE ORIGINAL DENGAN FILE TEMPORARY
    remove("database_siswa.txt");  // Hapus file original
    rename("temp.txt", "database_siswa.txt");  // Rename file temp jadi nama file original
    
    // TAMPILKAN HASIL
    if (ditemukan) {  // Jika flag ditemukan = 1 (true)
        printf("Data siswa dengan nomor registrasi %s berhasil dihapus!\n", nomorCari);
    } else {  // Jika flag ditemukan = 0 (false)
        printf("Data siswa dengan nomor registrasi %s tidak ditemukan!\n", nomorCari);
    }
}

// Fungsi 3: Mencari data siswa berdasarkan nomor registrasi
void cariSiswa() {
    char nomorCari[20];     // Array untuk menyimpan nomor registrasi yang dicari
    struct Siswa siswa;     // Variabel untuk membaca dan menyimpan data siswa dari file
    FILE *file;             // Pointer untuk menangani file
    int ditemukan = 0;      // Flag/penanda apakah data ditemukan (0 = belum, 1 = sudah)
    
    printf("\n--- Cari Data Siswa ---\n");  // Header untuk fitur cari data
    printf("Masukkan Nomor Registrasi yang dicari: ");  // Minta input nomor registrasi
    scanf("%s", nomorCari);  // Baca nomor registrasi dari user
    
    // BUKA FILE DATABASE
    file = fopen("database_siswa.txt", "r");  // Mode "r" = read (baca)
    if (file == NULL) {  // Cek apakah file bisa dibuka
        printf("Error: File database tidak ditemukan!\n");  // Error jika file tidak ada
        return;  // Keluar dari fungsi
    }
    
    // LOOP: BACA DAN CARI DATA
    // fscanf membaca data terformat dari file
    // Format: Nama|Umur|NomorRegistrasi
    while (fscanf(file, "%[^|]|%d|%[^\n]\n", 
                  siswa.nama, &siswa.umur, siswa.nomorRegistrasi) == 3) {
        
        // BANDINGKAN NOMOR REGISTRASI
        // strcmp mengembalikan 0 jika kedua string SAMA
        if (strcmp(siswa.nomorRegistrasi, nomorCari) == 0) {
            // DATA DITEMUKAN! Tampilkan informasinya
            printf("\nData Ditemukan:\n");  // Header hasil pencarian
            printf("Nama             : %s\n", siswa.nama);              // Tampilkan nama
            printf("Umur             : %d\n", siswa.umur);              // Tampilkan umur
            printf("Nomor Registrasi : %s\n", siswa.nomorRegistrasi);  // Tampilkan nomor registrasi
            ditemukan = 1;  // Set flag jadi 1 (true)
            break;  // KELUAR DARI LOOP (tidak perlu baca file lagi, sudah ketemu)
        }
    }
    
    // TUTUP FILE
    fclose(file);  // Selalu tutup file setelah selesai dipakai
    
    // CEK HASIL PENCARIAN
    if (!ditemukan) {  // Jika ditemukan masih 0 (false), berarti data tidak ada
        printf("Data siswa dengan nomor registrasi %s tidak ditemukan!\n", nomorCari);
    }
}

// Fungsi 4: Menampilkan semua data siswa dalam format tabel
void tampilkanSemuaSiswa() {
    struct Siswa siswa;  // Variabel untuk membaca data siswa dari file
    FILE *file;          // Pointer untuk menangani file
    int counter = 0;     // Counter untuk menghitung jumlah siswa
    
    printf("\n--- Daftar Semua Siswa ---\n");  // Header untuk fitur tampilkan semua data
    
    // BUKA FILE DATABASE
    file = fopen("database_siswa.txt", "r");  // Mode "r" = read (baca)
    if (file == NULL) {  // Cek apakah file bisa dibuka
        printf("Database masih kosong atau file tidak ditemukan!\n");  // Pesan jika file tidak ada
        return;  // Keluar dari fungsi
    }
    
    // TAMPILKAN HEADER TABEL
    // %-5s = string dengan lebar 5 karakter, aligned kiri (-)
    // %-30s = string dengan lebar 30 karakter, aligned kiri
    // %-10s = string dengan lebar 10 karakter, aligned kiri
    // %-20s = string dengan lebar 20 karakter, aligned kiri
    printf("%-5s %-30s %-10s %-20s\n", "No", "Nama", "Umur", "Nomor Registrasi");
    printf("==================================================================\n");  // Garis pemisah
    
    // LOOP: BACA DAN TAMPILKAN SETIAP DATA SISWA
    // fscanf membaca data dari file dengan format: Nama|Umur|NomorRegistrasi
    while (fscanf(file, "%[^|]|%d|%[^\n]\n", 
                  siswa.nama, &siswa.umur, siswa.nomorRegistrasi) == 3) {
        counter++;  // Increment counter (tambah 1) setiap kali berhasil baca data
        // Tampilkan data dalam format tabel yang rapi
        printf("%-5d %-30s %-10d %-20s\n", 
               counter,                    // Nomor urut
               siswa.nama,                 // Nama siswa
               siswa.umur,                 // Umur siswa
               siswa.nomorRegistrasi);     // Nomor registrasi siswa
    }
    
    // TUTUP FILE
    fclose(file);  // Selalu tutup file setelah selesai
    
    // TAMPILKAN RINGKASAN
    if (counter == 0) {  // Jika counter masih 0, berarti tidak ada data yang dibaca
        printf("Database masih kosong!\n");  // Pesan database kosong
    } else {  // Jika ada data
        printf("\nTotal: %d siswa\n", counter);  // Tampilkan total jumlah siswa
    }
}

// Fungsi utama program (main function)
// Program dimulai dari sini
int main() {
    int pilihan;  // Variabel untuk menyimpan pilihan menu dari user (1-5)
    
    // TAMPILKAN PESAN SELAMAT DATANG
    printf("Selamat Datang di Sistem Manajemen Database Siswa\n");
    printf("Program ini menggunakan file 'database_siswa.txt' untuk penyimpanan data\n");
    
    // LOOP MENU UTAMA
    // while (1) = loop tak terbatas (infinite loop)
    // Program akan terus jalan sampai user pilih keluar (pilihan 5)
    while (1) {
        tampilkanMenu();  // Panggil fungsi untuk menampilkan menu pilihan
        scanf("%d", &pilihan);  // Baca pilihan user (1-5) dan simpan ke variabel pilihan
        
        // PROSES PILIHAN USER DENGAN SWITCH-CASE
        // switch adalah alternatif dari if-else yang lebih rapi untuk banyak pilihan
        switch (pilihan) {
            case 1:  // Jika user pilih 1
                tambahSiswa();  // Panggil fungsi tambahSiswa()
                break;  // PENTING! break agar tidak lanjut ke case berikutnya
                
            case 2:  // Jika user pilih 2
                hapusSiswa();   // Panggil fungsi hapusSiswa()
                break;  // Keluar dari switch setelah selesai
                
            case 3:  // Jika user pilih 3
                cariSiswa();    // Panggil fungsi cariSiswa()
                break;  // Keluar dari switch setelah selesai
                
            case 4:  // Jika user pilih 4
                tampilkanSemuaSiswa();  // Panggil fungsi tampilkanSemuaSiswa()
                break;  // Keluar dari switch setelah selesai
                
            case 5:  // Jika user pilih 5 (keluar)
                printf("\nTerima kasih telah menggunakan program ini!\n");  // Pesan perpisahan
                return 0;  // KELUAR DARI PROGRAM (return 0 = program berakhir dengan sukses)
                
            default:  // Jika pilihan bukan 1-5 (pilihan tidak valid)
                printf("\nPilihan tidak valid! Silakan pilih 1-5.\n");  // Pesan error
        }
        // Setelah selesai proses satu pilihan, loop kembali ke awal (tampil menu lagi)
    }
    
    return 0;  // Return 0 menandakan program selesai dengan normal (tidak ada error)
                // Baris ini sebenarnya tidak akan pernah dijalankan karena ada return 0 di case 5
}
