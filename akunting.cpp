
#include <iostream>
#include <fstream>
#include <cctype>

#include "akun_model.cpp" //mengambil class akun di sub file 

using namespace std;

#if _WIN32
	char cmdclear[4] = "cls";
#elif _WIN64
	char cmdclear[4] = "cls";
#else
	char cmdclear[6] = "clear";
#endif

//utnuk penyimpanan simple data saja
char file[13] = "akunting.dat";

// *******************************************
// Fungsi deklarasi
// *******************************************
void akun_buat();
void akun_show_sp(int);	// specific record
void akun_ubah(int);
void akun_hapus(int);
void akun_show_all();
void tambah_kurang(int, int);
void intro();


// ***************************************************************
// Fungsi membuat akun baru
// ****************************************************************
void akun_buat() {
	// deklarasi account
	account ac;
	//output to file, binary, append
	ofstream outFile;
	outFile.open(file, ios::binary | ios::app);
	// memanggil fungsi buat_akun();
	ac.buat_akun();
	// Menulis ke File
	// Disini menggunakan cast ke pointer class ac yg telah dibuat, 
	// dengan menulis expektasi char
	outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
	// tutup file karena sudah selesai prosesnya
	outFile.close();
}
// ***************************************************************
// Fungsi membaca dan menampilkan spesifik data
// ****************************************************************
void akun_show_sp(int n) {
	account ac;
	bool exist_flag = false;	// penanda akun ada, defualt 0 / false
	//menggunakan ifstream dengan tujuan hanya open as read only saja
	ifstream inFile;
	//open file sebagai binari mode, dam 
	inFile.open(file, ios::binary);
	
	// memastikan file ada
	if (!inFile) {
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}
	cout << "\nDETAIL NOMINAL\n";

	while (inFile.read(reinterpret_cast<char *> (&ac), sizeof(account))) {
		if (ac.getAcNo() == n) {
			ac.show_akun();
			exist_flag = true;
		}
	}
	inFile.close();
	if (!exist_flag) {	//exist_flag false
		cout << "\n\nNomor Akun tidak ada";
		cout << "\nTekan Sembarang Tombol...";
	}
}

void akun_ubah(int n) {
	account ac;
	bool found_flag = false;
	fstream kFile;
	kFile.open(file, ios::binary | ios::in | ios::out);
	if (!kFile) {
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	while (!kFile.eof() && !found_flag) {
		// baca file konten
		kFile.read(reinterpret_cast<char *>(&ac), sizeof(account));
		if (ac.getAcNo() == n) {
			ac.buat_akun();
			cout << "\n\nIsi detail Akun baru" << endl;
			ac.ubah();
			// pointer pada awal akun
			int pos = (-1) * static_cast<int>(sizeof(account));
			// ke line sebelum entry data
			kFile.seekp(pos, ios::cur);
			// menulis / replace data lama
			kFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
			
			cout << "\n\n\t Data diperbarui...";
			cout << "\nTekan Sembarang Tombol...";
			found_flag = true;
		}
	}

	kFile.close();
	if (!found_flag) {
		cout << "\n\nData tidak ada...";
		cout << "\nTekan Sembarang Tombol...";
	}
}

void akun_hapus(int n) {
	bool found;
	//account declare
	account ac;
	//buka 2 file, satu sebagai input, satu sebagai output
	ifstream inFile;
	inFile.open(file, ios::binary);
	ofstream outFile;
	outFile.open("temp.dat", ios::binary | ios::out);

	if (!inFile) {
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	//baca file line by line mulai dari awal
	inFile.seekg(0, ios::beg);
	while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
		// akun No tidak sesuai
		if (ac.getAcNo() != n) {
			outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
			found = true;
		}
	}

	//tutup semua file
	inFile.close();
	outFile.close();
	//hapus file
	remove(file);
	//rename temp ke original (data)
	rename("temp.dat", file);
	cout << "\n\n\tData dihapus ...";
	cout << "\nTekan Sembarang Tombol...";
}

void akun_show_all() {
	account ac;
	ifstream inFile;
	inFile.open(file, ios::binary);

	if (!inFile) {
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	cout << "\n\n\tDAFTAR AKUN\n\n";
	cout << "=========================================================\n";
	cout << "A/c No.      NAMA                     TIPE        NOMINAL\n";
	cout << "=========================================================\n";
	while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
		ac.report();
	}
	inFile.close();
	cout << "\n\n\nTekan Sembarang Tombol...";
}

void tambah_kurang(int n, int option) {
	int amt;
	bool found = false;
	account ac;
	fstream File;
	File.open(file, ios::binary | ios::out | ios::in);

	if (!File) {
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	while (!File.eof() && !found) {
		File.read(reinterpret_cast<char *>(&ac), sizeof(account));
		if (ac.getAcNo() == n) {
			ac.show_akun();
			if (option == 1) {
				if (ac.getAcType() == 'H') {
					cout << "\n\n\tTAMBAH HUTANG ";
					cout << "\n\nIsi jumlah Hutang baru : ";
				} else {
					cout << "\n\n\tTAMBAH PIUTANG ";
					cout << "\n\nIsi jumlah Piutang baru : ";
				}
				
				cin >> amt;
				ac.tambah(amt);
			}

			if (option == 2) {
				if (ac.getAcType() == 'H') {
					cout << "\n\n\tBAYAR HUTANG ";
					cout << "\n\nIsi Jumlah Bayar Hutang : ";
				} else {
					cout << "\n\n\tBAYAR HUTANG ";
					cout << "\n\nIsi Jumlah Bayar Piutang : ";
				}

				cin >> amt;
				int bal = ac.getBal() - amt;
				if (bal < 0)
					if (ac.getAcType() == 'H') {
						cout << "Hutang Lunas";
					} else {
						cout << "Piutang Lunas";
					}
				else
					ac.bayar(amt);
			}
			// ubah akun di file
			int pos = (-1)*static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(account));
			cout << "\n\n\t Data diperbarui...";
			cout << "\nTekan Sembarang Tombol...";
			found = true;
		}
	}
	File.close();
	if (!found) {
		cout << "\n\n Data tidak ada...";
		cout << "\nTekan Sembarang Tombol...";
	}
}

void intro() {
	cout << "\n\t   SELAMAT DATANG";
	cout << "\n\t PERSONAL AKUNTING";
	cout << "\n\n\t by Kelompok 5 (TI):";
	cout << "\n\t - Arif Kurniawan (1002220036)";
	cout << "\n\t - Danny Ismarianto Ruhiyat (1002220014)";

	cout << "\n\nPress enter to continue...";

	cin.get();
}


int main() {
	char ch;
	int num;
	intro();

	do {
		system(cmdclear);
		cout << "\n\n\n\tMAIN MENU";
		cout << "\n\n\t01. BUAT AKUN";
		cout << "\n\n\t02. CATAT HUTANG/PIUTANG";
		cout << "\n\n\t03. BAYAR HUTANG/PIUTANG";
		cout << "\n\n\t04. SISA HUTANG/PIUTANG";
		cout << "\n\n\t05. LIST AKUN";
		cout << "\n\n\t06. TUTUP AKUN";
		cout << "\n\n\t07. UBAH / MODIF AKUN";
		cout << "\n\n\t08. KELUAR";
		cout << "\n\n\tPilih Aksi (1-8) : ";
		cin >> ch;

		system(cmdclear);

		switch (ch) {
			case '1':
				akun_buat();
				break;
			case '2':
				cout << "\n\n\tIsi nomor Akun : "; cin >> num;
				tambah_kurang(num, 1);
				break;
			case '3':
				cout << "\n\n\tIsi nomor Akun : "; cin >> num;
				tambah_kurang(num, 2);
				break;
			case '4':
				cout << "\n\n\tIsi nomor Akun : "; cin >> num;
				akun_show_sp(num);
				break;
			case '5':
				akun_show_all();
				break;
			case '6':
				cout << "\n\n\tIsi nomor Akun : "; cin >> num;
				akun_hapus(num);
				break;
			case '7':
				cout << "\n\n\tIsi nomor Akun : "; cin >> num;
				akun_ubah(num);
				break;
			case '8':
				cout << "\n\n\tTerima Kasih.";
				cout << "";
				break;
			default:cout << "\a";
		}

		cin.ignore();
		cin.get(); //sama dengan getline tapi tidak dengan baris baru, dan selalu queu
	} while (ch != '8');
	return 0;
}
