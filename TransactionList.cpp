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

void TransactionList::add(int origin, const std::string& type, int dest, int amt, int balAfter, int destBalAfter) {
    Transfer tr{origin, type, dest, amt, balAfter, destBalAfter, currentTime()};
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

// Load so that the first line in the file (newest transaction) becomes head
void TransactionList::load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return;
    head = nullptr;
    Node* tail = nullptr;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        Transfer tr;
        std::string tmp;
        std::getline(iss, tmp, ','); tr.originId         = std::stoi(tmp);
        std::getline(iss, tr.type,   ',');
        std::getline(iss, tmp,       ','); tr.destinationId  = std::stoi(tmp);
        std::getline(iss, tmp,       ','); tr.amount         = std::stoi(tmp);
        std::getline(iss, tmp,       ','); tr.balanceAfter   = std::stoi(tmp);
        std::getline(iss, tmp,       ','); tr.destBalanceAfter = std::stoi(tmp);
        std::getline(iss, tr.timestamp);

        // Append to tail preserves file order: head=first line=newest
        auto* node = new Node(tr);
        node->next = nullptr;
        if (!head)   head = node;
        else         tail->next = node;
        tail = node;
    }
}

// Save head→tail so that the newest (head) is written first in the file
void TransactionList::save(const std::string& file) const {
    std::ofstream out(file);
    for (auto* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        out << tr.originId         << ','
            << tr.type             << ','
            << tr.destinationId    << ','
            << tr.amount           << ','
            << tr.balanceAfter     << ','
            << tr.destBalanceAfter << ','
            << tr.timestamp        << '\n';
    }
}

void TransactionList::printAllAdmin() const {
    std::cout << "\n--- All Transactions (Admin) ---\n";
    std::cout << std::left
              << std::setw(4)  << "#"
              << std::setw(10) << "Origin"
              << std::setw(12) << "Type"
              << std::setw(12) << "Dest"
              << std::setw(10) << "Amount"
              << std::setw(12) << "OrigBal"
              << std::setw(12) << "DestBal"
              << std::setw(20) << "Timestamp" << '\n';
    std::cout << std::string(76, '-') << '\n';

    int idx = 0;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        ++idx;
        std::cout << std::left
                  << std::setw(4)  << idx
                  << std::setw(10) << tr.originId
                  << std::setw(12) << tr.type
                  << std::setw(12) << tr.destinationId
                  << std::setw(10) << tr.amount
                  << std::setw(12) << tr.balanceAfter
                  << std::setw(12) << tr.destBalanceAfter
                  << std::setw(20) << tr.timestamp << '\n';
    }
    std::cout << std::string(76, '-') << '\n';
}

void TransactionList::printUser(int userId) const {
    std::cout << "\n--- Your Transactions ---\n";
    std::cout << std::left
              << std::setw(4)  << "#"
              << std::setw(10) << "Origin"
              << std::setw(12) << "Type"
              << std::setw(12) << "Dest"
              << std::setw(10) << "Amount"
              << std::setw(12) << "Balance"
              << std::setw(20) << "Timestamp" << '\n';
    std::cout << std::string(76, '-') << '\n';

    int idx = 0;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        if (tr.originId == userId || tr.destinationId == userId) {
            ++idx;
            int showBal = (tr.originId == userId) ? tr.balanceAfter: tr.destBalanceAfter;
            std::cout << std::left
                      << std::setw(4)  << idx
                      << std::setw(10) << tr.originId
                      << std::setw(12) << tr.type
                      << std::setw(12) << tr.destinationId
                      << std::setw(10) << tr.amount
                      << std::setw(12) << showBal
                      << std::setw(20) << tr.timestamp << '\n';
        }
    }
    std::cout << std::string(76, '-') << '\n';
}

int TransactionList::printBetween(int userId, int destId) const {
    std::cout << "\n--- Matching Transactions ---\n";
    std::cout << std::left
              << std::setw(4)  << "#"
              << std::setw(10) << "Origin"
              << std::setw(12) << "Type"
              << std::setw(12) << "Dest"
              << std::setw(10) << "Amount"
              << std::setw(12) << "OrigBal"
              << std::setw(12) << "DestBal"
              << std::setw(20) << "Timestamp" << '\n';
    std::cout << std::string(82, '-') << '\n';

    int idx = 0;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& tr = cur->data;
        if ((tr.originId == userId && tr.destinationId == destId) ||
            (tr.originId == destId  && tr.destinationId == userId)) 
        {
            ++idx;
            std::cout << std::left
                      << std::setw(4)  << idx
                      << std::setw(10) << tr.originId
                      << std::setw(12) << tr.type
                      << std::setw(12) << tr.destinationId
                      << std::setw(10) << tr.amount
                      << std::setw(12) << tr.balanceAfter
                      << std::setw(12) << tr.destBalanceAfter
                      << std::setw(20) << tr.timestamp << '\n';
        }
    }
    std::cout << std::string(82, '-') << '\n';
    return idx;
}


bool TransactionList::undoBetween(int userId,
                                  int destId,
                                  int order,
                                  Transfer& outData)
{
    Node* cur = head;
    Node* prev = nullptr;
    int count = 0;
    while (cur) {
        // match in either direction
        if ((cur->data.originId    == userId && cur->data.destinationId == destId) ||
            (cur->data.originId    == destId  && cur->data.destinationId == userId))
        {
            ++count;
            if (count == order) {
                // unlink and return
                if (prev) prev->next = cur->next;
                else       head       = cur->next;
                outData = cur->data;
                delete cur;
                return true;
            }
        }
        prev = cur;
        cur  = cur->next;
    }
    return false;
}