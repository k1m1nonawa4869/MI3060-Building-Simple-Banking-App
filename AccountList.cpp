// File: AccountList.cpp
#include "AccountList.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

AccountList::AccountList() {
    for (int i = 0; i < TABLE_SIZE; ++i) table[i] = nullptr;
}

AccountList::~AccountList() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* cur = table[i];
        while (cur) {
            HashNode* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }
}

void AccountList::addAccount(const BankAccount& acct) {
    int idx = hash(acct.id);
    HashNode* node = new HashNode(acct);
    node->next = table[idx];
    table[idx] = node;
    std::cout << "Account " << acct.id
              << " ('" << acct.name << "') added." << std::endl;
}

bool AccountList::deleteById(int id) {
    int idx = hash(id);
    HashNode* cur = table[idx];
    HashNode* prev = nullptr;
    while (cur) {
        if (cur->account.id == id) {
            if (prev) prev->next = cur->next;
            else table[idx] = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

BankAccount* AccountList::findById(int id) const {
    int idx = hash(id);
    for (HashNode* cur = table[idx]; cur; cur = cur->next) {
        if (cur->account.id == id) return &cur->account;
    }
    return nullptr;
}

void AccountList::load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return;
    // Xóa bảng cũ nếu có
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* cur = table[i];
        while (cur) {
            HashNode* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        table[i] = nullptr;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        BankAccount acct;
        std::getline(iss, line, ','); acct.id = std::stoi(line);
        std::getline(iss, acct.name, ',');
        std::getline(iss, line, ','); acct.balance = std::stoi(line);
        std::getline(iss, line, ','); acct.setPin(line);
        char flag; iss >> flag; iss.ignore(1); acct.setLocked(flag=='1');
        std::getline(iss, line, ','); // created
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, line, "");
        std::getline(iss, line); // modified
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, acct.getCreated(), line);

        addAccount(acct);
    }
}

void AccountList::save(const std::string& file) const {
    std::ofstream out(file);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (HashNode* cur = table[i]; cur; cur = cur->next) {
            const auto& a = cur->account;
            out << a.id << ','
                << a.name << ','
                << a.balance << ','
                << a.getPin() << ','
                << (a.isLocked() ? '1' : '0') << ','
                << a.getCreated() << ','
                << a.getModified() << '\n';
        }
    }
}

void AccountList::printAllAdmin() const {
    std::cout << "\n--- All Accounts (Admin) ---\n";
    std::cout << std::left
              << std::setw(4)  << "#"
              << std::setw(6)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::setw(20) << "Created"
              << std::setw(20) << "Modified"
              << std::endl;
    std::cout << std::string(86, '-') << std::endl;
    int idx = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (HashNode* cur = table[i]; cur; cur = cur->next) {
            const auto& a = cur->account;
            ++idx;
            std::cout << std::left
                      << std::setw(4)  << idx
                      << std::setw(6)  << a.id
                      << std::setw(20) << a.name
                      << std::setw(10) << a.balance
                      << std::setw(10) << (a.isLocked() ? "Locked" : "Unlocked")
                      << std::setw(20) << a.getCreated()
                      << std::setw(20) << a.getModified()
                      << std::endl;
        }
    }
    std::cout << std::string(86, '-') << std::endl;
}

void AccountList::printUserNode(const BankAccount& a) const {
    std::cout << "\n--- Your Account ---\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::setw(20) << "Created"
              << std::setw(20) << "Modified"
              << std::setw(6)  << "PIN"
              << std::endl;
    std::cout << std::string(92, '-') << std::endl;
    std::cout << std::left
              << std::setw(6)  << a.id
              << std::setw(20) << a.name
              << std::setw(10) << a.balance
              << std::setw(10) << (a.isLocked() ? "Locked" : "Unlocked")
              << std::setw(20) << a.getCreated()
              << std::setw(20) << a.getModified()
              << std::setw(6)  << a.getPin()
              << std::endl;
    std::cout << std::string(92, '-') << std::endl;
}