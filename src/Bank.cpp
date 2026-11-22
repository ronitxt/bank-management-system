#include "Bank.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

Bank::Bank(std::string dataFile) : dataFile_(std::move(dataFile)) {}

bool Bank::load() {
    accounts_.clear();
    nextAccountNumber_ = 100001;

    std::ifstream input(dataFile_);
    if (!input) {
        // A missing data file is normal on the first run.
        return true;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        std::istringstream row(line);
        std::string numberText, name, balanceText;
        if (!std::getline(row, numberText, '|') ||
            !std::getline(row, name, '|') ||
            !std::getline(row, balanceText)) {
            continue;
        }

        try {
            const int number = std::stoi(numberText);
            const long long balance = std::stoll(balanceText);
            if (number <= 0 || name.empty() || balance < 0) continue;
            accounts_.emplace_back(number, name, balance);
            if (number >= nextAccountNumber_) nextAccountNumber_ = number + 1;
        } catch (...) {
            // Ignore malformed records rather than preventing the app from starting.
        }
    }
    return !input.bad();
}

bool Bank::save() const {
    const std::filesystem::path path(dataFile_);
    if (path.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error) return false;
    }

    const std::string temporaryFile = dataFile_ + ".tmp";
    {
        std::ofstream output(temporaryFile, std::ios::trunc);
        if (!output) return false;
        for (const auto& account : accounts_) {
            output << account.getAccountNumber() << '|'
                   << account.getCustomerName() << '|'
                   << account.getBalanceCents() << '\n';
        }
        output.flush();
        if (!output) return false;
    }

    std::error_code error;
    std::filesystem::rename(temporaryFile, dataFile_, error);
    if (error) {
        // Windows may not replace an existing file with rename; retain a fallback.
        std::filesystem::remove(dataFile_, error);
        error.clear();
        std::filesystem::rename(temporaryFile, dataFile_, error);
    }
    return !error;
}

BankAccount* Bank::findAccountMutable(int accountNumber) {
    for (auto& account : accounts_) {
        if (account.getAccountNumber() == accountNumber) return &account;
    }
    return nullptr;
}

int Bank::createAccount(const std::string& customerName, long long initialDepositCents) {
    if (customerName.empty() || initialDepositCents < 0) return -1;
    const int number = nextAccountNumber_++;
    accounts_.emplace_back(number, customerName, initialDepositCents);
    if (!save()) {
        accounts_.pop_back();
        --nextAccountNumber_;
        return -1;
    }
    return number;
}

bool Bank::deposit(int accountNumber, long long amountCents) {
    auto* account = findAccountMutable(accountNumber);
    if (!account || !account->deposit(amountCents)) return false;
    if (!save()) {
        account->withdraw(amountCents);
        return false;
    }
    return true;
}

bool Bank::withdraw(int accountNumber, long long amountCents) {
    auto* account = findAccountMutable(accountNumber);
    if (!account || !account->withdraw(amountCents)) return false;
    if (!save()) {
        account->deposit(amountCents);
        return false;
    }
    return true;
}

const BankAccount* Bank::findAccount(int accountNumber) const {
    for (const auto& account : accounts_) {
        if (account.getAccountNumber() == accountNumber) return &account;
    }
    return nullptr;
}

const std::vector<BankAccount>& Bank::getAccounts() const { return accounts_; }
