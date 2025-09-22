#include <iostream>
#include <string>
#include <vector>
using namespace std;

class IssuedBook; // forward declaration

// ================= Base Class 1: LibraryItem =================
class LibraryItem {
protected:
    int itemID;
    string title;
    string author;

public:
    LibraryItem() { itemID = 0; title = "Unknown"; author = "Unknown"; }
    LibraryItem(int id, string t, string a) { itemID = id; title = t; author = a; }

    void inputItem(const char* prompt = "Book") {
        cout << "Enter " << prompt << " ID: ";
        cin >> itemID;
        cin.ignore(1000, '\n'); // discard leftover newline
        do {
            cout << "Enter Title: ";
            getline(cin, title);
        } while (title.empty());
        do {
            cout << "Enter Author: ";
            getline(cin, author);
        } while (author.empty());
    }

    void displayItem() const {
        cout << "Item ID: " << itemID << "\nTitle: " << title << "\nAuthor: " << author << endl;
    }

    int getID() const { return itemID; }

    LibraryItem& operator--() {
        --itemID;
        return *this;
    }

    bool operator==(const LibraryItem &other) const {
        return this->itemID == other.itemID;
    }

    friend void showFriend(const IssuedBook &ib);
};

// ================= Base Class 2: Member =================
class Member {
protected:
    int memberID;
    string memberName;

public:
    Member() { memberID = 0; memberName = "Guest"; }
    Member(int id, string name) { memberID = id; memberName = name; }

    void inputMember(const char* type = "Student") {
        cout << "Enter " << type << " Member ID: ";
        cin >> memberID;
        cin.ignore(1000, '\n'); // discard leftover newline
        do {
            cout << "Enter Member Name: ";
            getline(cin, memberName);
        } while (memberName.empty());
    }

    void displayMember() const {
        cout << "Member ID: " << memberID << "\nMember Name: " << memberName << endl;
    }

    int getMemberID() const { return memberID; }
};

// ================= Derived Class (Multiple Inheritance) =================
class IssuedBook : public LibraryItem, public Member {
private:
    string dueDate;
    static int totalIssued;

public:
    IssuedBook() { LibraryItem(); Member(); dueDate = "Not Set"; totalIssued++; }

    IssuedBook(int bid, string bt, string ba, int mid, string mn, string dd)
        : LibraryItem(bid, bt, ba), Member(mid, mn), dueDate(dd) { totalIssued++; }

    void inputIssuedBook() {
        cout << "\n--- Book Details ---\n";
        inputItem();
        cout << "\n--- Member Details ---\n";
        inputMember();
        cout << "Enter Due Date (DD/MM/YYYY): ";
        getline(cin, dueDate);
    }

    void displayIssuedBook() const {
        cout << "\n--- Issued Book Record ---\n";
        displayItem();
        displayMember();
        cout << "Due Date: " << dueDate << endl;
    }

    static void showTotalIssued() {
        cout << "\nTotal books issued so far: " << totalIssued << endl;
    }

    // Function overloading
    void extendDueDate(const string &newDate) {
        dueDate = newDate;
        cout << "Due date updated to: " << dueDate << endl;
    }

    void extendDueDate(int extraDays) {
        cout << "Due date extended by " << extraDays << " days." << endl;
    }

    int getBookID() const { return getID(); }
    int getMemID() const { return getMemberID(); }

    friend void showFriend(const IssuedBook &ib);
};

int IssuedBook::totalIssued = 0;

// ================= Friend Function =================
void showFriend(const IssuedBook &ib) {
    cout << "\n(Friend Function Access) Item ID: " << ib.itemID
         << " | Member: " << ib.memberName << endl;
}

// ================= MENU SYSTEM =================
int main() {
    vector<IssuedBook> records;
    int choice;

    do {
        cout << "\n===== Library Management Menu =====\n";
        cout << "1. Issue New Book\n";
        cout << "2. Display All Records\n";
        cout << "3. Search by Book ID\n";
        cout << "4. Search by Member ID\n";
        cout << "5. Delete Record by Book ID\n";
        cout << "6. Update Due Date\n";
        cout << "7. Show Total Issued (Static)\n";
        cout << "8. Demonstrate Friend & Operator Overloading\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1: {
            IssuedBook b;
            b.inputIssuedBook();
            bool exists = false;
            for (size_t i = 0; i < records.size(); i++) {
                if (records[i].getBookID() == b.getBookID()) {
                    cout << "Book ID already exists. Cannot issue duplicate book!\n";
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                records.push_back(b);
                cout << "Book issued successfully!\n";
            }
            break;
        }

        case 2:
            if (records.empty())
                cout << "No records available.\n";
            else
                for (size_t i = 0; i < records.size(); i++)
                    records[i].displayIssuedBook();
            break;

        case 3: {
            int id;
            cout << "Enter Book ID to search: ";
            cin >> id;
            cin.ignore(1000, '\n');
            bool found = false;
            for (size_t i = 0; i < records.size(); i++) {
                if (records[i].getBookID() == id) {
                    records[i].displayIssuedBook();
                    found = true;
                }
            }
            if (!found) cout << "No record found!\n";
            break;
        }

        case 4: {
            int mid;
            cout << "Enter Member ID to search: ";
            cin >> mid;
            cin.ignore(1000, '\n');
            bool found = false;
            for (size_t i = 0; i < records.size(); i++) {
                if (records[i].getMemID() == mid) {
                    records[i].displayIssuedBook();
                    found = true;
                }
            }
            if (!found) cout << "No record found!\n";
            break;
        }

        case 5: {
            int id;
            cout << "Enter Book ID to delete: ";
            cin >> id;
            cin.ignore(1000, '\n');
            bool deleted = false;
            for (size_t i = 0; i < records.size(); i++) {
                if (records[i].getBookID() == id) {
                    records.erase(records.begin() + i);
                    cout << "Record deleted!\n";
                    deleted = true;
                    break;
                }
            }
            if (!deleted) cout << "No record found!\n";
            break;
        }

        case 6: {
            int id;
            cout << "Enter Book ID to update: ";
            cin >> id;
            cin.ignore(1000, '\n');
            bool updated = false;
            for (size_t i = 0; i < records.size(); i++) {
                if (records[i].getBookID() == id) {
                    cout << "1. Extend by Date (string)\n";
                    cout << "2. Extend by Days (int)\n";
                    int opt;
                    cin >> opt;
                    cin.ignore(1000, '\n');
                    if (opt == 1) {
                        string newDate;
                        cout << "Enter new date: ";
                        getline(cin, newDate);
                        records[i].extendDueDate(newDate);
                    } else {
                        int days;
                        cout << "Enter days to extend: ";
                        cin >> days;
                        cin.ignore(1000, '\n');
                        records[i].extendDueDate(days);
                    }
                    updated = true;
                }
            }
            if (!updated) cout << "No record found!\n";
            break;
        }

        case 7:
            IssuedBook::showTotalIssued();
            break;

        case 8:
            if (!records.empty()) {
                showFriend(records[0]);
                cout << "\nBefore Unary -- Operator: ";
                records[0].displayItem();
                --records[0];
                cout << "After Unary -- Operator: ";
                records[0].displayItem();

                if (records.size() > 1) {
                    if (records[0] == records[1])
                        cout << "Binary Overload: IDs are SAME\n";
                    else
                        cout << "Binary Overload: IDs are DIFFERENT\n";
                }
            } else {
                cout << "No records to demonstrate operators!\n";
            }
            break;

        case 0:
            cout << "Exiting... Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}

