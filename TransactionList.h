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
    int balanceAfter;      // origin's balance after action
    int destBalanceAfter;  // destination's balance after action
    std::string timestamp;
};

class TransactionList {
public:
    TransactionList();
    void add(int origin, const std::string& type, int dest, int amt, int balAfter, int destBalanceAfter);
    bool popLast(Transfer& out);
    void markDeleted(int delId);
    void load(const std::string& file);
    void save(const std::string& file) const;
    void printAllAdmin() const;
    void printUser(int userId) const;
    // Print all mutual transactions (newest→oldest) between userId and destId,
    // numbering them 1..N, and return the count.
    bool undoBetween(int userId, int destId, int order, Transfer& outData);
    int printBetween(int userId, int destId) const;

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