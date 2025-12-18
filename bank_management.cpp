#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <limits>

using namespace std;

class Bank {
private:
    int accNo;
    char name[50];
    float balance;
    char type; 

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);
        
        cout << "Enter Type (S/C): ";
        cin >> type;
        type = toupper(type);
        
        cout << "Enter Initial Balance (>=500 for Savings, >=1000 for Current): ";
        cin >> balance;
        cout << "\nAccount Created Successfully!";
    }

    void showAccount() const {
        cout << "\n" << left << setw(10) << accNo 
             << setw(20) << name 
             << setw(10) << type 
             << "₹" << balance;
    }

    void deposit(float amt) { balance += amt; }
    
    bool withdraw(float amt) {
        if (amt > balance) return false;
        balance -= amt;
        return true;
    }

    int getAccNo() const { return accNo; }
    float getBalance() const { return balance; }
};


void writeAccount() {
    Bank b;
    ofstream outFile("bank_data.dat", ios::binary | ios::app);
    b.createAccount();
    outFile.write(reinterpret_cast<char*>(&b), sizeof(Bank));
    outFile.close();
}

void displayAll() {
    Bank b;
    ifstream inFile("bank_data.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened!! Press any Key...";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n";
    cout << "====================================================\n";
    cout << left << setw(10) << "Acc No." << setw(20) << "Name" << setw(10) << "Type" << "Balance\n";
    cout << "----------------------------------------------------\n";
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Bank))) {
        b.showAccount();
    }
    inFile.close();
}

void updateBalance(int n, int opt) {
    bool found = false;
    Bank b;
    fstream file("bank_data.dat", ios::binary | ios::in | ios::out);
    
    if (!file) return;

    while (!found && file.read(reinterpret_cast<char*>(&b), sizeof(Bank))) {
        if (b.getAccNo() == n) {
            b.showAccount();
            if (opt == 1) {
                float amt;
                cout << "\n\nEnter Amount to Deposit: ";
                cin >> amt;
                b.deposit(amt);
            } else {
                float amt;
                cout << "\n\nEnter Amount to Withdraw: ";
                cin >> amt;
                if (!b.withdraw(amt)) cout << "Insufficient balance!";
            }
            int pos = (-1) * static_cast<int>(sizeof(b));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&b), sizeof(Bank));
            cout << "\n\tRecord Updated!";
            found = true;
        }
    }
    file.close();
    if (!found) cout << "\n\tRecord Not Found!";
}

void deleteAccount(int n) {
    Bank b;
    ifstream inFile("bank_data.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Bank))) {
        if (b.getAccNo() != n) {
            outFile.write(reinterpret_cast<char*>(&b), sizeof(Bank));
        }
    }
    inFile.close();
    outFile.close();
    remove("bank_data.dat");
    rename("Temp.dat", "bank_data.dat");
    cout << "\n\tRecord Deleted.";
}

int main() {
    int choice, num;
    do {
        cout << "\n\n\t--- MAIN MENU ---";
        cout << "\n\t01. NEW ACCOUNT";
        cout << "\n\t02. DEPOSIT AMOUNT";
        cout << "\n\t03. WITHDRAW AMOUNT";
        cout << "\n\t04. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t05. CLOSE AN ACCOUNT";
        cout << "\n\t06. EXIT";
        cout << "\n\tSelect Your Option (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: writeAccount(); break;
            case 2: 
                cout << "\nEnter Acc No: "; cin >> num;
                updateBalance(num, 1); break;
            case 3: 
                cout << "\nEnter Acc No: "; cin >> num;
                updateBalance(num, 2); break;
            case 4: displayAll(); break;
            case 5: 
                cout << "\nEnter Acc No: "; cin >> num;
                deleteAccount(num); break;
            case 6: cout << "\nThanks for using Bank Management System!"; break;
            default: cout << "Invalid Option!";
        }
    } while (choice != 6);
    return 0;
}
