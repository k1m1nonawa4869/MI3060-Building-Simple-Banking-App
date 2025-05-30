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

    // Mảng động chứa các ID 
    int* idArr = nullptr;
    int idArrSize = 0;

    // Danh sách liên kết chỉ số ID 
    struct IdIndexNode {
        int id;
        Node* accountPtr;
        IdIndexNode* next;
        IdIndexNode (int i, Node* acc) : id(i), accountPtr(acc), next(nullptr) {} 
    };
    IdIndexNode* idIndexHead = nullptr;
    void rebuildIdArray();
    void rebuildIdIndexList();
    void clearIdIndexList();
};

#endif // ACCOUNT_LIST_H