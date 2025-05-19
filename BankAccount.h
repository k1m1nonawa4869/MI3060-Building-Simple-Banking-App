// File: BankAccount.h
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>

class BankAccount {
private:
    int pin;
    bool locked;
public:
    int id;
    std::string name;
    int balance;

    BankAccount(int acctId = 0,
                const std::string& acctName = "",
                int acctPin = 0,
                int acctBalance = 0);

    bool verifyPin(int inputPin) const;
    void setPin(int newPin);
    int getPin() const;

    bool isLocked() const;
    void setLocked(bool s);
};

#endif // BANK_ACCOUNT_H
