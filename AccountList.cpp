// File: AccountList.cpp
#include "AccountList.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

AccountList::AccountList()
    : head(nullptr)
{}

// Hàm hoán đổi 2 phần tử trong mảng idArr
void swapInt(int& a, int& b) {
    int t = a; a = b; b = t;
}

// Hàm partition cho quicksort
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swapInt(arr[i], arr[j]);
        }
    }
    swapInt(arr[i + 1], arr[high]);
    return i + 1;
}

// Hàm quicksort cho mảng idArr
void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void AccountList::rebuildIdArray() {
    // Đếm số node
    int count = 0;
    for (Node* cur = head; cur; cur = cur->next) ++count;
    delete[] idArr;
    idArr = new int[count];
    idArrSize = count;
    int i = 0;
    for (Node* cur = head; cur; cur = cur->next, ++i) {
        idArr[i] = cur->account.id;
    }
    // Sắp xếp mảng ID tăng dần (quicksort)
    if (idArrSize > 1)
        quickSort(idArr, 0, idArrSize - 1);
    
}

void AccountList::clearIdIndexList() {
    while (idIndexHead) {
        IdIndexNode* tmp = idIndexHead;
        idIndexHead = idIndexHead->next;
        delete tmp;
    }
    idIndexHead = nullptr;
}

void AccountList::rebuildIdIndexList() {
    clearIdIndexList();
    for (Node* cur = head; cur; cur = cur->next) {
        IdIndexNode* newNode = new IdIndexNode(cur->account.id, cur);
        newNode->next = idIndexHead;
        idIndexHead = newNode;
    }
}

void AccountList::addAccount(const BankAccount& acct) {
    auto* node = new Node(acct);
    node->next = head;
    head = node;
    std::cout << "Account " << acct.id
              << " ('" << acct.name << "') added." << std::endl;
    rebuildIdArray();
    rebuildIdIndexList();
}

bool AccountList::deleteById(int id) {
    Node* cur = head;
    Node* prev = nullptr;
    while (cur) {
        if (cur->account.id == id) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            delete cur;
            rebuildIdArray();
            rebuildIdIndexList();
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

//this one has to be rewritten in new structure???
BankAccount* AccountList::findById(int id) const {
    // Tìm kiếm nhị phân trên mảng idArr
    int left = 0, right = idArrSize - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (idArr[mid] == id) {
            // Tìm Node* tương ứng trong danh sách liên kết chỉ số ID
            for (IdIndexNode* cur = idIndexHead; cur; cur = cur->next) {
                if (cur->id == id) return &(cur->accountPtr->account);
            }
        }
        if (idArr[mid] < id) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}

void AccountList::load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return;
    head = nullptr;
    Node* tail = nullptr;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        BankAccount acct;
        // parse CSV: id,name,balance,pin,locked,created,modified
        std::getline(iss, line, ','); 
        acct.id = std::stoi(line);
        std::getline(iss, acct.name, ',');
        std::getline(iss, line, ','); 
        acct.balance = std::stoi(line);
        std::getline(iss, line, ','); 
        acct.setPin(line);
        char flag; iss >> flag; iss.ignore(1); 
        acct.setLocked(flag=='1');
        std::getline(iss, line, ','); /* use line for created */ 
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, line, "");
        std::getline(iss, line); /* line now is modified */
        acct = BankAccount(acct.id, acct.name, acct.getPin(), acct.balance, acct.getCreated(), line);

        // **Append** to the tail
        Node* node = new Node(acct);
        node->next = nullptr;
        if (!head) {
            head = node;
        }
        else {    
            tail->next = node;
        }
        tail = node;
    }
    rebuildIdArray();
    rebuildIdIndexList();
}

void AccountList::save(const std::string& file) const {
    std::ofstream out(file);
    // **Write head→tail** so newest (head) appears first in the file
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& a = cur->account;
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
    
    int idx=0;
    for (Node* cur = head; cur; cur = cur->next) {
        const auto& a = cur->account;
        ++idx;
        std::cout << std::left
                  << std::setw(4)  << idx
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
              << std::setw(4)  << "#"
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

