// File: TransactionList.cpp
#include "TransactionList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

TransactionList::TransactionList() : head(nullptr) {}

std::string TransactionList::currentTime() const {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}

void TransactionList::add(int origin, const std::string& type, int dest, int amt, int balAfter) {
    Transfer tr{origin, type, dest, amt, balAfter, currentTime()};
    auto* node = new Node(tr);
    node->next = head;
    head = node;
}

bool TransactionList::popLast(Transfer& out) {
    if (!head) return false;
    Node* node = head;
    head = head->next;
    out = node->data;
    delete node;
    return true;
}

void TransactionList::markDeleted(int delId) {
    for (Node* cur = head; cur; cur = cur->next) {
        if (cur->data.originId == delId) cur->data.originId = -2;
        if (cur->data.destinationId == delId) cur->data.destinationId = -2;
    }
}

void TransactionList::load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        Transfer tr;
        std::getline(iss, line, ','); tr.originId = std::stoi(line);
        std::getline(iss, tr.type, ',');
        std::getline(iss, line, ','); tr.destinationId = std::stoi(line);
        std::getline(iss, line, ','); tr.amount = std::stoi(line);
        std::getline(iss, line, ','); tr.balanceAfter = std::stoi(line);
        std::getline(iss, tr.timestamp);
        auto* node = new Node(tr);
        node->next = head;
        head = node;
    }
}

void TransactionList::save(const std::string& file) const {
    std::ofstream out(file);
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        out << tr.originId << ','
            << tr.type << ','
            << tr.destinationId << ','
            << tr.amount << ','
            << tr.balanceAfter << ','
            << tr.timestamp << '\n';
    }
}

void TransactionList::printAllAdmin() const {
    std::cout << "\n--- All Transactions (Admin) ---\n";
    std::cout << std::left
              << std::setw(10) << "Origin"
              << std::setw(12) << "Type"
              << std::setw(12) << "Dest"
              << std::setw(10) << "Amount"
              << std::setw(12) << "Balance"
              << std::setw(20) << "Timestamp" << '\n';
    std::cout << std::string(76, '-') << '\n';
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        std::cout << std::left
                  << std::setw(10) << tr.originId
                  << std::setw(12) << tr.type
                  << std::setw(12) << tr.destinationId
                  << std::setw(10) << tr.amount
                  << std::setw(12) << tr.balanceAfter
                  << std::setw(20) << tr.timestamp << '\n';
    }
    std::cout << std::string(76, '-') << '\n';
}

void TransactionList::printUser(int userId) const {
    std::cout << "\n--- Your Transactions ---\n";
    std::cout << std::left
              << std::setw(10) << "Origin"
              << std::setw(12) << "Type"
              << std::setw(12) << "Dest"
              << std::setw(10) << "Amount"
              << std::setw(12) << "Balance"
              << std::setw(20) << "Timestamp" << '\n';
    std::cout << std::string(76, '-') << '\n';
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        if (tr.originId == userId || tr.destinationId == userId) {
            std::cout << std::left
                      << std::setw(10) << tr.originId
                      << std::setw(12) << tr.type
                      << std::setw(12) << tr.destinationId
                      << std::setw(10) << tr.amount
                      << std::setw(12) << tr.balanceAfter
                      << std::setw(20) << tr.timestamp << '\n';
        }
    }
    std::cout << std::string(76, '-') << '\n';
}
