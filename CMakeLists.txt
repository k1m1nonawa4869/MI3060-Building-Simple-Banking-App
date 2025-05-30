cmake_minimum_required(VERSION 3.10)
project(BankManagement)

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    BankAccount.cpp
    AccountList.cpp
    TransactionList.cpp
    Actions.cpp
    AccList.cpp
    AccTranList.cpp
    Utils.cpp 
)

# Add executable target
add_executable(bank_management ${SOURCES})

# Include directories
target_include_directories(bank_management PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})