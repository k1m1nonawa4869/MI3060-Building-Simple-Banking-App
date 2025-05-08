#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

// Lớp Employee để quản lý thông tin nhân viên ngân hàng
class Employee {
private:
    std::string employeeId;    // ID nhân viên
    std::string password;      // Mật khẩu
    std::string firstName;     // Tên
    std::string lastName;      // Họ

public:
    // Constructor
    Employee() {}  // Constructor mặc định cho std::map
    
    Employee(const std::string& id, const std::string& pwd, 
             const std::string& first, const std::string& last)
        : employeeId(id), password(pwd), firstName(first), lastName(last) {}

    // Getters
    std::string getEmployeeId() const { return employeeId; }
    std::string getPassword() const { return password; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }

    // Kiểm tra mật khẩu
    bool validatePassword(const std::string& pwd) const {
        return password == pwd;
    }

    // Đổi mật khẩu
    bool changePassword(const std::string& oldPwd, const std::string& newPwd) {
        if (password != oldPwd) {
            return false;
        }
        password = newPwd;
        return true;
    }
};

#endif // EMPLOYEE_H