#pragma once

#include "BankAccount.h"
#include <string>
#include <vector>

class Bank {
public:
    explicit Bank(std::string dataFile);

    bool load();
    bool save() const;
    int createAccount(const std::string& customerName, long long initialDepositCents);
    bool deposit(int accountNumber, long long amountCents);
    bool withdraw(int accountNumber, long long amountCents);
    const BankAccount* findAccount(int accountNumber) const;
    const std::vector<BankAccount>& getAccounts() const;

private:
    std::string dataFile_;
    std::vector<BankAccount> accounts_;
    int nextAccountNumber_ = 100001;
    BankAccount* findAccountMutable(int accountNumber);
};
