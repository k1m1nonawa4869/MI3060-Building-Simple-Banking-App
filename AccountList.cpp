// File: AccountList.cpp
#include "AccountList.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

AccountList::AccountList() : indexArray(nullptr), size(0), capacity(0), infoHead(nullptr) {}

AccountList::~AccountList() {
    // Xóa danh sách liên kết chứa thông tin
    InfoNode* cur = infoHead;
    while (cur) {
        InfoNode* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    // Xóa mảng index
    delete[] indexArray;
}

void AccountList::ensureCapacity() {
    if (size < capacity) return;
    int newCap = (capacity == 0) ? 8 : capacity * 2;
    IndexNode* newArr = new IndexNode[newCap];
    for (int i = 0; i < size; ++i) newArr[i] = indexArray[i];
    delete[] indexArray;
    indexArray = newArr;
    capacity = newCap;
}

void AccountList::addAccount(const BankAccount& acct) {
    // Thêm vào danh sách liên kết
    InfoNode* infoNode = new InfoNode(acct);
    infoNode->next = infoHead;
    infoHead = infoNode;

    // Thêm vào mảng index
    ensureCapacity();
    indexArray[size++] = IndexNode(acct.id, infoNode);

    // Sắp xếp lại mảng index theo ID (QuickSort)
    if (size > 1) quickSort(0, size - 1);

    std::cout << "Account " << acct.id
              << " ('" << acct.name << "') added." << std::endl;
    save("accounts.txt");
}

void AccountList::quickSort(int left, int right) {
    if (left >= right) return;
    int pivot = indexArray[(left + right) / 2].id;
    int i = left, j = right;
    while (i <= j) {
        while (indexArray[i].id < pivot) ++i;
        while (indexArray[j].id > pivot) --j;
        if (i <= j) {
            IndexNode temp = indexArray[i];
            indexArray[i] = indexArray[j];
            indexArray[j] = temp;
            ++i; --j;
        }
    }
    if (left < j) quickSort(left, j);
    if (i < right) quickSort(i, right);
}

int AccountList::binarySearch(int id) const {
    int l = 0, r = size - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (indexArray[m].id == id) return m;
        if (indexArray[m].id < id) l = m + 1;
        else r = m - 1;
    }
    return -1;
}
void AccountList::updateIndex(int idx, int newId) {
    // Cập nhật ID trong mảng indexArray
    indexArray[idx].id = newId;

    // Sắp xếp lại mảng indexArray để đảm bảo thứ tự
    quickSort(0, size - 1);
}
BankAccount* AccountList::findById(int id) const {
    int idx = binarySearch(id);
    if (idx == -1) return nullptr;
    return &(indexArray[idx].infoPtr->account);
}

bool AccountList::deleteById(int id) {
    int idx = binarySearch(id);
    if (idx == -1) return false;

    // Xóa khỏi danh sách liên kết
    InfoNode* prev = nullptr;
    InfoNode* cur = infoHead;
    while (cur && cur->account.id != id) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return false;
    if (prev) prev->next = cur->next;
    else infoHead = cur->next;
    delete cur;

    // Xóa khỏi mảng index
    for (int i = idx; i < size - 1; ++i)
        indexArray[i] = indexArray[i + 1];
    --size;

    std::cout << "Account " << id << " deleted." << std::endl;
    return true;
}

void AccountList::load(const std::string& file) {
    std::ifstream in(file.c_str());
    if (!in) return;
    // Xóa dữ liệu cũ
    InfoNode* cur = infoHead;
    while (cur) {
        InfoNode* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    infoHead = nullptr;
    delete[] indexArray;
    indexArray = nullptr;
    size = 0;
    capacity = 0;

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        BankAccount acct;
        std::getline(iss, line, ','); acct.id = std::stoi(line);
        std::getline(iss, acct.name, ',');
        std::getline(iss, line, ','); acct.balance = std::stoi(line);
        std::getline(iss, line, ','); acct.setPin(line);
        char flag; iss >> flag; iss.ignore(1); acct.setLocked(flag=='1');
        std::getline(iss, line, ','); // created
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, line, "");
        std::getline(iss, line); // modified
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, acct.getCreated(), line);

        addAccount(acct);
    }
}

void AccountList::save(const std::string& file) const {
    std::ofstream out(file.c_str());
    for (int i = 0; i < size; ++i) {
        const BankAccount& a = indexArray[i].infoPtr->account;
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
              << std::setw(4)  << "#"
              << std::setw(6)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(10) << "Balance"
              << std::setw(10) << "Status"
              << std::setw(20) << "Created"
              << std::setw(20) << "Modified"
              << std::endl;
    std::cout << std::string(86, '-') << std::endl;
    for (int i = 0; i < size; ++i) {
        const BankAccount& a = indexArray[i].infoPtr->account;
        std::cout << std::left
                  << std::setw(4)  << (i + 1)
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
//update AccID in dynamic array indexArray
bool AccountList::updateAccountId(int oldId, int newId) {
    int idx = binarySearch(oldId); // Tìm vị trí của ID cũ
    if (idx == -1) {
        return false; // Không tìm thấy ID cũ
    }
    updateIndex(idx, newId); // Cập nhật ID trong mảng indexArray
    return true;
}