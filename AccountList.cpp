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

//this part should be using other algorithm to make it O(nlogn)
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
        BankAccount acct;
        std::getline(iss, line, ','); acct.id = std::stoi(line);
        std::getline(iss, acct.name, ',');
        std::getline(iss, line, ','); acct.balance = std::stoi(line);
        std::getline(iss, line, ','); acct.setPin(std::stoi(line));
        std::getline(iss, line); acct.setLocked(line == "1");
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
            << (a.isLocked() ? '1' : '0') << '\n';
    }
}

void AccountList::printAllAdmin() const {
    std::cout << "\n--- All Accounts (Admin) ---\n";
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(25) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& a = cur->account;
        std::cout << std::left
                  << std::setw(10) << a.id
                  << std::setw(25) << a.name
                  << std::setw(10) << a.balance
                  << std::setw(10) << (a.isLocked() ? "Locked" : "Unlocked")
                  << std::endl;
    }
    std::cout << std::string(55, '-') << std::endl;
}

void AccountList::printUserNode(const BankAccount& a) const {
    std::cout << "--- Your Account ---";
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(25) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::setw(6)  << "PIN" << std::endl;
    std::cout << std::string(61, '-') << std::endl;
    std::cout << std::left
              << std::setw(10) << a.id
              << std::setw(25) << a.name
              << std::setw(10) << a.balance
              << std::setw(10) << (a.isLocked() ? "Locked" : "Unlocked")
              << std::setw(6)  << a.getPin() << std::endl;
    std::cout << std::string(61, '-') << std::endl;
}
