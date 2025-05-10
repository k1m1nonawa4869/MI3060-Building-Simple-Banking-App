#include <iostream>
#include <string>
#include <filesystem>
#include "account.h" 
#include "employee.h"
#include "bank_manager.h"
#include "file_manager.h"
#include "menu.h"

// Đảm bảo thư mục data tồn tại
void ensureDataDirectoryExists() {
    // Tạo thư mục data nếu chưa tồn tại
    if (!std::filesystem::exists("data")) {
        std::filesystem::create_directory("data");
        std::cout << "Đã tạo thư mục data để lưu trữ dữ liệu" << std::endl;
    }
}

int main() {
    // Đảm bảo thư mục data tồn tại
    ensureDataDirectoryExists();
    
    // Khởi tạo FileManager với đường dẫn file phù hợp
    FileManager fileManager("data/accounts.txt", "data/employees.txt");
    
    // Khởi tạo BankManager và kết nối với FileManager
    BankManager bankManager;
    bankManager.setFileManager(&fileManager);
    
    // Tải dữ liệu từ file (nếu có)
    std::map<std::string, Account> accounts;
    std::map<std::string, Employee> employees;
    
    // Thử tải dữ liệu từ file
    if (fileManager.loadAccounts(accounts)) {
        bankManager.setAccounts(accounts);
        std::cout << "Đã tải dữ liệu tài khoản từ file" << std::endl;
    } else {
        std::cout << "Không tìm thấy dữ liệu tài khoản, bắt đầu với danh sách trống" << std::endl;
    }
    
    if (fileManager.loadEmployees(employees)) {
        bankManager.setEmployees(employees);
        std::cout << "Đã tải dữ liệu nhân viên từ file" << std::endl;
    } else {
        std::cout << "Không tìm thấy dữ liệu nhân viên, bắt đầu với nhân viên mặc định" << std::endl;
    }
    
    // Khởi tạo Menu với BankManager đã được thiết lập
    Menu menu(bankManager);
    
    // Chạy chương trình
    std::cout << "Chào mừng đến với Hệ thống Quản lý Ngân hàng!" << std::endl;
    menu.run();
    
    // Lưu dữ liệu trước khi thoát (tùy chọn)
    std::cout << "Đang lưu dữ liệu trước khi thoát..." << std::endl;
    fileManager.saveAccounts(bankManager.getAccounts());
    
    std::cout << "Cảm ơn bạn đã sử dụng hệ thống. Tạm biệt!" << std::endl;
    return 0;
}
