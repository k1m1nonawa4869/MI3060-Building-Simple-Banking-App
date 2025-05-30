// File: AccountList.h (add markDeleted)
#ifndef ACCOUNT_LIST_H
#define ACCOUNT_LIST_H

#include "BankAccount.h"
#include <string>

class AccountList {
public:
    AccountList();
    void addAccount(const BankAccount& acct);
    bool deleteById(int id);
    BankAccount* findById(int id) const;
    void load(const std::string& file);
    void save(const std::string& file) const;
    void printAllAdmin() const;
    void printUserNode(const BankAccount& acct) const;

private:
    struct Node {
        BankAccount account;
        Node* next;
        Node(const BankAccount& a) : account(a), next(nullptr) {}
    };
    Node* head;
};

#endif // ACCOUNT_LIST_H