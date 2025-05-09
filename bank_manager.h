#ifndef BANK_MANAGER_H
#define BANK_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "account.h"
#include "employee.h"
#include "file_manager.h"

class BankManager {
private:
    std::map<std::string, Account> accounts;             // Danh sách tài khoản
    std::map<std::string, Employee> employees;           // Danh sách nhân viên
    Employee* currentEmployee;                           // Nhân viên đang đăng nhập
    std::string currentCustomerAccountId;                // ID tài khoản khách hàng đang đăng nhập
    FileManager* fileManager;                            // Con trỏ đến FileManager để lưu/đọc dữ liệu

    // Tạo ID tài khoản mới
    std::string generateAccountId() {
        // Đơn giản tạo ID dựa trên số lượng tài khoản hiện có
        std::stringstream ss;
        ss << "ACC" << std::setw(6) << std::setfill('0') << (accounts.size() + 1);
        return ss.str();
    }

public:
    // Constructor
    BankManager() : currentEmployee(nullptr), currentCustomerAccountId(""), fileManager(nullptr) {
        // Khởi tạo với các nhân viên mặc định
        employees["admin"] = Employee("admin", "admin123", "Administrator", "Admin");
        employees["emp001"] = Employee("emp001", "emp123", "Nguyễn", "Văn");
        employees["emp002"] = Employee("emp002", "emp456", "Trần", "Thị");
    }
    
    // Setter cho FileManager
    void setFileManager(FileManager* fm) {
        fileManager = fm;
    }
    
    // Setters cho tài khoản và nhân viên (để tải từ file)
    void setAccounts(const std::map<std::string, Account>& accs) {
        accounts = accs;
    }
    
    void setEmployees(const std::map<std::string, Employee>& emps) {
        employees = emps;
    }
    
    // Getter cho map accounts (để lưu vào file)
    const std::map<std::string, Account>& getAccounts() const {
        return accounts;
    }
    
    // Getter cho map employees (để lưu vào file)
    const std::map<std::string, Employee>& getEmployees() const {
        return employees;
    }
    
    // Kiểm tra đăng nhập khách hàng
    bool customerLogin(const std::string& accountId, const std::string& password) {
        if (!accountExists(accountId)) {
            std::cout << "Tài khoản không tồn tại!" << std::endl;
            return false;
        }
        
        if (!accounts[accountId].validatePassword(password)) {
            std::cout << "Sai mật khẩu!" << std::endl;
            return false;
        }
        
        // Lưu ID tài khoản khách hàng đang đăng nhập
        currentCustomerAccountId = accountId;
        std::cout << "Đăng nhập thành công! Xin chào " << accounts[accountId].getCustomerName() << std::endl;
        return true;
    }

    // Đăng xuất khách hàng
    void customerLogout() {
        if (!currentCustomerAccountId.empty()) {
            std::cout << "Đã đăng xuất khỏi tài khoản " << currentCustomerAccountId << std::endl;
            currentCustomerAccountId = "";
        } else {
            std::cout << "Không có khách hàng nào đang đăng nhập!" << std::endl;
        }
    }

    // Kiểm tra khách hàng đã đăng nhập chưa
    bool isCustomerLoggedIn() const {
        return !currentCustomerAccountId.empty();
    }

    // Lấy ID tài khoản khách hàng đang đăng nhập
    std::string getCurrentCustomerAccountId() const {
        return currentCustomerAccountId;
    }

    // Kiểm tra xem tài khoản có tồn tại không
    bool accountExists(const std::string& accountId) const {
        return accounts.find(accountId) != accounts.end();
    }

    // Tạo tài khoản mới cho khách hàng
    std::string createAccount(const std::string& customerName, double initialBalance, const std::string& password) {
        std::string accountId = generateAccountId();
        accounts[accountId] = Account(accountId, customerName, initialBalance, password);
        std::cout << "Tài khoản mới đã được tạo với ID: " << accountId << std::endl;
        
        // Lưu tài khoản mới vào file
        if (fileManager != nullptr) {
            fileManager->saveNewAccount(accounts[accountId]);
        } else {
            std::cout << "Cảnh báo: FileManager chưa được thiết lập, tài khoản không được lưu vào file!" << std::endl;
        }
        
        return accountId;
    }

    // Chức năng của khách hàng: Kiểm tra số dư
    bool checkBalance() {
        if (!isCustomerLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        accounts[currentCustomerAccountId].checkBalance();
        return true;
    }

    // Chức năng của khách hàng: Gửi tiền
    bool deposit(double amount) {
        if (!isCustomerLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        bool result = accounts[currentCustomerAccountId].deposit(amount);
        
        // Cập nhật thông tin tài khoản vào file sau khi gửi tiền
        if (result && fileManager != nullptr) {
            fileManager->updateAccount(accounts[currentCustomerAccountId]);
        }
        
        return result;
    }

    // Chức năng của khách hàng: Rút tiền
    bool withdraw(double amount) {
        if (!isCustomerLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        bool result = accounts[currentCustomerAccountId].withdraw(amount);
        
        // Cập nhật thông tin tài khoản vào file sau khi rút tiền
        if (result && fileManager != nullptr) {
            fileManager->updateAccount(accounts[currentCustomerAccountId]);
        }
        
        return result;
    }

    // Chức năng của khách hàng: Chuyển khoản
    bool transfer(const std::string& toAccountId, double amount) {
        if (!isCustomerLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        if (toAccountId.empty()) {
            std::cout << "ID tài khoản đích không được để trống!" << std::endl;
            return false;
        }
        
        if (toAccountId == currentCustomerAccountId) {
            std::cout << "Không thể chuyển khoản cho chính mình!" << std::endl;
            return false;
        }
        
        if (!accountExists(toAccountId)) {
            std::cout << "Tài khoản đích không tồn tại!" << std::endl;
            return false;
        }
        
        if (amount <= 0) {
            std::cout << "Số tiền chuyển khoản phải lớn hơn 0!" << std::endl;
            return false;
        }
        
        // Correct map access
        Account& sourceAccount = accounts[currentCustomerAccountId];
        Account& destAccount = accounts[toAccountId];
    
        bool result = sourceAccount.transferTo(destAccount, amount);
        
        // Cập nhật thông tin cả hai tài khoản vào file sau khi chuyển khoản
        if (result && fileManager != nullptr) {
            fileManager->updateAccount(sourceAccount);
            fileManager->updateAccount(destAccount);
        }
        
        return result;
    }

    // Chức năng của nhân viên: Đăng nhập
    bool employeeLogin(const std::string& employeeId, const std::string& password) {
        auto it = employees.find(employeeId);
        if (it == employees.end()) {
            std::cout << "ID nhân viên không tồn tại!" << std::endl;
            return false;
        }
        
        if (!it->second.validatePassword(password)) {
            std::cout << "Sai mật khẩu!" << std::endl;
            return false;
        }
        
        currentEmployee = &it->second;
        std::cout << "Đăng nhập thành công! Xin chào " << currentEmployee->getFullName() << std::endl;
        return true;
    }

    // Kiểm tra nhân viên đã đăng nhập chưa
    bool isEmployeeLoggedIn() const {
        return currentEmployee != nullptr;
    }

    // Đăng xuất nhân viên
    void employeeLogout() {
        if (currentEmployee) {
            std::cout << "Đã đăng xuất khỏi tài khoản " << currentEmployee->getEmployeeId() << std::endl;
            currentEmployee = nullptr;
        } else {
            std::cout << "Không có nhân viên nào đang đăng nhập!" << std::endl;
        }
    }

    // Thêm nhân viên mới (chỉ admin mới có quyền)
    bool addEmployee(const std::string& employeeId, const std::string& password,
                    const std::string& firstName, const std::string& lastName) {
        if (!isEmployeeLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        if (currentEmployee->getEmployeeId() != "admin") {
            std::cout << "Chỉ admin mới có quyền thêm nhân viên mới!" << std::endl;
            return false;
        }
        
        if (employees.find(employeeId) != employees.end()) {
            std::cout << "ID nhân viên đã tồn tại!" << std::endl;
            return false;
        }
        
        employees[employeeId] = Employee(employeeId, password, firstName, lastName);
        std::cout << "Đã thêm nhân viên mới: " << firstName << " " << lastName << std::endl;
        
        // Lưu nhân viên mới vào file
        if (fileManager != nullptr) {
            fileManager->saveNewEmployee(employees[employeeId]);
        }
        
        return true;
    }

    // Chức năng của nhân viên: Xử lý giao dịch (xem lịch sử giao dịch)
    bool processTransaction(const std::string& accountId) {
        if (!isEmployeeLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        if (!accountExists(accountId)) {
            std::cout << "Tài khoản không tồn tại!" << std::endl;
            return false;
        }
        
        accounts[accountId].displayTransactionHistory();
        return true;
    }

    // Chức năng của nhân viên: Báo cáo số dư
    bool generateBalanceReport() {
        if (!isEmployeeLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        std::cout << "===== BÁO CÁO SỐ DƯ TÀI KHOẢN =====" << std::endl;
        std::cout << std::left << std::setw(10) << "ID" 
                 << std::setw(30) << "Tên khách hàng" 
                 << std::right << std::setw(15) << "Số dư (VND)" << std::endl;
        std::cout << std::string(55, '-') << std::endl;
        
        double totalBalance = 0.0;
        for (const auto& pair : accounts) {
            const Account& account = pair.second;
            std::cout << std::left << std::setw(10) << account.getAccountId() 
                     << std::setw(30) << account.getCustomerName() 
                     << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
                     << account.getBalance() << std::endl;
            totalBalance += account.getBalance();
        }
        
        std::cout << std::string(55, '-') << std::endl;
        std::cout << std::left << std::setw(40) << "TỔNG SỐ DƯ:" 
                 << std::right << std::setw(15) << std::fixed << std::setprecision(2) 
                 << totalBalance << std::endl;
        return true;
    }

    // Lưu dữ liệu vào file
    bool saveData(const std::string& filename) {
        if (!isEmployeeLoggedIn()) {
            std::cout << "Bạn cần đăng nhập để thực hiện chức năng này!" << std::endl;
            return false;
        }
        
        if (fileManager == nullptr) {
            std::cout << "FileManager chưa được thiết lập!" << std::endl;
            return false;
        }
        
        // Lưu tất cả tài khoản
        fileManager->saveAccounts(accounts);
        
        std::cout << "Đã lưu dữ liệu vào file!" << std::endl;
        return true;
    }
};

#endif // BANK_MANAGER_H
