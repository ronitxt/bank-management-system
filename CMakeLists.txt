cmake_minimum_required(VERSION 3.16)
project(BankManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(bank_management
    src/main.cpp
    src/Bank.cpp
    src/BankAccount.cpp
)

target_include_directories(bank_management PRIVATE include)

if(MSVC)
    target_compile_options(bank_management PRIVATE /W4)
else()
    target_compile_options(bank_management PRIVATE -Wall -Wextra -Wpedantic)
endif()
