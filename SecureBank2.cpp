#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cctype>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    string customerName;
    double balance;
    int successfulDeposits;
    int successfulWithdrawals;
    int rejectedTransactions;
    bool created;

    static const size_t MIN_ACC_LEN = 10;
    static const size_t MAX_ACC_LEN = 12;

public:
    BankAccount()
        : balance(0.0), successfulDeposits(0), successfulWithdrawals(0),
          rejectedTransactions(0), created(false) {}

    // Account number must contain only digits, length 10 to 12
    static bool isValidAccountNumber(const string &acc) {
        if (acc.length() < MIN_ACC_LEN || acc.length() > MAX_ACC_LEN)
            return false;
        for (size_t i = 0; i < acc.length(); i++) {
            if (!isdigit(static_cast<unsigned char>(acc[i])))
                return false;
        }
        return true;
    }

    bool isCreated() const { return created; }

    bool createAccount(const string &accNo, const string &name, double openingBalance) {
        if (!isValidAccountNumber(accNo)) {
            cout << "Error: Account number must be 10 to 12 digits (numbers only).\n";
            return false;
        }
        if (name.empty()) {
            cout << "Error: Customer name cannot be empty.\n";
            return false;
        }
        if (openingBalance < 0) {
            cout << "Error: Opening balance cannot be negative.\n";
            return false;
        }
        accountNumber = accNo;
        customerName = name;
        balance = openingBalance;
        successfulDeposits = 0;
        successfulWithdrawals = 0;
        rejectedTransactions = 0;
        created = true;
        cout << "Account created successfully.\n";
        return true;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            rejectedTransactions++;
            cout << "Rejected: invalid amount. Deposit must be greater than zero.\n";
            return;
        }
        balance += amount;
        successfulDeposits++;
        cout << "Deposit successful. New balance: " << balance << "\n";
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            rejectedTransactions++;
            cout << "Rejected: invalid amount. Withdrawal must be greater than zero.\n";
            return;
        }
        if (amount > balance) {
            rejectedTransactions++;
            cout << "Rejected: insufficient balance.\n";
            return;
        }
        balance -= amount;
        successfulWithdrawals++;
        cout << "Withdrawal successful. New balance: " << balance << "\n";
    }

    void checkBalance() const {
        cout << "Current balance: " << balance << "\n";
    }

    void displayAccount() const {
        cout << "----- Account Details -----\n";
        cout << "Account Number : " << accountNumber << "\n";
        cout << "Customer Name  : " << customerName << "\n";
        cout << "Current Balance: " << balance << "\n";
    }

    void displayTransactionSummary() const {
        cout << "----- Transaction Summary -----\n";
        cout << "Successful deposits    : " << successfulDeposits << "\n";
        cout << "Successful withdrawals : " << successfulWithdrawals << "\n";
        cout << "Rejected transactions  : " << rejectedTransactions << "\n";
        cout << "Final balance          : " << balance << "\n";
    }
};

// Reads a double safely; returns false on invalid input
bool readAmount(const string &prompt, double &value) {
    cout << prompt;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

void showMenu() {
    cout << "\n===== SecureBank Account Management =====\n";
    cout << "1. Create Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Balance Enquiry\n";
    cout << "5. Display Account Details\n";
    cout << "6. Display Transaction Summary\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    BankAccount account;
    int choice = 0;

    cout << fixed << setprecision(2);

    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 7) {
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            continue;
        }

        // Block operations 2-6 until an account exists
        if (choice >= 2 && choice <= 6 && !account.isCreated()) {
            cout << "Error: Please create an account first (option 1).\n";
            continue;
        }

        switch (choice) {
        case 1: {
            if (account.isCreated()) {
                cout << "An account already exists for this session.\n";
                break;
            }
            string accNo, name;
            double opening;
            cout << "Enter account number (10 to 12 digits): ";
            getline(cin, accNo);
            cout << "Enter customer name: ";
            getline(cin, name);
            if (!readAmount("Enter opening balance: ", opening)) {
                cout << "Error: Invalid opening balance.\n";
                break;
            }
            account.createAccount(accNo, name, opening);
            break;
        }
        case 2: {
            double amount;
            if (!readAmount("Enter deposit amount: ", amount)) {
                cout << "Rejected: invalid amount.\n";
                break;
            }
            account.deposit(amount);
            break;
        }
        case 3: {
            double amount;
            if (!readAmount("Enter withdrawal amount: ", amount)) {
                cout << "Rejected: invalid amount.\n";
                break;
            }
            account.withdraw(amount);
            break;
        }
        case 4:
            account.checkBalance();
            break;
        case 5:
            account.displayAccount();
            break;
        case 6:
            account.displayTransactionSummary();
            break;
        case 7:
            cout << "Thank you for using SecureBank. Goodbye!\n";
            break;
        }
    } while (choice != 7);

    return 0;
}
