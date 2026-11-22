#include "BankAccount.h"

#include <utility>

BankAccount::BankAccount(int accountNumber, std::string customerName, long long balanceCents)
    : accountNumber_(accountNumber),
      customerName_(std::move(customerName)),
      balanceCents_(balanceCents) {}

int BankAccount::getAccountNumber() const { return accountNumber_; }
const std::string& BankAccount::getCustomerName() const { return customerName_; }
long long BankAccount::getBalanceCents() const { return balanceCents_; }

bool BankAccount::deposit(long long amountCents) {
    if (amountCents <= 0 || balanceCents_ > 9000000000000000LL - amountCents) return false;
    balanceCents_ += amountCents;
    return true;
}

bool BankAccount::withdraw(long long amountCents) {
    if (amountCents <= 0 || amountCents > balanceCents_) return false;
    balanceCents_ -= amountCents;
    return true;
}
