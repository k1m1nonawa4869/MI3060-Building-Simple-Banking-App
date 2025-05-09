#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <limits>
#include <string>
#include "bank_manager.h"

// Lớp Menu để quản lý giao diện người dùng
class Menu {
private:
    BankManager& bankManager;  // Tham chiếu đến đối tượng BankManager

    // Hiển thị menu chính
    void displayMainMenu() const {
        std::cout << "\n===== NGÂN HÀNG ABC - HỆ THỐNG QUẢN LÝ =====" << std::endl;
        std::cout << "1. Khu vực khách hàng" << std::endl;
        std::cout << "2. Khu vực nhân viên" << std::endl;
        std::cout << "0. Thoát" << std::endl;
        std::cout << "Vui lòng chọn: ";
    }

    // Hiển thị menu khách hàng
    void displayCustomerMenu() const {
        std::cout << "\n===== DỊCH VỤ KHÁCH HÀNG =====" << std::endl;
        std::cout << "1. Tạo tài khoản mới" << std::endl;
        std::cout << "2. Đăng nhập" << std::endl;
        std::cout << "0. Quay lại" << std::endl;
        std::cout << "Vui lòng chọn: ";
    }
    // hiển thị màn hình của khách hàng sau khi đăng nhập 
    void displayCustomerMenu1 () const {
        std::cout << "\n===== QUÝ KHÁCH MUỐN CHỌN DỊCH VỤ NÀO ? =====" << std::endl;
        std::cout << "1.Kiểm tra số dư" << std::endl;
        std::cout << "2.Gửi tiền" << std::endl;
        std::cout << "3.Rút tiền" << std::endl;
        std::cout << "4.Chuyển khoản" << std::endl;
        std::cout << "0.Đăng xuất" << std::endl;
        std::cout << "Lựa chọn của bạn là: " ;
    }

    // Hiển thị menu nhân viên
    void displayEmployeeMenu() const {
        std::cout << "\n===== QUẢN LÝ NGÂN HÀNG =====" << std::endl;
        std::cout << "1. Đăng nhập" << std::endl;
        std::cout << "2. Xử lý giao dịch (xem lịch sử)" << std::endl;
        std::cout << "3. Báo cáo số dư" << std::endl;
        std::cout << "4. Thêm nhân viên mới" << std::endl;
        std::cout << "5. Lưu dữ liệu" << std::endl;
        std::cout << "6. Đăng xuất" << std::endl;
        std::cout << "0. Quay lại" << std::endl;
        std::cout << "Vui lòng chọn: ";
    }

    // Xử lý menu khách hàng
    void processCustomerMenu() {
        int choice;
        std::string accountId, password;
        bool isLoggedIn = false;
        
        do {
            if (!bankManager.isCustomerLoggedIn() ) {
                displayCustomerMenu();
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm

                switch (choice) {
                    case 1:
                        createAccount();
                        break;
                    case 2:
                        // Đăng nhập
                        std::cout << "\n===== ĐĂNG NHẬP TÀI KHOẢN =====" << std::endl;
                        std::cout << "Nhập ID tài khoản: ";
                        std::getline(std::cin, accountId);
                    
                        std::cout << "Nhập mật khẩu: ";
                        std::getline(std::cin, password);
                    
                        isLoggedIn = bankManager.customerLogin(accountId, password);
                        break;
                    case 0: 
                        std::cout << "Quay lại menu chính." << std::endl;
                        break;
                    default:
                        std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
            }
        }
            else if (bankManager.isCustomerLoggedIn()) {
                displayCustomerMenu1();
                std::cin >> choice ;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                switch (choice) {
                    case 1: 
                        checkBalance();
                        break;
                    case 2: 
                        deposit();
                        break;
                    case 3: 
                        withdraw();
                        break;
                    case 4: 
                        transfer();
                        break;
                    case 0: 
                        bankManager.customerLogout ();
                        std::cout << "Đăng xuất thành công!" << std::endl;
                        isLoggedIn = false;
                        break;
                    default:
                        std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
            }   
        }
        } while (choice != 0);
    }

    // Xử lý menu nhân viên
    void processEmployeeMenu() {
        int choice;
        do {
            displayEmployeeMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm

            switch (choice) {
                case 1:
                    employeeLogin();
                    break;
                case 2:
                    processTransaction();
                    break;
                case 3:
                    generateBalanceReport();
                    break;
                case 4:
                    addEmployee();
                    break;
                case 5:
                    saveData();
                    break;
                case 6:
                    bankManager.employeeLogout();
                    break;
                case 0:
                    std::cout << "Quay lại menu chính." << std::endl;
                    break;
                default:
                    std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
            }
        } while (choice != 0);
    }

    // Tạo tài khoản mới
    void createAccount() {
        std::string customerName, password;
        double initialBalance;

        std::cout << "\n===== TẠO TÀI KHOẢN MỚI =====" << std::endl;
        std::cout << "Nhập tên khách hàng: ";
        std::getline(std::cin, customerName);
        
        std::cout << "Nhập số dư ban đầu: ";
        std::cin >> initialBalance;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "Tạo mật khẩu: ";
        std::getline(std::cin, password);
        
        bankManager.createAccount(customerName, initialBalance, password);
    }

    // Kiểm tra số dư
    void checkBalance() {
        std::string accountId, password;

        std::cout << "\n===== KIỂM TRA SỐ DƯ =====" << std::endl;
        
        bankManager.checkBalance();
    }

    // Gửi tiền
    void deposit() {
        std::string accountId, password;
        double amount;

        std::cout << "\n===== GỬI TIỀN =====" << std::endl;
        
        std::cout << "Nhập số tiền gửi: ";
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        bankManager.deposit(amount);
    }

    // Rút tiền
    void withdraw() {
        std::string accountId, password;
        double amount;

        std::cout << "\n===== RÚT TIỀN =====" << std::endl;
        
        std::cout << "Nhập số tiền rút: ";
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        bankManager.withdraw(amount);
    }

    // Chuyển khoản
    void transfer() {
        std::string toAccountId, password;
        double amount;

        std::cout << "\n===== CHUYỂN KHOẢN =====" << std::endl;
        
        std::cout << "Nhập ID tài khoản đích: ";
        std::getline(std::cin, toAccountId);
        
        std::cout << "Nhập số tiền chuyển: ";
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        bankManager.transfer(toAccountId, amount);
    }

    // Đăng nhập nhân viên
    void employeeLogin() {
        std::string employeeId, password;

        std::cout << "\n===== ĐĂNG NHẬP HỆ THỐNG =====" << std::endl;
        std::cout << "Nhập ID nhân viên: ";
        std::getline(std::cin, employeeId);
        
        std::cout << "Nhập mật khẩu: ";
        std::getline(std::cin, password);
        
        bankManager.employeeLogin(employeeId, password);
    }

    // Xử lý giao dịch
    void processTransaction() {
        if (!bankManager.isEmployeeLoggedIn()) {
            std::cout << "Vui lòng đăng nhập trước khi sử dụng chức năng này!" << std::endl;
            return;
        }

        std::string accountId;
        std::cout << "\n===== XỬ LÝ GIAO DỊCH =====" << std::endl;
        std::cout << "Nhập ID tài khoản cần xem: ";
        std::getline(std::cin, accountId);
        
        bankManager.processTransaction(accountId);
    }

    // Báo cáo số dư
    void generateBalanceReport() {
        if (!bankManager.isEmployeeLoggedIn()) {
            std::cout << "Vui lòng đăng nhập trước khi sử dụng chức năng này!" << std::endl;
            return;
        }

        std::cout << "\n===== BÁO CÁO SỐ DƯ =====" << std::endl;
        bankManager.generateBalanceReport();
    }

    // Thêm nhân viên mới
    void addEmployee() {
        if (!bankManager.isEmployeeLoggedIn()) {
            std::cout << "Vui lòng đăng nhập trước khi sử dụng chức năng này!" << std::endl;
            return;
        }

        std::string employeeId, password, firstName, lastName;

        std::cout << "\n===== THÊM NHÂN VIÊN MỚI =====" << std::endl;
        std::cout << "Nhập ID nhân viên: ";
        std::getline(std::cin, employeeId);
        
        std::cout << "Nhập mật khẩu: ";
        std::getline(std::cin, password);
        
        std::cout << "Nhập tên: ";
        std::getline(std::cin, firstName);
        
        std::cout << "Nhập họ: ";
        std::getline(std::cin, lastName);
        
        bankManager.addEmployee(employeeId, password, firstName, lastName);
    }

    // Lưu dữ liệu
    void saveData() {
        if (!bankManager.isEmployeeLoggedIn()) {
            std::cout << "Vui lòng đăng nhập trước khi sử dụng chức năng này!" << std::endl;
            return;
        }

        std::string filename;
        std::cout << "\n===== LƯU DỮ LIỆU =====" << std::endl;
        std::cout << "Nhập tên file: ";
        std::getline(std::cin, filename);
        
        bankManager.saveData(filename);
    }

public:
    // Constructor
    Menu(BankManager& bm) : bankManager(bm) {}

    // Khởi chạy menu chính
    void run() {
        int choice;
        do {
            displayMainMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm

            switch (choice) {
                case 1:
                    processCustomerMenu();
                    break;
                case 2:
                    processEmployeeMenu();
                    break;
                case 0:
                    std::cout << "Cảm ơn bạn đã sử dụng dịch vụ. Tạm biệt!" << std::endl;
                    break;
                default:
                    std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << std::endl;
            }
        } while (choice != 0);
    }
};

#endif // MENU_H
