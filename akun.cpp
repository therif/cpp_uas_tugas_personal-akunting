#include <iostream>
#include <iomanip>

class account {
	int acno;	// akun nomor
	char name[50];	// akun nama
	int nominal; //Utk Balance / Saldo
	char type; //Untuk Tipe H/P

		public:
		//bayar, subtract from balance / saldo
		void bayar(int);
		//Tambah hutang / Piutang (int n)
		void tambah(int);
		//buat akun
		void buat_akun();
		//tampilkan akun detail, dan buat const biar tidak di modif/ubah
		void show_akun() const;
		//ubah akun
		void ubah();
		//report - show data di tabular format, const biar tidak di modif/ubah
		void report() const;
		//ambil akun id/no, const biar tidak di modif/ubah
		int getAcNo() const;
		//ambil balance / saldo, const biar tidak di modif/ubah
		int getBal() const;
		//ambil tipe akun, const biar tidak di modif/ubah
		char getAcType() const;

};

void account::buat_akun() {
	std::cout << "\nIsi Nomor Akun : ";
	std::cin >> acno;
	std::cout << "\nIsi Nama Akun : ";
	std::cin.ignore(); //untuk meng-ignore sebuah new line
	std::cin.getline(name, 50);	// sets size of input to 50 characters
	
	while (true)
	{
		std::cout << "\nIsi Tipe Akun (Hutang/Piutang) as H or P : ";
		std::cin >> type;
		type = toupper(type);
		if (type == 'H' || type == 'P')
			break;
	}
	std::cout << "\nIsi Nominal Awal : ";
	std::cin >> nominal;
	std::cout << "\n\nAkun Dibuat... || Tekan Sembarang Tombol...";
}

void account::show_akun() const {
	std::cout << "\nAkun No. : " << acno;
	std::cout << "\nNama Akun : " << name;
	std::cout << "\nAkun Tipe : " << type;
	std::cout << "\nSaldo : " << nominal;
    std::cout << "\n\nTekan Sembarang Tombol...";
}

void account::ubah() {
	std::cout << "\nAkun No. : " << acno;
	std::cout << "\nIsi Nama Akun : ";
	std::cin.ignore();
	std::cin.getline(name, 50);
	std::cout << "\nUbah Tipe Akun (H/P) : ";
	std::cin >> type;
	type = toupper(type);
	std::cout << "\nUbah Jumlah Nominal : ";
	std::cin >> nominal;
}

void account::bayar(int n) {
	nominal -= n;
}

void account::tambah(int n) {
	nominal += n;
}

void account::report() const {
	// int lengthAcNo = std::to_string(acno).length(); 
	// int lengthAcNo = (acno==0)?1:log10(acno)+1;  

	int lengthAcName = sizeof(name)/sizeof(char);

	//setw(int) untuk manipulator set width
	std::cout << acno << std::setw(8) << " " << name << std::setw(50-lengthAcName) << " " << type << std::setw(6) << nominal << std::endl;
}

int account::getAcNo() const {
	return acno;
}

int account::getBal() const {
	return nominal;
}

char account::getAcType() const {
	return type;
}
