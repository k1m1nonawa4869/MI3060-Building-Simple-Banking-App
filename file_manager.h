#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include "account.h"
#include "employee.h"

// Lớp FileManager để quản lý việc lưu trữ và đọc dữ liệu từ file
class FileManager {
private:
    std::string accountsFilePath;    // Đường dẫn file lưu thông tin tài khoản
    std::string employeesFilePath;   // Đường dẫn file lưu thông tin nhân viên
    
    // Tạo thư mục nếu không tồn tại
    void createDirectoryIfNotExists(const std::string& path) {
        // Đơn giản hóa: Chúng ta sẽ giả định thư mục đã tồn tại
        // Trong thực tế, bạn có thể sử dụng các hàm của hệ thống để tạo thư mục
    }

public:
    // Constructor
    FileManager(const std::string& accountsPath = "accounts.txt", 
                const std::string& employeesPath = "employees.txt")
        : accountsFilePath(accountsPath), employeesFilePath(employeesPath) {
        createDirectoryIfNotExists("data/");
    }
    
    // Lưu thông tin tài khoản vào file
    bool saveAccounts(const std::map<std::string, Account>& accounts) {
        std::ofstream file(accountsFilePath);
        if (!file.is_open()) {
            std::cout << "Không thể mở file để lưu dữ liệu tài khoản!" << std::endl;
            return false;
        }
        
        // Ghi tiêu đề cột
        file << "account_id,customer_name,password,balance" << std::endl;
        
        // Ghi dữ liệu từng tài khoản
        for (const auto& pair : accounts) {
            const Account& account = pair.second;
            file << account.getAccountId() << ","
                 << account.getCustomerName() << ","
                 << account.getPassword() << ","
                 << account.getBalance() << std::endl;
        }
        
        file.close();
        std::cout << "Đã lưu thông tin tài khoản vào file: " << accountsFilePath << std::endl;
        return true;
    }
    
    // Đọc thông tin tài khoản từ file
    bool loadAccounts(std::map<std::string, Account>& accounts) {
        std::ifstream file(accountsFilePath);
        if (!file.is_open()) {
            std::cout << "Không tìm thấy file tài khoản hoặc không thể mở file!" << std::endl;
            return false;
        }
        
        accounts.clear();  // Xóa dữ liệu hiện tại
        
        std::string line;
        // Bỏ qua dòng tiêu đề
        std::getline(file, line);
        
        // Đọc dữ liệu từng dòng
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string accountId, customerName, password;
            double balance;
            
            // Phân tích dòng dữ liệu, định dạng: account_id,customer_name,password,balance
            std::getline(ss, accountId, ',');
            std::getline(ss, customerName, ',');
            std::getline(ss, password, ',');
            ss >> balance;
            
            // Tạo đối tượng Account mới và thêm vào map
            accounts[accountId] = Account(accountId, customerName, balance, password);
        }
        
        file.close();
        std::cout << "Đã đọc thông tin tài khoản từ file: " << accountsFilePath << std::endl;
        return true;
    }
    
    // Lưu thông tin nhân viên vào file
    bool saveAllEmployee(const Employee& employee) {
        std::ofstream file(employeesFilePath, std::ios::app);  // Mở file ở chế độ append
        if (!file.is_open()) {
            std::cout << "Không thể mở file để lưu dữ liệu nhân viên mới!" << std::endl;
            return false;
        }
    
        // Kiểm tra xem file có trống không, nếu trống thì ghi header
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "employee_id,password,first_name,last_name" << std::endl;
        }
    
        // Ghi thông tin nhân viên mới
        file << employee.getEmployeeId() << ","  // Gọi phương thức getEmployeeId()
             << employee.getPassword() << ","    // Gọi phương thức getPassword()
             << employee.getFirstName() << ","   // Gọi phương thức getFirstName()
             << employee.getLastName() << "\n";  // Gọi phương thức getLastName()
    
        file.close();
        std::cout << "Đã lưu thông tin nhân viên mới vào file: " << employeesFilePath << std::endl;
        return true;
    }
    
    // Đọc thông tin nhân viên từ file
    bool loadEmployees(std::map<std::string, Employee>& employees) {
        std::ifstream file(employeesFilePath);
        if (!file.is_open()) {
            std::cout << "Không tìm thấy file nhân viên hoặc không thể mở file!" << std::endl;
            return false;
        }
        
        employees.clear();  // Xóa dữ liệu hiện tại
        
        std::string line;
        // Bỏ qua dòng tiêu đề
        std::getline(file, line);
        
        // Đọc dữ liệu từng dòng
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string employeeId, password, firstName, lastName;
            
            // Phân tích dòng dữ liệu, định dạng: employee_id,password,first_name,last_name
            std::getline(ss, employeeId, ',');
            std::getline(ss, password, ',');
            std::getline(ss, firstName, ',');
            std::getline(ss, lastName, ',');
            
            // Tạo đối tượng Employee mới và thêm vào map
            employees[employeeId] = Employee(employeeId, password, firstName, lastName);
        }
        
        file.close();
        std::cout << "Đã đọc thông tin nhân viên từ file: " << employeesFilePath << std::endl;
        return true;
    }
    
    // Lưu một tài khoản mới vào file (thêm vào cuối file)
    bool saveNewAccount(const Account& account) {
        std::ofstream file(accountsFilePath, std::ios::app);  // Mở file ở chế độ append
        if (!file.is_open()) {
            std::cout << "Không thể mở file để lưu dữ liệu tài khoản mới!" << std::endl;
            return false;
        }
        
        // Kiểm tra xem file có trống không, nếu trống thì ghi header
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "account_id,customer_name,password,balance" << std::endl;
        }
        
        // Ghi thông tin tài khoản mới
        file << account.getAccountId() << ","
             << account.getCustomerName() << ","
             << account.getPassword() << ","
             << account.getBalance() << std::endl;
        
        file.close();
        std::cout << "Đã lưu thông tin tài khoản mới vào file: " << accountsFilePath << std::endl;
        return true;
    }
    
    // Lưu một nhân viên mới vào file (thêm vào cuối file)
    bool saveNewEmployee(const Employee& employee) {
        std::ofstream file(employeesFilePath, std::ios::app);  // Mở file ở chế độ append
        if (!file.is_open()) {
            std::cout << "Không thể mở file để lưu dữ liệu nhân viên mới!" << std::endl;
            return false;
        }
        
        // Kiểm tra xem file có trống không, nếu trống thì ghi header
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "employee_id,password,first_name,last_name" << std::endl;
        }
        
        // Ghi thông tin nhân viên mới
        file << employee.getEmployeeId() << ","
             << employee.getPassword() << ","
             << employee.getFirstName() << ","
             << employee.getLastName() << std::endl;
        
        file.close();
        std::cout << "Đã lưu thông tin nhân viên mới vào file: " << employeesFilePath << std::endl;
        return true;
    }
    
    // Cập nhật thông tin một tài khoản cụ thể trong file
    bool updateAccount(const Account& account) {
        // Đọc tất cả tài khoản từ file
        std::map<std::string, Account> accounts;
        if (!loadAccounts(accounts)) {
            return false;
        }
        
        // Cập nhật tài khoản cần thay đổi
        accounts[account.getAccountId()] = account;
        
        // Lưu lại toàn bộ dữ liệu
        return saveAccounts(accounts);
    }
};

#endif // FILE_MANAGER_H