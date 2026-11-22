# Bank Management System (C++)

A console-based educational bank management application built with C++17, object-oriented programming, and file handling.

## Features
- Create customer accounts with automatically assigned account numbers
- Deposit money
- Withdraw money with insufficient-balance checks
- Check balances and display account details
- Display all accounts
- Persist account records in `data/accounts.txt`
- Validate monetary input to two decimal places and store amounts as integer cents

## Project structure
```text
bank-management-system/
├── include/
│   ├── BankAccount.h
│   └── Bank.h
├── src/
│   ├── main.cpp
│   ├── BankAccount.cpp
│   └── Bank.cpp
├── data/
│   └── accounts.txt
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Build and run (GitHub Codespaces / Linux)
From the repository root:
```bash
cmake -S . -B build
cmake --build build -j2
./build/bank_management
```

If CMake or a C++ compiler is missing in your environment, install build tools on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y build-essential cmake
```

## Data format
The application writes records to `data/accounts.txt` as:
```text
accountNumber|customerName|balanceInCents
```
Balances are stored as integer cents to avoid floating-point rounding in basic money calculations.

## OOP concepts demonstrated
- **Encapsulation:** account state is private in `BankAccount`.
- **Classes and objects:** `BankAccount` represents an account and `Bank` manages accounts.
- **Constructors:** initialize account objects.
- **Member functions:** deposit, withdraw, lookup, load, and save operations.

## Educational-use notice
This is a learning project, not production banking software. It does not implement real customer authentication, encryption, audit controls, regulatory compliance, or secure multi-user transaction processing. Do not store real personal or financial information in this application.
