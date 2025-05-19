#include "BankAccount.h"

BankAccount::BankAccount(int acctId,
                         const std::string& acctName,
                         int acctPin,
                         int acctBalance)
    : id(acctId), name(acctName), balance(acctBalance), pin(acctPin), locked(false) {}

bool BankAccount::verifyPin(int inputPin) const {
    return inputPin == pin;
}

void BankAccount::setPin(int newPin) {
    pin = newPin;
}

int BankAccount::getPin() const {
    return pin;
}

bool BankAccount::isLocked() const {
    return locked;
}

void BankAccount::setLocked(bool s) {
    locked = s;
}