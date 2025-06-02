// File: AccountList.h (with InfoNode and IndexNode)
#pragma once
#include "BankAccount.h"
#include <string>

struct InfoNode {
    BankAccount account;
    InfoNode* next;
    InfoNode(const BankAccount& acct) : account(acct), next(nullptr) {}
};

struct IndexNode {
    int id;
    InfoNode* infoPtr;
    IndexNode() : id(0), infoPtr(nullptr) {}
    IndexNode(int id, InfoNode* ptr) : id(id), infoPtr(ptr) {}
};

class AccountList {
    IndexNode* indexArray; // mảng động các IndexNode
    int size;              // số lượng tài khoản
    int capacity;          // sức chứa hiện tại của mảng
    InfoNode* infoHead;    // đầu danh sách liên kết chứa thông tin

    void quickSort(int left, int right);
    int binarySearch(int id) const;
    void ensureCapacity();
    void updateIndex(int idx, int newId);
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

    bool updateAccountId(int oldId, int newId);
};