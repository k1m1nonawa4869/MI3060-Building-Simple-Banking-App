// File: BankAccount.h
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>

class BankAccount {
private:
    std::string pin;
    bool locked;
    std::string created;
    std::string modified;

public:
    int id;
    std::string name;
    int balance;

    // Constructor for new or loaded accounts
    BankAccount(int acctId = 0,
                const std::string& acctName = "",
                const std::string& acctPin = "",
                int acctBalance = 0,
                const std::string& createdTime = "",
                const std::string& modifiedTime = "");

    bool verifyPin(const std::string& inputPin) const;
    void setPin(const std::string& newPin);
    const std::string& getPin() const;

    bool isLocked() const;
    void setLocked(bool s);

    const std::string& getCreated() const;
    const std::string& getModified() const;
    void updateModified();
};

#endif // BANK_ACCOUNT_H