// File: AccountList.cpp
#include "AccountList.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

AccountList::AccountList()
    : head(nullptr)
{}

void AccountList::addAccount(const BankAccount& acct) {
    auto* node = new Node(acct);
    node->next = head;
    head = node;
    std::cout << "Account " << acct.id
              << " ('" << acct.name << "') added." << std::endl;
}

bool AccountList::deleteById(int id) {
    Node* cur = head;
    Node* prev = nullptr;
    while (cur) {
        if (cur->account.id == id) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

BankAccount* AccountList::findById(int id) const {
    for (Node* cur = head; cur; cur = cur->next) {
        if (cur->account.id == id) return &cur->account;
    }
    return nullptr;
}

void AccountList::load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int id, balance;
        std::string name, pin, created, modified;
        char flag;
        // CSV: id,name,balance,pin,locked,created,modified
        std::getline(iss, line, ','); id = std::stoi(line);
        std::getline(iss, name, ',');
        std::getline(iss, line, ','); balance = std::stoi(line);
        std::getline(iss, pin, ',');
        iss >> flag; iss.ignore(1); bool locked = (flag == '1');
        std::getline(iss, created, ',');
        std::getline(iss, modified);

        BankAccount acct(id, name, pin, balance, created, modified);
        acct.setLocked(locked);
        addAccount(acct);
    }
}

void AccountList::save(const std::string& file) const {
    std::ofstream out(file);
    for (Node* cur = head; cur; cur = cur->next) {
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

void AccountList::printAllAdmin() const {
    std::cout << "\n--- All Accounts (Admin) ---\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::setw(20) << "Created"
              << std::setw(20) << "Modified"
              << std::endl;
    std::cout << std::string(86, '-') << std::endl;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& a = cur->account;
        std::cout << std::left
                  << std::setw(6)  << a.id
                  << std::setw(20) << a.name
                  << std::setw(10) << a.balance
                  << std::setw(10) << (a.isLocked() ? "Locked" : "Unlocked")
                  << std::setw(20) << a.getCreated()
                  << std::setw(20) << a.getModified()
                  << std::endl;
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
