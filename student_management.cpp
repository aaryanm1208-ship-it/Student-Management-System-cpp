#include <iostream>
#include <fstream>
using namespace std;

class Student {
private:
    int roll;
    char name[50];
    float marks;

public:
    void input() {
        cout << "\nEnter Roll Number: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        cin.getline(name, 50);
        cout << "Enter Marks: ";
        cin >> marks;
    }

    void display() {
        cout << "\nRoll No: " << roll;
        cout << "\nName: " << name;
        cout << "\nMarks: " << marks << endl;
    }

    int getRoll() {
        return roll;
    }
};

void addStudent() {
    Student s;
    ofstream out("students.dat", ios::binary | ios::app);
    s.input();
    out.write((char*)&s, sizeof(s));
    out.close();
    cout << "\nStudent Record Added Successfully!\n";
}

void displayAll() {
    Student s;
    ifstream in("students.dat", ios::binary);
    cout << "\n--- Student Records ---\n";
    while (in.read((char*)&s, sizeof(s))) {
        s.display();
        cout << "----------------------";
    }
    in.close();
}

void searchStudent() {
    int r;
    Student s;
    bool found = false;
    ifstream in("students.dat", ios::binary);
    cout << "\nEnter Roll Number to Search: ";
    cin >> r;

    while (in.read((char*)&s, sizeof(s))) {
        if (s.getRoll() == r) {
            s.display();
            found = true;
            break;
        }
    }
    in.close();

    if (!found)
        cout << "\nStudent Not Found!\n";
}

void deleteStudent() {
    int r;
    Student s;
    ifstream in("students.dat", ios::binary);
    ofstream out("temp.dat", ios::binary);
    cout << "\nEnter Roll Number to Delete: ";
    cin >> r;

    bool found = false;

    while (in.read((char*)&s, sizeof(s))) {
        if (s.getRoll() != r) {
            out.write((char*)&s, sizeof(s));
        } else {
            found = true;
        }
    }

    in.close();
    out.close();
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        cout << "\nStudent Record Deleted Successfully!\n";
    else
        cout << "\nStudent Not Found!\n";
}

int main() {
    int choice;
    do {
        cout << "\n====== STUDENT MANAGEMENT SYSTEM ======";
        cout << "\n1. Add Student";
        cout << "\n2. Display All Students";
        cout << "\n3. Search Student";
        cout << "\n4. Delete Student";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            displayAll();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            deleteStudent();
            break;
        case 5:
            cout << "\nExiting Program...\n";
            break;
        default:
            cout << "\nInvalid Choice!\n";
        }
    } while (choice != 5);

    return 0;
}
