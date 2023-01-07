
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include "pch.h"
#include "Header.h"

using namespace std;

#if _WIN32
	char cmdclear[4] = "cls";
#elif _WIN64
	char cmdclear[4] = "cls";
#else
	char cmdclear[6] = "clear";
#endif

char file[13] = "akunting.dat";

class account 
{
	int acno;	// akun nomor
	char name[50];	// akun nama
	int deposit;
	char type;

		public:
		//withdraw money (int n), subtract from blance
		void withdraw(int);
		//deposit money (int n)
		void dep(int);
		//create account
		void create_account();
		//show account details CONST
		void show_account() const;
		//modify account
		void modify();
		//report - show data in tabular format CONST
		void report() const;
		//getAccountNumber CONST
		int getAcNo() const;
		//get balanceAmount CONST
		int getBal() const;
		//getAccountType CONST
		char getAcType() const;
};

void account::create_account()
{
	// show / direct user to enter details
	cout << "\nIsi Nomor Akun :";
	cin >> acno;
	cout << "\nIsi Nama Akun :";
	cin.ignore();
	cin.getline(name, 50);	// sets size of input to 50 characters
	
	while (true)
	{
		cout << "\nIsi Tipe Akun (Hutang/Piutang) as H or P :";
		cin >> type;
		type = toupper(type);
		if (type == 'H' || type == 'P')
			break;
	}
	cout << "\nIsi Nominal Awal:";
	cin>>deposit;
	cout << "\n\nAkun Dibuat...";
}
void account::show_account() const
{
	cout << "\nAkun No. : " << acno;
	cout << "\nNama Akun : " << name;
	cout << "\nAkun Tipe : " << type;
	cout << "\nSaldo : " << deposit;
}
void account::modify()
{
	cout << "\nAkun No. :" << acno;
	cout << "\nIsi Nama Akun :";
	cin.ignore();
	cin.getline(name, 50);
	cout << "\nUbah Tipe Akun (H/P) :";
	cin >> type;
	type = toupper(type);
	cout << "\nUbah Jumlah Nominal :";
	cin >> deposit;
}
void account::withdraw(int n)
{
	deposit -= n;
}
void account::dep(int n)
{
	deposit += n;
}
void account::report() const
{
	//setw(int) is set width
	cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}
int account::getAcNo() const
{
	return acno;
}
int account::getBal() const
{
	return deposit;
}
char account::getAcType() const
{
	return type;
}

// *******************************************
// function declaration
// *******************************************
void write_account();
void display_sp(int);	// specific record
void modify_account(int);
void delete_account(int);
void display_all();
void deposit_withdraw(int, int);
void intro();

// *******************************************
// File Operation Functions
// *******************************************

//***************************************************************
//    	function to write in file
//****************************************************************
void write_account()
{
	// need account
	account ac;
	//output to file, binary, append
	ofstream outFile;
	outFile.open(file, ios::binary | ios::app);
	// call account's create_account();
	ac.create_account();
	// put into file
	// We need to cast because write expects char
	outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
	//close file
	outFile.close();
}
//***************************************************************
//    	function to read specific record from file
//****************************************************************
void display_sp(int n)
{
	// display specific record, n as acno, using show_account()
	account ac;
	bool exist_flag = false;	// account exists flag, defualt 0 or false
	ifstream inFile;
	inFile.open(file, ios::binary);
	
	// make sure file exists
	if (!inFile)
	{
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}
	cout << "\nDETAIL NOMINAL\n";

	while (inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
	{
		if (ac.getAcNo() == n)
		{
			ac.show_account();
			exist_flag = true;
		}
	}
	inFile.close();
	if (!exist_flag)	//exist_flag is false
		cout << "\n\nNomor Akun tidak ada";
}
void modify_account(int n)
{
	account ac;
	bool found_flag = false;
	fstream kFile;
	kFile.open(file, ios::binary | ios::in | ios::out);
	if (!kFile)
	{
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}
	while (!kFile.eof() && !found_flag)
	{
		// read file contents
		kFile.read(reinterpret_cast<char *>(&ac), sizeof(account));
		if (ac.getAcNo() == n)
		{
			ac.show_account();
			cout << "\n\nIsi detail Akun baru" << endl;
			ac.modify();
			// have a pointer at the start of the account
			int pos = (-1) * static_cast<int>(sizeof(account));
			// goes to the line before data entry
			kFile.seekp(pos, ios::cur);
			// writes over old data
			kFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
			
			cout << "\n\n\t Data diperbarui";
			found_flag = true;
		}
	}
	kFile.close();
	if (!found_flag)
		cout << "\n\nData tidak ada";
}
void delete_account(int n)
{
	bool found;
	//account declare
	account ac;
	//open 2 files, one as input, one as output
	ifstream inFile;
	inFile.open(file, ios::binary);
	ofstream outFile;
	outFile.open("temp.dat", ios::binary | ios::out);

	if (!inFile)
	{
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	//read file line by line starting at begining
	inFile.seekg(0, ios::beg);
	while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
	{
		// everything but the account number
		if (ac.getAcNo() != n)
		{
			outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
			found = true;
		}
	}

	//close both files
	inFile.close();
	outFile.close();
	//delete one file
	remove(file);
	//rename temp as original
	rename("temp.dat", file);
	cout << "\n\n\tData dihapus ...";
}
void display_all()
{
	account ac;
	ifstream inFile;
	inFile.open(file, ios::binary);

	if (!inFile)
	{
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	cout << "\n\n\tDAFTAR AKUN\n\n";
	cout << "====================================================\n";
	cout << "A/c no.      NAME           Type  Nominal\n";
	cout << "====================================================\n";
	while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
	{
		ac.report();
	}
	inFile.close();
}
void deposit_withdraw(int n, int option)
{
	int amt;
	bool found = false;
	account ac;
	fstream File;
	File.open(file, ios::binary | ios::out | ios::in);

	if (!File)
	{
		cout << "File tidak dapat dibuka !! Tekan sembarang...";
		return;
	}

	while (!File.eof() && !found)
	{
		File.read(reinterpret_cast<char *>(&ac), sizeof(account));
		if (ac.getAcNo() == n)
		{
			ac.show_account();
			if (option == 1)
			{
				if (ac.getAcType() == 'H') {
					cout << "\n\n\tTAMBAH HUTANG ";
					cout << "\n\nIsi jumlah Hutang baru";
				} else {
					cout << "\n\n\tTAMBAH PIUTANG ";
					cout << "\n\nIsi jumlah Piutang baru";
				}
				
				cin >> amt;
				ac.dep(amt);
			}
			if (option == 2)
			{
				if (ac.getAcType() == 'H') {
					cout << "\n\n\tBAYAR HUTANG ";
					cout << "\n\nIsi Jumlah Bayar Hutang";
				} else {
					cout << "\n\n\tBAYAR HUTANG ";
					cout << "\n\nIsi Jumlah Bayar Piutang";
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
					ac.withdraw(amt);
			}
			// modify account in file
			int pos = (-1)*static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(account));
			cout << "\n\n\t Data diperbarui";
			found = true;
		}
	}
	File.close();
	if (!found)
		cout << "\n\n Data tidak ada ";
}

void intro()
{
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

	do
	{
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
		cout << "\n\n\tPilih Aksi (1-8) ";
		cin >> ch;
		system(cmdclear);
		switch (ch)
		{
		case '1':
			write_account();
			break;
		case '2':
			cout << "\n\n\tIsi nomor Akun : "; cin >> num;
			deposit_withdraw(num, 1);
			break;
		case '3':
			cout << "\n\n\tIsi nomor Akun : "; cin >> num;
			deposit_withdraw(num, 2);
			break;
		case '4':
			cout << "\n\n\tIsi nomor Akun : "; cin >> num;
			display_sp(num);
			break;
		case '5':
			display_all();
			break;
		case '6':
			cout << "\n\n\tIsi nomor Akun : "; cin >> num;
			delete_account(num);
			break;
		case '7':
			cout << "\n\n\tIsi nomor Akun : "; cin >> num;
			modify_account(num);
			break;
		case '8':
			cout << "\n\n\tTerima Kasih.";
			break;
		default:cout << "\a";
		}
		cin.ignore();
		cin.get();
	} while (ch != '8');
	return 0;
}
