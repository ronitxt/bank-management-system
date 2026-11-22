#include "Bank.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {
bool parseMoney(const std::string& text, long long& cents) {
    std::istringstream input(text);
    input >> std::ws;
    long long whole = 0;
    char dot = 0;
    if (!(input >> whole) || whole < 0) return false;
    long long fraction = 0;
    if (input >> dot) {
        if (dot != '.') return false;
        std::string digits;
        if (!(input >> digits) || digits.empty() || digits.size() > 2) return false;
        for (char c : digits) {
            if (c < '0' || c > '9') return false;
        }
        fraction = (digits.size() == 1) ? (digits[0] - '0') * 10 : (digits[0] - '0') * 10 + (digits[1] - '0');
    }
    input >> std::ws;
    if (!input.eof() || whole > (9000000000000000LL - fraction) / 100) return false;
    cents = whole * 100 + fraction;
    return true;
}

void printMoney(long long cents) {
    std::cout << "Rs. " << cents / 100 << '.' << std::setw(2) << std::setfill('0')
              << cents % 100 << std::setfill(' ');
}

int readAccountNumber() {
    int number;
    std::cout << "Enter account number: ";
    if (!(std::cin >> number)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return number;
}
}

int main() {
    Bank bank("data/accounts.txt");
    if (!bank.load()) {
        std::cerr << "Could not read account data. Check data/accounts.txt permissions.\n";
        return 1;
    }

    while (true) {
        std::cout << "\n=====================================\n"
                  << "       BANK MANAGEMENT SYSTEM\n"
                  << "=====================================\n"
                  << "1. Create new account\n"
                  << "2. Deposit money\n"
                  << "3. Withdraw money\n"
                  << "4. Check account balance\n"
                  << "5. Display account details\n"
                  << "6. Display all accounts\n"
                  << "7. Exit\n"
                  << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid menu number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string name, moneyText;
            std::cout << "Customer name: ";
            std::getline(std::cin, name);
            if (name.empty() || name.find('|') != std::string::npos) {
                std::cout << "Name cannot be empty or contain the | character.\n";
                continue;
            }
            std::cout << "Initial deposit (e.g. 500 or 500.50): ";
            std::getline(std::cin, moneyText);
            long long amount = 0;
            if (!parseMoney(moneyText, amount)) {
                std::cout << "Invalid amount. Use up to two decimal places.\n";
                continue;
            }
            const int number = bank.createAccount(name, amount);
            if (number < 0) std::cout << "Account could not be saved.\n";
            else std::cout << "Account created successfully. Account number: " << number << "\n";
        } else if (choice == 2 || choice == 3) {
            const int number = readAccountNumber();
            std::string moneyText;
            std::cout << "Enter amount: ";
            std::getline(std::cin, moneyText);
            long long amount = 0;
            if (number < 0 || !parseMoney(moneyText, amount) || amount == 0) {
                std::cout << "Invalid account number or amount.\n";
                continue;
            }
            const bool ok = (choice == 2) ? bank.deposit(number, amount) : bank.withdraw(number, amount);
            if (ok) std::cout << (choice == 2 ? "Deposit" : "Withdrawal") << " successful.\n";
            else std::cout << "Transaction failed. Check the account, amount and available balance.\n";
        } else if (choice == 4 || choice == 5) {
            const int number = readAccountNumber();
            const auto* account = bank.findAccount(number);
            if (!account) {
                std::cout << "Account not found.\n";
            } else {
                std::cout << "Account number: " << account->getAccountNumber() << "\n"
                          << "Customer name: " << account->getCustomerName() << "\n"
                          << "Balance: ";
                printMoney(account->getBalanceCents());
                std::cout << "\n";
            }
        } else if (choice == 6) {
            const auto& accounts = bank.getAccounts();
            if (accounts.empty()) std::cout << "No accounts found.\n";
            for (const auto& account : accounts) {
                std::cout << account.getAccountNumber() << " | " << account.getCustomerName() << " | ";
                printMoney(account.getBalanceCents());
                std::cout << "\n";
            }
        } else if (choice == 7) {
            std::cout << "Thank you for using the Bank Management System.\n";
            break;
        } else {
            std::cout << "Invalid choice. Please select 1 to 7.\n";
        }
    }
    return 0;
}
