// File: TransactionList.h
#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include "BankAccount.h"
#include <string>

struct Transfer {
    int originId;
    std::string type;
    int destinationId;
    int amount;
    int balanceAfter;
    std::string timestamp;
};

class TransactionList {
public:
    TransactionList();
    void add(int origin, const std::string& type, int dest, int amt, int balAfter);
    bool popLast(Transfer& out);
    void markDeleted(int delId);
    void load(const std::string& file);
    void save(const std::string& file) const;
    void printAllAdmin() const;
    void printUser(int userId) const;

private:
    struct Node {
        Transfer data;
        Node* next;
        Node(const Transfer& tr) : data(tr), next(nullptr) {}
    };
    Node* head;
    std::string currentTime() const;
};

#endif // TRANSACTION_LIST_H