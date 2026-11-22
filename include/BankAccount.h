#pragma once

#include <string>

class BankAccount {
public:
    BankAccount() = default;
    BankAccount(int accountNumber, std::string customerName, long long balanceCents);

    int getAccountNumber() const;
    const std::string& getCustomerName() const;
    long long getBalanceCents() const;

    bool deposit(long long amountCents);
    bool withdraw(long long amountCents);

private:
    int accountNumber_ = 0;
    std::string customerName_;
    long long balanceCents_ = 0;
};
