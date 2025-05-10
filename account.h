#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

// Lớp Transaction để lưu lịch sử giao dịch
class Transaction {
private:
    std::string type;        // Loại giao dịch: "deposit", "withdraw", "transfer"
    double amount;           // Số tiền
    std::string description; // Mô tả
    time_t timestamp;        // Thời gian giao dịch
    std::string fromAccount; // Tài khoản nguồn (nếu là transfer)
    std::string toAccount;   // Tài khoản đích (nếu là transfer)
    std::string timeStr;     // Chuỗi thời gian (để lưu và đọc từ file)

public:
    // Constructor thông thường
    Transaction(const std::string& type, double amount, const std::string& description,
                const std::string& fromAcc = "", const std::string& toAcc = "")
        : type(type), amount(amount), description(description), fromAccount(fromAcc), toAccount(toAcc) {
        timestamp = time(nullptr);
        
        // Chuyển đổi timestamp thành chuỗi thời gian
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &timestamp);
        timeStr = buffer;
        // Loại bỏ ký tự xuống dòng ở cuối
        if (!timeStr.empty() && timeStr[timeStr.size() - 1] == '\n') {
            timeStr.erase(timeStr.size() - 1);
        }
    }
    
    // Constructor đặc biệt để tạo giao dịch từ dữ liệu file
    Transaction(const std::string& type, double amount, const std::string& description,
                const std::string& fromAcc, const std::string& toAcc, const std::string& timeStrData)
        : type(type), amount(amount), description(description), fromAccount(fromAcc), toAccount(toAcc), timeStr(timeStrData) {
        // Sử dụng chuỗi thời gian đã cho
        // Chúng ta có thể chuyển đổi từ chuỗi thành timestamp nếu cần
        struct tm tm = {};
        std::istringstream ss(timeStrData);
        ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
        timestamp = mktime(&tm);
    }

    // Getters
    std::string getType() const { return type; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    time_t getTimestamp() const { return timestamp; }
    std::string getTimeStr() const { return timeStr; }
    std::string getFromAccount() const { return fromAccount; }
    std::string getToAccount() const { return toAccount; }

    // Hiển thị thông tin giao dịch
    void display() const {
        std::cout << "Thời gian: " << timeStr
                  << ", Loại giao dịch: " << type
                  << ", Số tiền: " << amount
                  << ", Mô tả: " << description;
        
        if (type == "transfer") {
            std::cout << ", Từ tài khoản: " << fromAccount
                      << ", Đến tài khoản: " << toAccount;
        }
        std::cout << std::endl;
    }
};

// Lớp Account để quản lý tài khoản khách hàng
class Account {
private:
    std::string accountId;         // ID tài khoản
    std::string customerName;      // Tên khách hàng
    double balance;                // Số dư
    std::vector<Transaction> transactionHistory; // Lịch sử giao dịch
    std::string password;          // Mật khẩu (cho khách hàng)

public:
    Account() : accountId(""), customerName(""), balance(0.0), password("") {}
    // Constructor
    Account(const std::string& id, const std::string& name, double initialBalance, const std::string& pwd)
        : accountId(id), customerName(name), balance(initialBalance), password(pwd) {
        // Ghi lại giao dịch đầu tiên là tạo tài khoản với số dư ban đầu
        addTransaction("create", initialBalance, "Tạo tài khoản mới với số dư ban đầu");
    }

    // Getters
    std::string getAccountId() const { return accountId; }
    std::string getCustomerName() const { return customerName; }
    double getBalance() const { return balance; }
    
    // Kiểm tra mật khẩu
    bool validatePassword(const std::string& pwd) const {
        return password == pwd;
    }
    
    // Lấy mật khẩu (cho FileManager để lưu)
    std::string getPassword() const {
        return password;
    }
    
    // Lấy lịch sử giao dịch
    const std::vector<Transaction>& getTransactionHistory() const {
        return transactionHistory;
    }

    // Thêm giao dịch vào lịch sử
    void addTransaction(const std::string& type, double amount, const std::string& description,
                        const std::string& fromAcc = "", const std::string& toAcc = "") {
        Transaction trans(type, amount, description, fromAcc, toAcc);
        transactionHistory.push_back(trans);
    }
    
    // Thêm giao dịch từ file vào lịch sử
    void addTransactionFromFile(const std::string& type, double amount, const std::string& description,
                             const std::string& fromAcc, const std::string& toAcc, const std::string& timeStr) {
        Transaction trans(type, amount, description, fromAcc, toAcc, timeStr);
        transactionHistory.push_back(trans);
    }

    // Kiểm tra số dư
    void checkBalance() const {
        std::cout << "Số dư tài khoản " << accountId << ": " << balance << " VND" << std::endl;
    }

    // NẠP tiền vào tài khoản khách hàng 
    bool deposit(double amount) {
        if (amount <= 0) {
            std::cout << "Số tiền gửi phải lớn hơn 0!" << std::endl;
            return false;
        }
        
        balance += amount;
        addTransaction("deposit", amount, "Gửi tiền");
        std::cout << "Nạp tiền thành công. Số dư mới: " << balance << " VND" << std::endl;
        return true;
    }

    // Rút tiền
    bool withdraw(double amount) {
        if (amount <= 0) {
            std::cout << "Số tiền rút phải lớn hơn 0!" << std::endl;
            return false;
        }
        
        if (amount > balance) {
            std::cout << "Số dư không đủ để thực hiện giao dịch!" << std::endl;
            return false;
        }
        
        balance -= amount;
        addTransaction("withdraw", amount, "Rút tiền");
        std::cout << "Rút tiền thành công. Số dư mới: " << balance << " VND" << std::endl;
        return true;
    }

    // Chuyển khoản (được gọi từ tài khoản nguồn)
    bool transferTo(Account& recipient, double amount) {
        if (amount <= 0) {
            std::cout << "Số tiền chuyển phải lớn hơn 0!" << std::endl;
            return false;
        }
        
        if (amount > balance) {
            std::cout << "Số dư không đủ để thực hiện giao dịch!" << std::endl;
            return false;
        }
        
        balance -= amount;
        recipient.receiveTransfer(amount, accountId);
        
        addTransaction("transfer", amount, "Chuyển tiền", accountId, recipient.getAccountId());
        std::cout << "Chuyển khoản thành công. Số dư mới: " << balance << " VND" << std::endl;
        return true;
    }

    // Nhận tiền chuyển khoản (được gọi từ tài khoản đích)
    void receiveTransfer(double amount, const std::string& fromAccountId) {
        balance += amount;
        addTransaction("transfer", amount, "Nhận tiền chuyển khoản", fromAccountId, accountId);
    }

    // Hiển thị lịch sử giao dịch
    void displayTransactionHistory() const {
        std::cout << "Lịch sử giao dịch của tài khoản " << accountId << ":" << std::endl;
        for (const auto& transaction : transactionHistory) {
            transaction.display();
        }
    }
};

#endif // ACCOUNT_H
