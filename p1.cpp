#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    int accNum;
    string name;
    double bal;

public:
    BankAccount(int a, string n, double b) {
        accNum = a;
        name = n;
        bal = b;
    }

    virtual ~BankAccount() {}

    int getAccNum() { return accNum; }
    string getName() { return name; }
    double getBalance() { return bal; }

    void deposit(double amt) {
        if (amt > 0) {
            bal += amt;
            cout << "Deposited: " << amt << " | New Balance: " << bal << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    virtual bool withdraw(double amt) {
        if (amt > 0 && amt <= bal) {
            bal -= amt;
            cout << "Withdrawn: " << amt << " | Remaining Balance: " << bal << endl;
            return true;
        } else {
            cout << "Insufficient balance or invalid amount!" << endl;
            return false;
        }
    }

    virtual void calculateInterest() {
        cout << "No interest for basic account." << endl;
    }

    virtual void displayAccountInfo() {
        cout << "Acc No: " << accNum << "  Holder: " << name << " Balance: " << bal;
    }
};

class SavingsAccount : public BankAccount {
private:
    double rate;

public:
    SavingsAccount(int a, string n, double b, double r) : BankAccount(a, n, b) {
        rate = r;
    }

    double getRate() { return rate; }

    void calculateInterest() override {
        double interest = getBalance() * (rate / 100);
        cout << "Savings Interest (at " << rate << "%): " << interest << endl;
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << "Savings  Rate: " << rate << "%" << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double limit;

public:
    CheckingAccount(int a, string n, double b, double l) : BankAccount(a, n, b) {
        limit = l;
    }

    double getLimit() { return limit; }

    bool checkOverdraft(double amt) {
        return amt <= (getBalance() + limit);
    }

    bool withdraw(double amt) override {
        if (amt > 0 && checkOverdraft(amt)) {
            double currentBal = getBalance();
            if (amt <= currentBal) {
                BankAccount::withdraw(amt);
            } else {
                double over = amt - currentBal;
                BankAccount::withdraw(currentBal);
                cout << "Overdraft used: " << over << " (Limit remaining: " << (limit - over) << ")" << endl;
            }
            return true;
        } else {
            cout << "Withdrawal exceeds overdraft limit!" << endl;
            return false;
        }
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << "  Checking  Overdraft Limit: " << limit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int term;
    double rate;

public:
    FixedDepositAccount(int a, string n, double b, int t, double r) : BankAccount(a, n, b) {
        term = t;
        rate = r;
    }

    int getTerm() { return term; }

    void calculateInterest() override {
        double interest = getBalance() * (rate / 100) * (term / 12.0);
        cout << "Fixed Deposit Interest (" << term << " months at " << rate << "%): " << interest << endl;
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << " Fixed Deposit Term: " << term << " months  Rate: " << rate << "%" << endl;
    }
};

int main() {
    BankAccount* list[50];
    int count = 0;
    int ch;

    do {
        cout << "\n=== BANKING SYSTEM ===" << endl;
        cout << "1. Create Savings Account" << endl;
        cout << "2. Create Checking Account" << endl;
        cout << "3. Create Fixed Deposit Account" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Withdraw" << endl;
        cout << "6. Calculate Interest (Polymorphism)" << endl;
        cout << "7. View All Accounts" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter choice: ";
        cin >> ch;

        if (ch >= 1 && ch <= 3) {
            int a;
            string n;
            double b;

            cout << "Enter Account Number: "; cin >> a;
            cout << "Enter Holder Name: "; cin.ignore(); getline(cin, n);
            cout << "Enter Initial Balance: "; cin >> b;

            if (ch == 1) {
                double r;
                cout << "Enter Interest Rate (%): "; cin >> r;
                list[count++] = new SavingsAccount(a, n, b, r);
            } else if (ch == 2) {
                double l;
                cout << "Enter Overdraft Limit: "; cin >> l;
                list[count++] = new CheckingAccount(a, n, b, l);
            } else if (ch == 3) {
                int t;
                double r;
                cout << "Enter Duration (months): "; cin >> t;
                cout << "Enter Interest Rate (%): "; cin >> r;
                list[count++] = new FixedDepositAccount(a, n, b, t, r);
            }
            cout << "Account created successfully!" << endl;
        } else if (ch == 4 || ch == 5) {
            int a;
            double amt;
            cout << "Enter Account Number: "; cin >> a;
            
            int found = -1;
            for (int i = 0; i < count; i++) {
                if (list[i]->getAccNum() == a) {
                    found = i;
                    break;
                }
            }

            if (found != -1) {
                cout << "Enter Amount: "; cin >> amt;
                if (ch == 4) {
                    list[found]->deposit(amt);
                } else {
                    list[found]->withdraw(amt);
                }
            } else {
                cout << "Account not found!" << endl;
            }
        } else if (ch == 6) {
            if (count == 0) {
                cout << "No accounts available." << endl;
            } else {
                cout << "\nCalculating Interest for All Accounts" << endl;
                for (int i = 0; i < count; i++) {
                    cout << "Account " << list[i]->getAccNum() << ": ";
                    list[i]->calculateInterest();
                }
            }
        } else if (ch == 7) {
            if (count == 0) {
                cout << "No accounts to display." << endl;
            } else {
                cout << "\n All Account Details" << endl;
                for (int i = 0; i < count; i++) {
                    list[i]->displayAccountInfo();
                }
            }
        }
    } while (ch != 8);

    for (int i = 0; i < count; i++) {
        delete list[i];
    }

    return 0;
}