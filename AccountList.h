// File: AccountList.h (add markDeleted)
#pragma once
#include "BankAccount.h"
#include <string>

class AccountList {
    static const int TABLE_SIZE = 101; // Số bucket, nên là số nguyên tố
    struct HashNode {
        BankAccount account;
        HashNode* next;
        HashNode(const BankAccount& acct) : account(acct), next(nullptr) {}
    };
    HashNode* table[TABLE_SIZE];

    int hash(int id) const { return id % TABLE_SIZE; }

public:
    AccountList();
    ~AccountList();

    void addAccount(const BankAccount& acct);
    bool deleteById(int id);
    BankAccount* findById(int id) const;

    void load(const std::string& file);
    void save(const std::string& file) const;

    void printAllAdmin() const;
    void printUserNode(const BankAccount& a) const;
};